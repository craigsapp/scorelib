//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Fri Feb 28 23:43:13 PST 2014
// Filename:      ScorePage_data.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_data.cpp
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
      sort(it->begin(), it->end(), sortP3);
    }
}



//////////////////////////////
//
// ScorePage::getUnsortedSystemItems --
//

void ScorePage::getUnsortedSystemItems(vectorSIp& sysseq, 
      int sysindex) {
   if (analysis_info.systems == 0) {
      analyzeSystems();
   }

   sysseq.clear();
   if ((sysindex < 0) || (sysindex >= getSystemCount())) {
      return;
   }
   sysseq.reserve(item_storage.size());

   int p2;
   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      p2 = (*it)->getStaffNumber();
      int systemnumber = getSystemIndex(p2);
      if (systemnumber != sysindex) {
         continue;
      }
      sysseq.push_back(*it);
   }
}



//////////////////////////////
//
// ScorePage::getHorizontallySortedSystemItems -- Return a horizontally
//      sorted list of all ScoreItems on the given system.
//      System index 0 is the top system on the page.  Returns
//      an empty list if the system index is invalid.
//      Curently extracts from the complete list for the page.
//      Maybe pre-calculate such data.
//

void ScorePage::getHorizontallySortedSystemItems(vectorSIp& sysseq, 
      int sysindex) {
   getUnsortedSystemItems(sysseq, sysindex);
   sort(sysseq.begin(), sysseq.end(), sortP3);
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

// Alias:

void ScorePage::getUnsortedStaffItems(vectorSIp& items, int staffnum) {
   getUnsortedStaffItems(staffnum, items);
}



//////////////////////////////
//
// ScorePage::getSortedStaffItems -- returns all items on a page
//    attached to the given staff number (P2).
//

void ScorePage::getSortedStaffItems(int staffnum, vectorSIp& items) {
   getUnsortedStaffItems(staffnum, items);
   sort(items.begin(), items.end(), sortP3);
}

// Alias:

void ScorePage::getSortedStaffItems(vectorSIp& items, int staffnum) {
   getSortedStaffItems(staffnum, items);
}



