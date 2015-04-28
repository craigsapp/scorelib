//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 24 00:59:26 PDT 2014
// Last Modified: Thu Apr 24 00:59:29 PDT 2014
// Filename:      ScorePageSet_page.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_page.cpp
// Syntax:        C++11
//
// Description:   Functions related to AddressSystem  access into ScorePageSets.
//

#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// ScorePageSet::analyzePitch -- Do pitch analysis on every page in set.
//

void ScorePageSet::analyzePitch(void) {
   ScorePageSet& sps = *this;
   int pcount = getPageCount();
   int ocount;
   int i, j;
   for (i=0; i<pcount; i++) {
      ocount = sps[i].getOverlayCount();
      for (j=0; j<ocount; j++) {
         sps[i][j].analyzePitch();
      }
   }
}



