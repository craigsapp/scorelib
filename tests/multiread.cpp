//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 10 19:26:58 PDT 2014
// Last Modified: Sat Apr 12 02:51:56 PDT 2014
// Filename:      multiread.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/multiread.cpp
// Syntax:        C++ 11
//
// Description:   Test reading multiple pages in a single PPMX file or multiple
//                PMX/MUS files.
//

#include "scorelib.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   ScorePageSet infiles(opts);
   cout << infiles;
   return 0;
}

///////////////////////////////////////////////////////////////////////////

