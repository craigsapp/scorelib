//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Thu Feb 20 23:29:45 PST 2014
// Filename:      ScorePage.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage.cpp
// Syntax:        C++11
//
// Description:   This class handles analysis of relationships between 
//                SCORE items on the page, such as system, pitch, and chord 
//                groupings.
//

#include "ScorePage.h"
#include <algorithm>

using namespace std;


///////////////////////////////
//
// ScorePage::ScorePage --
//

ScorePage::ScorePage(void) : ScorePageBase() {
   // do nothing
}


ScorePage::ScorePage(const char* filename) : ScorePageBase(filename) {
   // do nothing
}


ScorePage::ScorePage(istream& instream) : ScorePageBase(instream) {
   // do nothing
}


ScorePage::ScorePage(const ScorePage& apage) : 
      ScorePageBase((ScorePageBase)apage) {
   // do nothing
}


ScorePage::ScorePage(const ScorePageBase& apage) : 
      ScorePageBase(apage) {
   // do nothing
}



//////////////////////////////
//
// ScorePage::~ScorePage --
//

ScorePage::~ScorePage() {
   // do nothing
}



///////////////////////////////////////////////////////////////////////////
//
// Private functions.
//

//////////////////////////////
//
// ScorePage::sortPageHorizontally --  The primary analysis function
// to which most other analysis functions depend.
//

void ScorePage::sortPageHorizontally  (void) {
   analysis_info.setInvalid("sorted");
   vectorSIp& sortlist = itemlist_P3sorted;
   sortlist.resize(item_storage.size());

   auto it = item_storage.begin();
   auto it2 = sortlist.begin();

   for ( ; ((it != item_storage.end()) && (it2 != sortlist.end())); 
           it++, it2++) {
      *it2 = *it;
   }
   sort(sortlist.begin(), sortlist.end(), sortP3);
   analysis_info.setValid("notmodified");
   analysis_info.setValid("sorted");
}




