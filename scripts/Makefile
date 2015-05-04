##
## Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
## Creation Date: Sun May  3 18:53:04 PDT 2015
## Last Modified: Sun May  3 18:53:06 PDT 2015
## Filename:      scorelib/scripts/Makefile
## Language:      GNU makefile
##
## Description:   Makefile to install scripts in an bin directory (hopefully
##                in the $PATH).
##
## To run this makefile, type (without quotes) "make".
##

TARGET=/usr/local/bin

LIST=ppmxsplit bboxpad

all: install

install: $(LIST)

ppmxsplit:
	cp perl/ppmxsplit/ppmxsplit $(TARGET)
	chmod 0755 $(TARGET)/ppmxsplit

bboxpad:
	cp perl/bboxpad/bboxpad $(TARGET)
	chmod 0755 $(TARGET)/bboxpad




