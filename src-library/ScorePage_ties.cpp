//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul  8 23:04:49 PDT 2014
// Last Modified: Tue Jul  8 23:04:52 PDT 2014
// Filename:      ScorePage_tie.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_tie.cpp
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
// ScorePage::analyzeTies -- Identify ties on each staff, not checking
//      for ties which cross system breaks (that is handled at the
//      ScoreSegment level).
//

int ScorePage::analyzeTies(void) {
   if (!analysis_info.layersIsValid()) {
      analyzeLayers();
   }
   analysis_info.setInvalid("staffslursties");

   vectorVSIp staffsequence;
   getHorizontallySortedStaffItems(staffsequence);

   unsigned int i;
   for (i=1; i<staffsequence.size(); i++) {
      if (staffsequence[i].size() == 0) {
         // nothing on staff
         continue;
      }
      private_analyzeStaffTies(staffsequence[i]);
   }

   analysis_info.setValid("staffslursties");
   return 1;
}



//////////////////////////////
//
// ScorePage::private_analyzeStaffTies --
//

void ScorePage::private_analyzeStaffTies(vectorSIp& items) {
   vectorVSIp notes;
   vectorSIp  slurs;
   map<SCORE_FLOAT, int> offsettoindex;

   slurs.reserve(32);
   notes.reserve(100);
   notes.resize(1);
   int i;
   SCORE_FLOAT offset;
   SCORE_FLOAT current = 0.0;
   offsettoindex[0.0] = 0;
   SCORE_FLOAT tolerance = 0.001;
   SCORE_FLOAT dur;
   ScoreItem* sip;
   
   // first place notes
   for (i=0; i<items.size(); i++) {
      if (items[i]->isSlurItem()) {
         // Should already be selected for a single staff, but
         // being careful:
         if (items[i]->getP8() >= 0) {
            continue;
         }
         slurs.push_back(items[i]);
      }
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
         offsettoindex[offset] = notes.size();
      }
   }

   // Have a rhythmic list of notes and a list of slurs/ties.
   // Now go through each slur/tie and identify it's rhythmic position

   for (i=0; i<slurs.size(); i++) {
      identifySlurStartStopOffsets(i, slurs, notes, offsettoindex);
   }
}



//////////////////////////////
//
// ScorePage::identifySlurStartStopOffsets --
//

void ScorePage::identifySlurStartStopOffsets(int index, vectorSIp& slurs, 
      vectorVSIp& notes, map<SCORE_FLOAT, int>& offsettoindex) {

   ScoreItem* sip = slurs[index];

   int staffnum = sip->getStaffNum();
   int p2;
   SCORE_FLOAT starthpos = sip->getHPos();
   SCORE_FLOAT endhpos   = sip->getHPosRight();
   SCORE_FLOAT startvpos = sip->getVPos();
   SCORE_FLOAT endvpos   = sip->getVPosRight();
   SCORE_FLOAT hpos     = 0.0;
   SCORE_FLOAT lasthpos = -100.0;
   SCORE_FLOAT diff1, diff2;

   SCORE_FLOAT startoffset = -1000.0;
   SCORE_FLOAT endoffset   =  1000.0;

   int i, j;
   int startindex = -1;
   int endindex   = -1;
   int startQ = 0;
   int endQ   = 0;

   for (i=0; i<notes.size(); i++) {
      p2 = notes[i][0]->getStaffNumber();
      if (p2 != staffnum) {
         continue;
      }
      lasthpos = hpos;
      hpos = notes[i][0]->getHPos();

      if (!startQ) {
         if (hpos == starthpos) {
            startoffset = notes[i][0]->getParameterDouble(ns_auto, 
                     np_staffOffsetDuration);
            startQ = 1;
            startindex = i;
         } else if (hpos > starthpos) {
            diff1 = hpos - starthpos;
            diff2 = starthpos - lasthpos;

            if (diff1 < diff2) {
               startoffset = notes[i][0]->getParameterDouble(ns_auto, 
                     np_staffOffsetDuration);
               startQ = 1;
               startindex = i;
            } else if (i > 0) {
               startoffset = notes[i-1][0]->getParameterDouble(ns_auto, 
                     np_staffOffsetDuration);
               startQ = 1;
               startindex = i;
            } else {
               startoffset = 0.0;
               startQ = 1;
               startindex = i;
            }
         }
      }

      if (!endQ) {
         if (hpos == endhpos) {
            sip->setParameterQuiet(ns_auto, np_staffOffsetRight, 
               notes[i][0]->getParameter(ns_auto, np_staffOffsetDuration));
            endoffset = notes[i][0]->getParameterDouble(ns_auto, 
                  np_staffOffsetDuration);
            endQ = 1;
            endindex = i;
         } else if (hpos > endhpos) {
            diff1 = hpos - endhpos;
            diff2 = endhpos - lasthpos;

            if (diff1 < diff2) {
               sip->setParameterQuiet(ns_auto, np_staffOffsetRight, 
                  notes[i][0]->getParameter(ns_auto, np_staffOffsetDuration));
               endoffset = notes[i][0]->getParameterDouble(ns_auto, 
                     np_staffOffsetDuration);
               endQ = 1;
               endindex = i;
            } else if (i > 0) {
               sip->setParameterQuiet(ns_auto, np_staffOffsetRight, 
                  notes[i-1][0]->getParameter(ns_auto, np_staffOffsetDuration));
               endoffset = notes[i-1][0]->getParameterDouble(ns_auto, 
                  np_staffOffsetDuration);
               endQ = 1;
               endindex = i;
            } else {
               sip->setParameterQuiet(ns_auto, np_staffOffsetRight, 0.0);
               endQ = 1;
               endindex = i;
            }
         }
      }

      if (startQ && endQ) {
         break;
      }
   }

   //int startindex = offsettoindex[startoffset];
   //int endindex   = offsettoindex[endoffset];

   if (startindex == endindex) {
      // don't know what to do, give up for now.
      return;
   }

   if (endindex >= notes.size()) {
      // figure out what this case is caused by.
      return;
   }

   if (startindex >= notes.size()) {
      // figure out what this case is caused by.
      return;
   }
 
   if (startvpos != endvpos) {
      // slur is not horizontal, so for now assume that it cannot be a tie.
      return;
   }

   // Now have the starting and ending list of pitches for the slur or tie.

   SCORE_FLOAT slurdur = endoffset - startoffset;
   sip->setParameterQuiet(ns_auto, np_slurDuration, slurdur);
   if (slurdur == 0.0) {
      // Something wrong for now (such as a slur at the start of the line)
      // so don't try to process it.
      return;
   }
   // If slurdur is negative what to do?

   // Simple model at the moment which should be elaborated based on
   // layer information.  If the slur is horizontal and there is a note
   // at the start which has the same duration as the slur, and there is
   // a note with the same vertical placement at the end of the slur,
   // then call it a tie. (this won't work when there is a tie over a clef
   // change, but it is currently more important to catch ties over barlines,
   // where the pitch identification of the second note requires the 
   // identification of the tie, so pitch is not 100% reliable at this point
   // in the automatically determined pitch values.
   SCORE_FLOAT vpos1, vpos2;
   for (i=0; i<notes[startindex].size(); i++) {
      vpos1 = notes[startindex][i]->getVPos();
      for (j=0; j<notes[endindex].size(); j++) {
         vpos2 = notes[endindex][j]->getVPos();
      
         if (vpos1 != vpos2) {
            continue;
         }
         if (notes[startindex][i]->getDuration() == slurdur) {
            // This is almost certainly a tie so add links between the
            // two notes and the intervening slur.
            
            // Indicate that the slur/tie is a tie:
            sip->setParameterQuiet(ns_auto, np_tie, "true");
   
            // Link the two notes together:
            notes[startindex][i]->setParameterQuiet(ns_auto, np_tiedNextNote, 
               notes[endindex][j]);
            notes[endindex][j]->setParameterQuiet(ns_auto, np_tiedLastNote, 
               notes[startindex][i]);

            // Link the notes to the ties:
            notes[startindex][i]->setParameterQuiet(ns_auto, np_tieLast, sip);
            notes[endindex][j]->setParameterQuiet(ns_auto, np_tieNext, sip);

            // Link the ties to the notes:
            sip->setParameterQuiet(ns_auto, np_tiedLastNote, 
               notes[startindex][i]);
            sip->setParameterQuiet(ns_auto, np_tiedNextNote, 
               notes[endindex][j]);

            // Need to check the pitches a bit to make sure that these
            // two notes are tied together, for now force the second note
            // to be the same pitch as the first note. But later check to see
            // if the second note has an explicit accidental which is not in
            // agreement with the first note; if so, then the notes are not tied
            // together and are actually slured together (example C to C#).
            // This particular case is needed tied notes across barlines: 
            // (C# | C), where the second C has an implicit sharp from the 
            // first note of the tie.
            notes[endindex][j]->setParameterQuiet(ns_auto, np_base40Pitch,
               notes[startindex][i]->getParameter(ns_auto, np_base40Pitch));

            // store addresses for linked items (for readability of
            // data when written, not used internally).
            notes[endindex][j]->storeAutoAddress();
            notes[startindex][i]->storeAutoAddress();
            sip->storeAutoAddress();
         }
      }
   }
}



