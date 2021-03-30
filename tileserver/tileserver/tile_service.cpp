#include <tileserver/tile_service.hpp>

using namespace tileserver;

#include <vector>
#include <sstream>
#include <iterator>

TileService::TileService(const postgis::DB &db): db(db) {

};

struct Params {
    int z;
    float x;
    float y;

    std::vector<std::string> errors;
};

const Params parseRequestParams(const http::PathParams &params) {
    Params out;

    try {
        out.z = std::stoi(params[1]);
    } catch (std::invalid_argument) {
        out.errors.push_back("Invalid param `z`. Must be a whole number");
    }

    try {
        out.x = std::stoi(params[2]);
    } catch (std::invalid_argument) {
        out.errors.push_back("Invalid param `x`. Must be a whole number");
    }

    try {
        out.y = std::stoi(params[3]);
    } catch (std::invalid_argument) {
        out.errors.push_back("Invalid param `y`. Must be a whole number");
    }

    return out;
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const Params params = parseRequestParams(request.pathParams);
    if (!params.errors.empty()) {
        std::ostringstream errorString;
        std::copy(params.errors.begin(), params.errors.end(), std::ostream_iterator<std::string>(errorString, "; "));
        response.body = errorString.str();
        response.status = http::INVALID;
        return;
    }

    response.body = "params are valid!";
    response.status = http::OK;
};
