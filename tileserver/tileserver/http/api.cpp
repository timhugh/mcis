#include <chrono>

#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>

using namespace tileserver::http;

const Request formatRequest(const httplib::Request &request) {
    return {};
};

const httplib::Server::Handler wrapService(const Service &service) {
    return [&service](const httplib::Request &httpRequest, httplib::Response &httpResponse) {
        auto start = std::chrono::high_resolution_clock::now();

        const Request apiRequest = formatRequest(httpRequest);
        Response apiResponse;

        service.call(apiRequest, apiResponse);

        httpResponse.body = apiResponse.body;
        httpResponse.status = 200;

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        spdlog::info("{} {} - {} ns", httpRequest.method, httpRequest.path, duration.count());
    };
};

void API::addRoute(
    const std::string path,
    const Method method,
    const Service &service
) {
    const httplib::Server::Handler wrappedService = wrapService(service);
    switch (method)
    {
    case Method::GET:
        httpServer.Get(path.c_str(), wrappedService);
        break;
    }
};

void API::start() {
    spdlog::info("Starting server on port {}", config.port);
    httpServer.listen(config.address.c_str(), config.port);
};
