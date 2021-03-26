CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -pthread -I./libs -I. -I./libs/spdlog/include $(shell pkg-config --cflags libpq)
LDFLAGS=-L$(shell pg_config --pkglibdir) $(shell pkg-config --libs libpq)

SRCS=tileserver/main.cpp tileserver/http/api.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: all
all: build/tileserver

build/tileserver: $(OBJS) Makefile
	mkdir -p build
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*
	rm -f $(OBJS)
