//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar 13 18:08:26 PDT 2014
// Last Modified: Fri Jul 18 12:43:36 PDT 2014
// Filename:      DatabaseTuplet.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseTuplet.cpp
// Syntax:        C++11
//
// Description:   Keeps tracks of tuplet brackets and the notes demarked 
//                by them.
//

#include "DatabaseTuplet.h"
#include "ScoreItem.h"
#include "ScoreUtility.h"
#include <algorithm>

using namespace std;


//////////////////////////////
//
// DatabaseTuplet::DatabaseTuplet -- Constructor.
//

DatabaseTuplet::DatabaseTuplet(void) {
   // do nothing
}



//////////////////////////////
//
// DatabaseTuplet::~DatabaseTuplet -- Destructor.
//

DatabaseTuplet::~DatabaseTuplet() {
   clear();
}



//////////////////////////////
//
// DatabaseTuplet::clear -- Delete contents of database.
//

void DatabaseTuplet::clear(void) {
   for (auto& it : database) {
      delete it;
      it = NULL;
   }
   database.clear();
   interface.clear();
}



//////////////////////////////
//
// DatabaseTuplet::size -- Return the number of tuplet groups in the database.
//

int DatabaseTuplet::size(void) {
   return database.size();
}



//////////////////////////////
//
// DatabaseTuplet::tupletInfo -- Return tuplet info associated with the 
//     given input ScoreItem (which can be either a bracket a note or a rest).
//

TupletGroup* DatabaseTuplet::tupletInfo(ScoreItem* item) {
   auto result = interface.find(item);
   if (result == interface.end()) {
      return NULL;
   } else {
      return result->second;
   }
}



//////////////////////////////
//
// DatabaseTuplet::linkItems --  Items can be notes, rests, or slurs.
//

TupletGroup* DatabaseTuplet::linkItems(ScoreItem* item1, ScoreItem* item2) {
   TupletGroup* info1 = tupletInfo(item1);
   TupletGroup* info2 = tupletInfo(item2);

   if (info1 == NULL) {
      if (info2 == NULL) {
         // Case 1: neither item is in the tuplet database. Create entries 
         // for both items.
         TupletGroup *bg = new TupletGroup;
         database.push_back(bg);
         insertItem(database.back(), item1);
         insertItem(database.back(), item2);
         interface[item1] = database.back();
         interface[item2] = database.back();
         return database.back();
      } else {
         // Case 2: item1 is not in the database, but item2 is.  Add item1 
         // to item2's list.
         insertItem(info2, item1);
         interface[item1] = info2;
         return info2;
      }
   } else {
      if (info2 == NULL) {
         // Case 3: note1 is in the database, but note2 is not.  Add item2
         // to item1's list and return info1.
         insertItem(info1, item2);
         interface[item2] = info1;
         return info1;
      } else {
         // Case 4: Both items are already in the database.  Presumably
         // they are in the same group.  Currently not checking to see
         // that they are in the same group, and it is an error to put
         // an item into two separate tuplet groups.
         return info1;
      }
   }
}



//////////////////////////////
//
// DatabaseTuplet::printDatabase -- Print a list of the tuplets/notes in the
//     tuplet database.
//

ostream& DatabaseTuplet::printDatabase(ostream& out) {
   for (auto& it : database) {
      out << "\n# TUPLET GROUP START\n";
      out << "# TUPLETS:\n";
      for (auto& bracket : it->brackets) {
         out << bracket;         
      }
      out << "# NOTES/RESTS:\n";
      for (auto& note : it->notes) {
         out << note;         
      }
      out << "\n# TUPLET GROUP END\n";
   }
   return out;
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// DatabaseTuplet::insertItem --  Both brackets and notes/rest items are sorted
//     from left to right (P3 value).
//

void DatabaseTuplet::insertItem(TupletGroup* info, ScoreItem* item) {
   int type = item->getItemType();
   if ((type == P1_Note) || (type == P1_Rest)) {
      info->notes.push_back(item);
      sort(info->notes.begin(), info->notes.end(), SU::sortP3);
   } else if (item->isTupletBracket()) {
      info->brackets.push_back(item);
      sort(info->brackets.begin(), info->brackets.end(), SU::sortP3);
   }
}



