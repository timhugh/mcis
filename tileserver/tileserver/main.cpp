#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>

using namespace tileserver;

int main() {
    spdlog::info("Starting tileserver...");

    http::API api(http::Config::fromEnvironment());

    api.addRoute("/", http::Method::GET, [](const http::Request &, http::Response &response) {
        response.body = "DOES IT WORK?!?";
    });

    api.start();
};

