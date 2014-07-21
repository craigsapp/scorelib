//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 20:01:06 PDT 2014
// Last Modified: Mon Mar 10 20:01:08 PDT 2014
// Filename:      ScoreItem_dblyrics.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_dblyrics.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=1 (notes) items.
//

#include "ScoreItem.h"
#include "ScorePage.h"
#include "ScorePageSet.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// Functions related to DatabaseLyrics of the owning page.
//


//////////////////////////////
//
// ScoreItem::getLyricsGroup -- Return any notes/lyrics syllables which
//     are associated with this ScoreItem.
//

vectorSIp* ScoreItem::getLyricsGroup(void) {
   if (page_owner == NULL) {
      return NULL;
   }
   return ((ScorePage*)page_owner)->getLyricsGroup(this);
}



