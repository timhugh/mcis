#pragma once

namespace tileserver {
    namespace http {
        struct Config {
            const int port;
            const std::string address;

            static const Config fromEnvironment();
        };
    };
};
