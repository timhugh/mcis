#include <tileserver/http/api.hpp>

httplib::Server::Handler wrapHandler(const tileserver::http::Handler apiHandler) {
    return [apiHandler](const httplib::Request &, httplib::Response &httpResponse) {
        const tileserver::http::Request apiRequest;
        tileserver::http::Response apiResponse;

        apiHandler(apiRequest, apiResponse);

        httpResponse.body = apiResponse.body;
        httpResponse.status = 200;
    };
};

void tileserver::http::API::addRoute(
    const char* path,
    const tileserver::http::Method method,
    const tileserver::http::Handler handler
) {
    httplib::Server::Handler wrappedHandler = wrapHandler(handler);
    switch (method)
    {
    case tileserver::http::Method::GET:
        httpServer.Get(path, wrappedHandler);
        break;
    }
};

void tileserver::http::API::start() {
    httpServer.listen(config->address, config->port);
};
