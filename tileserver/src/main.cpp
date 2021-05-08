#include <postgis/db.hpp>
#include <string>

int main()
{
    const std::string postgisConn = "postgres://postgres:postgres@postgis:5432/mcis";
    postgis::DB db(postgisConn);

    const std::string query = "SELECT * FROM land";
    db.transact([query](auto &xact) {
        std::unique_ptr<Result> res;
        return res;
    });
}
