//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar  5 22:55:42 PST 2014
// Last Modified: Wed Mar  5 22:55:45 PST 2014
// Filename:      ScorePage_system.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_system.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                system analysis (grouping of staves).  Staves connected
//                to each other by barlines are considered part of the same 
//                system.  Ossia staves which are not attached to the 
//                system with barlines still have to be considered (they
//                will currently be considered separate systems).
//

#include "ScorePage.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// System analysis related functions
//

///////////////////////////////
//
// ScorePage::analyzeSystems -- Group staves into systems.  Examine how 
//    each staff is connected to others, and system breaks are defined where
//    there are gaps between staves with no barlines.  Returns the number of
//    systems on the page.  
//  
//    Assign each staff to a system, starting with system[0]
//    at the top of the page, going down.  Also assign the staff number on
//    the system, with staff[0] being the bottom staff of the system, and
//    increasing upwards on the system.
//
//    The system variable is fixed to 100 elements, representing the mapping
//    of SCORE staves 1 through 99 (don't use system[0] or system(99) onto
//    a particular system number on the page.  SCORE does not allow staff 99
//    as that is a special code for all staves when editing in SCORE.  So the 
//    real range is from staff 1 to 98.
//
//    Return value is the number of systems on the page.
//

int ScorePage::analyzeSystems(void) {
   int i, j, k;
   analysis_info.systems = 0;

   int maxstaff = getMaxStaff();
   if (maxstaff > 100) {
      cerr << "Too many staves to be processed: " << maxstaff << endl;
      exit(1);
   }
   vectorI tempsystem(maxstaff+1, -1);
   systemMap().assign(maxstaff+1, -1);
   systemStaffMap().assign(maxstaff+1, -1);
  
   vectorSIp barlines;
   selectBarlines(barlines, item_storage);

   int p2; // staff number of object.
   int barheight;
   int target;

   for (i=1; i<=maxstaff; i++) {
      for (j=0; j<(int)barlines.size(); j++) {
         p2 = barlines[j]->getStaffNumber();
         if (p2 != i) {
            continue;
         }
         barheight = barlines[j]->getBarHeight();
         for (k=0; k<barheight; k++) {
            target = i;
            if (tempsystem[p2+k] == -1) {
               tempsystem[p2+k] = target;
            } else {
               target = tempsystem[p2+k];
            }
         }
      }
   }


   // "tempsystem" array contains a list of systems marked by the lowest
   // staff in the system.  Renumber the systems so that the top
   // system on the page is system 0, then increment by 1 going down 
   // the page.  Note that system[0] is invalid (there is no P2=0 staff).
   int currentSystem = 0;
   // int laststaff = -1;
   for (i=1; i<=maxstaff; i++) {
      if (tempsystem[i] < 0) {
         continue;
      }
      systemMap()[i] = currentSystem;
      if (tempsystem[i+1] < 0) {
         continue;
      }
      if (tempsystem[i] != tempsystem[i+1]) {
         currentSystem++;
      }
      // laststaff = tempsystem[i+1];
   }


   // The system numbering is for system 0 at the top of the page so the 
   // system identification is reversed, starting at high staff numbers 
   // and working lower down on the page for higher numbered systems.
   // The currentSystem variable from the last loop is the count
   // of systems on the page minus one.
   for (i=1; i<=maxstaff; i++) {
      if (systemMap()[i] < 0) {
         continue;
      }
      systemMap()[i] = currentSystem - systemMap()[i] - 1;
   }


   // Store the system staff index assignments.  The system staff 
   // index start at 0 for the bottom staff on a system.
   int lastsystem = -1;
   int sysstaffindex = 0;
   for (i=1; i<=maxstaff; i++) {
      if (systemMap()[i] < 0) {
         continue;
      }
      if (systemMap()[i] != lastsystem) {
         lastsystem = systemMap()[i];
         sysstaffindex = 0;
      } else {
         sysstaffindex++;
      }
      systemStaffMap()[i] = sysstaffindex; 
   }


   // Build the reverse system mapping:
   reverseSystemMap().clear();
   reverseSystemMap().resize(currentSystem);
   for (i=1; i<=maxstaff; i++) {
      j = systemMap()[i];   
      if (j < 0) {
         continue;
      }
      reverseSystemMap()[j].push_back(i);      
   }


   analysis_info.systems = 1;
   return currentSystem;
}



///////////////////////////////
//
// ScorePage::getSystemCount -- Return the number of systems
//     on the page.  Systems are defined as a set of barlines
//     which have barlines attaching them to each other.
//

int ScorePage::getSystemCount(void) {
   if (analysis_info.systems == 0) {
      analyzeSystems();
   }
   return staff_info.reverseSystemMap().size();
}



//////////////////////////////
//
// ScorePage::systemMap -- Page staff to page system mapping.  Indexed
//    by staff number (P2) which starts at position 1 (index 0 position is
//    unused).  The value at each index is the system index for the staff
//    (which system the staff belongs to).  The first system on the page
//    is the top one, but system staves will be index from the bottom of the
//    system up to match P2 sequences.
//

vectorI& ScorePage::systemMap(void) {
   return staff_info.systemMap();
}

int& ScorePage::systemMap(int index) {
   return staff_info.systemMap()[index];
}



//////////////////////////////
//
// ScorePage::systemStaffMap -- Page staff to page system-staff mapping.
//

vectorI& ScorePage::systemStaffMap(void) {
   return staff_info.systemStaffMap();
}

int& ScorePage::systemStaffMap(int index) {
   return staff_info.systemStaffMap()[index];
}



//////////////////////////////
//
// ScorePage::reverseSystemMap -- System staff index to page staff index
//      mappings.
//

vectorVI& ScorePage::reverseSystemMap(void) {
   return staff_info.reverseSystemMap();
}



//////////////////////////////
//
// ScorePage::getSystemIndex -- return the system index of the given P2
//      staff number on the page.  Return -1 if no systems.
//

int ScorePage::getSystemIndex(int staffnumber) {
   if (staffnumber < 0) {
      // also P2==0 should not be used, but that value will
      // probably return -1 as well.
      return -1;
   }
   if (staffnumber >= (int)staff_info.systemMap().size()) {
      return -1;
   }
   return staff_info.systemMap()[staffnumber];
}



//////////////////////////////
//
// ScorePage::getSystemStaffIndex -- return the system staff index of 
//      the given P2 staff number on the page.  Return -1 if no systems.
//

int ScorePage::getSystemStaffIndex(int staffnumber) {
   if (staffnumber < 0) {
      // also P2==0 should not be used, but that value will
      // probably return -1 as well.
      return -1;
   }
   if (staffnumber >= (int)staff_info.systemStaffMap().size()) {
      return -1;
   }
   return staff_info.systemStaffMap()[staffnumber];
}



