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



////////////////////////
//
// ScoreItem::getPrintedAccidental -- Returns the accidental information
//     extracted from the 1's digit of P5 for notes:
//     0 =  0 (no accidental)
//     1 = -1 (flat)
//     2 = +2 (sharp)
//     3 =  0 (natural)
//     4 = -2 (double flat)
//     5 = +2 (double sharp)
//

int ScoreItem::getPrintedAccidental(void) {
   if (!isNoteItem()) {
      return 0;
   }
   int ones = getParameterDigit(P5, 0);
   switch (ones) {
      case 0:  return  0; // no printed accidental, but sounding accidental
                          //    will depend on larger musical context
      case 1:  return -1; // flat sign
      case 2:  return +1; // sharp sign
      case 3:  return  0; // natural sign
      case 4:  return -2; // double flat sign
      case 5:  return +2; // double sharp sign
      default: return  0; // unknown
   }
}



//////////////////////////////
//
// ScoreItem::hasNatural -- Returns true if there is a natural
//    sign on the note.  The natural sign may be displayed as an
//    editorial marking.
//

int ScoreItem::hasNatural(void) {
   if (!isNoteItem()) {
      return 0;
   }

   // first check the editorial accidentals:
   int artic = getPInt(P11);
   switch (artic) {
      case 1: return 0;   // editorial flat
      case 2: return 0;   // editorial sharp
      case 3: return 1;   // editorial natural
   }

   // then check regular accidental:
   int ones = getParameterDigit(P5, 0);
   if (ones == 3) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// ScoreItem::hasEditorialAccidental -- True if there is an editorial 
//     accidental.  The editorial accidental is stored in P11 for 
//     articulations.
//     P11 == 1 = flat
//            2 = sharp
//            3 = natural
//

int ScoreItem::hasEditorialAccidental(void) {
   if (!isNoteItem()) {
      return 0;
   }
   int artic = getPInt(P11);
   switch (artic) {
      case 1:  return 1;
      case 2:  return 2;
      case 3:  return 3;
   }
   return 0;
}




