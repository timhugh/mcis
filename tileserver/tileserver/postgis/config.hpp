#pragma once
#include <string>

namespace tileserver {
    namespace postgis {
        struct Config {
            const std::string host;
            const int port;
            const std::string database;
            const std::string user;
            const std::string password;

            static const Config fromEnvironment();
        };
    };
};
