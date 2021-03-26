#pragma once

#include <string>

#include <spdlog/spdlog.h>

namespace tileserver {
    namespace environment {
        const std::string require(const std::string name);
        const std::string getOrDefault(const std::string name, const std::string defaultValue);
    };
};
