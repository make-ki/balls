CXX = g++
CFLAGS = $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib)

.PHONY: b clean

b:
	mkdir -p build
	$(CXX) play.cpp $(CFLAGS) $(LIBS) -o build/play

clean:
	rm -rf build
