CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -g -pthread -I./libs -I. -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

SRCS=tileserver/main.cpp tileserver/env.cpp tileserver/http/api.cpp tileserver/http/config.cpp tileserver/postgis/db.cpp tileserver/postgis/config.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: all
all: build/tileserver

build/tileserver: $(OBJS) Makefile
	mkdir -p build
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@
	objcopy --only-keep-debug $@ $@.debug
	objcopy --strip-debug $@
	objcopy --add-gnu-debuglink $@.debug $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/* $(OBJS)
