//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  8 22:22:54 PDT 2014
// Last Modified: Tue Jul  8 22:22:57 PDT 2014
// Filename:      tieinfo.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/tieinfo.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/tieinfo
// Syntax:        C++ 11
//
// Description:   List automatic identification of note ties.
//

#include "scorelib.h"
#include <sstream>

using namespace std;

void   processPage          (ScorePage& infile, Options& opts);
void   printMarkedTies      (ScorePage& page);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("m|mark=b", "mark ties in red; tied notes in blue in notation");
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   for (int i=0; i<infiles.getPageCount(); i++) {
      processPage(infiles[i][0], opts);
      if (opts.getBoolean("mark")) {
         printMarkedTies(infiles[i][0]);
      } else {
         cout << infiles[i][0];
      }
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// printMarkedTies -- print slurs in red, and tied notes in blue.
//    Using SVG coloring.
//

void printMarkedTies(ScorePage& page) {
   int i;
   ScoreItem* sip;

   stringstream ties;
   stringstream tiednotes;

   for (i=0; i<page.getItemCount(); i++) {
      sip = page.getItem(i);
      if (sip->isTie()) {
         ties << sip;
      } else if (sip->inTieGroup()) {
         tiednotes << sip;
      } else {
         cout << sip;
      }
   }

   if (ties.rdbuf()->in_avail()) {
      cout << "\n\n";
      cout << "t 1 1\n";
      cout << "_99%svg%<g color=\"red\" stroke=\"red\">\n";
      cout << ties.str();
      cout << "t 1 1\n";
      cout << "_99%svg%<\\g>\n";
      cout << "\n\n";
   }

   if (tiednotes.rdbuf()->in_avail()) {
      cout << "\n\n";
      cout << "t 1 1\n";
      cout << "_99%svg%<g color=\"blue\" stroke=\"blue\">\n";
      cout << tiednotes.str();
      cout << "t 1 1\n";
      cout << "_99%svg%<\\g>\n";
      cout << "\n\n";
   }

}



//////////////////////////////
//
// processPage --
//

void processPage(ScorePage& infile, Options& opts) {
    infile.analyzeTies();
}



