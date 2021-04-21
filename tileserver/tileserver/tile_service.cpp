#include <iterator>
#include <math.h>
#include <sstream>
#include <tileserver/http/common.hpp>
#include <tileserver/tile_service.hpp>
#include <vector>
#include <spdlog/spdlog.h>

using namespace tileserver;

TileService::TileService(const postgis::DB &db): db(db) {};

struct TileParams {
    const int z, x, y;
};

const std::string validateTile(const TileParams &params) {
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

// Params:
// layer label
// attribute column(s)
// tile z
// tile x
// tile y
// table name
const char* TILE_QUERY_FORMAT = R"QUERY(
    SELECT ST_AsMVT(q, '%s') as tiles
    FROM (
        SELECT
            %s,
            ST_AsMVTGeom(
                geom,
                ST_TileEnvelope(%d, %d, %d),
                extent => 4096,
                buffer => 64
            ) geom
        FROM %s
    ) q;
)QUERY";

const std::string tileToSql(const TileParams &params) {
    char buf[4096];
    sprintf(buf, TILE_QUERY_FORMAT, "land", "gid, label", params.z, params.x, params.y, "land");
    return buf;
};

const std::vector<char> TileService::generateTile(const int x, const int y, const int z) const {
    const TileParams params = { z, x, y };
    const std::string validationError = validateTile(params);
    if(!validationError.empty()) {
        throw validationError;
    }
    spdlog::debug("valid tile with x:{} y:{} z:{}", params.x, params.y, params.z);

    std::string pbfQuery = tileToSql(params);
    spdlog::debug("generated query: {}", pbfQuery);

    return db.binaryQuery(pbfQuery);
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const int z = std::stoi(request.pathParams[1]);
    const int x = std::stoi(request.pathParams[2]);
    const int y = std::stoi(request.pathParams[3]);

    try{
        const std::vector<char> tile = generateTile(x, y, z);
        const std::string content(tile.begin(), tile.end());
        response.content = content;
        response.contentLength = tile.size();
        response.contentType = http::PROTOBUF;
    } catch (std::string error) {
        response.content = "Failed to fetch vector tiles: " + error;
        response.contentType = http::PLAIN_TEXT;
        response.status = http::ERROR;
    }
};
