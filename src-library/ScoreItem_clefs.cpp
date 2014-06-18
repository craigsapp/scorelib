//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_clefs.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_clefs.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=3 (clef) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getMiddleCVpos -- return the vertical position of middle C
//    given a particular clef.  For treble clef, middle C is at vpos 1; for
//    bass clef middle C is at vpos 13, etc.  Returns 0 if the item is
//    not a clef.
//

int ScoreItem::getMiddleCVpos(void) {
   if (!isClefItem()) {
      return 0;
   }

   int cleftype    = getPInt(P5);
   int clef8       = 0;
   double cleffrac = getParameterFraction(P5);
   int vpos        = getVPos();
   int fractens    = (int)(cleffrac*10.0 + 0.5);
   if (cleffrac == 0.8) {
      clef8 = 1;
   }

   int output = 1;   // default for treble clef
   switch (cleftype) {
      case 0:                 // G clef
         output += vpos;
         if (clef8) {         // vocal tenor clef
            output -= 7;
         }
         break;
      case 1:                 // F clef
         output = 13 + vpos;
         break;
      case 2:                 // Alto C-clef
         output = 7 + vpos;
         break;
      case 3:                 // Tenor C-clef
         output = 9 + vpos;
         break;
   }

   // Deal with soprano clef, mezzo-soprano clef, baritone clef, etc. here.

   // Handle clefs (such as vocal tenor clef) which transpose
   // their music one octave lower (do other octave transposings as well):
   if (fractens == 8) {
      output += 7;
   }
   return output;
}



///////////////////////////////
//
// ScoreItem::getStaffLine -- 1=bottom line of staff, 5=top line 
//     of 5-lined staff.  Will be confused if clef if placed
//     on a staff space...
//

int ScoreItem::getStaffLine(void) {
   if (!isClefItem()) {
      return 0;
   }

   int line = 0;
   int p5 = getP5Int();
   switch (p5) {
      case 0: line = 2; break;  // treble clef
      case 1: line = 4; break;  // bass clef
      case 2: line = 3; break;  // alto clef
      case 3: line = 4; break;  // tenor clef
      case 4: line = 3; break;  // percussion clef
   }
   int p4 = int(getVerticalPosition() + 0.5);
   return line + int(p4/2);
}



///////////////////////////////
//
// ScoreItem::getClefLetter -- C for C clef, F for F clef, G for G clef.
//    X for percussion clef, and ? for other clefs.
//

char ScoreItem::getClefLetter(void) { 
   if (!isClefItem()) {
      return '?';
   }

   int p5 = getP5Int();
   switch (p5) {
      case 0: return 'G';         // treble clef
      case 1: return 'F';         // bass clef
      case 2: return 'C';         // alto clef
      case 3: return 'C';         // tenor clef
      case 4: return 'X';         // percussion clef
   }
   return '?';
}



///////////////////////////////
//
// ScoreItem::soundsOctaveDown --
//

bool ScoreItem::soundsOctaveDown(void) { 
   if (!isClefItem()) {
      return false;
   }
   SCORE_FLOAT tolerance = 0.001;
   SCORE_FLOAT p5fraction = getParameterFraction(P5);
   if (fabs(p5fraction - 0.8) < tolerance) {
      return true;
   }
   return false;
}



