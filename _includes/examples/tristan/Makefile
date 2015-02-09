#
# Programmer:    Craig Stuart Sapp <craig.stanford.edu>
# Creation Date: Sat Feb  7 18:52:30 PST 2015
# Last Modified: Sat Feb  7 18:52:32 PST 2015
# Filename:      scorelib/_includes/examples/tristan/Makefile
# Syntax:        GNU Makefile
#
# Description:   Commands used to produce the musical example for the page:
#                   http://scorelib.sapp.org/formats/ppmx
#                The individual systems were manullly split out of the
#                Full-page PPMX data.  Some of the final SVG code was 
#                incorrect, so some manual tweaking of the SVG images
#                was done (and will have to be done again if the SVG
#                images are re-calculated).
#

FILE1 = tristan1
FILE2 = tristan2

BINDIR = ../../../bin

FCL = $(BINDIR)/fixclasslink
ANS = $(BINDIR)/addnamespace


info:
	@echo "make markup == create marked up PMX files"
	@echo "make html == create marked up HTML code for PMX files"

all: markup html svg

markup:
	$(BINDIR)/makeclasslink -sp "TR1" $(FILE1).pmx > $(FILE1)-markup.pmx
	$(BINDIR)/makeclasslink -sp "TR2" $(FILE2).pmx > $(FILE2)-markup.pmx

html:
	$(BINDIR)/makeclasslink -h -p "TR1" $(FILE1).pmx > $(FILE1)-markup.html
	$(BINDIR)/makeclasslink -h -p "TR2" $(FILE2).pmx > $(FILE2)-markup.html

redo-svg: svg svg-fix

svg:
#	$(BINDIR)/score2svg t1a.pmx > t1a.svg
	$(BINDIR)/score2svg t1b.pmx > t1b.svg
	$(BINDIR)/score2svg t1c.pmx > t1c.svg

#	$(BINDIR)/score2svg t2a.pmx > t2a.svg
#	$(BINDIR)/score2svg t2b.pmx > t2b.svg
#	$(BINDIR)/score2svg t2c.pmx > t2c.svg
#	$(BINDIR)/score2svg t2d.pmx > t2d.svg

svg-fix:
#	$(FCL) t2a.svg | $(ANS) -n _T2a > $(FILE2)a-markup-svg.html
#	$(FCL) t2b.svg | $(ANS) -n _T2b > $(FILE2)b-markup-svg.html
#	$(FCL) t2c.svg | $(ANS) -n _T2c > $(FILE2)c-markup-svg.html
#	$(FCL) t2d.svg | $(ANS) -n _T2d > $(FILE2)d-markup-svg.html

#	$(FCL) t1a.svg | $(ANS) -n _T1a > $(FILE1)a-markup-svg.html
	$(FCL) t1b.svg | $(ANS) -n _T1b > $(FILE1)b-markup-svg.html
	$(FCL) t1c.svg | $(ANS) -n _T1c > $(FILE1)c-markup-svg.html



