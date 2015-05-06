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

LIST=ppmxsplit bboxpad svgscale embedpmx uniqsvgids webscore-insert

all: install

install: $(LIST)

ppmxsplit:
	cp perl/ppmxsplit/ppmxsplit $(TARGET)
	chmod 0755 $(TARGET)/ppmxsplit

bboxpad:
	cp perl/bboxpad/bboxpad $(TARGET)
	chmod 0755 $(TARGET)/bboxpad

svgscale:
	cp perl/svgscale/svgscale $(TARGET)
	chmod 0755 $(TARGET)/svgscale

embedpmx:
	cp perl/embedpmx/embedpmx $(TARGET)
	chmod 0755 $(TARGET)/embedpmx

uniqsvgids:
	cp perl/uniqsvgids/uniqsvgids $(TARGET)
	chmod 0755 $(TARGET)/uniqsvgids

webscore-insert:
	cp perl/webscore-insert/webscore-insert $(TARGET)
	chmod 0755 $(TARGET)/webscore-insert

