#include <tileserver/tile_service.hpp>

using namespace tileserver;

#include <vector>
#include <sstream>
#include <iterator>

TileService::TileService(const postgis::DB &db): db(db) {

};

struct Params {
    const int x, y, z;
};

const Params parseRequestParams(const http::PathParams &params) {
    return {
        std::stoi(params[1]),
        std::stoi(params[2]),
        std::stoi(params[3]),
    };
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const Params params = parseRequestParams(request.pathParams);

    response.body = "params are valid!";
};
