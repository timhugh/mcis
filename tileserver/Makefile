CC=g++
CFLAGS=-Wall -Wextra -pthread -I./libs -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
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

clean:
	rm -f build/*
