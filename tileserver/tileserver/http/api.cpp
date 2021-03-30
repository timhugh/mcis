#include <chrono>

#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>

using namespace tileserver;

const http::Request formatRequest(const httplib::Request &request) {
    http::PathParams pathParams;
    for (auto match : request.matches) {
        pathParams.push_back(match);
    }
    return {
        pathParams,
    };
};

const httplib::Server::Handler wrapService(const http::Service &service) {
    return [&service](const httplib::Request &httpRequest, httplib::Response &httpResponse) {
        auto start = std::chrono::high_resolution_clock::now();

        const http::Request apiRequest = formatRequest(httpRequest);
        http::Response apiResponse;

        service.call(apiRequest, apiResponse);

        httpResponse.body = apiResponse.body;
        httpResponse.status = apiResponse.status;

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        spdlog::info("method={} path={} time_elapsed={}ns status={}", httpRequest.method, httpRequest.path, duration.count(), apiResponse.status);
    };
};

void http::API::addRoute(
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

void http::API::start() {
    spdlog::info("Starting server on port {}", config.port);
    httpServer.listen(config.address.c_str(), config.port);
};
