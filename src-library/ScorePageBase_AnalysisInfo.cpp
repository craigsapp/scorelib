//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sun Feb 16 20:32:02 PST 2014
// Filename:      ScorePageBase_AnalysisInfo.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageBase_AnalysisInfo.cpp
// Syntax:        C++11
//
// Description:   Support class for structural analyses in ScorePageBase.
//


#include "ScorePageBase_AnalysisInfo.h"

using namespace std;


//////////////////////////////
//
// AnalysisInfo::AnalysisInfo -- Constructor.
//

AnalysisInfo::AnalysisInfo(void) {
   clear();
}


AnalysisInfo::AnalysisInfo(const AnalysisInfo& info) {
   *this = info;
}


//////////////////////////////
//
// AnalysisInfo::AnalysisInfo -- Destructor.
//

AnalysisInfo::~AnalysisInfo() {
   clear();
}


//////////////////////////////
//
// AnalysisInfo::operator= --
//

AnalysisInfo& AnalysisInfo::operator=(const AnalysisInfo& info) {
   if (this == &info) {
      return *this;
   }
   notmodified    = info.notmodified;
   staves         = info.staves;
   staffdurations = info.staffdurations;
   systems        = info.systems;
   return *this;
}



//////////////////////////////
//
// AnalysisInfo::clear -- Invalidate all analyses which have been
//     done on the page data.
//

void AnalysisInfo::clear(void) {
   notmodified    = 0;
   staves         = 0;
   staffdurations = 0;
   systems        = 0;
}



