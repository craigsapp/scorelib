//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 23 15:25:19 PDT 2014
// Last Modified: Wed Apr 23 15:25:21 PDT 2014
// Filename:      SystemMeasure.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/SystemMeasure.cpp
// Syntax:        C++11
//
// Description:   Used in analyzeBarlines() in ScorePage class.  This class
//                stores all ScoreItems present in a measure of music.


#include "SystemMeasure.h"
#include "ScoreUtility.h"

using namespace std;


//////////////////////////////
//
// SystemMeasure::SystemMeasure --
//

SystemMeasure::SystemMeasure(void) {
   measure_duration  = -1.0;
   system_offset     = -1.0;
}



//////////////////////////////
//
// SystemMeasure::~SystemMeasure --
//

SystemMeasure::~SystemMeasure(void) {
   clear();
}



//////////////////////////////
//
// SystemMeasure::clear --
//

void SystemMeasure::clear(void) {
   start_bars.resize(0);
   end_bars.resize(0);
   measure_items.resize(0);
   measure_duration = -1.0;
   system_offset    = -1.0;
}



//////////////////////////////
//
// SystemMeasure::addItem --
//

void SystemMeasure::addItem(ScoreItem* item) {
   SCORE_FLOAT offset = item->getStaffOffsetDuration();
   if (system_offset < 0.0) {
      system_offset = offset;
   }

   SCORE_FLOAT idur   = item->getDuration();
   idur = SU::increaseDurationPrecision(idur);
   SCORE_FLOAT curdur = (offset + idur) - system_offset;
   if (measure_duration < curdur) {
      measure_duration = curdur;
      measure_duration = SU::increaseDurationPrecision(measure_duration);
   }

   if (!item->isBarlineItem()) {
      measure_items.push_back(item);
      return;
   }

   if (curdur == 0.0) {
      start_bars.push_back(item);
      measure_items.push_back(item);
   } else {
      end_bars.push_back(item);
   }
}



//////////////////////////////
//
// SystemMeasure::getSystemOffsetDuration --
//

SCORE_FLOAT SystemMeasure::getSystemOffsetDuration(void) {
   return system_offset;
}



//////////////////////////////
//
// SystemMeasure::getMeasureDuration --
//

SCORE_FLOAT SystemMeasure::getMeasureDuration(void) {
   if (measure_duration - (int)measure_duration < 0.0001) {
      measure_duration = (int)measure_duration;
   }
   return measure_duration;
}

//
// Alias:
//

SCORE_FLOAT SystemMeasure::getDuration(void) {
   return getMeasureDuration();
}



//////////////////////////////
//
// SystemMeasure::getStartBarlines --
//

vectorSIp& SystemMeasure::getStartBarlines(void) {
   return start_bars;
}



//////////////////////////////
//
// SystemMeasure::getEndBarlines --
//

vectorSIp& SystemMeasure::getEndBarlines(void) {
   return end_bars;
}



//////////////////////////////
//
// SystemMeasure::operator[] --
//

ScoreItem* SystemMeasure::operator[](int index) {
   return measure_items[index];
}



//////////////////////////////
//
// SystemMeasure::getSize -- Return the number of ScoreItems in the main list.
//

int SystemMeasure::getSize(void) {
   return measure_items.size();
}



//////////////////////////////
//
// SystemMeasure::getItems --
//

vectorSIp& SystemMeasure::getItems(void) {
   return measure_items;
}



//////////////////////////////
//
// SystemMeasure::getP3Width -- return the difference in P3
//     between the first barline and the last barline (or item P3 if no
//     barline at end).
//

SCORE_FLOAT SystemMeasure::getP3Width(void) {
   return getP3Right() - getP3();
}

//
// Aliases:
//

SCORE_FLOAT SystemMeasure::getHorizontalWidth(void) {
   return getP3Width();
}


SCORE_FLOAT SystemMeasure::getHWidth(void) {
   return getP3Width();
}


//////////////////////////////
//
// SystemMeasure::getP3Right --
//

SCORE_FLOAT SystemMeasure::getP3Right(void) {
   if (end_bars.size() > 0) {
      return end_bars.back()->getHPos();
   } else if (measure_items.size() > 0) {
      return measure_items.back()->getHPos();
   } else {
      return 200.0;
   }
}

//
// Aliases:
//

SCORE_FLOAT SystemMeasure::getHorizontalPositionRight(void) {
   return getP3Right();
}


SCORE_FLOAT SystemMeasure::getHPosRight(void) {
   return getP3Right();
}



//////////////////////////////
//
// SystemMeasure::getP3 -- Return left horizontal position
//     of the starting barline of the measure.
//

SCORE_FLOAT SystemMeasure::getP3(void) {
   if (start_bars.size() > 0) {
      return start_bars[0]->getHPos();
   } else if (measure_items.size() > 0) {
      return measure_items[0]->getHPos();
   } else {
      return 0.0;
   }
}

//
// Aliases:
//

SCORE_FLOAT SystemMeasure::getP3Left(void) {
   return getP3();
}


SCORE_FLOAT SystemMeasure::getHorizontalPosition(void) {
   return getP3();
}


SCORE_FLOAT SystemMeasure::getHorizontalPositionLeft(void) {
   return getP3();
}


SCORE_FLOAT SystemMeasure::getHPosLeft(void) {
   return getP3();
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& out, SystemMeasure& measure) {
   out << measure.getItems();
   return out;
}



