#pragma once

#include <tileserver/http/service.hpp>
#include <tileserver/postgis/db.hpp>

using namespace tileserver;

namespace tileserver {
    class TileService : public http::Service {
        const tileserver::postgis::DB &db;

        public:
            TileService(const tileserver::postgis::DB &db);
            void call(const http::Request &, http::Response &response) const;
    };
};
