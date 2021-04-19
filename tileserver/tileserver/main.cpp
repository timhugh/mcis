#include <spdlog/spdlog.h>
#include <tileserver/http/api.hpp>
#include <tileserver/postgis/db.hpp>
#include <tileserver/tile_service.hpp>

using namespace tileserver;

class POIService : public tileserver::http::Service {
    const postgis::DB &db;

    public:
        POIService(const postgis::DB &db): db(db) {};
        void call(const tileserver::http::Request &, tileserver::http::Response &response) const {
            response.content = db.getPOIs();
        };
};

int main() {
    // TODO: set by environment
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting tileserver...");

    try {
        const http::Config httpConfig = http::Config::fromEnvironment();
        const postgis::Config postgisConfig = postgis::Config::fromEnvironment();

        http::API api(httpConfig);
        postgis::DB db(postgisConfig);
        POIService poiService(db);
        TileService tileService(db);

        api.addRoute("/", http::Method::GET, poiService);
        api.addRoute(R"(/(\d+)/(\d+)/(\d+).vector.pbf)", http::GET, tileService);

        api.start();

    } catch(std::string exception) {
        spdlog::critical("Quitting with error: {}", exception);
    } catch(...) {
        spdlog::critical("Quitting with unknown error");
    }
};
