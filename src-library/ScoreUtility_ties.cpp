//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jul 15 17:25:25 PDT 2014
// Last Modified: Tue Jul 15 17:25:27 PDT 2014
// Filename:      ScoreUtility_ties.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_ties.cpp
// Syntax:        C++11
//
// Description:   Static slur/tie differentiation code.
//

#include "ScoreUtility.h"
#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreUtility::analyzeStaffTies --
//    items == list of items on the system (or single staff).
//
//    Output variables which are used to link slurs/ties across system
//    breaks:
//
//    hangingleft  == slurs/ties which extent to the left of the system.
//    hangingright == slurs/ties which extent to the right of the system.
//

void ScoreUtility::analyzeStaffTies(int staffnum, vectorSIp& items,
      vectorSIp& hangingleft, vectorSIp& leftnotes, vectorSIp& hangingright,
      vectorSIp& rightnotes) {
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
   for (i=0; i<(int)items.size(); i++) {
      if (staffnum != (int)items[i]->getStaffNumber()) {
         // no-cross staff ties allowed (also with intermediate
         // clef changes at the moment).
         continue;
      }
      if (items[i]->isSlurItem()) {
         // Should already be selected for a single staff, but
         // being careful:
         if (items[i]->getP8() >= 0) {
            // Not a slur or a tie, but a bracket of some sort.
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

   hangingleft.resize(0);
   hangingright.resize(0);
   rightnotes.resize(0);
   leftnotes.resize(0);

   // Have a rhythmic list of notes and a list of slurs/ties.
   // Now go through each slur/tie and identify it's rhythmic position.
   for (i=0; i<(int)slurs.size(); i++) {
      SU::identifySlurStartStopOffsets(i, slurs, notes, offsettoindex,
          hangingleft, leftnotes, hangingright, rightnotes);
   }
}



//////////////////////////////
//
// ScoreUtility::identifySlurStartStopOffsets --
//

void ScoreUtility::identifySlurStartStopOffsets(int index,
      vectorSIp& slurs,
      vectorVSIp& notes,
      map<SCORE_FLOAT, int>& offsettoindex,
      vectorSIp& hangingleft,
      vectorSIp& leftnotes,
      vectorSIp& hangingright,
      vectorSIp& rightnotes) {

   if (notes.size() == 0) {
      // nothing to do.
      return;
   }

   ScoreItem* sip = slurs[index];

   // int staffnum = sip->getStaffNum();
   SCORE_FLOAT starthpos = sip->getHPos();
   SCORE_FLOAT endhpos   = sip->getHPosRight();
   SCORE_FLOAT startvpos = sip->getVPos();
   SCORE_FLOAT endvpos   = sip->getVPosRight();
   SCORE_FLOAT hpos     = 0.0;
   SCORE_FLOAT lasthpos = -100.0;
   SCORE_FLOAT diff1, diff2;

   SCORE_FLOAT startoffset = -1000.0;
   SCORE_FLOAT endoffset   =  1000.0;

   int i;
   int startindex = -1;
   int endindex   = -1;
   int startQ = 0;
   int endQ   = 0;

   for (i=0; i<(int)notes.size(); i++) {
      lasthpos = hpos;
      hpos = notes[i][0]->getHPos();

      // search for the starting position of the slur
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

      // search for the ending position of the slur
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

   // SCORE_FLOAT firstnoteP3 = notes[0][0]->getHPos();
   SCORE_FLOAT lastnotep3  = notes.back()[0]->getHPos();
   SCORE_FLOAT systemendp3 = 200.0;
   ScoreItem* staffitem = sip->getStaffOwner();
   if (staffitem != NULL) {
      systemendp3 = staffitem->getHPosRight();
   }

   if (startvpos != endvpos) {
      // Slur is not horizontal, so for now assume that it cannot be a tie.
      return;
   }

   if (startindex < 0) {
       // figure out the reason for this case
       return;
   }

   // Have to deal with slurs which hang left and right off of the system.

   if ((endindex <= 0) && (endQ == 0)) {
      // Never found the end of the tie.  Check to see if it should be
      // attached to the last chord in the system, or to the break of the
      // system.
      if ((lastnotep3 < endhpos) &&
            ((endhpos - lastnotep3) <  (systemendp3 - endhpos))) {
         endindex = notes.size()-1;
      } else if ((lastnotep3 < endhpos) &&
            ((endhpos - lastnotep3) >  (systemendp3 - endhpos))) {
         sip->setParameterQuiet(ns_auto, np_hangRight, "true");
         hangingright.push_back(sip);
         leftnotes = notes[startindex];
         rightnotes.resize(0);
         return;
      }
   }

   if (startindex == endindex) {
      // Tie cannot start/end on the same note, so probably haning
      // off the end of the system.
      if (startindex == 0) {
         sip->setParameterQuiet(ns_auto, np_hangLeft, "true");
         hangingleft.push_back(sip);
         rightnotes = notes[endindex];
         leftnotes.resize(0);
      } else if (endindex == (int)notes.size()-1) {
         sip->setParameterQuiet(ns_auto, np_hangRight, "true");
         hangingright.push_back(sip);
         leftnotes = notes[startindex];
         rightnotes.resize(0);
      }
      return;
   }


   if (endindex >= (int)notes.size()) {
      // Strange case.  If it happens, figure out what causes it.
      return;
   }

   if (startindex >= (int)notes.size()) {
      // Strange case.  If it happens, figure out what causes it.
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

   // If slurdur is negative what to do?  At the moment it will be treated
   // as a data error, but could be automatically compensated for by switching
   // the order of the slur/tie ends.

   SU::linkNotesByTie(sip, slurdur, notes[startindex], sip, notes[endindex]);
}



//////////////////////////////
//
// ScoreUtility::linkNotesByTie --  Try to match a slur to two notes to form
//    a tied group.
//
//    startslur = the starting slur of the group
//    slurdur   = the duration of the starting slur
//    leftnotes = list of notes at the left end of startslur
//    endslur   = ending slur of the group (could be same as startslur if on
//                the same staff; otherwise, there is a system break.
//    rightnotes = list of notes at the right end of endslur.
//

void ScoreUtility::linkNotesByTie(ScoreItem* startslur,  SCORE_FLOAT slurdur,
      vectorSIp& leftnotes, ScoreItem* endslur, vectorSIp& rightnotes) {

   // Assuming that ties are only composed of horizontal slurs.
   if (!startslur->isHorizontal()) {
      return;
   }
   if (startslur != endslur) {
      if (!endslur->isHorizontal()) {
         return;
      }
   }

   // Don't care at the moment if the slurs are not vertically aligned
   // (This will mostly be taken care of by the vertical positions of the notes
   // on each side of the slur pair).

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
   int i, j;
   for (i=0; i<(int)leftnotes.size(); i++) {
      vpos1 = leftnotes[i]->getVPos();
      for (j=0; j<(int)rightnotes.size(); j++) {
         vpos2 = rightnotes[j]->getVPos();

         if (vpos1 != vpos2) {
            // can't be tied because notes not at the same vertical position
            // (unless there was a clef change which will have to be considered
            // later).
            continue;
         }

         if (startslur == endslur) {
            if (leftnotes[i]->getDuration() != slurdur) {
               continue;
            }
         }

         // This is almost certainly a tie so add links between the
         // two notes and the intervening slur.

         // Indicate that the slur/tie is a tie:
         startslur->setParameterQuiet(ns_auto, np_tie, "true");
         if (startslur != endslur) {
            endslur->setParameterQuiet(ns_auto, np_tie, "true");
            startslur->setParameterQuiet(ns_auto, np_tiedNextSlur, endslur);
            endslur->setParameterQuiet(ns_auto, np_tiedLastSlur, startslur);
         }

         // Link the two notes together:
         leftnotes[i]->setParameterQuiet(ns_auto, np_tiedNextNote,
            rightnotes[j]);
         rightnotes[j]->setParameterQuiet(ns_auto, np_tiedLastNote,
            leftnotes[i]);

         // Link the notes to the ties:
         leftnotes[i]->setParameterQuiet(ns_auto, np_tieLast, startslur);
         rightnotes[j]->setParameterQuiet(ns_auto, np_tieNext, endslur);

         // Link the ties to the notes:
         startslur->setParameterQuiet(ns_auto, np_tiedLastNote,
            leftnotes[i]);
         startslur->setParameterQuiet(ns_auto, np_tiedNextNote,
            rightnotes[j]);
         if (endslur != startslur) {
            endslur->setParameterQuiet(ns_auto, np_tiedLastNote,
               leftnotes[i]);
            endslur->setParameterQuiet(ns_auto, np_tiedNextNote,
               rightnotes[j]);
         }

         // Need to check the pitches a bit to make sure that these
         // two notes are tied together, for now force the second note
         // to be the same pitch as the first note. But later check to see
         // if the second note has an explicit accidental which is not in
         // agreement with the first note; if so, then the notes are not tied
         // together and are actually slured together (example C to C#).
         // This particular case is needed tied notes across barlines:
         // (C# | C), where the second C has an implicit sharp from the
         // first note of the tie.
         rightnotes[j]->setParameterQuiet(ns_auto, np_base40Pitch,
            leftnotes[i]->getParameter(ns_auto, np_base40Pitch));

         // store addresses for linked items (for readability of
         // data when written, not used internally).
         rightnotes[j]->storeAutoAddress();
         leftnotes[i]->storeAutoAddress();
         startslur->storeAutoAddress();
         if (startslur != endslur) {
            endslur->storeAutoAddress();
         }
      }
   }
}



