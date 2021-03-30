#pragma once

#include <functional>
#include <map>
#include <string>

namespace tileserver {
    namespace http {
        typedef std::vector<std::string> PathParams;

        enum Method {
            GET,
        };

        struct Request {
            const PathParams pathParams;
        };

        enum ResponseStatus {
            OK = 200,
            INVALID = 422,
            ERROR = 500,
        };

        struct Response {
            std::string body;
            ResponseStatus status = OK;
        };

        typedef std::function<void(const Request&, Response&)> Handler;
    };
};
