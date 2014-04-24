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
   opts.define("p|pages=b", "List the number of pages in the input data");
   opts.define("c|count=b", "List the number of segments in the input data");
   opts.define("i|info=b", "Display part information for each segment");
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   if (opts.getBoolean("pages")) {
      cout << infiles.getPageCount() << endl;
      exit(0);
   }

   infiles.analyzeSegmentsByIndent();

   if (opts.getBoolean("count")) {
      cout << infiles.getSegmentCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("info")) {
      cout << "Segment count:\t" << infiles.getSegmentCount() << endl;
      for (int i=0; i<infiles.getSegmentCount(); i++) {
         cout << "\nSEGMENT:\t" << i << "\t======================" << endl;
         cout << infiles.getSegment(i);
      }
      exit(0);
   }

   cout << "RESULT:" << endl;
   cout << endl;
   cout << infiles;

   return 0;
}

///////////////////////////////////////////////////////////////////////////


