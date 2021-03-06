//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 24 00:59:26 PDT 2014
// Last Modified: Thu Apr 24 00:59:29 PDT 2014
// Filename:      ScorePageSet_address.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_address.cpp
// Syntax:        C++11
//
// Description:   Functions related to AddressSystem access into ScorePageSets.
//

#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// ScorePageSet::getPage -- Return a ScorePage related to the given
//       AddressSystem.
//

ScorePage* ScorePageSet::getPage(const AddressSystem& anAddress) {
   int page    = anAddress.getPageIndex();
   int overlay = anAddress.getOverlayIndex();
   if (overlay < 0) {
      overlay = 0;
   }
   return &(*this)[page][overlay];
}


//////////////////////////////
//
// ScorePageSet::getSystemItems -- Return a list of ScoreItems on the
//     given page/system address.
//

vectorSIp& ScorePageSet::getSystemItems(const AddressSystem& anAddress) {
   ScorePage* page = getPage(anAddress);
   int sysindex = anAddress.getSystemIndex();
   return page->getSystemItems(sysindex);
}



