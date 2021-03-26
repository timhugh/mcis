#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>

using namespace tileserver;

int main() {
    spdlog::info("Starting tileserver...");

    http::Config config = { 3000, "0.0.0.0" };

    http::API api(&config);

    api.addRoute("/", http::Method::GET, [](const http::Request &, http::Response &response) {
        response.body = "DOES IT WORK?!?";
    });

    api.start();
};

