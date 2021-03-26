CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -pthread -I./libs -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

SRC=tileserver/main.cpp

.PHONY: tileserver
tileserver: build/tileserver

build/tileserver: $(SRC)
	mkdir -p build
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: debug
debug: build/tileserver-debug
build/tileserver-debug: $(SRC)
	mkdir -p build
	$(CC) $< $(CFLAGS) $(LDFLAGS) -g -o $@

.PHONY: docker
docker:
	docker build . -t mcis/tileserver:latest

clean:
	rm -f build/*
