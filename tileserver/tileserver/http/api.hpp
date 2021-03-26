#pragma once

#include <cpp-httplib/httplib.h>
#include <tileserver/http/common.hpp>
#include <tileserver/http/config.hpp>
#include <tileserver/http/service.hpp>

namespace tileserver {
    namespace http {
        class API {
            const Config &config;
            httplib::Server httpServer;

            public:
                API(const Config &config): config(config) {};
                void addRoute(const std::string path, const Method method, const Service &service);
                void start();
        };
    };
};
