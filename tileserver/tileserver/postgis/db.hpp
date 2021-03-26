#pragma once

#include <tileserver/postgis/config.hpp>

namespace tileserver {
    namespace postgis
    {
        class DB {
            const Config &config;

            public:
                DB(const Config &config): config(config) {};
        };
    };
};