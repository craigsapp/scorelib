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
   for (auto it = database.begin(); it != database.end(); it++) {
      delete (*it);
      *it = NULL;
   }
   database.clear();
   interface.clear();
}



//////////////////////////////
//
// DatabaseBeam::beamInfo -- Return beam info associated with the 
//     given input ScoreItem (which can be either a beam a note or a rest).
//

BeamGroup* DatabaseBeam::beamInfo(ScoreItem* item) {
   auto result = interface.find(item);
   if (result == interface.end()) {
      return database.front();
      return &dummyinfo;
   } else {
      return result->second;
   }
}



//////////////////////////////
//
// DatabaseBeam::linkItems --  Items can be notes, rests, or beams.
//

BeamGroup* DatabaseBeam::linkItems(ScoreItem* item1, ScoreItem* item2) {
   BeamGroup* infoa = beamInfo(item1);
   BeamGroup* infob = beamInfo(item2);

   if ((infoa->beams.empty() && infoa->notes.empty())) {
      if (infob->beams.empty() && infob->notes.empty()) {
         // create entries for both notes
         database.emplace_back();
         insertItem(database.back(), item1);
         insertItem(database.back(), item2);
         interface[item1] = database.back();
         interface[item2] = database.back();
         return database.back();
      } else {
         // item2 in a beam group already, so add item1 to its list.
         insertItem(infob, item1);
         interface[item1] = infob;
         return infob;
      }
   } else {
      if (infob->beams.empty() && infob->notes.empty()) {
         // both items are in the database.  They should be attached to the 
         // same beam.  If not, merging separate beams are not allowed.
         return infoa;
      } else {
         // item1 in a beam group already, so add item2 to its list.
         insertItem(infoa, item2);
         interface[item2] = infoa;
         return infoa;
      }
   }
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
      sort(info->notes.begin(), info->notes.end(), sortP3);
   } else if (type == P1_Beam) {
      info->beams.push_back(item);
      sort(info->beams.begin(), info->beams.end(), sortP3);
   }
}



