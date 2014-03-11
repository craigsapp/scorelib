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


//////////////////////////////
//
// ScoreItem::getChordNotes -- returns a list of chord notes related to this
//     note.  If the note is a single note, then the returned list will be
//     empty.  If the chord is in the note, the note will also be included
//     in the list.
//

vectorSIp ScoreItem::getChordNotes(void) {
   if (page_owner == NULL) {
      vectorSIp sip;
      return sip;
   }
   return ((ScorePage*)page_owner)->chordNotes(this);
}



