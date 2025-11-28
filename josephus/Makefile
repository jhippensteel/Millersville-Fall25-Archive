CXX := g++
CXXFLAGS := -std=c++23
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: clean

Driver: Driver.o Josephus.o

clean:
	-rm -rf Driver Driver.o Josephus.o
