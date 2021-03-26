#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>
#include <tileserver/postgis/db.hpp>

using namespace tileserver;

int main() {
    spdlog::info("Starting tileserver...");

    const http::Config httpConfig = http::Config::fromEnvironment();
    http::API api(httpConfig);
    const postgis::Config postgisConfig = postgis::Config::fromEnvironment();
    postgis::DB db(postgisConfig);

    api.addRoute("/", http::Method::GET, [](const http::Request &, http::Response &response) {
        response.body = "DOES IT WORK?!?";
    });

    api.start();
};
