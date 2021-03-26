#include <spdlog/spdlog.h>

#include <tileserver/http/api.hpp>
#include <tileserver/postgis/db.hpp>

using namespace tileserver;

class POIService : public tileserver::http::Service {
    postgis::DB &db;

    public:
        POIService(postgis::DB &db): db(db) {};
        void call(const tileserver::http::Request &, tileserver::http::Response &response) const {
            response.body = db.getPOIs();
        };
};

int main() {
    spdlog::info("Starting tileserver...");

    try {
        const http::Config httpConfig = http::Config::fromEnvironment();
        http::API api(httpConfig);
        const postgis::Config postgisConfig = postgis::Config::fromEnvironment();
        postgis::DB db(postgisConfig);

        POIService poiService(db);

        api.addRoute("/", http::Method::GET, poiService);
        api.start();
    } catch(std::string exception) {
        spdlog::critical("Quitting with error: {}",  exception);
    } catch(...) {
        spdlog::critical("Quitting with unknown error");
    }
};
