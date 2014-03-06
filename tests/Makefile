## Makefile for scorelib test programs.
##
## Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
## Creation Date: Sun Mar  2 21:07:02 PST 2014
## Last Modified: Mon Mar  3 00:09:19 PST 2014
## Filename:      ...optionlib/tests/Makefile
##
## Description: This Makefile compiles various test programs for using
##              the scorelib library.
##
## To run this makefile, type (without quotes) "make tests" (or 
## "gmake library" on FreeBSD computers).  The options library must be
## created first.
##

CPP_FILES     := $(wildcard *.cpp)
PROGRAM_FILES := ${CPP_FILES:.cpp=}

FLAGS          = -std=c++11
FLAGS_EDIT     = -std=c++11 -DSCOREITEMEDIT
INCLUDE        = -I../include
LIBS           = -L../lib -lscorelib
LIBS_EDIT      = -L../lib -lscorelib

# Add optionlib to include and libraries:
INCLUDE       += -I../external/optionlib/include
LIBS          += -L../external/optionlib/lib -loptionlib

help:
	@echo
	@echo "Type the name of a program to compile it."
	@echo "Available programs:"
	@echo
	@echo $(PROGRAM_FILES) | fmt -w 50
	@echo
	@echo "Type 'make all' to compile all programs."
	@echo "Type 'make touch' before recompiling programs if library changes."
	@echo "Type 'make clean' to remove all compiled programs."
	@echo

all: tests

tests: $(PROGRAM_FILES)

clean:
	-rm $(PROGRAM_FILES)

library:
	(cd ..; make library)

touch:
	touch *.cpp

%: %.cpp
	g++ $(FLAGS) $(INCLUDE) -o $@ $@.cpp $(LIBS) && strip $@


itemtest: itemtest.cpp
	g++ $(FLAGS_EDIT) $(INCLUDE) -o $@ $@.cpp $(LIBS_EDIT) && strip $@



