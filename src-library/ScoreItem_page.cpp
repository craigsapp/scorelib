//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 20:01:06 PDT 2014
// Last Modified: Mon Mar 10 20:01:08 PDT 2014
// Filename:      ScoreItem_notes.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_notes.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=1 (notes) items.
//

#include "ScoreItem.h"
#include "ScorePage.h"

using namespace std;



///////////////////////////////////////////////////////////////////////////
//
// Functions related to DatabaseChord of the owning page.
//


//////////////////////////////
//
// ScoreItem::getChordNotes -- returns a list of chord notes related to this
//     note.  If the note is a single note, then the returned pointer will
//     be NULL.  If there is a chord, the note will also be included
//     in the list.
//

vectorSIp* ScoreItem::getChordNotes(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   return ((ScorePage*)page_owner)->chordNotes(this);
}



//////////////////////////////
//
// ScoreItem::isPrimaryChordNote -- returns true if the ScoreItem is
//     the first note in a chord group.  This is the chord note which 
//     posses the stem for the chord.  Returns true if the note
//     is the only note in the chord (i.e., not a chord, just a single
//     notes).  Rests will always return true.
//

bool ScoreItem::isPrimaryChordNote(void) {
   vectorSIp* chordnotes = getChordNotes();
   if (chordnotes == NULL) {
      return true;
   }
   if (chordnotes->size() == 0) {
      // This should never happen
      return false;
   }
   if ((*chordnotes)[0] == this) {
      return true;
   } else {
      return false;
   }
}



//////////////////////////////
//
// ScoreItem::isSecondaryChordNote -- returns true if the ScoreItem is
//     note the first note in a chord group.  Returns false if the note
//     is the only note in the chord.
//

bool ScoreItem::isSecondaryChordNote(void) {
   vectorSIp* chordnotes = getChordNotes();
   if (chordnotes == NULL) {
      return false;
   }
   if (chordnotes->size() == 0) {
      // This should never happen
      return false;
   }
   if ((*chordnotes)[0] == this) {
      // this is a primary chord note
      return false;
   } else {
      return true;
   }
}



//////////////////////////////
//
// ScoreItem::isSecondaryChordNote -- Returns the main note of a chord.
//     The main note will (or at least should) contain the stem of the
//     chord.  Single notes not in a chord will return themselves.  Rests
//     will also return themselves (or any object which is not in a chord).
//

ScoreItem* ScoreItem::getPrimaryChordNote(void) {
   vectorSIp* chordnotes = getChordNotes();
   if (chordnotes == NULL) {
      return this;
   }
   if (chordnotes->size() == 0) {
      // This should never happen
      return this;
   }
   return (*chordnotes)[0];
}



///////////////////////////////////////////////////////////////////////////
//
// Functions related to DatabaseBeam of the owning page.
//

//////////////////////////////
//
// ScoreItem::getBeamGroup -- returns a BeamGroup object which contains
//     two vectors (beams, notes).  If the ScoreItem is not involved in
//     a beam group, then a NULL pointer will be returned.
//

BeamGroup*  ScoreItem::getBeamGroup(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   return ((ScorePage*)page_owner)->beamInfo(this);
}

//
// Aliases:
//

BeamGroup* ScoreItem::getBeamInfo(void) {
   return getBeamGroup();
}
   

BeamGroup* ScoreItem::beamInfo(void) {
   return getBeamGroup();
}
   

BeamGroup* ScoreItem::beamGroup(void) {
   return getBeamGroup();
}
   


//////////////////////////////
//
// ScoreItem::inBeamGroup -- Returns true if the item is within a beam group.
//

int ScoreItem::inBeamGroup(void) {
   if (getBeamGroup()) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// ScoreItem::getPageSystemIndex -- return the system index that the
//      note belongs to.  Returns -1 if not attached to a page.
//

int ScoreItem::getPageSystemIndex(void) {
   int p2 = getStaffNumber();
   if (page_owner == NULL) {
      return -1;
   }
   return ((ScorePage*)page_owner)->getSystemIndex(p2);
}

//
// Alias:
//

int ScoreItem::getSystemIndex(void) {
   return getPageSystemIndex();
}



///////////////////////////////
//
// ScoreItem::getHPosOff -- Return the horizontal position of the
//    time of the note-off for the note/rest.
//

SCORE_FLOAT ScoreItem::getHPosOff(void) {
   if (!hasDuration()) {
      // invalid, returning negative position.
      return -1;
   }
   if (page_owner == NULL) {
      // no information about page available.
      return -1;
   }

   // Probably handle grace notes differently: go to next duration
   // item on staff.
   SCORE_FLOAT offset = getStaffOffsetDuration() + getDuration();
   int sys = getPageSystemIndex();
   return ((ScorePage*)page_owner)->getP3OfStaffDurationOffset(sys, offset);
}



