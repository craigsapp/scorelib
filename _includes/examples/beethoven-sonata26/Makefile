#
# This example used on the about page.
#

BINDIR = ../../../bin
BASE = beethoven-sonata26

all: svg html svg2

svg:
	$(BINDIR)/makeclasslink $(BASE).pmx > $(BASE).pmx-svg

html:
	$(BINDIR)/makeclasslink -h $(BASE).pmx > $(BASE)-pmx.html

svg2:	
	$(BINDIR)/score2svg -s 1.35 $(BASE).pmx-svg | \
		tail -n +5 > $(BASE)-svg.html
	
