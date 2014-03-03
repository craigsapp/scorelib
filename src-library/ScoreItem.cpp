//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Thu Feb  6 01:12:14 PST 2014
// Filename:      ScoreItem.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem.cpp
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

// Alias:

void ScoreItem::setPDigit(int pindex, int position, int value) {
   setParameterDigit(pindex, position, value);
}



//////////////////////////////
//
// ScoreItem::setParameterIntegerPart -- set the integer portion of
//    the given fixed parameter value.  The fractional part will remain
//    the same.  Currently the sign of the value will be that of the 
//    integer being inserted.
//

void ScoreItem::setParameterIntegerPart(int pindex, int intval) {
   SCORE_FLOAT value = getParameter(pindex);
   // SCORE_FLOAT sign = 1.0;
   if (value < 0.0) {
      value = -value;
   //    sign = -1.0;
   }
   SCORE_FLOAT result = (int)value - value;
   if (intval < 0) {
      result -= intval;
   } else {
      result += intval;
   }

   setParameter(pindex, result);
}

// Alias:

void ScoreItem::setPIntPart(int pindex, int intval) {
   setParameterIntegerPart(pindex, intval);
}


///////////////////////////////////////////////////////////////////////////
//
// P1 processing functions.
//

//////////////////////////////
//
// ScoreItem::getItemType -- Return the int portion of P1 values.
//

int ScoreItem::getItemType(void) {
   return (int)getParameter(P1);
}



//////////////////////////////
//
// ScoreItem::is*Item -- returns true if the integer part of the
//    P1 value matches the item type related to the functions.
//

bool ScoreItem::isNoteItem   (void) 
      { return getItemType() == P1_Note               ? true : false; }
bool ScoreItem::isRestItem   (void) 
      { return getItemType() == P1_Rest               ? true : false; }
bool ScoreItem::isClefItem   (void) 
      { return getItemType() == P1_Clef               ? true : false; }
bool ScoreItem::isLineItem   (void) 
      { return getItemType() == P1_Line               ? true : false; }
bool ScoreItem::isSlurItem   (void) 
      { return getItemType() == P1_Slur               ? true : false; }
bool ScoreItem::isBeamItem   (void) 
      { return getItemType() == P1_Beam               ? true : false; }
bool ScoreItem::isTrillItem  (void) 
      { return getItemType() == P1_Trill              ? true : false; }
bool ScoreItem::isStaffItem  (void) 
      { return getItemType() == P1_Staff              ? true : false; }
bool ScoreItem::isSymbolItem (void) 
      { return getItemType() == P1_Symbol             ? true : false; }
bool ScoreItem::isNumberItem (void) 
      { return getItemType() == P1_Number             ? true : false; }
bool ScoreItem::isUserItem   (void) 
      { return getItemType() == P1_User               ? true : false; }
bool ScoreItem::isSpecialItem(void) 
      { return getItemType() == P1_Special            ? true : false; }
bool ScoreItem::isBadLuckItem(void) 
      { return getItemType() == P1_BadLuck            ? true : false; }
bool ScoreItem::isBarlineItem(void) 
      { return getItemType() == P1_Barline            ? true : false; }
bool ScoreItem::isEpsItem    (void) 
      { return getItemType() == P1_ImportedEPSGraphic ? true : false; }
bool ScoreItem::isTextItem   (void) 
      { return getItemType() == P1_Text               ? true : false; }
bool ScoreItem::isKeySigItem (void) 
      { return getItemType() == P1_KeySignature       ? true : false; }
bool ScoreItem::isTimeSigItem(void) 
      { return getItemType() == P1_MeterSignature     ? true : false; }



///////////////////////////////////////////////////////////////////////////
//
// P2 related functions.
//

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

// Alias:

unsigned int ScoreItem::getStaffNum(void) {
   return getStaffNumber();
}



//////////////////////////////
//
// ScoreItem::setStaffNumber -- Set the staff number of the object.
//

void ScoreItem::setStaffNumber(int staffnum) {
   setParameterIntegerPart(P2, staffnum);
}

// Alias:

void ScoreItem::setStaffNum(int staffnum) {
   setStaffNumber(staffnum);
}



///////////////////////////////////////////////////////////////////////////
//
// P3 related functions.
//

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



///////////////////////////////////////////////////////////////////////////
//
// P4 related functions.
//

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




