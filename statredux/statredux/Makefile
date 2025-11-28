CXX := g++
CXXFLAGS := -g -Wall
LINK.o := $(CXX)

.PHONY: clean handin

Driver : Driver.o Statistician.o

handin : handin.tar

handin.tar : Statistician.cpp Driver.cpp
	tar cvf $@ $^

clean :
	-$(RM) -f $(wildcard *.o) Driver handin.tar
