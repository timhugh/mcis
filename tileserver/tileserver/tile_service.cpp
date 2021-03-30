#include <tileserver/tile_service.hpp>

using namespace tileserver;

#include <vector>
#include <sstream>
#include <iterator>
#include <math.h>

TileService::TileService(const postgis::DB &db): db(db) {

};

struct Params {
    const int z, x, y;
};

const Params parseRequestParams(const http::PathParams &params) {
    return {
        std::stoi(params[1]),
        std::stoi(params[2]),
        std::stoi(params[3]),
    };
};

const std::string validateTile(const Params &params) {
    if (params.z < 0 || params.z > 24) {
        return "Zoom level must be between 0 and 24";
    }
    const int maxSize = pow(2, params.z);
    if (
        params.x < 0 || params.x > maxSize ||
        params.y < 0 || params.y > maxSize
    ) {
        return "X and Y must be between 0 and " + std::to_string(maxSize) + " for zoom level " + std::to_string(params.z);
    }

    return "";
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const Params params = parseRequestParams(request.pathParams);
    const std::string validationError = validateTile(params);
    if (!validationError.empty()) {
        response.body = validationError;
        response.status = http::INVALID;
        return;
    }

    response.body = "params are valid!";
};
