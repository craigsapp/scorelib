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
   opts.define("c|count=b", "count the number of input pages");
   opts.define("i|info=b", "count input pages, overlays, and systems");
   opts.process(argc, argv);
   
   ScorePageSet infile(opts);

   if (opts.getBoolean("count")) {
      cout << infile.getPageCount() << endl;
      exit(0);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


