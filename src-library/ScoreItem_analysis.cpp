//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 23:50:08 PDT 2014
// Last Modified: Mon Mar 10 23:50:12 PDT 2014
// Filename:      ScoreItem_analysis.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_analysis.cpp
// Syntax:        C++11
//
// Description:   ScoreItem functions dependent on page-level analyses.
//

#include "ScoreItem.h"
#include "ScoreUtility.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getHumdrumPitch -- convert any analysis@base40Pitch parameter
//     into a Humdrum **kern note name. 
//

string ScoreItem::getHumdrumPitch(void) {
   int base40 = getParameterInt(ns_auto, np_base40Pitch);
   if (base40 <= 0) {
      return "";
   }
   string output = SU::base40ToKern(base40);
   return output;
}



