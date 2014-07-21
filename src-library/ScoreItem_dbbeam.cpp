//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 20:01:06 PDT 2014
// Last Modified: Mon Mar 10 20:01:08 PDT 2014
// Filename:      ScoreItem_dbbeam.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_dbbeam.cpp
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
// Functions related to DatabaseBeam of the owning page.
//

//////////////////////////////
//
// ScoreItem::getBeamGroup -- returns a BeamGroup object which contains
//     two vectors (beams, notes).  If the ScoreItem is not involved in
//     a beam group, then a NULL pointer will be returned.
//

BeamGroup*  ScoreItem::getBeamGroup(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   return ((ScorePage*)page_owner)->beamInfo(this);
}

//
// Aliases:
//

BeamGroup* ScoreItem::getBeamInfo(void) {
   return getBeamGroup();
}
   

BeamGroup* ScoreItem::beamInfo(void) {
   return getBeamGroup();
}
   

BeamGroup* ScoreItem::beamGroup(void) {
   return getBeamGroup();
}
   


//////////////////////////////
//
// ScoreItem::inBeamGroup -- Returns true if the item is within a beam group.
//

int ScoreItem::inBeamGroup(void) {
   if (getBeamGroup()) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// ScoreItem::isFirstNoteInBeam --
//

bool ScoreItem::isFirstNoteInBeamGroup(void) {
   BeamGroup* bg = getBeamGroup();
   if (bg == NULL) {
      return false;
   }
   if (bg->notes.size() == 0) {
      return false;
   }
   if (this == bg->notes[0]) {
      return true;
   }
   return true;
}



