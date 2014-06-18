//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_keysigs.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_keysigs.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=17 
//       (key signature) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getDiatonicAccidentalState -- returns an array of seven integers 
//     representing the accidental state of each diatonic pitch in 
//     this order:  C, D, E, F, G, A, B.
//
//     Return value is the key signature count, such as -4 for four flats.
//
//     Input parameter states is an array of 7 numbers, one for each
//     diatonic pitch class.
//

int ScoreItem::getDiatonicAccidentalState(vectorI& states) {
   if (!isKeySignatureItem()) {
      return -1000;
   }

   fill(states.begin(), states.end(), 0);

   int keyval = getAccidentalCount();

   // If 100's digit of P5 is 1, then the key signature
   // is a cancellation signature, with all values natural
   if ((abs(keyval) >= 100) && (abs(keyval) < 200)) {
      keyval = 0;
   }

   if (keyval == 0) {
      // nothing else left to do
      return keyval;
   }

   if (keyval < 0) {
      if (keyval <= -1) { states[6] = -1; } // B-flat
      if (keyval <= -2) { states[2] = -1; } // E-flat
      if (keyval <= -3) { states[5] = -1; } // A-flat
      if (keyval <= -4) { states[1] = -1; } // D-flat
      if (keyval <= -5) { states[4] = -1; } // G-flat
      if (keyval <= -6) { states[0] = -1; } // C-flat
      if (keyval <= -7) { states[3] = -1; } // F-flat
   } else if (keyval > 0) {
      if (keyval >= +1) { states[3] = +1; } // F-sharp
      if (keyval >= +2) { states[0] = +1; } // C-sharp
      if (keyval >= +3) { states[4] = +1; } // G-sharp
      if (keyval >= +4) { states[1] = +1; } // D-sharp
      if (keyval >= +5) { states[5] = +1; } // A-sharp
      if (keyval >= +6) { states[2] = +1; } // E-sharp
      if (keyval >= +7) { states[6] = +1; } // B-sharp
   }

   return keyval;
}



//////////////////////////////
//
// ScoreItem::getAccidentalCount --  Return the number of accidentals
//    in the key signature.  -1 == one flat, 2 == two sharps, 
//    0 == no sharps/flats.
//

int ScoreItem::getAccidentalCount(void) {
   if (!isKeySignatureItem()) {
      return 0;
   }
   int p5 = getP5Int();
   if ((p5 > 7) || (p5 < -7)) {
      return 0;
   }
   return p5;
}



