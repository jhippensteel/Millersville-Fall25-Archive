CXX := g++
CPPFLAGS := -I./support -I.
CXXFLAGS := -std=c++23 -g
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: grade clean

autograder:

autograder.cpp: List.hpp support/TestList.hpp

grade : autograder
	./autograder

clean:
	-rm -vf autograder
