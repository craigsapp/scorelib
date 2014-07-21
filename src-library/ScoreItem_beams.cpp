//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_beams.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_beams.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=6 (beam) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::isTupletBeam -- returns true if the beam has a tuplet
//     number in P8.
//

bool ScoreItem::isTupletBeam(void) {
   if (!isBeamItem()) {
      return false;
   }
   int p8 = getP8Int();
   if (p8 > 0) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::getTupletNumber -- returns tuplet number of beam or slur item.
//     For beams number is in P8.
//

int ScoreItem::getTupletNumber(void) {
   int p8 = getP8Int();
   if (isBeamItem()) {
      if (p8 > 0) {
         return P8;
      }
      return 0;
   } else if (isSlurItem()) {
      if (p8 != 3) {
         return 0;
      }
      int p9 = getP9Int();
      if (p9 == 0) {
         return 3;
      } else if (p9 > 0) {
         return p9;
      }
   } else {
      return 0;
   }

   if (p8 > 0) {
      return true;
   }
   return false;
}



