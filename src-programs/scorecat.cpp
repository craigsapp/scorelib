//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 12 20:16:47 PDT 2014
// Last Modified: Sat Apr 12 20:56:24 PDT 2014
// Filename:      scorecat.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/scorecat.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/scorecat
// Syntax:        C++ 11
//
// Description:   Concatenate multiple score pages into a single output stream.
//

#include "scorelib.h"

using namespace std;

void    printBySystem       (ScorePageSet& infiles);
void    printPageBySystem   (ScorePage& page);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("s|system=b", "sort data by system for each page");
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   if (opts.getBoolean("system")) {
      printBySystem(infiles);
   } else {
      cout << infiles;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// printBySystem -- print each page/overlay by system (top to bottom of page).
//

void printBySystem(ScorePageSet& infiles) {
   int i, j;
   for (i=0; i<infiles.getPageCount(); i++) {
      if (i>0) {
         cout << "\n";
      }
      cout << "###ScorePage:\t" << infiles[i][0].getFilenameBase() << endl;
      cout << "\n";
      for (j=0; j<infiles[i].getOverlayCount(); j++) {
         if (j>0) {
            cout << "\n###ScoreOverlay:\t" 
                 << infiles[i][j].getFilenameBase() << endl;
         }
         printPageBySystem(infiles[i][j]);
      }
   }
}



//////////////////////////////
//
// printPageBySystem -- Print a page of music sorted by system and
//    horizontal position (time).
//

void printPageBySystem(ScorePage& page) {
   int scount = page.getSystemCount();
   int i;
   for (i=0; i<scount; i++) {
      if (i>0) { 
         cout << "\n";
      }
      cout << "##System:\t" << i+1 << "\n";
      sort(page.systemItems(i).begin(), page.systemItems(i).end(), 
            SU::sortP3P1P2P4);
      cout << page.systemItems(i); 
   }
}



