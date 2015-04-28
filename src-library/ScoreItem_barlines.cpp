//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 00:04:22 PST 2014
// Last Modified: Thu Mar  6 00:04:27 PST 2014
// Filename:      ScoreItem_barlines.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_barlines.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=14 (barline) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getBarlineHeight --  The number of staves that a barline
//    connect to is stored in P4.  If P4 is negative, then the barline
//    is invisible.  This function currently does not allow the P4 value
//    to be larger than 99 (or less than -99).  This is because SCORE
//    limits the number of staves on the page (MS-DOS versions of SCORE
//    limit the staff count for a page to 32).  Returns 0 if the item
//    is not a barline.
//

int ScoreItem::getBarlineHeight(void) {
   if (!isBarlineItem()) {
      return 0;
   }
   int p4 = abs(getParameterInt(P4));
   if (p4 == 0) {
      p4 = 1;
   }
   p4 = p4 % 100;
   return p4;
}


//
// Aliases:
//

int ScoreItem::getBarHeight(void) {
   return getBarlineHeight();
}


int ScoreItem::getHeight(void) {
   return getBarlineHeight();
}




