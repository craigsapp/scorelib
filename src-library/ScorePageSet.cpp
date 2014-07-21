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


ScorePageSet::ScorePageSet(Options& opts) {
   read(opts);
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
   clearSegments();
}



//////////////////////////////
//
// ScorePageSet::getPageCount -- return the number of pages in the set.
//     This count does not include page overlays (see getOverlayCount()).
//

int ScorePageSet::getPageCount(void) {
   return page_sequence.size();
}



//////////////////////////////
//
// ScorePageSet::getOverlayCount -- return the number of pages in the set.
//     This count does not include page overlays (see getOverlayCount()).
//     Returns -1 if the sequential page index is invalid.
//

int ScorePageSet::getOverlayCount(int pindex) {
   if ((pindex < 0) || ((unsigned int)pindex >= page_sequence.size())) {
      return -1;
   }
   return page_sequence[pindex]->size();
}



//////////////////////////////
//
// ScorePageSet::getPage -- Return a pointer to the requested page.
//     When a single index, return the first page in an overlay.
//

ScorePage* ScorePageSet::getPage(int pindex) {
   return &((*page_sequence[pindex])[0]);
}


ScorePage* ScorePageSet::getPage(int pindex, int oindex) {
   return &((*page_sequence[pindex])[oindex]);
}



//////////////////////////////
//
// ScorePageSet::createPage -- append a new page to the end
//    of the page list, returning a pointer to the new page.
//

ScorePage* ScorePageSet::createPage(void) {
   ScorePage* sp = new ScorePage;
   appendPage(sp);
   return sp;
}



//////////////////////////////
//
// ScorePageSet::appendPage -- Receive a page of data as input and
//    store in the page set.  The page will be considered "owned" by
//    this object, and will be automatically deleted when the object
//    is deconstructed.  Returns the page sequence number (indexed from 0)
//    for the page in the list of pages.
//

int ScorePageSet::appendPage(ScorePage* page) {
   ScorePageOverlay* pageoverlay = new ScorePageOverlay;
   pageoverlay->appendOverlay(page);
   page_storage.push_back(pageoverlay);
   page_sequence.push_back(pageoverlay);
   return page_sequence.size() - 1;
}



//////////////////////////////
//
// ScorePageSet::appendOverlay -- Receive a page of data as input and
//    store in the page set.  The page will be considered "owned" by
//    this object, and will be automatically deleted when the object
//    is deconstructed.  Returns the page overlay number (indexed from 0)
//    for the assigned page.
//

void ScorePageSet::appendOverlay(ScorePage* page) {
   if (page_sequence.size() == 0) {
      ScorePageOverlay* spo = new ScorePageOverlay;
      page_sequence.push_back(spo);
      appendOverlay(page);
      return;
   }
   page_sequence.back()->appendOverlay(page);
}



void ScorePageSet::appendOverlay(ScorePage* page, int pindex) {
   page_sequence[pindex]->appendOverlay(page);
}



//////////////////////////////
//
// ScorePageSet::operator[] --
//

ScorePageOverlay& ScorePageSet::operator[](int oindex) {
   return *(page_sequence[oindex]);
}



//////////////////////////////
//
// ScorePageSet::setPageOwnerships -- set each page's owner
//    to this object.
//

void ScorePageSet::setPageOwnerships(void) {
   ScorePageSet& t = *this;
   int i, j;
   int overlaycount = getPageCount();
   for (i=0; i<overlaycount; i++) {
      for (j=0; j<t[i].size(); j++) {
         t[i][j].setSetOwner(this);
      }
   }
}



//////////////////////////////
//
// ScorePageSet::analyzeStaffDurations -- Calculate durations
//     of each staff and duration offsets for items on the staff
//     (for each page).
//

void ScorePageSet::analyzeStaffDurations(void) {
   ScorePageSet& t = *this;
   int i;
   for (i=0; i<getPageCount(); i++) {
      t[i][0].analyzeStaffDurations();
   }
}



