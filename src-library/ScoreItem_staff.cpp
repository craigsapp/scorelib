//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_staff.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_staff.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=3 (clef) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getPartNumber -- Return the part number of the staff.
//

double ScoreItem::getPartNumber(void) {
   return getParameter(P9);
}


//
// Alias, integer part only.
//


int ScoreItem::getPartNumberInt(void) {
   return (int)getPartNumber();
}


//
// Aliases
//


double ScoreItem::getInstrumentNumber(void) {
   return getPartNumber();
}


int ScoreItem::getInstrumentNumberInt(void) {
   return (int)getPartNumber();
}



//////////////////////////////
//
// ScoreItem::getScale -- Return the scaling factor for staves and text.
//     Text items also have a horizontal scaling factor which this function
//     does not return (only vertical size for text).
//

SCORE_FLOAT ScoreItem::getScale(void) {
   int p1 = getP1Int();
   SCORE_FLOAT output = 0.0;
   SCORE_FLOAT tolerance = 0.001;
   switch (p1) {
      case P1_Staff:
         output = fabs(getP5());
         if (output < tolerance) {
            output = 1.0;
         }
         break;

      case P1_Text:
         output = fabs(getP6());
         if (output < tolerance) {
            output = 1.0;
         }
         break;
   }

   return output;
}



