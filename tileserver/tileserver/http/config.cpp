#include <string>
#include <tileserver/http/config.hpp>
#include <tileserver/env.hpp>

using namespace tileserver::http;

const std::string
    ADDRESS = "HTTP_ADDRESS",
    DEFAULT_ADDRESS = "0.0.0.0",
    PORT = "HTTP_PORT",
    DEFAULT_PORT = "3000";

const Config tileserver::http::Config::fromEnvironment() {
    return {
        std::stoi(environment::getOrDefault(PORT, DEFAULT_PORT)),
        environment::getOrDefault(ADDRESS, DEFAULT_ADDRESS),
    };
};
