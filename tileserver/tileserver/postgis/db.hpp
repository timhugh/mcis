#pragma once

#include <postgresql/libpq-fe.h>
#include <tileserver/postgis/config.hpp>

namespace tileserver {
    namespace postgis
    {
        class DB {
            const Config &config;

            PGconn *conn;

            public:
                DB(const Config &config);
                ~DB();

                // TODO: these are all just for debugging/iterating and should go away later
                const std::string getPOIs() const;
                const std::string executeRawSql(const std::string query) const;
        };
    };
};