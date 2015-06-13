#
# Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
# Creation Date: Mon Feb  9 00:16:04 PST 2015
# Last Modified: Mon Jun  8 22:12:07 PDT 2015
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


server: serve
serve:
	jekyll serve --watch --host 127.0.0.1 --port 4000
	@echo "Go to the webpage: http://127.0.0.1:4000"


sq: serve-quiet
server-quiet: serve-quiet
serve-quiet: 
	jekyll serve --detach --watch --host 127.0.0.1 --port 4000 &> /dev/null &
	@echo "Go to the webpage: http://127.0.0.1:4000"


kill:       stop-server
ss:         stop-server
stopserver: stop-server
stop-server:
	kill -9 `ps ax | grep "jekyll.*4000" | grep serve | sed 's/ .*//'`


