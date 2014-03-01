//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Fri Feb 28 23:43:13 PST 2014
// Filename:      ScorePage_data.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_data.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                data access from the protected variable item_storage.
//

#include "ScorePage.h"
#include <algorithm>

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// Functions that return data lists in various formats.
//

//////////////////////////////
//
// ScorePage::getHorizontallySortedStaffItems --
//

void  ScorePage::getHorizontallySortedStaffItems(vectorVSIp& staffsequence) {
   int staffcount = getMaxStaff();
   staffsequence.resize(staffcount+1);
   vectorVSIp::iterator it;
   for (it = staffsequence.begin(); it != staffsequence.end(); it++) {
      it->reserve(32);
   }

   listSIp::iterator it2;
   for (it2 = item_storage.begin(); it2 != item_storage.end(); it2++) {
      int staffnum = (*it2)->getStaffNumber();
      if (staffnum <= 0) {
         continue;
      }
      if (staffsequence.size() < (unsigned int)(staffnum + 1)) {
         staffsequence.resize(staffnum+1);
      }
      staffsequence[staffnum].push_back(*it2);
   }

   for (it = staffsequence.begin(); it != staffsequence.end(); it++) {
      sort(it->begin(), it->end(), sortP3P1P4);
    }
}



//////////////////////////////
//
// ScorePage::getUnsortedStaffItems -- returns all items on a page
//    attached to the given staff number (P2).
//

void ScorePage::getUnsortedStaffItems(int staffnum, vectorSIp& items) {
   items.clear();
   listSIp::iterator it;
   int num;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      num = (*it)->getStaffNumber();
      if (num != staffnum) {
         continue;
      }
      items.push_back(*it);
   }
}



//////////////////////////////
//
// ScorePage::getSortedStaffItems -- returns all items on a page
//    attached to the given staff number (P2).
//

void ScorePage::getSortedStaffItems(int staffnum, vectorSIp& items) {
   getUnsortedStaffItems(staffnum, items);
   sort(items.begin(), items.end(), sortP3P1P4);
}



