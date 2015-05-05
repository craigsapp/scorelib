//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 14:05:41 PDT 2014
// Last Modified: Thu May  1 14:05:43 PDT 2014
// Filename:      ScorePage_segment.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_segment.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                ScorePageSet ScoreSegments.
//

#include "ScorePage.h"

using namespace std;


//////////////////////////////
//
// ScorePage::getPartIndex --
//

int ScorePage::getPartIndex(int p2value) {
   return segmentpart_map[p2value];
}



//////////////////////////////
//
// ScorePage::setStaffPartIndex --
//

void ScorePage::setStaffPartIndex(int p2val, int partindex) {
   if (p2val < 0) {
      return;
   }
   if (p2val >= (int)segmentpart_map.size()) {
      segmentpart_map.resize(p2val+1);
   }
   segmentpart_map[p2val] = partindex;
}



