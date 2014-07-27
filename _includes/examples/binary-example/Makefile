#
# This example used on the formats/binary page.
#

BINDIR = ../../../bin
BASE = binary-example

all: svg html svg2

svg:
	$(BINDIR)/makeclasslink $(BASE).pmx > $(BASE).pmx-svg

html:
	$(BINDIR)/makeclasslink -h $(BASE).pmx > $(BASE)-pmx.html

svg2:	
	$(BINDIR)/score2svg -s 2 $(BASE).pmx-svg | tail -n +5 > $(BASE)-svg.html
	
clean:
	-rm *.svg *.html *.pmx-svg
