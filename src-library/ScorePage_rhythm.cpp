//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 24 16:40:24 PDT 2014
// Last Modified: Thu Apr 24 16:40:26 PDT 2014
// Filename:      ScorePage_rhythm.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_rhythm.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                rhythmic analysis.
//

#include "ScorePage.h"
#include "ScoreUtility.h"
#include <algorithm>
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePage::getSystemLCMRhythm -- Return the smallest rhythmic
//    duration which can represent all other rhythms on the page
//    as integer units of that rhythm.  The return value is in 
//    reference to quarter notes, so if the smallest duration
//    on the system is a whole note, then return value will be "1"
//    for quarter notes.
//

int ScorePage::getSystemLCMRhythm(int systemindex) {
   if (!analysis_info.systemsIsValid()) {
      analyzeSystems();
   }
   if (!analysis_info.durationIsValid()) {
      analyzeStaffDurations();
   }


   set<int> numbers;
   RationalNumber rn;
   RationalNumber rd;
   vectorSIp& sysitems = this->getSystemItems(systemindex);
   for (auto& it : sysitems) {
      if (!it->hasDuration()) {
         continue;
      }
      rn = it->getDurationIncludingDots();

      if (rn <= 0) {
         continue;
      }
      numbers.insert(rn.getDenominator());
   }

   int output = ScoreUtility::lcm(numbers);
   return output;
}



