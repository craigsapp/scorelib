//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jul 18 17:04:49 PDT 2014
// Last Modified: Fri Jul 18 17:04:57 PDT 2014
// Filename:      ScorePageSet_tuplets.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_tuplets.cpp
// Syntax:        C++11
//
// Description:   Functions related to tuplet identification 
//      within ScorePages.
//

#include "ScorePageSet.h"
#include "ScoreUtility.h"
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePageSet::analyzeTuplets -- Analyze tuplets for all page in set.
//

void ScorePageSet::analyzeTuplets(void) {
   for (unsigned int i=0; i<getPageCount(); i++) {
      (*this)[i][0].analyzeTuplets();
   }
}



