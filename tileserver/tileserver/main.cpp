#include <tileserver/http/api.hpp>

using namespace tileserver::http;

int main() {
    Config config = { 3000, "0.0.0.0" };
    API api(&config);
    api.addRoute("/", GET, [](const Request &, Response &response) {
        response.body = "DOES IT WORK?!?";
    });
    api.start();
};

