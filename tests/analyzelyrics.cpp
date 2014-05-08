//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 01:10:49 PDT 2014
// Last Modified: Wed Apr 30 01:10:52 PDT 2014
// Filename:      analyzelyrics.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/analyzelyrics.cpp
// Syntax:        C++11
//
// Description:   This program analyzes lyrics of music, identifying verses.
//

#include "scorelib.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   ScorePageSet infiles(opts);
   infiles.analyzeSegmentsByIndent();
   infiles.analyzeLyrics();
   cout << infiles;
   return 0;
}

///////////////////////////////////////////////////////////////////////////



