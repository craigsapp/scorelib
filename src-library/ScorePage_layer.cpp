//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  8 12:03:53 PDT 2014
// Last Modified: Tue Jul  8 12:03:55 PDT 2014
// Filename:      ScorePage_layer.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_layer.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                melodic layer analysis.
//

#include "ScorePage.h"
#include <algorithm>
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeLayers -- Calulate the durations of each
//      staff on the page.  This function also assigns a durational
//      offset to each item on the staff as a side effect.
//

int ScorePage::analyzeLayers(void) {
   if (!analysis_info.durationIsValid()) {
      analyzeStaffDurations();
   }
   analysis_info.setInvalid("layers");

   vectorVSIp staffsequence;
   getHorizontallySortedStaffItems(staffsequence);

   unsigned int i;
   for (i=1; i<staffsequence.size(); i++) {
      if (staffsequence[i].size() == 0) {
         // nothing on staff
         continue;
      }
      private_analyzeStaffLayers(staffsequence[i]);
   }

   analysis_info.setValid("layers");
   return 1;
}



//////////////////////////////
//
// ScorePage::private_analyzeStaffLayers --
//
// * notes beamed together are in the same layer
//

void ScorePage::private_analyzeStaffLayers(vectorSIp& items) {
   vectorVSIp notes;
   notes.reserve(100);
   notes.resize(1);
   int i;
   SCORE_FLOAT offset;
   SCORE_FLOAT current = 0.0;
   SCORE_FLOAT tolerance = 0.001;
   SCORE_FLOAT dur;
   ScoreItem* sip;

   for (i=0; i<items.size(); i++) {
      if (!items[i]->hasDuration()) {
         continue;
      }
      sip = items[i];
      dur = sip->getDuration();
      if (dur <= 0.0) {
         // don't deal with grace notes at the moment...
         continue;
      }
      if (sip->isInvisible()) {
         // don't deal with invisible rests for now...
         continue;
      }
      if (sip->isSecondaryChordNote()) {
         // chord notes are all in the same layer.  The
         // primary chord note will set the secondary notes
         // in the chord.
         continue;
      }
      offset = sip->getStaffOffsetDuration();
      if (fabs(offset - current) < tolerance) {
         // append note to current array position
         notes.back().push_back(sip);
      } else{
         // add new rhythm cell onto list
         notes.emplace_back();
         notes.back().push_back(sip);
         current = offset;
      }
   }

   int maxlayers = 0;
   for (i=0; i<notes.size(); i++) {
      if (notes[i].size() > (unsigned int)maxlayers) {
         maxlayers = notes[i].size();
      }
   }

   int layer = 1;
   // assign the first layer.
   for (i=0; i<notes.size(); i++) {
      if (notes[i].size() == 1) {
         setChordLayer(notes[i][0], layer);
      } else {
         chooseLayer(notes[i], layer);
      }
   }

   // assign secondary layers
   // loop from layer=2 to layer=maxlayers first searching for the
   // first non-assigned notes then tracking them across the staff
   // by duration.

}



/////////////////////////////
//
// ScorePage::chooseLayer -- Determine the note which should be assigned
//     to the given layer.
//

void ScorePage::chooseLayer(vectorSIp& notes, int layer) {

   // do something here.

}



/////////////////////////////
//
// ScorePage::setChordLayer -- Set the layer for a note, and any of the secondary
//    chord notes attached to the note.
//

void ScorePage::setChordLayer(ScoreItem* note, int layer) {
   int i;
   vectorSIp* chordnotes = chordNotes(note);
   if (chordnotes == NULL) {
      note->setParameterQuiet(ns_auto, np_layer, layer);
      return;
   }
   for (i=0; i<chordnotes->size(); i++) {
      (*chordnotes)[i]->setParameterQuiet(ns_auto, np_layer, layer);
   }
}



