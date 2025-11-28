CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Wno-terminate -Wno-unused-parameter
LDLIBS := -lCatch2
LINK.o := $(CXX)

.PHONY: handin clean grade submit

FILES := DivideAndConquer.hpp

autograder:

autograder.cpp: $(FILES)

submit : $(FILES)
	autolab submit $<

grade : autograder
	./autograder

clean :
	-@rm -vf autograder *~
