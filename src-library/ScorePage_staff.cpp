//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Fri Feb 28 23:47:03 PST 2014
// Filename:      ScorePage_staff.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_staff.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff analysis.
//


#include "ScorePage.h"
#include <algorithm>

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// Staff analysis related functions
//

//////////////////////////////
//
// ScorePage::analyzeStaves -- Calculate basic information about staves
//    on the page.  Analysis info is stored in the staff_info varaible.
//

void ScorePage::analyzeStaves(void) {
   analysis_info.staves = 0;

   listSIp::iterator it;
   int staffnum;
   int itemtype;
   staff_info.clear();

   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      itemtype = (*it)->getItemType();
      if (itemtype != P1_Staff) {
         continue;
      }
      staffnum = (*it)->getStaffNumber();
      
      if ((staffnum < 0) || (staffnum >= (int)MAX_STAFF_COUNT)) {
         continue;
      }
      if (staff_info.getStaffItemsSize() < staffnum + 1) {
         staff_info.setStaffItemsSize(staffnum+1);
      }
      staff_info.appendStaffItem(staffnum, *it);
   }

   analysis_info.staves = 1;
}

//
// Functions dependent on the analyzeStaves function:
//

//////////////////////////////
//
// ScorePage::getMaxStaff --  Returns the highest staff number of
//    all P1=8 objects on the page.  Any staff number larger than
//    MAX_STAFF_COUNT will be ignored.
//

int ScorePage::getMaxStaff(void) {
   if (analysis_info.staves == 0) {
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
   if (analysis_info.staves == 0) {
      analyzeStaves();
   }

  return staff_info.getStaffItems();
}



//////////////////////////////
//
// ScorePage::stavesAreConsecutive -- Returns true if all staff positions
//    from 1 to the maximum used staff contains at least one P1=8 staff item.
//    If there are no staff items, then will return true.
//

bool ScorePage::stavesAreConsecutive(void) {
   if (analysis_info.staves == 0) {
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



