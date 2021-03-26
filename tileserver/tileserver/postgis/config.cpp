#include <string>
#include <tileserver/postgis/config.hpp>
#include <tileserver/env.hpp>

using namespace tileserver::postgis;

const std::string
    HOST = "POSTGRES_HOST",
    PORT = "POSTGRES_PORT",
    DATABASE = "POSTGRES_DATABASE",
    USER = "POSTGRES_USER",
    PASSWORD = "POSTGRES_PASSWORD";

const Config tileserver::postgis::Config::fromEnvironment() {
    return {
        environment::require(HOST),
        std::stoi(environment::require(PORT)),
        environment::require(DATABASE),
        environment::require(USER),
        environment::require(PASSWORD),
    };
};