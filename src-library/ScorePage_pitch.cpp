//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Mon Mar 10 23:58:07 PDT 2014
// Filename:      ScorePage_pitch.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_pitch.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff pitch analysis.
//

#include "ScorePage.h"
#include "ScoreUtility.h"
#include "ScoreDefs.h"
#include <algorithm>
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePage::analyzePitch -- Determine the pitch of every note.  However
//    second tied notes will be incorrect when the ties cross over a barline.
//    These tied notes must be analyzed after ties and slurs have been
//    distinguished.  The input to analyzePitch assumes that only the
//    data for one system is being provided, and it is sorted by horizontal
//    position.
//

void ScorePage::analyzePitch(void) {
   if (!analysis_info.systemPitchesIsValid()) {
      analyzeSystems();
   }
   analysis_info.invalidate("systempitches");

   vectorSIp sysseq;
   int i;
   for (i=0; i<getSystemCount(); i++) {
      getHorizontallySortedSystemItems(sysseq, i);
      analyzeSystemPitch(sysseq);
   }

   analysis_info.validate("systempitches");
}



///////////////////////////////
//
// ScorePage::analyzeSystemPitch --  calculate the pitch of all staves
//   on a system at the same time.  The pitch needs to be calculated by
//   system in the most general sense of cross-staff beaming; otherwise,
//   analyzePitchOnStaff should work.
//

void ScorePage::analyzeSystemPitch(vectorSIp& systemitems) {
   if (!analysis_info.systemPitchesIsValid()) {
      analyzeSystems();
   }

   // middleCVpos -- the vertical position of middle C on the staff.  This
   //    is determined by the clef.  For Treble clef, the vertical position
   //    of middle C is 1.  For bass clef, middle C is at 13 (ledger line
   //    above staff.  For alto clef, middle C is at 7 (middle line on staff).
   vectorI  middleCVpos(MAX_STAFF_COUNT, 0);

   // keysig is the current key signature on the staff.  When a new key
   // is encountered, this variable is updated.  Each staff has a different
   // key signature state.
   vectorVI keysig(MAX_STAFF_COUNT, vectorI(7, 0));
 
   // pitchstate keeps track of the current chormatic state of each 
   // diatonic pitch on each staff in the system.  These values are
   // reset after each barline.
   vectorVI pitchstate(MAX_STAFF_COUNT, vectorI(70, 0)); 

   int vpos, base40, diatonic, accidental, printedaccidental;
   int haseditorial, sysstaff;


   // Assume treble clef if no clef given:
   fill(middleCVpos.begin(), middleCVpos.end(), 1);
   ScoreItem* curr;
   int p2;
   string base40string;

   for (unsigned int i=0; i<systemitems.size(); i++) {
      curr = systemitems[i];
      p2 = curr->getStaffNumber();
      sysstaff = getSystemStaffIndex(p2);
      if (curr->isClefItem()) {
         middleCVpos[sysstaff] = curr->getMiddleCVpos();
         continue;
      } else if (curr->isKeySignatureItem()) {
         curr->getDiatonicAccidentalState(keysig[sysstaff]);
         fillPitchStatesWithKeySig(pitchstate, sysstaff, keysig);
         continue;
      } else if (curr->isBarlineItem()) {
         int length = curr->getPInt(P4);
         resetPitchSpellings(sysstaff, length, pitchstate, keysig);
         // Deal with tied notes tied across barlines later...
         continue;
      }
      if (!curr->isNoteItem()) {
         continue;
      }

      vpos          = curr->getPInt(P4);  // deal with +/- rounding?
      diatonic      = (vpos%100) - middleCVpos[sysstaff] + 7*4;
      base40        = SU::base7ToBase40(diatonic);
      accidental    = curr->getPrintedAccidental();
      haseditorial  = curr->hasEditorialAccidental();
      if (!curr->hasPrintedAccidental()) {
         accidental = pitchstate[sysstaff][diatonic];
      }
      if (!haseditorial) {
         printedaccidental = curr->getPrintedAccidental();

         // if the printed accidental matches the pitch state,
         // then mark the note item as possessing a cautionary accidental
         if (printedaccidental == pitchstate[sysstaff][diatonic]) {
            curr->setParameter(ns_auto, np_courtesy, "true");
         }
         if (abs(printedaccidental) < 100) {
            // extrema range means no accidental on the note.
            pitchstate[sysstaff][diatonic] = accidental;
         }
      }
      base40 += accidental;
      base40string  = to_string(base40);

      // Add a textual pitch name for readability:
      base40string += "\t(";
      base40string += SU::base40ToKern(base40);
      base40string += ")";

      curr->setParameter(ns_auto, np_base40Pitch, base40string);
   }

}



///////////////////////////////////////////////////////////////////////////
//
// Private functions:
//

//////////////////////////////
//
// ScorePage::resetPitchSpellings --  Reset diatonic pitch states after
//    a barline has been encountered.
//

void ScorePage::resetPitchSpellings(int staffidx, int barheight, 
      vectorVI& pitchstate, vectorVI& keysig) {
   if (barheight == 0) {
      barheight = 1;
   }

   int i, j;
   for (i=0; i<barheight; i++) {
      for (j=0; j<70; j++) {
         pitchstate[staffidx+i][j] = keysig[staffidx+i][j%7];
      }
   }
}



//////////////////////////////
//
// ScorePage::fillPitchStatesWithKeySig -- Note: ignoring a keysignature in the
//    middle of a measure for now.   Sets all diatonic pitches to have
//    spellings from the keysignature.
//

void ScorePage::fillPitchStatesWithKeySig(vectorVI& pitchstate, int staffidx,
      vectorVI& keysig) {
   int i, ii;
   for (i=0; i<70; i++) {
      ii = i % 7;
      pitchstate[staffidx][i] = keysig[staffidx][ii];
   }
}



