//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  8 00:03:52 PST 2014
// Filename:      ScorePageBase_AnalysisInfo.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageBase_AnalysisInfo.h
// Syntax:        C++11
//
// Description:   Support class for structural analyses in ScorePageBase.
//

#ifndef _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED
#define _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED

#include "ScoreItem.h"
#include "DatabaseAnalysis.h"
#include <iostream>

using namespace std;


class AnalysisInfo {

   friend class DatabaseAnalysis;

   public:
                    AnalysisInfo             (void);
                    AnalysisInfo             (const AnalysisInfo& info);
                   ~AnalysisInfo             (void);

      AnalysisInfo& operator=                (const AnalysisInfo& info);
      void          clear                    (void);

      void          invalidateModified       (void);
      ostream&      print                    (ostream& out = cout);

      // Tests to see if various analyses have been done.
      int           isModified               (void);
      int           sortedIsValid            (void);
      int           stavesIsValid            (void);
      int           durationIsValid          (void);
      int           systemsIsValid           (void);
      int           systemPitchesIsValid     (void);
      int           staffSlursTiesIsValid    (void);
      int           staffTiesIsValid         (void);
      int           staffSlursIsValid        (void);
      int           chordsIsValid            (void);
      int           beamsIsValid             (void);
      int           tupletsIsValid           (void);
      int           barlinesIsValid          (void);
      int           layersIsValid            (void);
      int           p3IsValid                (void);

      void          setValid                 (const string& node);
      void          setInvalid               (const string& node);
      void          invalidate               (const string& node);
      void          validate                 (const string& node);

   private:
      // notmodified: true if data has not been modified
      // (no ScoreItem parameter changes and no additions/deletions).
      // Dependencies for notmodified: none
      int notmodified;

      // sorted: true if the data on the page has been sorted by P3 value.
      // All staves will be interleaved into one stream.
      // Dependencies for sorted: notmodified
      int sorted;

      // staves: true if staff analysis has been done.  The data for each
      // staff will be separated into separate storage.
      // Dependencies for staves: sorted (notmodified)
      int staves;

      // duration: true if duration analysis has been done for each staff.
      // Dependencies for duration: staves (sorted, notmodified)
      int duration;

      // systems: true if system identification has been done.
      // Dependencies: staves (sorted, notmodified)
      int systems;

      // systempitches: true if pitch analysis has been done.
      // Dependencies: systems (staves, sorted, notmodified)
      int systempitches;

      // staffslursties: true if slurs/ties have been differentiated at the
      // staff level (slurs/ties across system breaks are not analyzed, this
      // is done at the ScoreSegment level).
      int staffslursties;

      // chords: true if notes have been grouped into chords.
      // Dependencies for chords: staves, (sorted, notmodified).
      int chords;

      // beams: true if beamed notes have been grouped into chords.
      // Dependencies for beams: staves, (sorted, notmodified).
      int beams;

      // beams: true if tuplet notes have been grouped.
      // Dependencies for beams: beams, (staves, sorted, notmodified).
      int tuplets;

      // barlines: true if barline analysis has been done.
      // Dependencies for barlines: systems, (staves, sorted, notmodified).
      int barlines;

      // layers: true if layers have been identified.
      // Dependencies for layers: staves, (sorted, notmodified).
      int layers;

      // p3: true if P3 analysis has been done.  This analysis 
      // organizes ScoreItems* into a list of P3 values.  The P3 analysis
      // also cross-indexes the P3 value and staff offset durations of 
      // items on a system.
      int p3;

   private:
      DatabaseAnalysis database;
      void          initializeDatabase       (void);
};



#endif /* _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED */

