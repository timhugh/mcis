#include <cpp-httplib/httplib.h>
#include <spdlog/spdlog.h>
#include <postgresql/libpq-fe.h>

const char* env(const char* name) {
    const char* value = getenv(name);
    if (value == NULL || value[0] == '\0') {
        spdlog::error("Missing value for required environment variable '{}'", name);
        exit(1);
    }
    return value;
}

int main() {
    spdlog::info("Gathering required configuration...");
    const int httpPort = atoi(env("HTTP_PORT"));
    const char* httpAddress = env("HTTP_ADDRESS");

    const char* postgresHost = env("POSTGRES_HOST");
    const int postgresPort = atoi(env("POSTGRES_PORT"));
    const char* postgresUser = env("POSTGRES_USER");
    const char* postgresPassword = env("POSTGRES_PASSWORD");
    const char* postgresDatabase = env("POSTGRES_DATABASE");

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

