#pragma once

#include <tileserver/http/common.hpp>

namespace tileserver {
    namespace http {
        class Service {
            public:
                virtual ~Service() {};
                virtual void call(const Request &, Response &) const = 0;
        };
    };
};
