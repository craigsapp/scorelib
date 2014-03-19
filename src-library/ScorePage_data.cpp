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
// Staff-level data lists:
//

//////////////////////////////
//
// ScorePage::getHorizontallySortedStaffItems --  Returns an array of
//    staves, each with data sorted by P3 for that particular staff.
//

void  ScorePage::getHorizontallySortedStaffItems(vectorVSIp& staffsequence) {
   int staffcount = getMaxStaff();
   staffsequence.resize(staffcount+1);
   for (auto& it : staffsequence) {
      it.reserve(32);
   }

   for (auto& it2 : item_storage) {
      int staffnum = it2->getStaffNumber();
      if (staffnum <= 0) {
         continue;
      }
      if (staffsequence.size() < (unsigned int)(staffnum + 1)) {
         staffsequence.resize(staffnum+1);
      }
      staffsequence[staffnum].push_back(it2);
   }

   for (auto& it : staffsequence) {
      sort(it.begin(), it.end(), sortP3);
    }
}



//////////////////////////////
//
// ScorePage::getUnsortedStaffItems -- returns all items on a page
//    attached to the given staff number (P2).
//

void ScorePage::getUnsortedStaffItems(int staffnum, vectorSIp& items) {
   items.clear();
   int num;
   for (auto& it : item_storage) {
      num = it->getStaffNumber();
      if (num != staffnum) {
         continue;
      }
      items.push_back(it);
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



///////////////////////////////////////////////////////////////////////////
//
// System-level data lists (should automatically run system analysis
// for the page if necessary).
//

//////////////////////////////
//
// ScorePage::getUnsortedSystemItems --
//

void ScorePage::getUnsortedSystemItems(vectorSIp& sysseq, int sysindex) {
   if (!analysis_info.systemsIsValid()) {
      analyzeSystems();
   }

   sysseq.clear();
   if ((sysindex < 0) || (sysindex >= getSystemCount())) {
      return;
   }
   sysseq.reserve(item_storage.size());

   int p2;
   for (auto& it : item_storage) {
      p2 = it->getStaffNumber();
      int systemnumber = getSystemIndex(p2);
      if (systemnumber != sysindex) {
         continue;
      }
      sysseq.push_back(it);
   }
}

// Alias:

void ScorePage::getUnsortedSystemItems(int sysindex, vectorSIp& sysseq) {
   return getUnsortedSystemItems(sysseq, sysindex);
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

// Alias:

void ScorePage::getHorizontallySortedSystemItems(int sysindex, 
      vectorSIp& sysseq) {
   getHorizontallySortedSystemItems(sysseq, sysindex);
}



