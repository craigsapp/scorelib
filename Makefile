## main scorelib GNU makefile
##
## Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
## Creation Date: Thu Feb  6 22:53:03 PST 2014
## Last Modified: Wed Feb  4 21:23:52 PST 2015
## Filename:      ...scorelib/Makefile
##
## Description: This Makefile can create the score library or programs
##
## To run this makefile, type (without quotes) "make library" (or
## "gmake library" on FreeBSD computers), then "make programs".
##

# targets which don't actually refer to files
.PHONY: src-programs src-library include bin lib obj tests update status scripts script

###########################################################################
#                                                                         #
#                                                                         #

all: info library examples

info:
	@echo ""
	@echo This makefile will create either the score library file
	@echo or will compile the score-processing programs.  You may
	@echo have to make the library first if it does not exist.
	@echo Type one of the following:
	@echo "   $(MAKE) library"
	@echo or
	@echo "   $(MAKE) programs"
	@echo ""
	@echo To compile a specific program called xxx, type:
	@echo "   $(MAKE) xxx"
	@echo ""
	@echo Typing \"make\" alone with compile both the library and all programs.
	@echo ""

library:
	-@mkdir -p obj
	$(MAKE) -f Makefile.library

tests: library
	(cd tests; $(MAKE) all)

update: pull library-update programs-update

status:
	-git status

pull:
	-git pull

examples-update: programs-update
example-update:  programs-update
program-update:  programs-update
programs-update:
	touch src-programs/*.cpp
	$(MAKE) -f Makefile.programs

libupdate: library-update
updatelib: library-update
uplib: library-update
libup: library-update
library-update:
	-@mkdir -p obj
	$(MAKE) -f Makefile.library library

clean: cleantests
	$(MAKE) -f Makefile.library clean
	-rm -rf bin
	-rm -rf lib

cleantests:
	(cd tests; $(MAKE) clean)

superclean: clean
	@echo "Erased all compiled content"

examples: programs
programs:
	$(MAKE) -f Makefile.programs

%:
	-mkdir -p bin
	@echo compiling file $@
	$(MAKE) -f Makefile.programs $@

script: scripts
scripts:
	(cd scripts && $(MAKE) install)
	
install:
	-cp  bin/* /usr/local/bin

#                                                                         #
#                                                                         #
###########################################################################



