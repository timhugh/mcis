CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -g -pthread -I./libs -I. -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

.PHONY: all
all: build/demo

build/demo: dependencies demo.cpp Makefile
	mkdir -p build
	$(CC) demo.cpp $(CFLAGS) $(LDFLAGS) -o $@
	# objcopy --only-keep-debug $@ $@.debug
	# objcopy --strip-debug $@
	# objcopy --add-gnu-debuglink $@.debug $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: dependencies
dependencies:
	$(MAKE) -j -C ./libs/ all

.PHONY: clean
clean:
	rm -rf build/* $(OBJS) libs/spdlog* libs/cpp-httplib* libs/crossguid*
