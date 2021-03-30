#include <iterator>
#include <math.h>
#include <sstream>
#include <tileserver/http/common.hpp>
#include <tileserver/tile_service.hpp>
#include <vector>
#include <spdlog/spdlog.h>

using namespace tileserver;

TileService::TileService(const postgis::DB &db): db(db) {

};

struct Tile {
    const int z, x, y;
};

struct Envelope {
    const double xmin, xmax, ymin, ymax;
};

const Tile parseRequestParams(const http::PathParams &params) {
    return {
        std::stoi(params[1]),
        std::stoi(params[2]),
        std::stoi(params[3]),
    };
};

const std::string validateTile(const Tile &params) {
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

// World constants for EPSG:3857
const double worldMercMax = 20037508.3427892;
const double worldMercMin = worldMercMax * -1;
const double worldMercSize = worldMercMax - worldMercMin;

const Envelope tileToEnvelope(const Tile &tile) {
    const int tileSize = pow(2, tile.z);
    spdlog::debug("tilesize for z={}: {}", tile.z, tileSize);
    const double tileMercSize = worldMercSize / tileSize;
    spdlog::debug("projected tilesize: {}", tileMercSize);
    return {
        worldMercMin + (tileMercSize * tile.x),
        worldMercMin + (tileMercSize * (tile.x + 1)),
        worldMercMax - (tileMercSize * (tile.y + 1)),
        worldMercMax - (tileMercSize * tile.y),
    };
};

const int DEFAULT_DENSIFY_FACTOR = 4;

const std::string envelopeToBoundsSql(const Envelope &envelope) {
    const double segmentSize = (envelope.xmax - envelope.xmin) / DEFAULT_DENSIFY_FACTOR;
    char buf[1024];
    sprintf(buf, "ST_Segmentize(ST_MakeEnvelope(%f, %f, %f, %f, 3857), %f)", envelope.xmin, envelope.ymin, envelope.xmax, envelope.ymax, segmentSize);
    return buf;
};

// Params:
//  boundsSQL
//  boundsSQL
//  attrColumns
//  table
const char* TILE_QUERY_FORMAT = R"QUERY(
    WITH
    bounds AS (
        SELECT %s AS geom,
        %s::box2d as b2d
    ),
    mvtgeom AS (
        SELECT ST_AsMVTGeom(ST_Transform(t.geom, 3857), bounds.b2d) AS geom,
            %s
        FROM %s t, bounds
        WHERE ST_Intersects(t.geom, ST_Transform(bounds.geom, 26918))
    )
    SELECT ST_AsMVT(mvtgeom.*) FROM mvtgeom
)QUERY";

const std::string envelopeToSQL(const Envelope &envelope) {
    const std::string bounds = envelopeToBoundsSql(envelope);
    char buf[4096];
    sprintf(buf, TILE_QUERY_FORMAT, bounds.c_str(), bounds.c_str(), "label", "land");
    return buf;
};

void TileService::call(const http::Request &request, http::Response &response) const {
    const Tile tile = parseRequestParams(request.pathParams);

    const std::string validationError = validateTile(tile);
    if (!validationError.empty()) {
        response.content = validationError;
        response.status = http::INVALID;
        return;
    }
    spdlog::debug("valid tile with x:{} y:{} z:{}", tile.x, tile.y, tile.z);

    const Envelope envelope = tileToEnvelope(tile);
    spdlog::debug("converted to envelope xmin:{} xmax:{} ymin:{} ymax:{}", envelope.xmin, envelope.xmax, envelope.ymin, envelope.ymax);

    std::string pbfQuery = envelopeToSQL(envelope);
    spdlog::debug("generated query: {}", pbfQuery);

    response.content = db.executeRawSql(pbfQuery);
    response.contentType = "application/vnd.mapbox-vector-tile";
};
