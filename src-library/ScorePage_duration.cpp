//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Thu Feb 20 23:29:45 PST 2014
// Filename:      ScorePage_duration.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_duration.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff duration analysis.
//

#include "ScorePage.h"
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// Staff duration analysis related functions
//

//////////////////////////////
//
// ScorePage::analyzeStaffDurations -- Calulate the durations of each
//      staff on the page.  This function also assigns a durational
//      offset to each item on the staff as a side effect.
//

void ScorePage::analyzeStaffDurations(void) {
   if (analysis_info.staves == 0) {
      analyzeStaves();
   }
   analysis_info.staffdurations = 0;

   vectorVSIp staffsequence;
   getHorizontallySortedStaffItems(staffsequence);

   SCORE_FLOAT staffduration;
   unsigned int i;
   for (i=1; i<staffsequence.size(); i++) {
      staffduration = calculateStaffDuration(staffsequence[i]);
      setStaffDuration(i, staffduration);
   }

   analysis_info.staffdurations = 1;
}



//////////////////////////////
//
// ScorePage::calculateStaffDurations -- Calculates the total duration
//      of the note/rest sequence on a staff.  The input is presumed to
//      be sorted according to P3 (from left to right on the staff).
//      In specific P3P1P4 sorted for best results.  This function has 
//      a side effect which sets the durational offset of each object 
//      from the start of the staff.
//

SCORE_FLOAT ScorePage::calculateStaffDuration(vectorSIp& staffitems) {
   int length = staffitems.size();
   int i, j;
   int chord;
   SCORE_FLOAT chordP3;
   SCORE_FLOAT testP3;
   SCORE_FLOAT chorddur;
   SCORE_FLOAT testdur;
   SCORE_FLOAT currentdur = 0.0;
   for (i=0; i<length; i++) {
      
      staffitems[i]->setStaffOffsetDuration(currentdur);
      if (!staffitems[i]->hasDuration()) {
         continue;
      }
      chorddur = staffitems[i]->getDuration();
      chordP3  = staffitems[i]->getHPos();
      chord = 0;
      for (j=i+1; j<length; j++) {
         if (!staffitems[j]->hasDuration()) {
            break;
         }
         testP3 = staffitems[j]->getHPos();
         if (testP3 != chordP3) {
            break;
         }
         chord++;
         staffitems[j]->setStaffOffsetDuration(currentdur);
         testdur = staffitems[j]->getDuration();
         if (testdur < chorddur) {
            chorddur = testdur;
         }
      }
      currentdur += chorddur;
      if (chord == 0) {
         continue;
      }
      i += chord;
   }

   return currentdur;
}



//////////////////////////////
//
// ScorePage::setStaffDuration --
//

void ScorePage::setStaffDuration(int staffnum, SCORE_FLOAT duration) {
   staff_info.setStaffDuration(staffnum, duration);
}



//////////////////////////////
//
// ScorePage::getStaffDuration --
//

SCORE_FLOAT ScorePage::getStaffDuration(int staffnum) {
   return staff_info.getStaffDuration(staffnum);
}



