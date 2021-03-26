CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -pthread -I./libs -I. -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

SRCS=tileserver/main.cpp tileserver/http/api.cpp tileserver/http/config.cpp
OBJS=$(SRCS:.cpp=.o)
OBJS_DEBUG=$(SRCS:.cpp=.debug.o)

.PHONY: default
default: build/tileserver

.PHONY: all
all: build/tileserver build/tileserver-debug

build/tileserver: $(OBJS) Makefile
	mkdir -p build
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: debug
debug: build/tileserver-debug
build/tileserver-debug: $(OBJS_DEBUG) Makefile
	mkdir -p build
	$(CC) $(OBJS_DEBUG) -g $(CFLAGS) $(LDFLAGS) -o $@

%.debug.o: %.cpp
	$(CC) -g $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/* $(OBJS) $(OBJS_DEBUG)
