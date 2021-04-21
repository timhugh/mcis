#include <cpp-httplib/httplib.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <exception>
#include <string>
#include <string.h>
#include <postgresql/libpq-fe.h>

// Params:
// layer label
// attribute column(s)
// tile z
// tile x
// tile y
// table name
const char* TILE_QUERY_FORMAT = R"QUERY(
    SELECT ST_AsMVT(q, '%s') as tiles
    FROM (
        SELECT
            %s,
            ST_AsMVTGeom(
                geom,
                ST_TileEnvelope(%d, %d, %d),
                extent => 4096,
                buffer => 64
            ) geom
        FROM %s
    ) q;
)QUERY";

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting alternate tileserver");

    try {
        const char* pgConnectString = "postgres://postgres:postgres@localhost:5432/mcis";

        httplib::Server httpServer;
        httpServer.Get(R"(/(\d+)/(\d+)/(\d+).vector.pbf)", [pgConnectString](const httplib::Request &request, httplib::Response &response) {
            auto start = std::chrono::high_resolution_clock::now();
            int httpStatus;

            try {
                PGconn* conn = PQconnectdb(pgConnectString);
                if(PQstatus(conn) != CONNECTION_OK) {
                    std::string err = "Failed to connect to postgis" + std::string(PQerrorMessage(conn));
                    PQfinish(conn);
                    throw err;
                }

                const int z = std::stoi(request.matches[1]);
                const int x = std::stoi(request.matches[2]);
                const int y = std::stoi(request.matches[3]);
                spdlog::debug("generating tile z={} x={} y={}", z, x, y);

                char query[4096]; // way bigger than we need
                sprintf(query, TILE_QUERY_FORMAT, "land", "gid, label", z, x, y, "land");

                spdlog::debug("executing sql: {}", query);

                PGresult *res = PQexecParams(conn, query, 0, {}, {}, {}, {}, 1);
                if(PQresultStatus(res) != PGRES_TUPLES_OK) {
                    std::string err = "Failed to fetch tile: " + std::string(PQerrorMessage(conn));
                    PQclear(res);
                    PQfinish(conn);
                    throw err;
                }

                const int resultLength = PQgetlength(res, 0, 0);
                const char* tile = PQgetvalue(res, 0, 0);
                response.set_content_provider(
                    resultLength,
                    "application/x-protobuf",
                    [tile](size_t offset, size_t length, httplib::DataSink &sink) {
                        const char* t = tile;
                        sink.write(&t[offset], std::min(length, size_t(4)));
                        return true;
                    }
                );
                httpStatus = 200;
            } catch(std::string exception) {
                spdlog::error("exception: {}", exception);
                response.set_content(exception, "text/plain");
                httpStatus = 500;
            } catch(std::exception exception) {
                spdlog::error("exception: {}", exception.what());
                response.set_content(exception.what(), "text/plain");
                httpStatus = 500;
            }

            response.status = httpStatus;
            response.set_header("Access-Control-Allow-Origin", "*");

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            spdlog::info("method={} path={} time_elapsed={}ns status={}", request.method, request.path, duration.count(), httpStatus);
        });

        spdlog::info("Listening on port 3000");
        httpServer.listen("0.0.0.0", 3000);
    } catch(std::string exception) {
        spdlog::critical("Quitting with error: {}", exception);
    } catch(std::exception exception) {
        spdlog::critical("Quitting with error: {}", exception.what());
    } catch(...) {
        spdlog::critical("Quitting with unknown error");
    }
}