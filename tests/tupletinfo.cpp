//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jul 18 17:46:10 PDT 2014
// Last Modified: Fri Jul 18 17:46:13 PDT 2014
// Filename:      tupletinfo.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/tupletinfo.cpp
// Syntax:        C++ 11
//
// Description:   List all of the tuplets on page(s).
//

#include "ScorePageSet.h"
#include "Options.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   ScorePageSet infiles(opts);
   infiles.analyzeTuplets();

   for (int i=0; i<infiles.getPageCount(); i++) {
      cout << "PAGE " << i + 1 << ":" << endl;
      infiles[i][0].printTupletDatabase(cout);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


