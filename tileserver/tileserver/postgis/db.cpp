#include <tileserver/postgis/db.hpp>

using namespace tileserver::postgis;

std::string connectionString(const Config &config) {
    return "postgres://" + config.user + ":" + config.password + "@" + config.host + ":" + std::to_string(config.port) + "/" + config.database;
};

tileserver::postgis::DB::DB(const Config &config): config(config) {
    const std::string connString = connectionString(config);
    conn = PQconnectdb(connString.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        const std::string error = "Failed to connect to postgis: " + std::string(PQerrorMessage(conn));
        throw error;
    }
};

tileserver::postgis::DB::~DB() {
    PQfinish(conn);
};

const std::string tileserver::postgis::DB::getPOIs() const {
    PGresult *res = PQexec(conn, "SELECT * FROM poi");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::string msg = "Failed to read POIs from DB: " + std::string(PQerrorMessage(conn));
        PQclear(res);
        throw msg;
    }

    std::string response;

    const int numFields = PQnfields(res);
    const int numResults = PQntuples(res);

    for (int i = 0; i < numFields; i++) {
        response += std::string(PQfname(res, i)) + ",";
    }
    response += "\n";

    for (int i = 0; i < numResults; i++) {
        for (int j = 0; j < numFields; j++) {
            response += std::string(PQgetvalue(res, i, j)) + ",";
        }
        response += "\n";
    }

    return response;
}