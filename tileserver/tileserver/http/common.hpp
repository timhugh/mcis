#pragma once

#include <functional>
#include <map>
#include <string>

namespace tileserver {
    namespace http {
        using PathParams = std::vector<std::string>;
        using Headers = std::map<std::string, std::string>;

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
            std::string content;
            std::string contentType = "text/plain";
            Headers headers;
            ResponseStatus status = OK;
        };

        using Handler = std::function<void(const Request&, Response&)>;
    };
};
