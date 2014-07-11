//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 15:21:27 PDT 2014
// Last Modified: Fri May  2 15:15:46 PDT 2014
// Filename:      ScorePageSet_ties.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_ties.cpp
// Syntax:        C++11
//
// Description:   Functions related to lyrics identification 
//      within ScoreSegments.
//

#include "ScorePageSet.h"
#include "ScoreUtility.h"
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePageSet::analyzeTies -- Analyze ties for all parts in all segments.
//

void ScorePageSet::analyzeTies(void) {
   for (unsigned int i=0; i<getPageCount(); i++) {
      (*this)[i][0].analyzeTies();
   }
}

