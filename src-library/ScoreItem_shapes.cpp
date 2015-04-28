//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_shapes.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_shapes.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=12
//       (special shapes) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::isCircular -- Is either a circle or an ellipse
// (or arcs of circle/ellipse).
//

bool ScoreItem::isCircular(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 1) {
      return false;
   }

   SCORE_FLOAT p12 = getP12();
   if (p12 != 0.0) {
      // a polygon
      return false;
   }

   return true;
}



//////////////////////////////
//
// ScoreItem::isCircle -- The circular object is a circle (not an ellipse).
//    Also, not a partial circle (arc).
//

bool ScoreItem::isCircle(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 1) {
      return false;
   }

   SCORE_FLOAT p12 = getP12();
   if (p12 != 0.0) {
      // a polygon
      return false;
   }
   // arcing information
   SCORE_FLOAT p10 = getP10();
   SCORE_FLOAT p11 = getP11();

   if ((p10 != 0.0) && (p11 != 0.0)) {
      if (fabs(p11 - p10) < 360) {
         // arc, but doesn't cycle at least one time.
         return false;
      }
   }

   // circle should be round
   SCORE_FLOAT p6 = getP6();
   SCORE_FLOAT p7 = getP7();
   double threshold = 0.01;
   if ((p7 == 0) || (fabs(p7-p6) < threshold)) {
      return true;
   } else {
      // ellipse
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isEllipse -- The circular object is an ellipse (not a circle).
//

bool ScoreItem::isEllipse(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 1) {
      return false;
   }
   SCORE_FLOAT p6 = getP6();
   SCORE_FLOAT p7 = getP7();

   SCORE_FLOAT p12 = getP12();
   if (p12 != 0.0) {
      // a polygon
      return false;
   }
   // arcing information
   SCORE_FLOAT p10 = getP10();
   SCORE_FLOAT p11 = getP11();
   if (!((p10 == p11) && (p11 == 0.0))) {
      return false;
   }
   if (fabs(p11 - p10) < 360) {
      // arc, but cycles at least one time.
      return false;
   }

   double threshold = 0.01;
   if ((p7 == 0) || (fabs(p7-p6) < threshold)) {
      // circle
      return false;
   } else {
      // ellipse
      return true;
   }
}



//////////////////////////////
//
// ScoreItem::isRectangular -- A rectangle or square. True if P1=12 and p5=0.
//

bool ScoreItem::isRectangular(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 == 0) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isSquare -- A square (not a rectangle). True if P1=12 and p5=0,
//    and p6==p7 or p7=0.
//

bool ScoreItem::isSquare(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 0) {
      return false;
   }
   SCORE_FLOAT p6 = getP6();
   SCORE_FLOAT p7 = getP7();
   double threshold = 0.01;
   if ((p7 == 0) || (fabs(p7-p6) < threshold)) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::isRectangle -- A rectangle (not a square). True if P1=12 and p5=0,
//    and p6!=p7 or p7=0.
//

bool ScoreItem::isRectangle(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 0) {
      return false;
   }
   SCORE_FLOAT p6 = getP6();
   SCORE_FLOAT p7 = getP7();
   double threshold = 0.01;
   if ((p7 == 0) || (fabs(p7-p6) < threshold)) {
      return false;
   }

   // check for parallelogram
   SCORE_FLOAT p10 = getP10();
   if (p10 != 0.0) {
      return false;
   }

   return true;
}



//////////////////////////////
//
// ScoreItem::isParallelogram -- A rectangle or square with non-right-angle corners.
// True if P1=12 and p5=0, and p10!=0
//

bool ScoreItem::isParallelogram(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (p5 != 0) {
      return false;
   }
   SCORE_FLOAT p10 = getP10();
   if (p10 != 0.0) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// ScoreItem::isFilled -- A rectangular or circular object may be filled.
//

bool ScoreItem::isFilled(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (!(p5 == 0 || p5 == 1)) {
      return false;
   }

   int p13 = getP13Int();
   if (p13 == 1) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isUnfilled -- A rectangular or circular object may not be filled.
//

bool ScoreItem::isUnfilled(void) {
   if (!isShapeItem()) {
      return false;
   }
   int p5 = getP5Int();
   if (!(p5 == 0 || p5 == 1)) {
      return false;
   }

   int p13 = getP13Int();
   if (p13 == 0) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isFilledCircle -- True if item is a circle and it is filled.
//

bool ScoreItem::isFilledCircle(void) {
   if (isCircle() && isFilled()) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isUnfilledCircle -- True if item is a circle and it is not filled.
//

bool ScoreItem::isUnfilledCircle(void) {
   if (isCircle() && isUnfilled()) {
      return true;
   } else {
      return false;
   }
}




//////////////////////////////
//
// ScoreItem::getVerticalRadius -- Returns P7 of item if a shape.  If P7==0, then
//    returns P6.  Units are diatonic steps (1/2 of distance between
//    staff lines).  This is the radius of a circle; the 1/2 height of
//    an ellipse, square, rectangle, or parallelogram.
//

SCORE_FLOAT ScoreItem::getVerticalRadius(void) {
   if (!isShapeItem()) {
      return 0.0;
   }
   SCORE_FLOAT p7 = getP7();
   SCORE_FLOAT p6;
   if (p7 == 0.0) {
      p6 = getP6();
      if (p6 == 0.0) {
         return 1.0/2.0;
      } else {
         return p6/2.0;
      }
   } else {
      return p7/2.0;
   }
}



//////////////////////////////
//
// ScoreItem::getHorizontalRadius -- Returns P6 of item if a shape.  Units are
// diatonic steps (1/2 of the distance between staff lines).  This is the radius
// of a circle; the 1/2 width of an ellipse, square, rectangle, or parallelogram.
//

SCORE_FLOAT ScoreItem::getHorizontalRadius(void) {
   if (!isShapeItem()) {
      return 0.0;
   }
   SCORE_FLOAT p6 = getP6();
   if (p6 == 0.0) {
      return 1.0/2.0;
   } else {
      return getP6()/2.0;
   }
}



