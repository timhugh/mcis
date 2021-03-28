#pragma once

#include <functional>
#include <map>
#include <string>

namespace tileserver {
    namespace http {
        typedef std::map<std::string, std::string> Params;
        typedef std::vector<std::string> PathParams;

        enum Method {
            GET,
        };

        struct Request {
            const Params params;
            const PathParams pathParams;
        };

        enum ResponseStatus {
            OK = 200,
            INVALID = 422,
            ERROR = 500,
        };

        struct Response {
            std::string body;
            ResponseStatus status;
        };

        typedef std::function<void(const Request&, Response&)> Handler;
    };
};
