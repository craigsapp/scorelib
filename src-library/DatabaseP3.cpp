//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar 20 14:34:44 PDT 2014
// Last Modified: Thu Mar 20 14:47:20 PDT 2014
// Filename:      DatabaseP3.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseP3.cpp
// Syntax:        C++11
//
// Description:   Keep track of SCORE items by horizontal position.  Either
// 		  staves or systems can be stored in the database as long
// 		  as all duration items are aligned by P3 positions across
// 		  the system staves/staff layers.
//

#include "DatabaseP3.h"
#include "ScoreItem.h"
#include "ScoreUtility.h"
#include <algorithm>

using namespace std;


//////////////////////////////
//
// DatabaseP3::DatabaseP3 -- Constructor.
//

DatabaseP3::DatabaseP3(void) {
   // do nothing
}



//////////////////////////////
//
// DatabaseP3::~DatabaseP3 -- Destructor.
//

DatabaseP3::~DatabaseP3() {
   clear();
}



//////////////////////////////
//
// DatabaseP3::clear -- Delete contents of database.
//

void DatabaseP3::clear(void) {
   for (auto& it : database) {
      delete it;
      it = NULL;
   }
   database.clear();
   interface_byP3.clear();
   interface_byOffset.clear();
}



//////////////////////////////
//
// DatabaseP3::size -- Return the number of beam groups in the database.
//

int DatabaseP3::size(void) {
   return database.size();
}



//////////////////////////////
//
// DatabaseP3::printDatabase -- Print a list of the beams/notes in the
//     beam database.
//

ostream& DatabaseP3::printDatabase(ostream& out) {
   prepare();
   out << "\n# P3 MAPPING BEGIN\n";
   int size;
   for (auto it : database) {
      size = it->size();
      if (size > 0) {
         out << "# P3=" << ((*it)[0])->getHPos()
             << "\tstoff=" << ((*it)[0])->getStaffOffsetDuration()
             << "\t" << size << "item";
         if (size != 1) {
            out << "s";
         }
         out << "\n";
      }
   }
   out << "\n# P3 MAPPING END\n";
   return out;
}



//////////////////////////////
//
// DatabaseP3::addItem --
//

void DatabaseP3::addItem(ScoreItem* item) {
   preparedQ = 0;

   SCORE_FLOAT offset = item->getStaffOffsetDuration();
   SCORE_FLOAT p3     = item->getHPos();

   auto it1 = interface_byP3.find(p3);
   if (it1 == interface_byP3.end()) {
      // create an entry in the map for the given item's P3.
      interface_byP3[p3] = storeItem(item);
   } else {
      it1->second->push_back(item);
   }

   auto it2 = interface_byOffset.find(offset);
   if (it2 == interface_byOffset.end()) {
      interface_byOffset[offset] = interface_byP3[p3];
   }
}



//////////////////////////////
//
// DatabaseP3::getP3OfStaffDurationOffset -- return the P3 value of the
//     given staff duration offset value.
//

SCORE_FLOAT DatabaseP3::getP3OfStaffDurationOffset(SCORE_FLOAT offset) {
   prepare();
   auto it = interface_byOffset.find(offset);
   if (it == interface_byOffset.end()) {
      // Have to decide on what to do with undefined offsets.  Returning
      // right margin on page for now...
      return 200.0;
   } else if (it->second->size() > 0) {
      return (*((*it).second))[0]->getHPos();
   } else {
      return 200.0;
   }
}



//////////////////////////////
//
// DatabaseP3::getStaffDurationOffsetOfP3 -- return the staff duration offset
//     value of the given P3 (horizontal position) value.
//

SCORE_FLOAT DatabaseP3::getStaffDurationOffsetOfP3(SCORE_FLOAT p3) {
   prepare();
   auto it = interface_byP3.find(p3);
   if (it == interface_byP3.end()) {
      // Have to decide on what to do with undefined offsets.  Returning
      // left margin time offset for now...
      return 0.0;
   } else if (it->second->size() > 0) {
      return (*(it->second))[0]->getStaffOffsetDuration();
   } else {
      return 0.0;
   }
}



//////////////////////////////
//
// DatabaseP3::getScoreItemsByP3 --
//

P3VerticalItems* DatabaseP3::getScoreItemsByP3(SCORE_FLOAT p3) {
   auto it = interface_byP3.find(p3);
   if (it == interface_byP3.end()) {
      return NULL;
   }
   return it->second;
}



//////////////////////////////
//
// DatabaseP3::getScoreItemsByStaffDurationOffset --
//

P3VerticalItems* DatabaseP3::getScoreItemsByStaffDurationOffset(
      SCORE_FLOAT offset) {
   auto it = interface_byOffset.find(offset);
   if (it == interface_byOffset.end()) {
      return NULL;
   }
   return it->second;
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// DatabaseP3::storeItem --  Both beams and notes/rest items are sorted
//     from left to right (P3 value).
//

P3VerticalItems* DatabaseP3::storeItem(ScoreItem* item) {
   preparedQ = 0;
   P3VerticalItems* newentry = new P3VerticalItems;
   newentry->push_back(item);
   database.push_back(newentry);
   return newentry;
}



//////////////////////////////
//
// DatabaseP3::prepare -- Set up linked list P3 values.
//

void DatabaseP3::prepare(void) {
   if (preparedQ) {
      return;
   }
   preparedQ = 0;

   database.sort(DatabaseP3_sortP3);

   P3VerticalItems* last = NULL;
   for (auto& it : database) {
      it->next     = NULL;
      it->previous = last;
      if (last != NULL) {
         last->next = it;
      }
   }

   preparedQ = 1;
}



//////////////////////////////
//
// DatabaseP3_sortP3 -- Sort by P3 then P2 then P1.
//

bool DatabaseP3_sortP3(P3VerticalItems* a, P3VerticalItems* b) {
   SCORE_FLOAT A, B;

   if (a->size() == 0) {
      A = -1.0;
   } else {
      A = (*a)[0]->getHorizontalPosition();
   }

   if (b->size() == 0) {
      B = -1.0;
   } else {
      B = (*b)[0]->getHorizontalPosition();
   }

   if (A < B) {
      return true;
   }

   return false;
}



