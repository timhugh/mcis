#include <tileserver/postgis/db.hpp>

using namespace tileserver;

std::string connectionString(const postgis::Config &config) {
    return "postgres://" + config.user + ":" + config.password + "@" + config.host + ":" + std::to_string(config.port) + "/" + config.database;
};

postgis::DB::DB(const postgis::Config &config): config(config) {};

PGconn* postgis::DB::createConnection() const {
    const std::string connString = connectionString(config);
    PGconn *conn = PQconnectdb(connString.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        const std::string error = "Failed to connect to postgis: " + std::string(PQerrorMessage(conn));
        throw error;
    }

    return conn;
};

void postgis::DB::withConnection(std::function<void(PGconn*)> transaction) const {
    auto conn = createConnection();
    transaction(conn);
    PQfinish(conn);
};

const std::string postgis::DB::getPOIs() const {
    PGresult *res;

    withConnection([&res](PGconn *conn) {
        res = PQexec(conn, "SELECT * FROM poi");
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::string msg = "Failed to read POIs from DB: " + std::string(PQerrorMessage(conn));
            PQclear(res);
            throw msg;
        }
    });

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
};

const std::string postgis::DB::executeRawSql(const std::string query) const {
    PGresult *res;
    withConnection([&res, query](PGconn *conn) {
        res = PQexecParams(conn, query.c_str(), 0, {}, {}, {}, {}, 1);
        // res = PQexec(conn, query.c_str());
        if(PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::string errorMsg = "Failed to execute sql: " + std::string(PQerrorMessage(conn));
            PQclear(res);
            throw errorMsg;
        }
    });

    return PQgetvalue(res, 0, 0);
};
