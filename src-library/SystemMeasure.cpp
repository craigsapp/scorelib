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
   SCORE_FLOAT curdur = (offset + idur) - system_offset;
   if (measure_duration < curdur) {
      measure_duration = curdur;
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
// operator<< --
//

ostream& operator<<(ostream& out, SystemMeasure& measure) {
   out << measure.getItems();
   return out;
}



