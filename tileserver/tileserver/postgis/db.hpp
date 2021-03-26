#pragma once

#include <tileserver/postgis/config.hpp>

namespace tileserver {
    namespace postgis
    {
        class DB {
            Config config;

            public:
                DB();
        }
    };
};