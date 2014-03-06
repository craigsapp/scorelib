//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Wed Mar  5 22:40:45 PST 2014
// Filename:      ScorePage_duration.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_duration.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff duration analysis.
//

#include "ScorePage.h"
#include <algorithm>
#include <set>

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
//      This function has a side effect which sets the durational offset 
//      of each object from the start of the staff.
//

SCORE_FLOAT ScorePage::calculateStaffDuration(vectorSIp& staffitems) {

   // expectedOffset is needed to keep track of multiple streams
   // of notes which may alternate with each other such as two voices
   // on a staff which have these rhythmic patterns:
   //     voice 1:     E Q   Q   E
   //     voice 2:     Q   Q   Q
   //     composite:   E E E E E E
   //
   set<SCORE_FLOAT> expectedOffset;

   unsigned int i;
   const SCORE_FLOAT htolerance = 0.05;  // hpos within this range are equiv.
   const SCORE_FLOAT rtolerance = 0.003; // rhythms within this range are equiv.
   SCORE_FLOAT dur;
   SCORE_FLOAT hpos;
   SCORE_FLOAT nextevent;
   SCORE_FLOAT activeHpos = 0.0;
   SCORE_FLOAT currentStaffDurOffset = 0.0;

   for (i=0; i<staffitems.size(); i++) {
      if (!staffitems[i]->hasDuration()) {
         continue;
      }
      dur  = staffitems[i]->getDuration();
      hpos = staffitems[i]->getHPos();
      if (dur <= 0.0) {
         staffitems[i]->setStaffOffsetDuration(currentStaffDurOffset);
         continue;
      }
      if (hpos > activeHpos + htolerance) {
         if (expectedOffset.size() == 0) {
            currentStaffDurOffset = 0.0;
         } else {
            currentStaffDurOffset = *expectedOffset.begin();
            expectedOffset.erase(expectedOffset.begin());
         }
         activeHpos = hpos;
      }
      staffitems[i]->setStaffOffsetDuration(currentStaffDurOffset);
      nextevent = dur + currentStaffDurOffset;
      if (fabs(nextevent - (int)(nextevent+rtolerance)) <= rtolerance) {
         // Compensate for round-off errors related to tuplets at quarter notes:
         nextevent = (int)(nextevent + rtolerance);
      }
      expectedOffset.insert(nextevent);
   }

   // Maybe assign staff duration offsets to non-durational offsets here.

   // There should be one last event in the storage which represents 
   // the duration of the system:
   if (expectedOffset.size() > 0) {
      return *expectedOffset.begin();
   } else {
      return 0.0;
   }
}



//////////////////////////////
//
// ScorePage::setStaffDuration -- Set the duration of the staff in terms
//    of quatert notes (sum of P7 values for notes and rests that are
//    found on the given staff).
//

void ScorePage::setStaffDuration(int staffnum, SCORE_FLOAT duration) {
   staff_info.setStaffDuration(staffnum, duration);
}



//////////////////////////////
//
// ScorePage::getStaffDuration --  The sum of the durations for notes
//     and rests (P7 values) found on the staff.  
//

SCORE_FLOAT ScorePage::getStaffDuration(int staffnum) {
   if (analysis_info.staves == 0) {
      analyzeStaves();
   }
   return staff_info.getStaffDuration(staffnum);
}



