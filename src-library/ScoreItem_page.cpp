//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 20:01:06 PDT 2014
// Last Modified: Mon Mar 10 20:01:08 PDT 2014
// Filename:      ScoreItem_notes.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_notes.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=1 (notes) items.
//

#include "ScoreItem.h"
#include "ScorePage.h"
#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getPageSystemIndex -- return the system index that the
//      note belongs to.  Returns -1 if not attached to a page.
//

int ScoreItem::getPageSystemIndex(void) {
   int p2 = getStaffNumber();
   if (page_owner == NULL) {
      return -1;
   }
   return ((ScorePage*)page_owner)->getSystemIndex(p2);
}

//
// Alias:
//

int ScoreItem::getSystemIndex(void) {
   return getPageSystemIndex();
}



///////////////////////////////
//
// ScoreItem::getHPosOff -- Return the horizontal position of the
//    time of the note-off for the note/rest.
//

SCORE_FLOAT ScoreItem::getHPosOff(void) {
   if (!hasDuration()) {
      // invalid, returning negative position.
      return -1;
   }
   if (page_owner == NULL) {
      // no information about page available.
      return -1;
   }

   // Probably handle grace notes differently: go to next duration
   // item on staff.
   SCORE_FLOAT offset = getStaffOffsetDuration() + getDuration();
   int sys = getPageSystemIndex();
   return ((ScorePage*)page_owner)->getP3OfStaffDurationOffset(sys, offset);
}



//////////////////////////////
//
// ScoreItem::getPartIndex --
//

int ScoreItem::getPartIndex(void) {
   int p2 = getStaffNumber();
   if (page_owner == NULL) {
      return -1;
   }
   return ((ScorePage*)page_owner)->getPartIndex(p2);
}




//////////////////////////////
//
// ScoreItem::getStaffOwner --
//

ScoreItem* ScoreItem::getStaffOwner(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   int p2 = getStaffNumber();
   vectorVSIp& staves = ((ScorePage*)page_owner)->getStaffItemListNotConst();
   if ((int)staves.size() > p2) {
      if ((int)staves[p2].size() > 0) {
         return staves[p2][0];
      }
   }
   return NULL;
}



//////////////////////////////
//
// ScoreItem::getStaffScale --
//

SCORE_FLOAT ScoreItem::getStaffScale(void) {
   ScoreItem* staff = getStaffOwner();
   if (staff == NULL) {
      return 1.0;
   } else {
      return staff->getScale();
   }
}



//////////////////////////////
//
// ScoreItem::getStaffVerticalOffset --
//

SCORE_FLOAT ScoreItem::getStaffVerticalOffset(void) {
   ScoreItem* staff = getStaffOwner();
   if (staff == NULL) {
      return 0.0;
   } else {
      return staff->getVPos();
   }
}



//////////////////////////////
//
// ScoreItem::getPageIndex -- return the page index within a
//    ScorePage set for which this staff belongs.  Returns -1
//    if not within a ScorePageSet object.
//

int ScoreItem::getPageIndex(void) {
   ScorePage* page = getPageOwner();
   if (page == NULL) {
      return -1;
   }
   ScorePageSet* pageset = page->getSetOwner();
   if (pageset == NULL) {
      return -1;
   }
   ScorePageSet& pset = *pageset;
   int pagecount = pset.getPageCount();
   int i, j;
   for (i=0; i<pagecount; i++) {
      for (j=0; j<pset[i].size(); j++) {
         if (page == &(pset[i][j])) {
            return i;
         }
      }
   }
   return -1;
}




//////////////////////////////
//
// ScoreItem::setPageOwner -- set a pointer to the page which
//   owns this SCORE item (the page which manages the memory allocation
//   for the item.)
//

void ScoreItem::setPageOwner(void* page) {
   page_owner = (void*)page;
}



//////////////////////////////
//
// ScoreItem::getPageOwner --
//

ScorePage* ScoreItem::getPageOwner(void) {
   return (ScorePage*)page_owner;
}



