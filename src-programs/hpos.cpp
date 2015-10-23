//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct  6 11:23:50 PDT 2015
// Last Modified: Wed Oct  7 17:35:37 PDT 2015
// Filename:      hpos.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/hpos.cpp
// Documentation: http://scorelib.sapp.org/program/hpos
// Syntax:        C++ 11
//
// Description:   Marks the horizontal position of items on a system.
//
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void   processOptions       (Options& opts, int argc, char** argv);
void   labelHpos            (ScorePageSet& infiles);
void   labelHpos            (ScorePage& infile, int pageindex, int pagecount);

double mincutoff = -1.0;
double maxcutoff = 201.0;
double scale = 6.0 / 7.0;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options options;
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   labelHpos(infiles);
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// labelHpos -- Mark horizontal positions of everything on staff
//    other than at position 0 or 200.
//

void labelHpos(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   for (int i=0; i<pagecount; i++) {
      labelHpos(infiles[i][0], i, pagecount);
      break;  // just one page for now
   }
}

void labelHpos(ScorePage& infile, int pageindex, int pagecount) {
   auto& items = infile.lowLevelDataAccess();
   ScoreItem si;
   ScoreItem sj;
   double hp;
   for (auto& it : items) {
      cout << it;
      
      hp = it->getHPos();
      if (hp <= mincutoff) {
         continue;
      }
      if (hp >= maxcutoff) {
         continue;
      }

      si.setP1N(16);
      si.setP2N(it->getP2());
      si.setP3N(it->getHPos());
      si.setFixedText(to_string(it->getHPos()));
      si.setP9N(90);
      si.setP4N(-4);
      cout << si;

      sj.setP1N(4);
      sj.setP2N(it->getP2());
      sj.setP3N(it->getHPos());
      sj.setP4N(15);
      sj.setP5N(15);
      sj.setP6N(it->getHPos() + 16.0);
      sj.setP9N(90);
      cout << sj;

   }
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.process(argc, argv);

}



