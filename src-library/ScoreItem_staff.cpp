//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 23:24:21 PST 2014
// Last Modified: Thu Mar  6 23:24:23 PST 2014
// Filename:      ScoreItem_staff.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_staff.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=3 (clef) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getPartNumber -- Return the part number of the staff.
//

double ScoreItem::getPartNumber(void) {
   return getParameter(P9);
}


//
// Alias, integer part only.
//


int ScoreItem::getPartNumberInt(void) {
   return (int)getPartNumber();
}


//
// Aliases
//


double ScoreItem::getInstrumentNumber(void) {
   return getPartNumber();
}


int ScoreItem::getInstrumentNumberInt(void) {
   return (int)getPartNumber();
}



//////////////////////////////
//
// ScoreItem::getScale -- Return the scaling factor for staves and text.
//     Text items also have a horizontal scaling factor which this function
//     does not return (only vertical size for text).
//

SCORE_FLOAT ScoreItem::getScale(void) {
   int p1 = getP1Int();
   SCORE_FLOAT output = 0.0;
   SCORE_FLOAT tolerance = 0.001;
   switch (p1) {
      case P1_Staff:
         output = fabs(getP5());
         if (output < tolerance) {
            output = 1.0;
         }
         break;

      case P1_Text:
         output = fabs(getP6());
         if (output < tolerance) {
            output = 1.0;
         }
         break;
   }

   return output;
}



//////////////////////////////
//
// ScoreItem::convertStaffP4ToP10Inches -- Convert P4 representation (diatonic
//   step method) of vertical placements into P10 version (absolute
//   distance from bottom line of first staff).
//
//   The conversion equation is:
//      P10 = (P2-1) * 0.7875" + P4 * P5 * 0.04375"
//   0.7875 is the default distance between staves from bottom line of
//   one to the bottom line of the other.  0.04375" is the default vertical
//   distance of a diatonic step (1/2 of the distance between lines in
//   a staff.
//
//   Note that the physical units of P10 are in inches.  This function will
//   not do anything if P10 is already non-zero.
//

void ScoreItem::convertStaffP4ToP10Inches(void) {
   if (!isStaffItem()) {
      return;
   }
   SCORE_FLOAT p10 = getP10();
   if (p10 != 0.0) {
      // Already using P10, so don't try to convert P4, which may now be zero.
      return;
   }
   setStaffP10InchesFromP4Value(getP4());
}



//////////////////////////////
//
// ScoreItem::convertStaffP4ToP10Centimeters -- Convert P4 representation
//   (diatonic step method) of vertical placements into P10 version (absolute
//   distance from bottom line of first staff).
//
//   Note that the physical units of P10 are in centimeters.  This function
//   will not do anything if P10 is already non-zero.
//

void ScoreItem::convertStaffP4ToP10Centimeters(void) {
   convertStaffP4ToP10Inches();
   setP10(getP10() * 2.54);
}



//////////////////////////////
//
// ScoreItem::convertStaffP10InchesToP4Value -- Convert P10 representation
//   (absolute distance from bottom line of first staff) of vertical
//   placements into P4 version (diatonic step method).
//
// The conversion equation is:
//   P4 = (P10 / 0.04375 - 18.0 * (P2 - 1)) / P5
//
// Note that the physical units of P10 are in inches.  This function
// will not do anything if P10 is not zero.
//

void ScoreItem::convertStaffP10InchesToP4Value(void) {
   SCORE_FLOAT p10 = getP10();
   if (p10 == 0.0) {
      // not using P10, so already using P4 system.
      return;
   }
   setStaffP4FromP10Inches(p10);
}



//////////////////////////////
//
// ScoreItem::convertStaffP10CentimetersToP4Value -- Convert P10 representation
//   (absolute distance from bottom line of first staff) of vertical
//   placements into P4 version (diatonic step method).
//
// Note that the physical units of P10 are in centimeters.  This function
// will not do anything if P10 is not zero.
//

void ScoreItem::convertStaffP10CentimetersToP4Value(void) {
   SCORE_FLOAT p10 = getP10();
   if (p10 == 0.0) {
      // not using P10, so already using P4 system.
      return;
   }
   setStaffP4FromP10Inches(p10 / 2.54);
}



//////////////////////////////
//
// ScoreItem::setStaffP4FromP10Inches -- Given a staff P10 value, calculate
//   and set the P4 value which corresponds with this value, where the
//   input P10 value is in inches.
//

void ScoreItem::setStaffP4FromP10Inches(SCORE_FLOAT p10) {
   if (!isStaffItem()) {
      return;
   }
   int p2 = (int)getStaffNumber();
   SCORE_FLOAT p4;
   SCORE_FLOAT p5 = getScale();
   p4 = (p10 / 0.04375 - 18.0 * (p2 - 1)) / p5;
   setP4(p4);
   // P10 should no longer being used, so must clear
   // that parameter to make P4 visible.
   setP10(0.0);
}



//////////////////////////////
//
// ScoreItem::setStaffP4FromP10Centimeters -- Given a staff P10 value, calculate
//   and set the P4 value which corresponds with this value, where the
//   input P10 value is in centimeters.
//

void ScoreItem::setStaffP4FromP10Centimeters(SCORE_FLOAT p10) {
   setStaffP4FromP10Inches(p10 / 2.54);
}



//////////////////////////////
//
// ScoreItem::setStaffP10InchesFromP4Value -- Given a staff P4 value,
//   calculate and set the P10 value which corresponds with this value.
//

void ScoreItem::setStaffP10InchesFromP4Value(SCORE_FLOAT p4) {
   if (!isStaffItem()) {
      return;
   }
   int p2 = (int)getStaffNumber();
   SCORE_FLOAT p5 = getScale();
   SCORE_FLOAT p10 = (p2 - 1) * 0.7875 + p4 * p5 * 0.04375;
   setP10(p10);
   // P4 is no longer being used, so clear that parameter if not zero:
   setP4(0.0);
}



//////////////////////////////
//
// ScoreItem::setStaffP10CentimetersFromP4Value -- Given a staff P4 value,
//   calculate and set the P10 value which corresponds with this value.
//

void ScoreItem::setStaffP10CentimetersFromP4Value(SCORE_FLOAT p4) {
   setStaffP10InchesFromP4Value(p4);
   setP10(getP10() * 2.54);
}



