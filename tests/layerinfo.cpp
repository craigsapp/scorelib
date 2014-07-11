//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  8 22:22:54 PDT 2014
// Last Modified: Tue Jul  8 22:22:57 PDT 2014
// Filename:      layerinfo.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/layerinfo.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/layerinfo
// Syntax:        C++ 11
//
// Description:   List automatic identification of note layers.
//

#include "scorelib.h"

using namespace std;

void   processPage          (ScorePage& infile, Options& opts);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   for (int i=0; i<infiles.getPageCount(); i++) {
      processPage(infiles[i][0], opts);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////



//////////////////////////////
//
// processPage --
//

void processPage(ScorePage& infile, Options& opts) {

    infile.analyzeLayers();
    cout << infile;
    cout << "\n\n";

}



