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

        enum ContentType {
            PLAIN_TEXT,
            PROTOBUF,
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
            int contentLength = 0;
            ContentType contentType = PLAIN_TEXT;
            Headers headers;
            ResponseStatus status = OK;
        };

        using Handler = std::function<void(const Request&, Response&)>;
    };
};
