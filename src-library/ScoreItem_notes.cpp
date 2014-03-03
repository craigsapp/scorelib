//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 01:12:06 PST 2014
// Last Modified: Mon Feb 17 01:12:09 PST 2014
// Filename:      ScoreItem_notes.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_notes.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=1 (notes) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::stemUp --  change the 10's digit of P5 for notes (P1=1) to "1".
//    returns 1 if P5 was changed.
//

int ScoreItem::stemUp(void) {
   if (getPInt(P1) != 1) {
      return 0; // Not a note.
   }
   int tensdigit = getPDigit(P5, 1);
   if (tensdigit != 2) {
      setPDigit(P5, 1, 2);
      return 1;
   }
   return 0;
}



//////////////////////////////
//
// ScoreItem::stemDown --  change the 10's digit of P5 for notes (P1=1) to "2".
//    returns 1 if P5 was changed.
//

int ScoreItem::stemDown(void) {
   if (getPInt(P1) != 1) {
      return 0; // Not a note.
   }
   int tensdigit = getPDigit(P5, 1);
   if (tensdigit != 1) {
      setPDigit(P5, 1, 1);
      return 1;
   }
   return 0;
}



//////////////////////////////
//
// ScoreItem::stemFlip --  change the 10's digit of P5 for notes (P1=1)
//    from "2" to "1" or "1" to "2".  If 10's digit is not "1" or "2", then
//    don't do anything.
//

int ScoreItem::stemFlip(void) {
   if (getPInt(P1) != 1) {
      return 0; // Not a note.
   }
   int tensdigit = getPDigit(P5, 1);
   if (tensdigit == 1) {
      setPDigit(P5, 1, 2);
      return 1;
   } else if (tensdigit == 1) {
      setPDigit(P5, 1, 1);
      return 1;
   }
   return 0;
}



