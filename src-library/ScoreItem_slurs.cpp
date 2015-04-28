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



