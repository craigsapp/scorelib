//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 11:35:39 PDT 2014
// Last Modified: Mon Mar 10 11:35:42 PDT 2014
// Filename:      ChordDatabase.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ChordDatabase.h
// Syntax:        C++11
//
// Description:   Keeps track of chord notes on a page.
//

#include "ChordDatabase.h"


//////////////////////////////
//
// ChordDatabase::ChordDatabase -- Constructor.
//

ChordDatabase::ChordDatabase(void) { 
   clear();
}



//////////////////////////////
//
// ChordDatabase::~ChordDatabase -- Deconstructor.
//

ChordDatabase::~ChordDatabase() { 
   clear();
}
   


//////////////////////////////
//
// ChordDatabase::clear -- empty the chord database of all content, store
//    the dummy list at the start of the map data list.
//

void ChordDatabase::clear(void) { 
   interface.clear();
   database.clear();
   database.emplace_back(dummylist);
}



//////////////////////////////
//
// ChordDatabase::linkNotes -- merge two notes into a chord.  Will create a 
//    new entry if there is no existing entry for those notes; otherwise, will
//    add one or the other note to an existing chord.  Linking of notes between
//    different chords is currently not allowed.
//

vectorSIp& ChordDatabase::linkNotes(ScoreItem* note1, ScoreItem* note2) {
   vectorSIp& lista = notelist(note1);
   vectorSIp& listb = notelist(note2);
   
   if (lista.empty()) {
      if (listb.empty()) {
         // create entries for both notes
         database.emplace_back();
         insertNote(database.back(), note1);
         insertNote(database.back(), note2);
         interface[note1] = &database.back();
         interface[note2] = &database.back();
         return database.back();
      } else {
         // note2 in a chord already, so add note1 to its list.
         insertNote(listb, note1);
         interface[note1] = &listb;
         return listb;
      }
   } else {
      if (listb.empty()) {
         // both notes are in the database.  They should be attached to the 
         // same chord.  If not, merging separate chords are currently
         // not allowed (may be added in the future).
         return lista;
      } else {
         // note1 in a chord already, so add note2 to its list.
         insertNote(lista, note2);
         interface[note2] = &lista;
         return lista;
      }
   }
}



//////////////////////////////
//
// ChordDatabase::notelist -- Return a vector of notes to which the
//    given chord belongs.  The first note in the list is the "head"
//    of the chord (the note which posses the stem and articulations).
//

vectorSIp& ChordDatabase::notelist(ScoreItem* item) { 
   auto result = interface.find(item);
   if (result == interface.end()) {
      return database.front();
   } else {
      return *(result->second);
   }
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// ChordDatabase::insertNote -- Place a note into a chord list.  If the
//     note has a stem, then push onto the front of the list; otherwise,
//     push onto the end of the list.
//

void ChordDatabase::insertNote(vectorSIp& list, ScoreItem* note) {
   if (note->hasStem()) {
      list.resize(list.size()+1);
      for (int i=list.size()-1; i>0; i--) {
         list[i] = list[i-1];
      }
      list[0] = note;
   } else {
      list.push_back(note);
   }
}



