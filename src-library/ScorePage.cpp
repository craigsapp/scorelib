//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:35:50 PST 2014
// Last Modified: Thu Feb 20 23:29:45 PST 2014
// Filename:      ScorePage.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage.cpp
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
// Staff system analysis related functions
//


///////////////////////////////
//
// ScorePage::getSystemCount -- Return the number of systems
//     on the page.  Systems are defined as a set of barlines
//     which have barlines attaching them to each other.
//

int ScorePage::getSystemCount(void) {
   return 0;
}

//////////////////////////////
//
// ScorePage::analyzeSystems --  Still need to consider ossia.
//

void ScorePage::analyzeSystems(void) {

}



