#pragma once

#include <functional>
#include <map>
#include <string>

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

        typedef std::function<void(const Request&, Response&)> Handler;
    };
};
