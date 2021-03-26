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

                const std::string getPOIs() const;
        };
    };
};