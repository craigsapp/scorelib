//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jun  4 15:08:09 PDT 2015
// Last Modified: Thu Jun  4 15:08:11 PDT 2015
// Filename:      ScoreItem_numbers.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_numbers.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=10 (number) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getNumber -- Returns P5 value.  Returns zero if not
//     a number item.
//

double ScoreItem::getNumber(void) {
   if (!isNumberItem()) {
      return 0.0;
   }
   return getP5();
}



