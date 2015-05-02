//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jul 10 15:56:57 PDT 2014
// Last Modified: Thu Jul 10 15:56:59 PDT 2014
// Filename:      ScoreItem_slurs.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_slurs.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=5 (slur/tie) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::isTie -- Returns true if the slur/tie item is a slur
//      between two notes.  Slur analysis needs to be done first.
//      At the moment not checking automatically to see if it has been done.
//

bool ScoreItem::isTie(void) {
   if (!isSlurItem()) {
      return false;
   }
   if (getParameterBool(ns_auto, np_tie)) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isTupletBracket -- Returns true if a slur item and
//   P8==+3.
//

bool ScoreItem::isTupletBracket(void) {
   if (!isSlurItem()) {
      return false;
   }
   int p8 = getP8Int();
   if (p8 == 3) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::getVisualLeftPosition -- Return the visual
//    left position of the slur, which is the P3 value plus
//    an offset related to P8.  There are also other offset
//    methods which need to be added later (fractional P8, and
//    for when P16=1).  The visual left position will also
//    be affected by P20 of slur (which should match the P20
//    of the note it is attached to)--This is a WinScore feature
//    and not present in MS-DOS SCORE.
//

SCORE_FLOAT ScoreItem::getVisualLeftPosition(void) {
   SCORE_FLOAT p3 = getP3();
   SCORE_FLOAT scale;  // staff scaling for slur
   switch (getP8Int()) {
      case 0: // left end of slur is left-justified on note, so just return p3
      case -8:
      case 9:
         return p3;
         break;
      case -1: // left end of slur is centered on note
      case -5:
      case -7:
         scale = getStaffScale();
         return p3 + 1.5 * scale;
         break;
      case -2: // left end of slur is 1/2 note length after note
         scale = getStaffScale();
         return p3 + 4.5 * scale;
         break;
      case -3: // left end of slur is 1 note length after note
         scale = getStaffScale();
         return p3 + 6.0 * scale;
         break;
      case -4: // left end of slur is right justified to note
      case -6:
         scale = getStaffScale();
         return p3 + 3.0 * scale;
         break;
      default:
         // deal with special cases later here.  P8=1-9 should be this case.
         return p3;
   }
}



//////////////////////////////
//
// ScoreItem::getVisualRightPosition -- Return the visual
//    right position of the slur, which is the P6 value plus
//    an offset related to P8.  There are also other offset
//    methods which need to be added later (fractional P8, and
//    for when P16=1).  The visual right position will also
//    be affected by P20 of slur (which should match the P20
//    of the note it is attached to)--This is a WinScore feature
//    and not present in MS-DOS SCORE.
//

SCORE_FLOAT  ScoreItem::getVisualRightPosition(void) {
   SCORE_FLOAT p6 = getP6();
   SCORE_FLOAT scale;  // staff scaling for slur
   switch (getP8Int()) {
      case 0: // right end of slur is left-justified on note, so just return p6
      case -2:
      case -3:
      case -7:
         return p6;
         break;
      case -1: // right end of slur is centered on note
      case -6:
      case -8:
         scale = getStaffScale();
         return p6 + 1.5 * scale;
         break;
      case -4: // right end of slur is right justified to note
      case -5:
      case -9:
         scale = getStaffScale();
         return p6 + 3.0 * scale;
         break;
      default:
         // deal with special cases later here.  P8=1-9 should be this case.
         return p6;
   }
}



