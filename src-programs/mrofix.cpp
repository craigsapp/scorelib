//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 23 14:03:10 PDT 2015
// Last Modified: Thu Apr 23 14:37:26 PDT 2015
// Filename:      mrofix.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/mrofix.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/mrofix
// Syntax:        C++ 11
//
// Description:   Clean up output from mro2score converter.
//

#include "scorelib.h"

using namespace std;

void   processPage          (ScorePage& infile, Options& opts);
void   fixRests             (ScorePage& infile);
void   assignPartNumbers    (ScorePage& infile);
void   makeThinSlurs        (ScorePage& infile);
void   makeStaffP4Values    (ScorePage& infile);
void   removeShortTextItems (ScorePage& infile);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("r|rest|rests=b", "fix vertical placement of rests");
   opts.define("V|no-staff-adjust=b", "do not convert P10 to P4 for staves");
   opts.define("T|no-text-clean=b", "do not convert remove short text items");
   opts.define("S|no-thin-slurs=b", "do not make slurs thinner");
   opts.process(argc, argv);
   
   ScorePageSet infiles(opts);

   for (int i=0; i<infiles.getPageCount(); i++) {
      processPage(*infiles.getPage(i), opts);
      cout << *infiles.getPage(i);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// processPage --
//

void processPage(ScorePage& infile, Options& opts) {

   if (opts.getBoolean("rests")) {
      fixRests(infile);
   }

   if (!opts.getBoolean("no-staff-adjust")) {
      makeStaffP4Values(infile);
   }

   if (!opts.getBoolean("no-text-clean")) {
      removeShortTextItems(infile);
   }

   if (!opts.getBoolean("no-thin-slurs")) {
      makeThinSlurs(infile);
   }

}



//////////////////////////////
//
// makeStaffP4Values --
//

void makeStaffP4Values(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();
   for (auto it : data) {
	it->convertStaffP10InchesToP4Value();
   }
}



//////////////////////////////
//
// fixRests -- Fix vertical placement of rests.
//

void fixRests(ScorePage& infile) {
   // 99 Raise 16th note rests by one unit:
   // if p1=2 and p7=0.25 then p4=p4+1

   // 99 Lower quarter-note rests by one unit:
   // if p1=2 and p7=1 then p4=p4-1
}



//////////////////////////////
//
// removeShortTextItems --
//

void removeShortTextItems(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();

   for (auto it = data.begin(); it != data.end(); ) {
      if (!(*it)->isTextItem()) {
         it++;
         continue;
      }
      if ((*it)->getP13Int() >= 2) {
         it++;
         continue;
      }
      // delete the item since the text is too short.
      it = data.erase(it);
   }
}



//////////////////////////////
//
// makeThinSlurs --
//

void makeThinSlurs(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();

   for (auto& it : data) {
      // thin slurs:
      if (!it->isSlurItem()) {
         // need to be more refined: could be a repeat bracket, etc.
         continue;
      }

      // if p1=5 then p13=0.65
      it->setP13(0.65);

      // make short slurs even thinner:
      // if p1=5 and p6-p3 < 5 then p13=0.5
      if (it->getP6() - it->getP3() < 5.0) {
         it->setP13(0.5);
      }
   }
}



//////////////////////////////
//
// assignPartNumbers -- If P9 of staff is set to zero, then fill in
//   with the staff number of the current system, with the top staff
//   on the system being "1".
//

void assignPartNumbers(ScorePage& infile) {
   vectorVSIp& staffItems = infile.staffItems();
   int i, j;
   for (i=0; i<staffItems.size(); i++) {
      int ssize = staffItems[i].size();
      for (j=0; j<ssize; j++) {
         if (staffItems[i][j]->getP9Int() != 0) {
            continue;
         }
         staffItems[i][j]->setP9(ssize-j);
      }
   }
}



