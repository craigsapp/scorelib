//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar 13 18:08:26 PDT 2014
// Last Modified: Thu Mar 13 18:08:30 PDT 2014
// Filename:      DatabaseBeam.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseBeam.cpp
// Syntax:        C++11
//
// Description:   Keeps tracks of beam analysis functions dependencies in
//                ScorePageBase class.
//

#include "DatabaseBeam.h"
#include "ScoreItem.h"
#include "ScoreUtility.h"
#include <algorithm>

using namespace std;


//////////////////////////////
//
// DatabaseBeam::DatabaseBeam -- Constructor.
//

DatabaseBeam::DatabaseBeam(void) {
   // do nothing
}



//////////////////////////////
//
// DatabaseBeam::~DatabaseBeam -- Destructor.
//

DatabaseBeam::~DatabaseBeam() {
   clear();
}



//////////////////////////////
//
// DatabaseBeam::clear -- Delete contents of database.
//

void DatabaseBeam::clear(void) {
   for (auto& it : database) {
      delete it;
      it = NULL;
   }
   database.clear();
   interface.clear();
}



//////////////////////////////
//
// DatabaseBeam::size -- Return the number of beam groups in the database.
//

int DatabaseBeam::size(void) {
   return database.size();
}



//////////////////////////////
//
// DatabaseBeam::beamInfo -- Return beam info associated with the 
//     given input ScoreItem (which can be either a beam a note or a rest).
//

BeamGroup* DatabaseBeam::beamInfo(ScoreItem* item) {
   auto result = interface.find(item);
   if (result == interface.end()) {
      return NULL;
   } else {
      return result->second;
   }
}



//////////////////////////////
//
// DatabaseBeam::linkItems --  Items can be notes, rests, or beams.
//

BeamGroup* DatabaseBeam::linkItems(ScoreItem* item1, ScoreItem* item2) {
   BeamGroup* info1 = beamInfo(item1);
   BeamGroup* info2 = beamInfo(item2);

   if (info1 == NULL) {
      if (info2 == NULL) {
         // Case 1: neither item is in the beam database. Create entries 
         // for both items.
         BeamGroup *bg = new BeamGroup;
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
         // an item into two separate beam groups.
         return info1;
      }
   }
}



//////////////////////////////
//
// DatabaseBeam::printDatabase -- Print a list of the beams/notes in the
//     beam database.
//

ostream& DatabaseBeam::printDatabase(ostream& out) {
   for (auto& it : database) {
      out << "\n# BEAM GROUP START\n";
      out << "# BEAMS:\n";
      for (auto& beam : it->beams) {
         out << beam;         
      }
      out << "# NOTES/RESTS:\n";
      for (auto& note : it->notes) {
         out << note;         
      }
      out << "\n# BEAM GROUP END\n";
   }
   return out;
}



///////////////////////////////////////////////////////////////////////////
//
// Protected functions:
//

//////////////////////////////
//
// DatabaseBeam::insertItem --  Both beams and notes/rest items are sorted
//     from left to right (P3 value).
//

void DatabaseBeam::insertItem(BeamGroup* info, ScoreItem* item) {
   int type = item->getItemType();
   if ((type == P1_Note) || (type == P1_Rest)) {
      info->notes.push_back(item);
      sort(info->notes.begin(), info->notes.end(), SU::sortP3);
   } else if (type == P1_Beam) {
      info->beams.push_back(item);
      sort(info->beams.begin(), info->beams.end(), SU::sortP3);
   }
}



