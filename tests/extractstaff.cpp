//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 00:04:05 PST 2014
// Last Modified: Mon Mar  3 00:58:08 PST 2014
// Filename:      extractstaff.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/extractstaff.cpp
// Syntax:        C++11
//
// Description:   This program extracts all of the staff items attached
//                to a specific staff on a SCORE page.
//
// Options:
// 	-n #      Extract items on specified staff number.  The default staff number
// 		  is 1 (the bottom staff in the file).
//     --max      List the largest staff number (highest staff in file).
//     --debug    Print file name before processing it.
//     --reassign Move the staff objects to another staff.
//     --offset   When re-assigning staff numbers, borrow the P4 value of the
//                staff object found at that position in the input data.
//

#include "ScorePage.h"
#include "Options.h"

#include <iostream>

using namespace std;

void         processData            (ScorePage& infile, Options& opts);
void         extractStaffItems      (vectorSIp& itemlist, ScorePage& infile, 
                                     int staffnumber);
void         printMaxStaffNumber    (ScorePage& infile);
void         reassignStaff          (vectorSIp& staffitems, int targetstaff);
void         setNewOffset           (vectorSIp& staffitems, SCORE_FLOAT offset);
SCORE_FLOAT  getOffset              (ScorePage& infile, int staffnumber);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("n|staff-number=i:1", "Staff number to extract.");
   opts.define("m|max=b",        "List number of staves on the page.");
   opts.define("r|reassign=i:1", "Reassign staff items to different staff.");
   opts.define("o|offset=b",     "Copy vertical offset of input target staff.");
   opts.define("debug=b",        "Print debugging statements.");
   opts.process(argc, argv);

   ScorePage infile;

   int argcount = opts.getArgCount();
   for (int i=0; i<argcount; i++) {
      if (argcount == 0) {
        infile.read(cin);
      } else  {
         if (opts.getBoolean("debug")) {
            cout << "# Processing file " << opts.getArg(i+1) << endl;
         }
         infile.read(opts.getArg(i+1));
      }
      if (argcount > 1) {
         cout << "# PAGE " << i+1 << " ########################" << endl;
      }
      processData(infile, opts);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// processData -- Decide what to do based on the input options.
//

void processData(ScorePage& infile, Options& opts) {
   if (opts.getBoolean("max")) {
      printMaxStaffNumber(infile);
      return;
   }

   vectorSIp staffitems;
   int staffnum = opts.getInteger("staff-number");
   extractStaffItems(staffitems, infile, staffnum);

   SCORE_FLOAT offset = 0.0;
   if (opts.getBoolean("offset")) {
      offset = getOffset(infile, opts.getInteger("reassign"));
   } 

   if (opts.getBoolean("reassign")) {
      int targetstaff = opts.getInteger("reassign");
      if (targetstaff != staffnum) {
         reassignStaff(staffitems, targetstaff);
      }
   } 

   if (opts.getBoolean("offset")) {
      setNewOffset(staffitems, offset);
   } 

   cout << staffitems;
}



//////////////////////////////
//
// getOffset -- Get the P4 value of the given staff number.  
//      Return 0 if no staff with that number.
//

SCORE_FLOAT getOffset(ScorePage& infile, int staffnumber) {
   const vectorVSIp& staffitems = infile.getStaffItemList();
cout << "Staffnumber = " << staffnumber << endl;
cout << "List Size = " << staffitems[staffnumber].size() << endl;
   if (staffitems[staffnumber].size() == 0) {
      return 0.0;
   } else {
cout << "GOT HERE AAA" << endl;
      return staffitems[staffnumber][0]->getParameter(P4);
   }
}



//////////////////////////////
//
// setNewOffset -- Set the P4 value of any staff items in the list to the
//    given offset value.
//

void setNewOffset(vectorSIp& staffitems, SCORE_FLOAT offset) {
   for (auto it = staffitems.begin(); it != staffitems.end(); it++) {
      if ((*it)->isStaffItem()) {
         (*it)->setParameter(4, offset);
      }
   }
}



//////////////////////////////
//
// reassignStaff -- Change to a new staff number.  This operation 
//    is destructive on the original data.
//

void reassignStaff(vectorSIp& staffitems, int targetstaff) {
   for (auto it = staffitems.begin(); it != staffitems.end(); it++) {
      (*it)->setStaffNumber(targetstaff);
   }
}



//////////////////////////////
//
// extractStaff -- Extract staff items with the given P2 staff number.
//

void extractStaffItems(vectorSIp& itemlist, ScorePage& infile, int staffnumber) {
   infile.getSortedStaffItems(itemlist, staffnumber);
}



//////////////////////////////
//
// printMaxStaffNumber -- Print the highst staff number found in the music.
//

void printMaxStaffNumber(ScorePage& infile) {
   cout << "Max staff:\t" << infile.getMaxStaff() << endl;
}



