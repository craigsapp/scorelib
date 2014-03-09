//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Wed Mar  5 22:40:45 PST 2014
// Filename:      ScorePage_pitch.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_pitch.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff pitch analysis.
//

#include "ScorePage.h"
#include <algorithm>
#include <set>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// Staff pitch analysis related functions
//

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
   int haseditorial, hasnatural, sysstaff;


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
      base40        = base7ToBase40(diatonic);
      accidental    = curr->getPrintedAccidental();
      hasnatural    = curr->hasNatural();
      haseditorial  = curr->hasEditorialAccidental();
      if ((accidental == 0) && (hasnatural == 0))  {
         accidental = pitchstate[sysstaff][diatonic];
      }
      if (!haseditorial) {
         printedaccidental = curr->getPrintedAccidental();

         // if the printed accidental matches the pitch state,
         // then mark the note item as possessing a cautionary accidental
         if ((printedaccidental == 1) &&
               (pitchstate[sysstaff][diatonic] == -1)) {
            curr->setParameter("cautionaryAccidental", "true");
         } else if ((printedaccidental == 2) &&
               (pitchstate[sysstaff][diatonic] == +1)) {
            curr->setParameter("cautionaryAccidental", "true");
         } else if ((printedaccidental == 3) &&
               (pitchstate[sysstaff][diatonic] == 0)) {
            curr->setParameter("cautionaryAccidental", "true");
         } else if ((printedaccidental == 4) &&
               (pitchstate[sysstaff][diatonic] == -2)) {
            curr->setParameter("cautionaryAccidental", "true");
         } else if ((printedaccidental == 5) &&
               (pitchstate[sysstaff][diatonic] == +2)) {
            curr->setParameter("cautionaryAccidental", "true");
         }
         pitchstate[sysstaff][diatonic] = accidental;
      }
      base40 += accidental;
      base40string  = to_string(base40);
      base40string += "\t(";
      base40string += base40ToKern(base40);
      base40string += ")";

      curr->setParameter("base40Pitch", base40string);
   }

}



///////////////////////////////////////////////////////////////////////////
//
// Static functions:
//

//////////////////////////////
//
// ScorePage::base7ToBase40 -- convert diatonic Base-7 note to Base-40 pitch.
//

int ScorePage::base7ToBase40(int base7) {
   if (base7 <= 0) {
      return -1;
   }
   int octave = base7 / 7;
   int chroma = base7 % 7;
   int output = 0;
   switch (chroma) {
      case 0:  output =  0;  break;   // 0 = C -> 0
      case 1:  output =  6;  break;   // 1 = D -> 6
      case 2:  output = 12;  break;   // 2 = E -> 12
      case 3:  output = 17;  break;   // 3 = F -> 17
      case 4:  output = 23;  break;   // 4 = G -> 23
      case 5:  output = 29;  break;   // 5 = A -> 29
      case 6:  output = 35;  break;   // 6 = B -> 35
      default: output = 0;
   }

   // the offset by two so that division gives octave:
   return output + 40 * octave + 2;
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



//////////////////////////////
//
// ScorePage::base40ToKern --  Convert a base-40 pitch number into
//     a Humdrum **kern pitch name.
//

string ScorePage::base40ToKern(int base40) {
   string chromastring;

   int octave = base40 / 40;
   if (octave > 12 || octave < -1) {
      cerr << "Error: unreasonable octave value: " << octave << endl;
      exit(1);
   }
   int chroma = base40 % 40;

   switch (chroma) {
      case  0: chromastring = "c--"; break;
      case  1: chromastring = "c-";  break;
      case  2: chromastring = "c";   break;
      case  3: chromastring = "c#";  break;
      case  4: chromastring = "c##"; break;
      //    5: not used
      case  6: chromastring = "d--"; break;
      case  7: chromastring = "d-";  break;
      case  8: chromastring = "d";   break;
      case  9: chromastring = "d#";  break;
      case 10: chromastring = "d##"; break;
      //   11: not used
      case 12: chromastring = "e--"; break;
      case 13: chromastring = "e-";  break;
      case 14: chromastring = "e";   break;
      case 15: chromastring = "e#";  break;
      case 16: chromastring = "e##"; break;
      case 17: chromastring = "f--"; break;
      case 18: chromastring = "f-";  break;
      case 19: chromastring = "f";   break;
      case 20: chromastring = "f#";  break;
      case 21: chromastring = "f##"; break;
      //   22: not used
      case 23: chromastring = "g--"; break;
      case 24: chromastring = "g-";  break;
      case 25: chromastring = "g";   break;
      case 26: chromastring = "g#";  break;
      case 27: chromastring = "g##"; break;
      //   28: not used
      case 29: chromastring = "a--"; break;
      case 30: chromastring = "a-";  break;
      case 31: chromastring = "a";   break;
      case 32: chromastring = "a#";  break;
      case 33: chromastring = "a##"; break;
      //   34: not used
      case 35: chromastring = "b--"; break;
      case 36: chromastring = "b-";  break;
      case 37: chromastring = "b";   break;
      case 38: chromastring = "b#";  break;
      case 39: chromastring = "b##"; break;
      default: chromastring = "X";
   }
   
   if (octave >= 4) {
      chromastring[0] = std::tolower(chromastring[0]);
   } else {
      chromastring[0] = std::toupper(chromastring[0]);
   }
   int repeat = 0;
   switch (octave) {
      case 4:  repeat = 0; break;
      case 5:  repeat = 1; break;
      case 6:  repeat = 2; break;
      case 7:  repeat = 3; break;
      case 8:  repeat = 4; break;
      case 9:  repeat = 5; break;
      case 3:  repeat = 0; break;
      case 2:  repeat = 1; break;
      case 1:  repeat = 2; break;
      case 0:  repeat = 3; break;
      case -1: repeat = 4; break;
      default:
         cerr << "Error: unknown octave value: " << octave << endl;
         cerr << "for base-40 pitch: " << base40 << endl;
         exit(1);
   }

   string output;
   for (int i=0; i<repeat; i++) {
      output += chromastring[0];
   }
   output += chromastring;

   return output;
}



