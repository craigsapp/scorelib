//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Thu Feb  6 01:12:14 PST 2014
// Filename:      ScoreItem.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem.cpp
// Syntax:        C++11
//
// Description:   The ScoreItem class stores one SCORE item as well as
//                adds key/value named paramters for analysis and extensions.
//                This class adds manipulation to the ScoreItemEdit /
//                ScoreItemBase classes.
//

#include "ScoreItem.h"
#include <math.h>

using namespace std;


//////////////////////////////
//
// ScoreItem::ScoreItem -- constructor.
//

#ifdef SCOREITEMEDIT

   ScoreItem::ScoreItem(void) : ScoreItemEdit()  {
      // do nothing
   }


   ScoreItem::ScoreItem(const ScoreItem& anItem) : 
         ScoreItemEdit((ScoreItemEdit)anItem)  {
      // do nothing 
   }


   ScoreItem::ScoreItem(const ScoreItemBase& anItem) : 
         ScoreItemBase(anItem)  {
      // do nothing 
   }


   ScoreItem::ScoreItem(const vectorSF& parameters) : 
         ScoreItemEdit(parameters)  {
      // do nothing 
   }

#else

   ScoreItem::ScoreItem(void) : ScoreItemBase()  {
      // do nothing
   }


   ScoreItem::ScoreItem(const ScoreItem& anItem) : 
         ScoreItemBase((ScoreItemBase)anItem)  {
      // do nothing 
   }


   ScoreItem::ScoreItem(const ScoreItemBase& anItem) : 
         ScoreItemBase(anItem)  {
      // do nothing 
   }


   ScoreItem::ScoreItem(const vectorSF& parameters) : 
         ScoreItemBase(parameters)  {
      // do nothing 
   }

#endif


//////////////////////////////
//
// ScoreItem::ScoreItem -- deconstructor.
//

ScoreItem::~ScoreItem() {
   // do nothing
}



//////////////////////////////
//
// ScoreItem::operator= --
//

ScoreItem& ScoreItem::operator=(const ScoreItem& anItem) {
   if (this == &anItem) {
      return *this;
   }

   #ifdef SCOREITEMEDIT
      *(ScoreItemEdit*)this = (ScoreItemEdit)anItem;
   #else
      *(ScoreItemBase*)this = (ScoreItemBase)anItem;
   #endif 

   return *this;
}



//////////////////////////////
//
// ScoreItem::getParameterDigit -- 
//

int ScoreItem::getParameterDigit(int pindex, int position) {
   return int(fabs(getP(pindex)) / (SCORE_FLOAT)pow(10, position)) % 10;
}


int ScoreItem::getPDigit(int pindex, int position) {
   return getParameterDigit(pindex, position);
}



//////////////////////////////
//
// ScoreItem::setParameterDigit -- 
//

void ScoreItem::setParameterDigit(int pindex, int position, int value) {
   SCORE_FLOAT newvalue = getP(pindex);
   int sign = 1;
   if (newvalue < 0) {
      newvalue = -newvalue;
      sign = -1;
   }
   int olddigit = (int(newvalue / (SCORE_FLOAT)pow(10, position)) % 10);
   newvalue -= olddigit * pow(10, position);
   newvalue += value * pow(10, position);
   setP(pindex, newvalue*sign);
}


void ScoreItem::setPDigit(int pindex, int position, int value) {
   setParameterDigit(pindex, position, value);
}



//////////////////////////////
//
// ScoreItem::getItemType -- Return the int portion of P1 values.
//

int ScoreItem::getItemType(void) {
   return (int)getParameter(P1);
}



//////////////////////////////
//
// ScoreItem::getStaffNumber -- return P2 value.
//

unsigned int ScoreItem::getStaffNumber(void) {
   int value = (int)getParameter(P2);
   if (value < 0) {
      return 0;
   } else {
      return (unsigned int)value;
   }
}



//////////////////////////////
//
// ScoreItem::getHorizontalPosition -- return P3 value.
//

SCORE_FLOAT ScoreItem::getHorizontalPosition(void) {
   return getParameter(P3);
}

// Alias:

SCORE_FLOAT ScoreItem::getHPos(void) {
   return getHorizontalPosition();
}



//////////////////////////////
//
// ScoreItem::getVerticalPosition -- return P4 value, excluding
//    100's digit or higher.
//

SCORE_FLOAT ScoreItem::getVerticalPosition(void) {
   SCORE_FLOAT value = getParameter(P4);
   if ((value < 100.0) && (value > -100.0)) {
      return value;
   }

   // remove 100's and higher digits.
   SCORE_FLOAT sign;
   if (value < 0.0) {
      sign = -1.0;
      value = -value;
   } else {
      sign = 1.0;
   }

   int high = int(value / 100) * 100;
   return sign * (value - high);
}

// Alias:

SCORE_FLOAT ScoreItem::getVPos(void) {
   return getVerticalPosition();
}


///////////////////////////////////////////////////////////////////////////
//
// Duration processing functions:
//


//////////////////////////////
//
// ScoreItem::hasDuration -- Returns true if the items posses a duration
//   parameters.  This includes Notes (P1=1) and Rests (P1=2). 
//

bool ScoreItem::hasDuration(void) {
   int p1 = getPInt(P1);
   if ((p1 == 1) || (p1 == 2)) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::getDuration -- returns the duration of the item.
//    Returns 0.0 if the item is not a note or rest.  If the 
//    P7 value of notes/rests is greater than 60, then return 0
//    (since it represents a grace note).
//

SCORE_FLOAT ScoreItem::getDuration(void) {
   if (!hasDuration()) {
      return 0.0;
   }

   SCORE_FLOAT p7 = getP(P7);
   if (p7 > SCORE_MAX_DURATION) {
      return 0.0;
   } else if (p7 < 0.0) {
      return 0.0;
   } else {
      return p7;
   }
}



//////////////////////////////
//
// ScoreItem::setStaffOffsetDuration -- set the durational offset from the
//    start of the owning staff to the P3 position of this ScoreItem.
//

void ScoreItem::setStaffOffsetDuration(SCORE_FLOAT duration) {
//   staff_duration_offset = duration;
   setParameter("analysis", "staffOffsetDuration", duration);
}


//////////////////////////////
//
// ScoreItem::getStaffOffsetDuration -- returns the durational offset
//      of the ScoreItem.  The value with be negative (-1.0) if it is
//      undefined/unanalyzed.  It will be 0.0 if at the start of a staff,
//      or positive if comming after any duration-posessing items (P1=1|2).
//

SCORE_FLOAT ScoreItem::getStaffOffsetDuration(void) {
//   return staff_duration_offset;
   return getParameterDouble("analysis", "staffOffsetDuration");
}




