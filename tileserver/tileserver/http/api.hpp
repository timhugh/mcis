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
            const Params params;
        };

        struct Response {
            std::string body;
        };

        typedef void (*Handler)(const Request&, Response&);

        class API {
            Config *config;
            httplib::Server httpServer;

            public:
                API(Config* config): config(config) {};
                void addRoute(const char* path, const Method method, const Handler handler);
                void start();
        };
    };
};
