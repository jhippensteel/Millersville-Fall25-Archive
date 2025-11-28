CXX := g++
CXXFLAGS := -g -std=c++23
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: all clean

all : TreeTest

TreeTest.cc : SearchTree.hpp

clean:
	-rm -rf TreeTest
