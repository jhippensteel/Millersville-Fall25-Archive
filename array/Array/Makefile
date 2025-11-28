CXX := g++
CXXFLAGS := -std=c++23 -g
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: all clean

all : ArrayDriver

ArrayDriver.cc: Array.hpp

ArrayDriver: ArrayDriver.cc

clean :
	rm -f ArrayDriver
