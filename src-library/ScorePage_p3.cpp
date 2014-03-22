//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar 21 00:51:52 PDT 2014
// Last Modified: Fri Mar 21 00:51:55 PDT 2014
// Filename:      ScorePage_p3.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_p3.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                horizontal position / staff offset cross-indexing.
//

#include "ScorePage.h"
#include <algorithm>

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeP3 -- 
//

void ScorePage::analyzeP3(void) {
   if (!analysis_info.p3IsValid()) {
      analyzeSystems();
      analyzeStaffDurations();
   }
   analysis_info.setInvalid("p3");

   int syscount = getSystemCount();

   p3_database.resize(syscount);

   for (int i=0; i<syscount; i++) {
      for (auto& it : systemItems(i)) {
         p3_database[i].addItem(it);
      }
   }

   analysis_info.setValid("p3");
}

//
// Functions dependent on the analyzeP3 function:
//


//////////////////////////////
//
// ScorePage::getP3OfStaffDurationOffset -- Given the input
//    system index and staff duration offset position, return
//    the P3 value of that duration on the system.
//

SCORE_FLOAT ScorePage::getP3OfStaffDurationOffset(int sys, SCORE_FLOAT offset) {
   if (!analysis_info.p3IsValid()) {
      analyzeP3();
   }
   return p3_database[sys].getP3OfStaffDurationOffset(offset);
}



