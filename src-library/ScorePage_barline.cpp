//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 23 14:02:51 PDT 2014
// Last Modified: Wed Apr 23 14:02:54 PDT 2014
// Filename:      ScorePage_barline.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_barline.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                system barline analysis.
//

#include "ScorePage.h"

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeBarlines -- Determine if notes are parts of chords.
//     Chords across multiple staves are not considered by this analysis yet.
//

void ScorePage::analyzeBarlines(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   if (!analysis_info.durationIsValid()) {
      analyzeStaffDurations();
   }
   analysis_info.invalidate("barlines");

   int systemcount = getSystemCount();

   vectorVSMp& sysmeasures = system_measures;
   listSMp& mstorage = measure_storage;

   for (auto& it : mstorage) {
      if (it != NULL) {
         delete it;
         it = NULL;
      }
   }
   mstorage.resize(0);

   sysmeasures.resize(systemcount);
   for (auto& it : sysmeasures) {
      it.resize(0);
   }

   for (int i=0; i<systemcount; i++) {
      analyzeBarlines(sysmeasures[i], i);
   }

   analysis_info.validate("barlines");
}



//////////////////////////////
//
// ScorePage::getSystemBarCount -- return the number of measures
//    on a system;
//

int ScorePage::getSystemBarCount(int sysindex) {
   if (!analysis_info.barlinesIsValid()) {
      analyzeBarlines();
   }
   return system_measures[sysindex].size();
}

//
// Alias:
//

int ScorePage::getSystemMeasureCount(int sysindex) {
   return getSystemBarCount(sysindex);
}



//////////////////////////////
//
// ScorePage::getSystemMeasures --
//

vectorSMp& ScorePage::getSystemMeasures(int sysindex) {
   if (!analysis_info.barlinesIsValid()) {
      analyzeBarlines();
   }
   return system_measures[sysindex];
}



//////////////////////////////
//
// ScorePage::getSystemMeasure --
//

SystemMeasure& ScorePage::getSystemMeasure(int sysindex, int measureindex) {
   if (!analysis_info.barlinesIsValid()) {
      analyzeBarlines();
   }
   return *system_measures[sysindex][measureindex];
}


///////////////////////////////////////////////////////////////////////////
//
// Private functions --
//

//////////////////////////////
//
// ScorePage::analyzeBarlines -- Segment the data on a particular system by
//      barlines present in the music.
//

void ScorePage::analyzeBarlines(vectorSMp& sysmeasures, int sysindex) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   if (!analysis_info.durationIsValid()) {
      analyzeStaffDurations();
   }

   sysmeasures.resize(0);  // memory leak if not already zero...
   listSMp& mstorage = measure_storage;

   vectorSIp sysitems;
   getHorizontallySortedSystemItems(sysitems, sysindex);

   // create a new measure and store in system list
   SystemMeasure* smp = new SystemMeasure;
   mstorage.push_back(smp);
   sysmeasures.push_back(smp);
   int currentbar = 0;
   SCORE_FLOAT currentdur = 0.0;
   int i;

   for (i=0; i<sysitems.size(); i++) {
      sysmeasures[currentbar]->addItem(sysitems[i]);
      if (!sysitems[i]->isBarlineItem()) {
         continue;
      }

      currentdur = sysmeasures[currentbar]->getDuration();

      if (currentdur > 0.0) {
         // add another measure to the list, and start adding barlines
         // (also add barlines to previous measure until a duration
         // item is added.
         smp = new SystemMeasure;
         mstorage.push_back(smp);
         sysmeasures.push_back(smp);
         currentdur = 0.0;
         currentbar++;
         sysmeasures[currentbar]->addItem(sysitems[i]);
         continue;
      }

      if ((currentdur <= 0.0) && (currentbar > 0)) {
         // also add to end of last measure
         sysmeasures[currentbar-1]->addItem(sysitems[i]);
      }
   }
}



