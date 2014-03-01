//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 03:18:48 PST 2014
// Last Modified: Sat Mar  1 03:18:50 PST 2014
// Filename:      ScorePageSet.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet.cpp
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// ScorePageSet::ScorePageSet -- Constructor.
//

ScorePageSet::ScorePageSet(void) {
   // do nothing
}



//////////////////////////////
//
// ScorePageSet::~ScorePageSet -- Deconstructor.
//

ScorePageSet::~ScorePageSet() {
   clear();
}



//////////////////////////////
//
// ScorePageSet::clear -- remove all pages.
//

void ScorePageSet::clear(void) {
   unsigned int i;
   for (i=0; i<pages.size(); i++) {
      if (pages[i] != NULL) {
         delete pages[i];
         pages[i] = NULL;
      }
   }
   pages.resize(0);
}



