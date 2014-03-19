//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  8 13:21:35 PST 2014
// Filename:      ScorePageBase_AnalysisInfo.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageBase_AnalysisInfo.cpp
// Syntax:        C++11
//
// Description:   Support class for structural analyses in ScorePageBase.
//


#include "ScorePageBase_AnalysisInfo.h"

using namespace std;


int AnalysisInfo::database_initialized = 0;
DatabaseAnalysis AnalysisInfo::database;


//////////////////////////////
//
// AnalysisInfo::AnalysisInfo -- Constructor.
//

AnalysisInfo::AnalysisInfo(void) {
   if (!database_initialized) {
      initializeDatabase();
      database_initialized = 1;
   }
   clear();
}


AnalysisInfo::AnalysisInfo(const AnalysisInfo& info) {
   *this = info;
}



//////////////////////////////
//
// AnalysisInfo::AnalysisInfo -- Destructor.
//

AnalysisInfo::~AnalysisInfo() {
   clear();
}



//////////////////////////////
//
// AnalysisInfo::operator= --
//

AnalysisInfo& AnalysisInfo::operator=(const AnalysisInfo& info) {
   if (this == &info) {
      return *this;
   }
   notmodified    = info.notmodified;
   sorted         = info.sorted;
   staves         = info.staves;
   duration       = info.duration;
   systems        = info.systems;
   systempitches  = info.systempitches;
   staffslursties = info.staffslursties;
   chords         = info.chords;
   layers         = info.layers;
   return *this;
}



//////////////////////////////
//
// AnalysisInfo::clear -- Invalidate all analyses which have been
//     done on the page data.
//

void AnalysisInfo::clear(void) {
   notmodified    = 0;
   sorted         = 0;
   staves         = 0;
   duration       = 0;
   systems        = 0;
   systempitches  = 0;
   staffslursties = 0;
   chords         = 0;
   layers         = 0;
}



//////////////////////////////
//
// AnalysisInfo::invalidateModified --
//

void AnalysisInfo::invalidateModified(void) {
   clear();
}



//////////////////////////////
//
// Tests to see if various analyses have been done.
//

int AnalysisInfo::isModified             (void) { return !notmodified; }
int AnalysisInfo::sortedIsValid          (void) { return sorted; }
int AnalysisInfo::stavesIsValid          (void) { return staves; }
int AnalysisInfo::durationIsValid        (void) { return duration; }
int AnalysisInfo::systemsIsValid         (void) { return systems; }
int AnalysisInfo::systemPitchesIsValid   (void) { return systempitches; }
int AnalysisInfo::staffSlursTiesIsValid  (void) { return staffslursties; }
int AnalysisInfo::chordsIsValid          (void) { return chords; }
int AnalysisInfo::beamsIsValid           (void) { return beams; }
int AnalysisInfo::barlinesIsValid        (void) { return barlines; }
int AnalysisInfo::layersIsValid          (void) { return layers; }



//////////////////////////////
//
// AnalysisInfo::invalidate -- Set to false all variables related
//    to the given node and its children.
//

void AnalysisInfo::invalidate(const string& nodename) {
   database.invalidateNode(nodename);
}

//
// Alias:
//

void AnalysisInfo::setInvalid(const string& nodename) {
   invalidate(nodename);
}



//////////////////////////////
//
// AnalysisInfo::validate -- Set to true all variables related
//    to the given node and its children.
//

void AnalysisInfo::validate(const string& nodename) {
   database.validateNode(nodename);
}

//
// Alias:
//

void AnalysisInfo::setValid(const string& nodename) {
   validate(nodename);
}



///////////////////////////////////////////////////////////////////////////
//
// Private functions.
//

//////////////////////////////
//
// AnalysisInfo::initializeDatabase -- set up the dependencies between
// analysises.
//

void AnalysisInfo::initializeDatabase(void) {
   database.addNode("notmodified", &notmodified);

   database.addChild("notmodified", "sorted",           &sorted);
   database.addChild("sorted",      "staves",           &staves);
   database.addChild("staves",      "chords",           &chords);
   database.addChild("staves",      "duration",         &duration);
   database.addChild("staves",      "staffslursties",   &staffslursties);
   database.addChild("staves",      "systems",          &systems);
   database.addChild("systems",     "barlines",         &barlines);
   database.addChild("systems",     "systempitches",    &systempitches);
   database.addChild("barlines",    "layers",           &layers);
   database.addChild("staves",      "chords",           &chords);
   database.addChild("duration",    "beams",            &beams);
}



