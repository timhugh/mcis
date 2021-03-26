#include <chrono>

#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>

using namespace tileserver::http;

Request formatRequest(const httplib::Request &request) {
    return {};
};

httplib::Server::Handler wrapHandler(Handler apiHandler) {
    return [apiHandler](const httplib::Request &httpRequest, httplib::Response &httpResponse) {
        auto start = std::chrono::high_resolution_clock::now();

        const Request apiRequest = formatRequest(httpRequest);
        Response apiResponse;

        apiHandler(apiRequest, apiResponse);

        httpResponse.body = apiResponse.body;
        httpResponse.status = 200;

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        spdlog::info("{} {} - {} ns", httpRequest.method, httpRequest.path, duration.count());
    };
};

void API::addRoute(
    const char* path,
    const Method method,
    const Handler handler
) {
    httplib::Server::Handler wrappedHandler = wrapHandler(handler);
    switch (method)
    {
    case Method::GET:
        httpServer.Get(path, wrappedHandler);
        break;
    }
};

void API::start() {
    spdlog::info("Starting server on port {}", config->port);
    httpServer.listen(config->address, config->port);
};
