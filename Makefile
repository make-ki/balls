CXX = g++
CFLAGS = $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib)
OPTS = -g -O0 -Wall -Wextra -fsanitize=address,undefined

.PHONY: b clean

b:
	mkdir -p build
	$(CXX) play.cpp $(CFLAGS) $(LIBS) $(OPTS) -o build/play

clean:
	rm -rf build
