//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  8 23:04:49 PDT 2014
// Last Modified: Tue Jul  8 23:04:52 PDT 2014
// Filename:      ScorePage_tie.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_tie.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                melodic layer analysis.
//

#include "ScorePage.h"
#include "ScoreUtility.h"
#include <algorithm>
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeTies -- Identify ties on each staff, not checking
//      for ties which cross system breaks (that is handled at the
//      ScoreSegment level).
//

int ScorePage::analyzeTies(void) {
   if (!analysis_info.layersIsValid()) {
      analyzeLayers();
   }
   analysis_info.setInvalid("staffslursties");

   vectorVSIp staffsequence;
   getHorizontallySortedStaffItems(staffsequence);

   vectorSIp lefthanging;
   vectorSIp righthanging;
   vectorSIp leftnotes;
   vectorSIp rightnotes;

   unsigned int i;
   int staffnum = 0;
   for (i=1; i<staffsequence.size(); i++) {
      if (staffsequence[i].size() == 0) {
         // nothing on staff
         continue;
      }
      staffnum = i;
      SU::analyzeStaffTies(staffnum, staffsequence[i],  righthanging, 
            leftnotes, lefthanging, rightnotes);
   }

   analysis_info.setValid("staffslursties");
   return 1;
}



