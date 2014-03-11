//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 14:30:07 PDT 2014
// Last Modified: Tue Mar 11 00:38:11 PDT 2014
// Filename:      ScorePage_chord.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_chord.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff chord analysis.
//

#include "ScorePage.h"
#include <algorithm>
#include <set>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// Staff pitch analysis related functions
//

//////////////////////////////
//
// ScorePage::analyzeChords -- Determine if notes are parts of chords.
//     Chords across multiple staves are not considered by this analysis yet.
//

void ScorePage::analyzeChords(void) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   analysis_info.invalidate("chords");

   int i;
   int maxstaff = getMaxStaff();
   for (i=1; i<=maxstaff; i++) {
      analyzeChordsOnStaff(i);
   }

   analysis_info.validate("chords");
}



//////////////////////////////
//
// ScorePage::chordNotes -- Given a ScoreItem pointer to a note,
//    this function returns a list of chord notes if the input
//    note belongs to a chord.  Will return an empty list if the
//    note is not part of a chord.
//

vectorSIp&  ScorePage::chordNotes(ScoreItem* chordtone) {
   if (!analysis_info.chordsIsValid()) {
      analyzeChords();
   }
   return chord_database.notelist(chordtone);
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// ScorePage::analyzeChordsOnStaff -- P3 value of chord items must match
//     exactly.  Might need to figure out a tolerance.
//

void ScorePage::analyzeChordsOnStaff(int p2index) {
   if (!analysis_info.stavesIsValid()) {
      analyzeStaves();
   }
   vectorSIp& staffitems = itemlist_staffsorted[p2index];
   map<SCORE_FLOAT, vectorSIp> p3list;
   SCORE_FLOAT p3;
   for (auto it = staffitems.begin(); it != staffitems.end(); it++) {
      if (!(*it)->isNoteItem()) {
         continue;
      }
      p3 = (*it)->getHPos();
      p3list[p3].push_back((*it));
   }

   for (auto it = p3list.begin(); it != p3list.end(); it++) {
      analyzeVerticalNoteSet(it->second);
   }
}



//////////////////////////////
//
// ScorePage::analyzeVerticalNoteSet -- group notes into chords.
//
//

void ScorePage::analyzeVerticalNoteSet(vectorSIp& data) {
   if (data.size() <= 1) {
      // no chords if there is only one note.
      return;
   }

   vectorSIp unstemmed_notes;
   vectorSIp stemmed_notes;
   vectorSF  minV;
   vectorSF  maxV;
   vectorSF  offset;

   for (auto it = data.begin(); it != data.end(); it++) {
      if ((*it)->hasStem()) {
         stemmed_notes.push_back(*it);
         minV.push_back((*it)->getStemBottomVPos());
         maxV.push_back((*it)->getStemTopVPos());
         offset.push_back((*it)->getP(P10));
      } else {
         unstemmed_notes.push_back(*it);
      }
   }

   if (unstemmed_notes.empty()) {
      // chords are only identified when any note does not have a stem.
      // If the notes have stems with the stems overlapping, then these
      // are not currently viewed as being a chord, but rather separate
      // chords (could be changed later).
      return;
   }

   // currently not checking P10, but that should be added:

   unsigned int i;
   SCORE_FLOAT vpos;
   for (auto it = unstemmed_notes.begin(); it != unstemmed_notes.end(); it++) {
      for (i=0; i<stemmed_notes.size(); i++) {
         vpos = (*it)->getVPos();
         if ((vpos >= minV[i]) && (vpos <= maxV[i])) {
            // The stemless note falls on the stem of the stemmed note,
            // so link them together:
            chord_database.linkNotes(stemmed_notes[i], *it);
         }
      }
   }
}



