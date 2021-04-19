#include <iterator>
#include <math.h>
#include <sstream>
#include <tileserver/http/common.hpp>
#include <tileserver/tile_service.hpp>
#include <vector>
#include <spdlog/spdlog.h>

using namespace tileserver;

TileService::TileService(const postgis::DB &db): db(db) {};

struct Tile {
    const int z, x, y;
};

const std::string validateTile(const Tile &tile) {
    if (tile.z < 0 || tile.z > 24) {
        return "Zoom level must be between 0 and 24";
    }
    const int maxSize = pow(2, tile.z);
    if (
        tile.x < 0 || tile.x > maxSize ||
        tile.y < 0 || tile.y > maxSize
    ) {
        return "X and Y must be between 0 and " + std::to_string(maxSize) + " for zoom level " + std::to_string(tile.z);
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

const std::string tileToSql(const Tile &tile) {
    char buf[4096];
    sprintf(buf, TILE_QUERY_FORMAT, "land", "gid, label", tile.z, tile.x, tile.y, "land");
    return buf;
};

const std::string TileService::generateTile(const int x, const int y, const int z) const {
    const Tile tile = { z, x, y };
    const std::string validationError = validateTile(tile);
    if(!validationError.empty()) {
        throw validationError;
    }
    spdlog::debug("valid tile with x:{} y:{} z:{}", tile.x, tile.y, tile.z);

    std::string pbfQuery = tileToSql(tile);
    spdlog::debug("generated query: {}", pbfQuery);

    return db.executeRawSql(pbfQuery);
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const int z = std::stoi(request.pathParams[1]);
    const int x = std::stoi(request.pathParams[2]);
    const int y = std::stoi(request.pathParams[3]);

    try{
        const std::string tile = generateTile(x, y, z);
        response.content = tile;
        response.contentType = "application/x-protobuf";
    } catch (std::string error) {
        response.content = "Failed to fetch vector tiles: " + error;
        response.contentType = "text/plain";
        response.status = http::ERROR;
    }
};
