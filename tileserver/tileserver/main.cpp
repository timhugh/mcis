#include <cpp-httplib/httplib.h>
#include <spdlog/spdlog.h>
#include <postgresql/libpq-fe.h>

int main() {
    const int httpPort = atoi(getenv("HTTP_PORT"));
    const char* httpAddress = getenv("HTTP_ADDRESS");

    const char* postgresHost = getenv("POSTGRES_HOST");
    const int postgresPort = atoi(getenv("POSTGRES_PORT"));
    const char* postgresUser = getenv("POSTGRES_USER");
    const char* postgresPassword = getenv("POSTGRES_PASSWORD");
    const char* postgresDatabase = getenv("POSTGRES_DB");

    char pgstring[255];
    sprintf(pgstring, "postgresql://%s:%s@%s:%d/%s", postgresUser, postgresPassword, postgresHost, postgresPort, postgresDatabase);

    PGconn *conn;

    conn = PQconnectdb(pgstring);
    if (PQstatus(conn) != CONNECTION_OK) {
        spdlog::error("Failed to connect to postgres. Quitting");
        PQfinish(conn);
        exit(1);
    }

    httplib::Server server;

    server.Get("/", [conn](const httplib::Request &, httplib::Response &response) {
        spdlog::info("GET /");
        PGresult *res = PQexec(conn, "SELECT * FROM poi");
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::string msg = "Failed to read POIS from DB: ";
            msg += PQerrorMessage(conn);
            response.set_content(msg, "plain/text");
            PQclear(res);
            return;
        }

        std::string responseBody;
        int nFields = PQnfields(res);
        for (int i = 0; i < nFields; i++) {
            responseBody += PQfname(res, i);
            responseBody += ",";
        }
        responseBody += "\n";

        for (int i = 0; i < PQntuples(res); i++) {
            for (int j = 0; j < nFields; j++) {
                responseBody += PQgetvalue(res, i, j);
                responseBody += ",";
            }
            responseBody += "\n";
        }

        response.set_content(responseBody, "plain/text");
    });

    spdlog::info("Tileserver started! Listening on {}:{}", httpAddress, httpPort);
    server.listen(httpAddress, httpPort);
};

