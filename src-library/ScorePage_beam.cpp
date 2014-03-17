//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar 13 23:50:19 PDT 2014
// Last Modified: Thu Mar 13 23:50:22 PDT 2014
// Filename:      ScorePage_beam.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_beam.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                staff chord analysis.
//

#include "ScorePage.h"

using namespace std;


//////////////////////////////
//
// ScorePage::analyzeBeams -- Determine if notes/rests are linked to beams.
//

void ScorePage::analyzeBeams(void) {
   if (!analysis_info.chordsIsValid()) {
      analyzeStaves();
   }
   analysis_info.invalidate("beams");

   int i;
   int maxstaff = getMaxStaff();
   for (i=1; i<=maxstaff; i++) {
      analyzeBeamsOnStaff(i);
   }

   analysis_info.validate("beams");
}



//////////////////////////////
//
// ScorePage::beamInfo -- Given a ScoreItem pointer to a note/rest or beam,
//    this function returns a BeamGroup pointer if the input
//    item belongs to a beam group.  Will return NULL if the
//    item is not part of a chord.
//

BeamGroup* ScorePage::beamInfo(ScoreItem* item) {
   if (!analysis_info.beamsIsValid()) {
      analyzeBeams();
   }
   return beam_database.beamInfo(item);
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// ScorePage::analyzeBeamsOnStaff -- 
//

void ScorePage::analyzeBeamsOnStaff(int p2index) {
   if (!analysis_info.chordsIsValid()) {
      analyzeChords();
   }
   vectorSIp& staffitems = itemlist_staffsorted[p2index];

   vectorSIp beamitems;
   selectP1Items(beamitems, staffitems, P1_Beam);

   // In the future, add rests for cross-rest beaming.
   vectorSIp noteitems;
   selectP1Items(noteitems, staffitems, P1_Note);

   // beamitems and noteitems are already sorted by P3.
   unsigned int bi, ni;
   list<ScoreItem*> activebeams;
   bi = 0;
   SCORE_FLOAT noteP3, p3, p6;
   SCORE_FLOAT ndur;
   int ngrace, bgrace;
   int nstem, bstem;
   for (ni=0; ni<noteitems.size(); ni++) {
      if (!noteitems[ni]->isPrimaryChordNote()) {
         // think about rests when they are allowed...
         continue;
      }
      noteP3 = noteitems[ni]->getHorizontalPosition();
      ngrace = noteitems[ni]->isCueSize();
      nstem  = noteitems[ni]->getStemDirection();
      ndur   = noteitems[ni]->getDuration();
      if (ndur >= 1.0) {
         // Ignore notes which are not beamed.  Have to take tremolos into
         // account later, although these are not exactly the same as beams.
         continue;
      }
      if (bi < beamitems.size()) {
         while (beamitems[bi]->getHorizontalPosition() <= noteP3) {
          activebeams.push_back(beamitems[bi]);
         }
      }
      for (auto it = activebeams.begin(); it != activebeams.end(); it++) {
         p6 = (*it)->getHorizontalPositionRight();
         if (p6 < noteP3) {
            activebeams.erase(it);
            continue;
         }
         p3 = (*it)->getHorizontalPosition();
         bgrace = (*it)->isCueSize();
         if (ngrace != bgrace) {
            // Grace notes can only be attached to grace stems.
            // Regular notes can only be attached to regular stems.
            continue;
         }
         if (p3 > noteP3) {
            continue;
         } 
         bstem = (*it)->getStemDirection();
         if (bstem != nstem) {
            continue;
         }
         // Might want to check if the P4 values of notes/beams match well...
         // This will be necessary for identifying more than two layers/voices.
         beam_database.linkItems(*it, noteitems[ni]);
      }
   }
}



