//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Apr 25 17:32:03 PDT 2014
// Last Modified: Fri Apr 25 17:32:05 PDT 2014
// Filename:      ScoreItem_rests.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_rests.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=2 (rests) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::isInvisible -- true if invisible.  Set up for rests right now,
//     but can be extended to P1=1 (notes), although notes are not commonly
//     made invisible.  Could also be applied to _99 font text and other
//     things which are "invisible" on the page.
//

int ScoreItem::isInvisible(void) {
   if (isRestItem()) {
      getP6Int() < 0.0 ? 1 : 0;
   } else if (isStaffItem()) {
      getP7() < 0.0 ? 1: 0;
   } else if (isBarlineItem()) {
      getP4() < 0.0 ? 1: 0;
   }
   return 0;
}



//////////////////////////////
//
// ScoreItem::isNoteOrRestItem --
//

bool ScoreItem::isNoteOrRestItem(void) {
   if (isNoteItem() || isRestItem()) {
      return true;
   }
   return false;
}



