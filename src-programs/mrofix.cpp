//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 23 14:03:10 PDT 2015
// Last Modified: Fri Apr 24 00:20:26 PDT 2015
// Filename:      mrofix.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-programs/mrofix.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/mrofix
// Syntax:        C++ 11
//
// Description:   Clean up output from mro2score converter.
//

#include "scorelib.h"

using namespace std;

void   processPage          (ScorePage& infile, Options& opts);
void   fixRests             (ScorePage& infile);
void   fixTuplets           (ScorePage& infile);
void   assignPartNumbers    (ScorePage& infile);
void   makeThinSlurs        (ScorePage& infile);
void   makeStaffP4Values    (ScorePage& infile);
void   removeShortTextItems (ScorePage& infile);
void   adjustAccidentals    (ScorePage& infile);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("A|no-accidental-adjust=b", "do not adjust accidentals");
   opts.define("P|no-part-assignment=b", "do not staves to parts");
   opts.define("R|no-rest-fix=b", "fix vertical placement of 16th-note rests");
   opts.define("S|no-thin-slurs=b", "do not make slurs thinner");
   opts.define("T|no-text-clean=b", "do not convert remove short text items");
   opts.define("U|no-tuplet-fix=b", "do not fix tuplet rhythms");
   opts.define("V|no-staff-adjust=b", "do not convert P10 to P4 for staves");
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

   if (!opts.getBoolean("no-rest-fix")) {
      fixRests(infile);
   }

   if (!opts.getBoolean("no-staff-adjust")) {
      makeStaffP4Values(infile);
   }

   if (!opts.getBoolean("no-accidental-adjust")) {
      adjustAccidentals(infile);
   }

   if (!opts.getBoolean("no-text-clean")) {
      removeShortTextItems(infile);
   }

   if (!opts.getBoolean("no-tuplet-fix")) {
      fixTuplets(infile);
   }

   if (!opts.getBoolean("no-thin-slurs")) {
      makeThinSlurs(infile);
   }

   if (!opts.getBoolean("no-part-assignment")) {
      assignPartNumbers(infile);
   }

}



//////////////////////////////
//
// adjustAccidentals --
//

void adjustAccidentals(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();
   SCORE_FLOAT fract;
   int acc;
   double threshold = 0.1;
   for (auto it : data) {
      if (!it->isNoteItem()) {
         continue;
      }
      // If the accidental is shifted slightly to the left,
      // then put it back in the default position.
      fract = it->getParameterFraction(5);
      if ((fract > 0.0) && (fract <= threshold)) {
         acc = it->getP5Int();
         it->setP5N(acc);
      }
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
// fixTuplets -- Fix the duration of tuplets to avoid round-off errors.
//     Should also add number to beams for tuplets...
//

void fixTuplets(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();
   SCORE_FLOAT duration;

   for (auto it : data) {
      if (!it->hasDuration()) {
         continue;
      }
      duration = it->getDuration();
      if (duration == 0.167) {
         // fix duration of triplet sixteenths.
         it->setP7N(0.1667);
      }
   }
}



//////////////////////////////
//
// fixRests -- Fix vertical placement of rests.
//

void fixRests(ScorePage& infile) {
   listSIp& data = infile.lowLevelDataAccess();

   SCORE_FLOAT duration;
   SCORE_FLOAT vpos;
   for (auto it : data) {
      if (!it->isRestItem()) {
         continue;
      }

      duration = it->getDuration();
      vpos = it->getVPos();
      if ((duration == 0.25) && (vpos == -1.0)) {
         // Raise 16th note rests by one unit:
         it->setP4N(0.0);  // change to setVpos() later for grace note rests.
      } else if ((duration == 1.0) && (vpos == 1.0)) {
         // Lower quarter-note rests by one unit:
         it->setP4N(0.0);  // change to setVpos() later for grace note rests.
      }
   }
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
      it->setP13N(0.65);

      // make short slurs even thinner:
      // if p1=5 and p6-p3 < 5 then p13=0.5
      if (it->getP6() - it->getP3() < 5.0) {
         it->setP13N(0.5);
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
   infile.analyzeSystems();
   // Get a list of the staff items on the page.  The list is two
   // dimensional: The first dimension is the P2 index value, the
   // second dimension is an index into the the list for the given
   // P2 index (in the rare case that there is more than one staff
   // item for a given staff position.
   vectorVSIp& staffItems = infile.getStaffItemListNotConst();

   // Get the mapping of systems/staves into page staves for the page.
   vectorVI& sysmap = infile.reverseSystemMap();

   // Now iterate through each system, and then each system-staff and
   // set the part number based on the system-staff (flipped and starting
   // at 1 rather than 0) for staff items on the page.
   int i, j, k, ksize;

cout << "SYSMAP SIZE = " << sysmap.size() << endl;
   for (i=0; i<(int)sysmap.size(); i++) {
      for (j=0; j<(int)sysmap[i].size(); j++) {
         ksize = staffItems[sysmap[i][j]].size();
         for (k=0; k<ksize; k++) {
            if (staffItems[sysmap[i][j]][k]->getP9() != 0) {
               // don't reassign part numbers if they are already present.
               continue;
            }
            staffItems[sysmap[i][j]][k]->setP9N(sysmap[i].size() - j);
         }
      }
   }
}



