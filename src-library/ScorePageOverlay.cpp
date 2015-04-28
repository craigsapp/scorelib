//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 10 15:11:26 PDT 2014
// Last Modified: Thu Apr 10 15:27:27 PDT 2014
// Filename:      ScorePageOverlay.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageOverlay.cpp
// Syntax:        C++11
//
// Description:   This class holds multiple ScorePages which are intended
//                to be displayed on the same page.
//

#include "ScorePageOverlay.h"

using namespace std;


//////////////////////////////
//
// ScorePageOverlay::ScorePageOverlay -- Constructor.
//

ScorePageOverlay::ScorePageOverlay(void) {
   // nothing to do.
}



//////////////////////////////
//
// ScorePageOverlay::~ScorePageOverlay -- Destructor.
//

ScorePageOverlay::~ScorePageOverlay() {
   clear();
}



//////////////////////////////
//
// ScorePageOverlay::clear -- Delete the content of all pages stored in
//       overlay_storage.
//

void ScorePageOverlay::clear(void) {
   for (auto& it : overlay_storage) {
      delete it;
      it = NULL;
   }
   overlay_storage.resize(0);
   overlay_sequence.resize(0);
}



//////////////////////////////
//
// ScorePageOverlay::getOverlayCount --
//

int ScorePageOverlay::getOverlayCount(void) {
   return overlay_sequence.size();
}

//
// Alias:
//

int ScorePageOverlay::size(void) {
   return overlay_sequence.size();
}



//////////////////////////////
//
// ScorePageOverlay::operator[] --
//

ScorePage& ScorePageOverlay::operator[](int oindex) {
   return *overlay_sequence[oindex];
}



//////////////////////////////
//
// ScorePageOverlay::getPage -- similar to operator[], but returns a pointer
//     to a ScorePage.
//

ScorePage* ScorePageOverlay::getPage(int oindex) {
   return overlay_sequence[oindex];
}



///////////////////////////////
//
// ScorePageOverlay::createOverlay --
//

ScorePage* ScorePageOverlay::createOverlay(void) {
   ScorePage* sp = new ScorePage;
   appendOverlay(sp);
   return sp;
}



//////////////////////////////
//
// ScorePageOverlay::appendOverlay -- Absorb a page of music (overlay)
//    and store it in overlay_storage.  Note that the input data should
//    be allocated on the heap, and this class will delete it when it
//    goes out of scope (or the clear() function is called).  Returns
//    the overlay index of the appended page.
//

int ScorePageOverlay::appendOverlay(ScorePage* page) {
   overlay_storage.push_back(page);
   overlay_sequence.push_back(page);
   return overlay_sequence.size() - 1;
}



