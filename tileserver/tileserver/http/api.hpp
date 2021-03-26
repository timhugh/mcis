#pragma once

#include <cpp-httplib/httplib.h>

#include <map>
#include <string>

#include <tileserver/http/config.hpp>

namespace tileserver {
    namespace http {
        typedef std::map<std::string, std::string> Params;

        enum Method {
            GET,
        };

        struct Request {
            const tileserver::http::Params params;
        };

        struct Response {
            std::string body;
        };

        typedef void (*Handler)(const tileserver::http::Request&, tileserver::http::Response&);

        class API {
            tileserver::http::Config *config;
            httplib::Server httpServer;

            public:
                API(Config* config): config(config) {};
                void addRoute(const char* path, const tileserver::http::Method method, const tileserver::http::Handler handler);
                void start();
        };
    };
};
