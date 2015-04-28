//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Fri Feb 28 23:47:03 PST 2014
// Filename:      ScorePage_staff.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_staff.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff analysis.
//

#include "ScorePage.h"
#include <algorithm>

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeStaves -- Calculate basic information about staves
//    on the page.  Analysis info is stored in the staff_info varaible.
//    This analysis function is dependent on the "sorted" analysis to
//    be valid before it can start.
//

void ScorePage::analyzeStaves(void) {
   if (!analysis_info.sortedIsValid()) {
      sortPageHorizontally();
   }
   analysis_info.setInvalid("staves");

   int staffnum;
   int itemtype;

   // don't clear system here...
   staff_info.clear();

   for (auto& it : item_storage) {
      itemtype = it->getItemType();
      if (itemtype != P1_Staff) {
         continue;
      }
      staffnum = it->getStaffNumber();

      if ((staffnum < 0) || (staffnum >= (int)MAX_STAFF_COUNT)) {
         continue;
      }
      if (staff_info.getStaffItemsSize() < staffnum + 1) {
         staff_info.setStaffItemsSize(staffnum+1);
      }
      staff_info.appendStaffItem(staffnum, it);
   }

   analysis_info.setValid("staves");

   fillStaffScoreItemLists();

}

//
// Functions dependent on the analyzeStaves function:
//


//////////////////////////////
//
// ScorePage::fillStaffScoreItemLists --
//

void ScorePage::fillStaffScoreItemLists(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }

   vectorVSIp& stafflist = itemlist_staffsorted;
   vectorSIp& itemlist = itemlist_P3sorted;
   int maxstaff = getMaxStaff();

   stafflist.resize(maxstaff+1);
   for (int i=0; i<=maxstaff; i++) {
      stafflist[i].reserve(itemlist.size());
      stafflist[i].resize(0);
   }


   int p2;
   for (auto& it : itemlist) {
      p2 = it->getStaffNumber();
      stafflist[p2].push_back(it);
   }
}



//////////////////////////////
//
// ScorePage::getMaxStaff --  Returns the highest staff number of
//    all P1=8 objects on the page.  Any staff number larger than
//    MAX_STAFF_COUNT will be ignored.
//

int ScorePage::getMaxStaff(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }

   return staff_info.getStaffItemsSize() - 1;
}



//////////////////////////////
//
// ScorePage::getStaffItemList -- Return a list of P1=8 items on the page.
//    The items are in a 2D vector, with the first dimension being the staff
//    number, and the second dimension being a list of the P1=8 staff items
//    that have the same P2 integer value.   Typically there will be only one
//    staff item for every used staff, but more than one is possible (although
//    the P5 values should match since the SCORE editor only considers the
//    case where the is one staff item on a staff position.
//

const vectorVSIp& ScorePage::getStaffItemList(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }

  return staff_info.getStaffItems();
}


vectorVSIp& ScorePage::getStaffItemListNotConst(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }

  return staff_info.getStaffItemsNotConst();
}



//////////////////////////////
//
// ScorePage::staffItems -- return a list of ScoreItems on a particular
// staff, sorted by horizontal position.
//

vectorVSIp& ScorePage::staffItems(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   return itemlist_staffsorted;
}


vectorSIp&  ScorePage::staffItems(int p2index) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }

   return itemlist_staffsorted[p2index];
}



//////////////////////////////
//
// ScorePage::stavesAreConsecutive -- Returns true if all staff positions
//    from 1 to the maximum used staff contains at least one P1=8 staff item.
//    If there are no staff items, then will return true.
//

bool ScorePage::stavesAreConsecutive(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   int i;
   for (i=1; i<staff_info.getStaffItemsSize(); i++) {
      if (staff_info.getStaffItemSize(i) == 0) {
         return false;
      }
   }
   return true;
}



