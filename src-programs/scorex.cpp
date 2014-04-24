//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 12 20:10:32 PDT 2014
// Last Modified: Sat Apr 12 20:10:37 PDT 2014
// Filename:      scorex.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/scorex.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/scorex
// Syntax:        C++ 11
//
// Description:   Extract pages, overlays or systems from multi-page input.
//

#include "scorelib.h"

using namespace std;

void   processData          (ScorePage& infile, Options& opts);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("c|count=b", "count the number of input pages/segments");
   opts.define("pc|page-count=b", "count the number of input pages");
   opts.define("sc|segment-count=b", "count the number of input segments");
   opts.define("i|info=b", "count input pages, overlays, and systems");
   opts.define("p|page=i:0", "Extract given page index (offset from 1)");
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   if (opts.getBoolean("count")) {
      cout << "Pages:\t\t" << infiles.getPageCount() << endl;
      infiles.analyzeSegmentsByIndent();
      cout << "Segments:\t" << infiles.getSegmentCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("page-count")) {
      cout << infiles.getPageCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("segment-count")) {
      infiles.analyzeSegmentsByIndent();
      cout << infiles.getSegmentCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("page")) {
      cout << infiles[opts.getInteger("page")-1];
      exit(0);
   }

   // extract segment here.



   return 0;
}

///////////////////////////////////////////////////////////////////////////


