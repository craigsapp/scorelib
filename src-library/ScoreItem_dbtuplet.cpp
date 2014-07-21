//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 20:01:06 PDT 2014
// Last Modified: Mon Mar 10 20:01:08 PDT 2014
// Filename:      ScoreItem_dbtuplet.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_dbtuplet.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=1 (notes) items.
//

#include "ScoreItem.h"
#include "ScorePage.h"
#include "ScorePageSet.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// Functions related to DatabaseTuplet of the owning page.
//

//////////////////////////////
//
// ScoreItem::getTupletGroup -- returns a TupletGroup object which contains
//     two vectors (tuplets, notes).  If the ScoreItem is not involved in
//     a tuplet group, then a NULL pointer will be returned.
//

TupletGroup*  ScoreItem::getTupletGroup(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   return ((ScorePage*)page_owner)->tupletInfo(this);
}

//
// Aliases:
//

TupletGroup* ScoreItem::getTupletInfo(void) {
   return getTupletGroup();
}
   

TupletGroup* ScoreItem::tupletInfo(void) {
   return getTupletGroup();
}
   

TupletGroup* ScoreItem::tupletGroup(void) {
   return getTupletGroup();
}



//////////////////////////////
//
// ScoreItem::inTupletGroup -- Returns true if the item is within a 
//     tuplet group.
//

int ScoreItem::inTupletGroup(void) {
   if (getTupletGroup()) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// ScoreItem::tupletGroupHasBracket -- Returns true if the tuplet group
//     contains a bracket (as opposed to a beam, or nothing).
//

bool ScoreItem::tupletGroupHasBracket(void) {
   TupletGroup* tg = getTupletGroup();
   if (tg == NULL) {
      return false;
   }
   if (tg->brackets.size() == 0) {
      return false;
   }
   if (tg->brackets[0]->isTupletBracket()) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::isFirstNoteInTuplet --
//

bool ScoreItem::isFirstNoteInTupletGroup(void) {
   TupletGroup* tg = getTupletGroup();
   if (tg == NULL) {
      return false;
   }
   if (tg->notes.size() == 0) {
      return false;
   }
   if (this == tg->notes[0]) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::isLastNoteInTuplet --
//

bool ScoreItem::isLastNoteInTupletGroup(void) {
   TupletGroup* tg = getTupletGroup();
   if (tg == NULL) {
      return false;
   }
   if (tg->notes.size() == 0) {
      return false;
   }
   if (this == tg->notes.back()) {
      return true;
   }
   return false;
}



