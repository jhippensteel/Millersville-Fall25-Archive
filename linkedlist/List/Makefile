CXX := g++
CXXFLAGS := -std=c++23 -g
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: all clean

all : ListDriver

ListDriver.cc: List.hpp

ListDriver: ListDriver.cc 

clean:
	rm -f ListDriver
