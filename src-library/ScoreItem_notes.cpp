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
#include <cmath>

using namespace std;


//////////////////////////////
//
// ScoreItem::hasStem -- Returns 0 if no stem, +1 for stem up and -1 for stem
//     down.  For Notes, the stem direction is located in P5 10's digit.  
//     For Beams, the stem direction is located in P7 10's digit.
//

int ScoreItem::hasStem(void) {
   int type = getItemType();
   if ((type == P1_Note) || (type == P1_Beam)) {
      int stemdir;
      if (type == P1_Note) {
         stemdir = getPDigit(P5, 1); 
      } else {
         stemdir = getPDigit(P7, 1); 
      }
      switch (stemdir) {
         case 1: return +1;
         case 2: return -1;
         case 0: return  0;
      }
      return 0;
   } else {
      return 0;
   }
}

//
// Alias:
//

int ScoreItem::getStemDirection(void) {
   return hasStem();
}



//////////////////////////////
//
// ScoreItem::stemUp --  change the 10's digit of P5 for notes (P1=1) to "1".
//    returns 1 if P5 was changed.
//

int ScoreItem::stemUp(void) {
   if (!isNoteItem()) {
      return 0;
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
   if (!isNoteItem()) {
      return 0;
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
   if (!isNoteItem()) {
      return 0;
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
//     0 = -123456 (no accidental)
//     1 = -1 (flat)
//     2 = +2 (sharp)
//     3 =  0 (natural)
//     4 = -2 (double flat)
//     5 = +2 (double sharp)
//
//     To see editorial accidental use getEditorialAccidental() and hasEditorialAccidental().
//

int ScoreItem::getPrintedAccidental(void) {
   if (!isNoteItem()) {
      return 0;
   }
   int ones = getParameterDigit(P5, 0);
   switch (ones) {
      case 0:  return -123456;  // no printed accidental, but sounding 
                                // accidental will depend on larger 
                                // musical context
      case 1:  return -1;       // flat sign
      case 2:  return +1;       // sharp sign
      case 3:  return  0;       // natural sign
      case 4:  return -2;       // double flat sign
      case 5:  return +2;       // double sharp sign
      default: return  1234556; // unknown
   }
}



//////////////////////////////
//
// ScoreItem::getEditorialAccidental -- 
//         -1 == flat
//          0 == natural
//         +1 == sharp
//    -123456 == no accidental
//

int ScoreItem::getEditorialAccidental(void) {
   int articulation = getP11Int();
   switch (articulation) {
      case 1: return -1;   // editorial flat
      case 2: return +1;   // editorial sharp
      case 3: return 0;    // editorial natural
   }
   return -123456;
}



//////////////////////////////
//
// ScoreItem::hasEditorialAccidental -- Returns true if there is an editorial accidental
//    on the note.
//

bool ScoreItem::hasEditorialAccidental(void) {
   if (!isNoteItem()) {
      return 0;
   }
   int articulation = getP11Int();
   switch (articulation) {
      case 1:   // editorial flat
      case 2:   // editorial sharp
      case 3:   // editorial natural
         return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::hasPrintedAccidental -- returns true if there is a printed
//     accidental attached to the note.
//

bool ScoreItem::hasPrintedAccidental(void) {
   if (abs(getPrintedAccidental()) < 100) {
      return true;
   } else {
      return false;
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
// ScoreItem::getStemBottomVPos -- Get the lowest vertical position
//     of a note's stem.
//

int ScoreItem::getStemBottomVPos(void) { 
   if (!isNoteItem()) {
      return 0;
   }
   int stemdirection = hasStem();
   SCORE_FLOAT vpos  = getVPos();

   if (stemdirection == 0) {
      return vpos;
   }

   SCORE_FLOAT slen  = getStemLength();
 
   if (stemdirection > 0) {
      return vpos;
   } else {
      return vpos - (7.0 + slen);
   }
}



//////////////////////////////
//
// ScoreItem::getStemTopVPos -- Get the highest vertical position of a 
//      note's stem.
//

int ScoreItem::getStemTopVPos(void) { 
   if (!isNoteItem()) {
      return 0;
   }
   int stemdirection = hasStem();
   SCORE_FLOAT vpos  = getVPos();

   if (stemdirection == 0) {
      return vpos;
   }

   SCORE_FLOAT slen  = getStemLength();
 
   if (stemdirection < 0) {
      return vpos;
   } else {
      return vpos + 7.0 + slen;
   }
}



//////////////////////////////
//
// ScoreItem::getStemLength -- Return the length of the stem (P8 value).
//    Not exactly the stem length since the offset is from 7.0, but 
//    close enough since the initial offset is constant. (add 7.0 for 
//    true vertical stem length).
//


int ScoreItem::getStemLength(void) {
   if (!isNoteItem()) {
      return 0;
   }
   return getP(P8);
}



//////////////////////////////
//
// ScoreItem::isCueSize -- Returns true if the note (rest, clef, beam) is a
//     cue-sized item.  For these items this is done by making P4 100's digit 1.
//

int ScoreItem::isCueSize(void) {
   int type = getItemType();
   if (!((type == P1_Note) || (type == P1_Rest) || (type == P1_Beam))) {
      return 0;
   }

   int value = getPDigit(P4, 2);
   if (value == 1) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// ScoreItem::removeArticulation -- Clear P7 if the item is a note (P1==1).
//

void ScoreItem::removeArticulation(void) { 
   if (!isNoteItem()) {
      return;
   }
   setParameter(P11, 0.0);
}



//////////////////////////////
//
// ScoreItem::getArticulation -- return P11 of P1=1 (note) items.
//

int ScoreItem::getArticulation(void) {
   if (!isNoteItem()) {
      return 0;
   }
   return getP11Int(); 
}



//////////////////////////////
//
// ScoreItem::hasFermata -- returns true if note has a fermata.  If negative
//    return value, then fermata is inverted. 
//

int ScoreItem::hasFermata(void) {
   if (!isNoteItem()) {
      return 0;
   }
   int articulation = getArticulation();
   if (abs(articulation) != 14) {
      return 0;
   }
   return articulation;
}



//////////////////////////////
//
// ScoreItem::hideNotehead -- Hide the notehead.  This is currently
//     not reversible.  A named parameter could store the notehead style
//     to make it recoverable.  To hide a notehead, set P6 to 7 or -1.
//

void ScoreItem::hideNotehead(void) { 
   if (!isNoteItem()) {
      return;
   }
   setParameter(P6, 7);
}



//////////////////////////////
//
// ScoreItem::hideStem -- Don't show the stem on a note.  Stem cannot
//      be restored after this function is called since the stem direction
//      is lost.
//

void ScoreItem::hideStem(void) { 
   if (!isNoteItem()) {
      return;
   }
   setParameterDigit(P5, 1, 0);
}



//////////////////////////////
//
// ScoreItem::setNoAccidental --
//
//

void ScoreItem::setNoAccidental(void) {
   if (!isNoteItem()) {
      return;
   }
   setParameterDigit(P5, 0, 0);
}



//////////////////////////////
//
// ScoreItem::setAccidentalParentheses -- Put parentheses around 
//    accidentals.
//
//

void ScoreItem::setAccidentalParentheses(void) {
   if (!isNoteItem()) {
      return;
   }
   setParameterDigit(P5, 2, 1);
}



//////////////////////////////
//
// ScoreItem::removeAccidentalParentheses -- Remove parentheses from around 
//    accidentals.
//
//

void ScoreItem::removeAccidentalParentheses(void) {
   if (!isNoteItem()) {
      return;
   }
   setParameterDigit(P5, 2, 0);
}



//////////////////////////////
//
// ScoreItem::removeFlags -- Remove flags from stem of notes.  The
//    flag count is stored in the 1's digit of P9.
//

void ScoreItem::removeFlags(void) {
   if (!isNoteItem()) {
      return;
   }
   setParameterDigit(P9, 0, 0);
}



//////////////////////////////
//
// ScoreItem::getDotCount -- return the number of augmentation dots.
//     The dot count is stored in the 10's digit of P9 for notes.  For
//     rests, the dot count is a positive value in P6 (-1 in P6 for rests
//     make it invisible).
//

int ScoreItem::getDotCount(void) {
   if (!hasDuration()) {
      return 0;
   }
   if (isNoteItem()) {
      return getParameterDigit(P9, 1);
   } else if (isRestItem()) {
      int p6 = getParameterInt(P6);
      if (p6 < 0)  {
         // Invisible rest. Augmentation dots, if any, are lost.
         return 0;
      } else {
         return p6;
      }
   } 

   return 0;
}



//////////////////////////////
//
// ScoreItem::inTieGroup --
//

bool ScoreItem::inTieGroup(void) { 
   if (!isNoteItem()) {
      return false;
   }
   return getParameterBool(ns_auto, np_tiedLastNote) ||
          getParameterBool(ns_auto, np_tiedNextNote);
}



//////////////////////////////
//
// ScoreItem::isTieGroupStart --
//

bool ScoreItem::isTieGroupStart(void) { 
   if (!isNoteItem()) {
      return false;
   }
   return !getParameterBool(ns_auto, np_tiedLastNote) &&
          getParameterBool(ns_auto, np_tiedNextNote);
}



//////////////////////////////
//
// ScoreItem::isTieGroupMiddle --
//

bool ScoreItem::isTieGroupMiddle(void) { 
   if (!isNoteItem()) {
      return false;
   }
   return getParameterBool(ns_auto, np_tiedLastNote) &&
          getParameterBool(ns_auto, np_tiedNextNote);
}


//////////////////////////////
//
// ScoreItem::isTieGroupEnd --
//

bool ScoreItem::isTieGroupEnd(void) { 
   if (!isNoteItem()) {
      return false;
   }
   return getParameterBool(ns_auto, np_tiedLastNote) &&
          !getParameterBool(ns_auto, np_tiedNextNote);
}





