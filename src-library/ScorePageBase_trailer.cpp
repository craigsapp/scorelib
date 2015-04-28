//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 02:51:26 PST 2014
// Filename:      ScorePageBase_trailer.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase_trailer.cpp
// Syntax:        C++11
//
// Description:   Binary file trailer processing functions for ScorePageBase.
//

#include "ScorePageBase.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// ScorePageBase trailer accessing functions.  The SCORE binary file trailer
// typically has 6 float values stored in reverse order (0) is the last
// value in the file:
//
// 0: -9999.0 which is the end of file marker for a SCORE binary file.
// 1: The size of the trailer after index 0 (including this value).  Typically
//    this is 5.0 but may be larger in later versions of score.
// 2: measurement units: 0.0 = inches, 1.0 = centimeters.
// 3: Program version number
// 4: Program serial number
// 5: 0.0 which means end of trailer.  This is the first 4-byte parameter
//    in the trailer stored in a binary file.  Typically this value would
//    be a non-zero parameter count for a SCORE item.  However, when a zero
//    is read for the parameter size, this means that the trailer begins
//    here (and must be read backwards to determine the size and meaning
//    of the fields in the trailer.
//

//////////////////////////////
//
// ScorePageBase::setVersionWinScore -- set version to WinScore.
//

void ScorePageBase::setVersionWinScore(void) {
   if (trailer.size() < 4) {
      trailer.resize(4, 0.0);
   }
   trailer[3] = 6.0;
}



//////////////////////////////
//
// ScorePageBase::getVersion -- Get the SCORE version that was red from
//      a binary SCORE file. Return 4.0 if undefined.
//

SCORE_FLOAT ScorePageBase::getVersion(void) {
   if (trailer.size() < 4) {
      return 4.0;
   } else {
      return trailer[3];
   }
}



//////////////////////////////
//
// ScorePageBase::setSerialNumber -- Set the SCORE serial number for writing in
//      binary SCORE files.
//

void ScorePageBase::setSerialNumber(uint32_t value) {
   union { uint32_t i; float f; } u;
   u.i = value;
   if (trailer.size() < 5) {
      trailer.resize(5, 0.0);
   }
   trailer[4] = u.f;
}


inline void ScorePageBase::setSerial(uint32_t value) {
   setSerialNumber(value);
}



//////////////////////////////
//
// ScorePageBase::getSerial -- Get the SCORE serial number that was read
//      from a binary SCORE file.  This value may be 0.0 if SCORE version 3
//      or less.
//

uint32_t ScorePageBase::getSerialNumber(void) {
   union { uint32_t i; float f; } u;
   if (trailer.size() > 4) {
      u.f = trailer[4];
      return u.i;
   } else {
      return 0;
   }
}


// Alias:

inline uint32_t ScorePageBase::getSerial(void) {
   return getSerialNumber();
}


//////////////////////////////
//
// ScorePageBase::getUnits -- return 0.0 for inches (or undefined),
//     or 1.0 for centimeters.  These units are neede for certain types
//     of staff vertical placement types.
//

SCORE_FLOAT ScorePageBase::getUnits(void) {
   if (trailer.size() > 2) {
      return trailer[2];
   } else {
      return 0.0;
   }
}



//////////////////////////////
//
// ScorePageBase::setUnitsInches -- Sets the trailer parameter for
//     units to 0.0 for inches.
//

void ScorePageBase::setUnitsInches(void) {
   if (trailer.size() > 2) {
      trailer[2] = 0.0;
   } else {
      trailer.resize(3, 0.0);
   }
}



//////////////////////////////
//
// ScorePageBase::setUnitsCentimeters -- Sets the trailer parameter for
//     units to 1.0 for centimeters.
//

void ScorePageBase::setUnitsCentimeters(void) {
   if (trailer.size() < 2) {
      trailer.resize(3, 0.0);
   }
   trailer[2] = 1.0;
}


