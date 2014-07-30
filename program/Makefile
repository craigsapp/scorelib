

all: programs build

build: 
	(cd ../; $(MAKE) build)


programs:
	for i in */index.dat; \
	do \
	   echo Processing $$i...; \
	   (cd `dirname $$i`; $(MAKE) index); \
	done

