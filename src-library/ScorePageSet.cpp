//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 03:18:48 PST 2014
// Last Modified: Sat Mar  1 03:18:50 PST 2014
// Filename:      ScorePageSet.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet.cpp
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
   for (auto& it : page_storage) {
      delete it;
      it = NULL;
   }
   page_storage.resize(0);
   page_sequence.resize(0);

}



