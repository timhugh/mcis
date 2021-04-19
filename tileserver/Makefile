CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -g -pthread -I./libs -I. -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

SRCS=tileserver/main.cpp \
	 tileserver/env.cpp \
	 tileserver/http/api.cpp \
	 tileserver/http/config.cpp \
	 tileserver/postgis/config.cpp \
	 tileserver/postgis/db.cpp \
	 tileserver/tile_service.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: all
all: build/tileserver

build/tileserver: $(OBJS) Makefile
	mkdir -p build
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@
	# objcopy --only-keep-debug $@ $@.debug
	# objcopy --strip-debug $@
	# objcopy --add-gnu-debuglink $@.debug $@

.PHONY: demo
demo: build/demo
build/demo: tileserver/demo.cpp Makefile
	mkdir -p build
	$(CC) tileserver/demo.cpp $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f build/* $(OBJS)
