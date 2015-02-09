#
# Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
# Creation Date: Mon Feb  9 00:16:04 PST 2015
# Last Modified: Mon Feb  9 00:16:07 PST 2015
# Filename:      scorelib/Makefile
# Syntax:        GNU Makefile
#
# Description:   Adds source-code lines for functions in ATON files for
#                class documentation.
#

all: docslot lines

lines:
	(cd class; $(MAKE) lines);

docslot:
	(cd _includes/docslot; $(MAKE));
