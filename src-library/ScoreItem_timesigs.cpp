//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 17 08:35:35 PDT 2014
// Last Modified: Sat May 17 08:35:38 PDT 2014
// Filename:      ScoreItem_timesigs.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_timesigs.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=18
//       (time signature) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::getTimeSignatureTop -- Returns the top number in a time
//     signature.  If the time signature is common time, "4" will be returned,
//     if the time signature is cut-time, "2" will be returned.
//

int ScoreItem::getTimeSignatureTop(void) {
   if (!isTimeSignatureItem()) {
      return 0;
   }

   int beats = getP5Int();
   int beattype = getP6Int();

   if (beattype == 0) {
      // Don't know what to do, so just ignore for now.
      // This is a single number (P5) probably used with another
      // time signature after it.
      return 0;
   }

   if ((beats == 99) && (beattype == 1)) {
      // Common time
      beats = 4;
      beattype = 4;
   } else if ((beats == 98) && (beattype == 1)) {
      // Cut time
      beats = 2;
      beattype = 2;
   }

   return beats;
}



//////////////////////////////
//
// ScoreItem::getTimeSignatureBottom -- Returns the bottom number in a time
//     signature.  If the time signature is common time, "4" will be returned,
//     if the time signature is cut-time, "2" will be returned.
//

int ScoreItem::getTimeSignatureBottom(void) {
   if (!isTimeSignatureItem()) {
      return 0;
   }
   int beats = getP5Int();
   int beattype = getP6Int();

   if (beattype == 0) {
      // Don't know what to do, so just ignore for now.
      // This is a single number (P5).
      return 0;
   }

   if ((beats == 99) && (beattype == 1)) {
      // Common time
      beats = 4;
      beattype = 4;
   } else if ((beats == 98) && (beattype == 1)) {
      // Cut time
      beats = 2;
      beattype = 2;
   }

   return beattype;
}



//////////////////////////////
//
// ScoreItem::isCommonTime -- Returns true if a common time symbol which
// typically represents 4/4 time signature.
//

bool ScoreItem::isCommonTime(void) {
   if (!isTimeSignatureItem()) {
      return false;
   }
   int beats = getP5Int();
   int beattype = getP6Int();

   if ((beats == 99) && (beattype == 1)) {
      return true;
   }

   return false;
}



//////////////////////////////
//
// ScoreItem::isCutTime -- Returns true if a cut time symbol which
// typically represents 2/2 time signature.
//

bool ScoreItem::isCutTime(void) {
   if (!isTimeSignatureItem()) {
      return false;
   }
   int beats = getP5Int();
   int beattype = getP6Int();

   if ((beats == 98) && (beattype == 1)) {
      return true;
   }

   return false;
}



//////////////////////////////
//
// ScoreItem::isSingleNumber -- Return true if P5 is 0 and P6 is not.
//     This represents a single number in the time signature.
//

bool ScoreItem::isSingleNumber(void) {
   if (!isTimeSignatureItem()) {
      return false;
   }
   int p5 = getP5Int();
   int p6 = getP6Int();
   if ((p5 == 0) && (p6 > 0)) {
      return true;
   }
   return false;
}



