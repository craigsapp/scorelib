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
   int p4          = getPInt(P4);
   int fractens    = (int)(cleffrac*10.0 + 0.5);
   if (cleffrac == 0.8) {
      clef8 = 1;
   }

   int output = 1;   // default for treble clef
   switch (cleftype) {
      case 0:                 // G clef
         output += p4;
         if (clef8) {         // vocal tenor clef
            output -= 7;
         }
         break;
      case 1:                 // F clef
         output = 13 + p4;
         break;
      case 2:                 // Alto C-clef
         output = 7 + p4;
         break;
      case 3:                 // Tenor C-clef
         output = 9 + p4;
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




