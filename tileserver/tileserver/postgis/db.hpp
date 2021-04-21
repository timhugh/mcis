#pragma once

#include <functional>
#include <postgresql/libpq-fe.h>
#include <string>
#include <tileserver/postgis/config.hpp>
#include <vector>

namespace tileserver {
    namespace postgis
    {
        class DB {
            const Config &config;

            public:
                DB(const Config &config);

                // TODO: these are all just for debugging/iterating and should go away later
                const std::string getPOIs() const;
                const std::vector<char> binaryQuery(const std::string query) const;

            private:
                PGconn* createConnection() const;
                void withConnection(std::function<void(PGconn*)> transaction) const;
        };
    };
};