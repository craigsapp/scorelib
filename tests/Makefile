## Makefile for scorelib test programs.
##
## Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
## Creation Date: Sun Mar  2 21:07:02 PST 2014
## Last Modified: Fri Apr 11 23:25:21 PDT 2014 Using clang++ v3.3 for regex
## Filename:      ...tests/Makefile
##
## Description: This Makefile compiles various test programs for using
##              the scorelib library.
##
## To run this makefile, type (without quotes) "make tests" (or 
## "gmake library" on FreeBSD computers). 
##

CPP_FILES     := $(wildcard *.cpp)
PROGRAM_FILES := ${CPP_FILES:.cpp=}

.PHONY: $(CPP_FILES)

COMPILER       = g++
FLAGS          = -std=c++11
FLAGS_EDIT     = -DSCOREITEMEDIT
INCLUDE        = -I../include
LIBS           = -L../lib -lscorelib 
LIBS_EDIT      = -L../lib -lscorelib

# Using clang 3.3 for now since gcc < 4.9 does not have C++11 regex implemented
COMPILER       = clang++
FLAGS         += -stdlib=libc++

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
	$(COMPILER) $(FLAGS) $(INCLUDE) -o $@ $@.cpp $(LIBS) && strip $@

# compile itemtest with edit-history enabled.
itemtest: itemtest.cpp
	$(COMPILER) $(FLAGS) $(FLAGS_EDIT) $(INCLUDE) -o $@ $@.cpp \
	      $(LIBS_EDIT) && strip $@

