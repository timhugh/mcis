#pragma once

#include <string>

#include <spdlog/spdlog.h>

namespace tileserver {
    namespace environment {
        const std::string require(const std::string name) {
            const char* value = getenv(name.c_str());
            if (value == nullptr) {
                std::string errMsg = "No value found for required environment variable ";
                errMsg += name;
                spdlog::critical(errMsg);
                throw errMsg;
            }
            return std::string(value);
        };

        const std::string getOrDefault(const std::string name, const std::string defaultValue) {
            char* value = getenv(name.c_str());
            if (value == nullptr) {
                return defaultValue;
            }
            return std::string(value);
        };
    };
};
