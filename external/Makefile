## GNU makefile for scorelib external libraries.
##
## Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
## Creation Date: Mon Mar  3 23:54:38 PST 2014
## Last Modified: Mon Mar  3 23:54:40 PST 2014
## Filename:      ...scorelib/external/Makefile
##
## Description: This Makefile handles making of external libraries.
##
## To run this makefile, type (without quotes) "make library" (or 
## "gmake library" on FreeBSD computers), then "make programs".
##

LIBRARIES := $(wildcard [a-z]*)

# targets which don't actually refer to files
.PHONY: $(LIBRARIES)


all: 
	for i in $(LIBRARIES); do (cd $$i; $(MAKE) library); done

clean:
	for i in $(LIBRARIES); do (cd $$i; $(MAKE) clean); done


