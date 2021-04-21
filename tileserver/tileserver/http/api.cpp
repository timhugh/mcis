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

        try {
            service.call(apiRequest, apiResponse);
        } catch (std::string errMsg) {
            spdlog::error(errMsg);
            apiResponse.content = errMsg;
            apiResponse.status = http::ERROR;
        } catch (std::exception ex) {
            spdlog::error(ex.what());
            apiResponse.content = apiResponse.content;
            apiResponse.status = http::ERROR;
        }

        for (std::pair<std::string, std::string> header : apiResponse.headers) {
            httpResponse.set_header(header.first.c_str(), header.second);
        }

        if (apiResponse.contentType == http::PROTOBUF) {
            if(apiResponse.contentLength == 0) {
                httpResponse.status = 204;
            } else {
                httpResponse.status = 200;
                httpResponse.set_content_provider(
                    apiResponse.contentLength,
                    "application/x-protobuf",
                    [&apiResponse](size_t offset, size_t length, httplib::DataSink &sink) {
                        sink.write(apiResponse.content.substr(offset, length).c_str(), length);
                        return true;
                    }
                );
            }
        } else {
            httpResponse.set_content(apiResponse.content.c_str(), "text/plain");
            httpResponse.status = apiResponse.status;
        }
        httpResponse.set_header("Access-Control-Allow-Origin", "*");

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
