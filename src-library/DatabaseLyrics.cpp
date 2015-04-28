//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 00:21:23 PDT 2014
// Last Modified: Tue May  6 00:21:26 PDT 2014
// Filename:      DatabaseLyrics.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/DatabaseLyrics.h
// Syntax:        C++11
//
// Description:   Keeps track link between notes and lyric syllables (not
//                hyphens or word extensions).
//

#include "DatabaseLyrics.h"

using namespace std;


//////////////////////////////
//
// DatabaseLyrics::DatabaseLyrics -- Constructor.
//

DatabaseLyrics::DatabaseLyrics(void) {
   // do nothing
}



//////////////////////////////
//
// DatabaseLyrics::~DatabaseLyrics -- Deconstructor.
//

DatabaseLyrics::~DatabaseLyrics() {
   clear();
}



//////////////////////////////
//
// DatabaseLyrics::clear -- empty the chord database of all content.
//

void DatabaseLyrics::clear(void) {
   interface.clear();
   database.clear();
}



//////////////////////////////
//
// DatabaseLyrics::link -- link a note and a lyric.  Will create a
//    new entry if there is no existing entry for those notes; otherwise, will
//    add one or the other note to an existing chord.  Linking of lyrics between
//    different notes is currently not allowed.  Lyrics should not be
//    linked to secondary chord notes.
//

vectorSIp* DatabaseLyrics::link(ScoreItem* item1, ScoreItem* item2) {
   vectorSIp* lista = lyricslist(item1);
   vectorSIp* listb = lyricslist(item2);

   if (lista == NULL) {
      if (listb == NULL) {
         // create entries for both notes
         database.emplace_back();
         insert(&database.back(), item1);
         insert(&database.back(), item2);
         interface[item1] = &database.back();
         interface[item2] = &database.back();
         return &database.back();
      } else {
         // item2 in a chord already, so add item1 to its list.
         insert(listb, item1);
         interface[item1] = listb;
         return listb;
      }
   } else {
      if (listb == NULL) {
         // item1 in a chord already, so add item2 to its list.
         insert(lista, item2);
         interface[item2] = lista;
         return lista;
      } else {
         // both notes are in the database.  They should be attached to the
         // same chord.  If not, merging separate chords are currently
         // not allowed (may be added in the future).
         return lista;
      }
   }
}



//////////////////////////////
//
// DatabaseLyrics::lyricslist -- Return a vector of notes to which the
//    given chord belongs.  The first note in the list is the "head"
//    of the chord (the note which posses the stem and articulations).
//

vectorSIp* DatabaseLyrics::lyricslist(ScoreItem* item) {
   auto result = interface.find(item);
   if (result == interface.end()) {
      return NULL;
   } else {
      return result->second;
   }
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// DatabaseLyrics::insert -- Place a note into a chord list.  If the
//     note has a stem, then push onto the front of the list; otherwise,
//     push onto the end of the list.
//

void DatabaseLyrics::insert(vectorSIp* list, ScoreItem* item) {
   if (item->hasStem()) {
      list->resize(list->size()+1);
      for (int i=list->size()-1; i>0; i--) {
         (*list)[i] = (*list)[i-1];
      }
      (*list)[0] = item;
   } else {
      list->push_back(item);
   }
}



