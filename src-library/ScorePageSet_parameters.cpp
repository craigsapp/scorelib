//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 14:05:41 PDT 2014
// Last Modified: Thu May  1 14:05:43 PDT 2014
// Filename:      ScorePage_parameters.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_parameters.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                processing all parameters on the page.
//

#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// ScorePageSet::copyParameterOverwrite --
//

void ScorePageSet::copyParameterOverwrite(const string& newnamespace,
      const string& oldnamespace, const string& parameter) {
   for (auto& it : page_storage) {
      int overlaycount = it->getOverlayCount();
      for (unsigned int j=0; j<overlaycount; j++) {
         it->getPage(j)->copyParameterOverwrite(newnamespace, oldnamespace,
               parameter);
      }
   }
}



//////////////////////////////
//
// ScorePageSet::copyParameterNoOverwrite --
//

void ScorePageSet::copyParameterNoOverwrite(const string& newnamespace,
      const string& oldnamespace, const string& parameter) {
   for (auto& it : page_storage) {
      int overlaycount = it->getOverlayCount();
      for (unsigned int j=0; j<overlaycount; j++) {
         it->getPage(j)->copyParameterNoOverwrite(newnamespace, oldnamespace,
               parameter);
      }
   }
}



//////////////////////////////
//
// ScorePageSet::deleteNamespace --
//

void ScorePageSet::deleteNamespace(const string& nspace) {
   for (auto& it : page_storage) {
      int overlaycount = it->getOverlayCount();
      for (unsigned int j=0; j<overlaycount; j++) {
         it->getPage(j)->deleteNamespace(nspace);
      }
   }
}



