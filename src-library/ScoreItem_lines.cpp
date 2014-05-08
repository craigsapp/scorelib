//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 29 00:06:41 PDT 2014
// Last Modified: Tue Apr 29 00:06:43 PDT 2014
// Filename:      ScoreItem_lines.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_lines.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=2 (lines) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::isHairpin -- 
//

bool ScoreItem::isHairpin(void) { 
   if (!isLineItem()) {
      return false;
   }

   // hairpins have P5=999
   if (getP5Int() != 999) {
      return false;
   }
   if ((getP7Int() == 0) || (getP7Int() == -1)) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isHairpinDescrescendo --
//

bool ScoreItem::isHairpinDecrescendo(void) {
   if (!isLineItem()) {
      return false;
   }

   // hairpins have P5=999
   if (getP5Int() != 999) {
      return false;
   }

   // decrescendo has P7 = -1
   if (getP7Int() == -1) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isHairpinCrescendo --
//

bool ScoreItem::isHairpinCrescendo(void) {
   if (!isLineItem()) {
      return false;
   }

   // hairpins have P5=999
   if (getP5Int() != 999) {
      return 0;
   }
   // crescendo has P7 = 0
   if (getP7Int() == 0) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isWavyLine -- 
//

bool ScoreItem::isWavyLine(void) {
   if (!isLineItem()) {
      return false;
   }

   // only hairpins have P5=999
   if (getP5Int() == 999) {
      return false;
   }

   // wavy line has P7 = -1
   if (getP7Int() == -1) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isDashedLine -- True if line is dashed.
//

bool ScoreItem::isDashedLine(void) {
   if (!isLineItem()) {
      return false;
   }

   // only hairpins have P5=999
   if (getP5Int() == 999) {
      return false;
   }

   // dashed line has P7 = 1
   if (getP7Int() == 1) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isPlainLine -- True if just a normal solid line.
//

bool ScoreItem::isPlainLine(void) {
   if (!isLineItem()) {
      return false;
   }

   // only hairpins have P5=999
   if (getP5Int() == 999) {
      return false;
   }

   // dashed line has P7 = false
   if (getP7Int() == 0) {
      return true;
   } else {
      return false;
   }
}



/////////////////////////////
//
// ScoreItem::isHorizontal -- returns true if the object is horizontal.
//     This is useful for analyzing the orientation of objects which have
//     two horizontal endpoints, such as beams, slurs, and lines.  Will
//     need to be extended some from the basic algorithm given here (such
//     as monitoring angle parameters).
//

bool ScoreItem::isHorizontal(void) {
   SCORE_FLOAT p4 = getVPos();
   SCORE_FLOAT p4r = getVPosRight();
   if (fabs(p4 - p4r) < 0.001) {
      return true;
   }

   return false;
}




