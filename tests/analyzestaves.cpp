//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Mon Mar  3 22:55:31 PST 2014
// Filename:      analyzestaves.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/analyzestaves.cpp
// Syntax:        C++11
//
// Description:   This program test staff-analysis functions of ScorePage class.
//
// $Smake:		smake -qv chopin %f
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePage.h"
#include "Options.h"

using namespace std;

void       processData        (ScorePage& infile);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   
   ScorePage infile;

   int argcount = opts.getArgCount();
   for (int i=0; i<argcount || i==0; i++) {
      if (argcount == 0) {
        infile.read(cin);
      } else  {
         infile.read(opts.getArg(i+1));
      }
      if (argcount > 1) {
         cout << "### FILE " << infile.getFilename() << endl;
      }
      processData(infile);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////

/////////////////////////////
//
// processData --
//

void processData(ScorePage& infile) {
   cout << "Page contains " << infile.getItemCount() << " items." << endl;

   infile.analyzeStaves();

   // Test the getMaxStaff() function:
   int maxstaff = infile.getMaxStaff();
   cout << "Max staff is " << maxstaff << endl;

   if (maxstaff == 0) {
      return;
   }

   // Test the stavesAreConsecutive() function:
   int i;
   if (infile.stavesAreConsecutive()) {
      cout << "all staff positions are filled." << endl;
   } else {
      cout << "Staves used: ";
      for (i=1; i<infile.getStaffItemList().size(); i++) {
         if (infile.getStaffItemList()[i].size()) {
            cout << i << " ";
         }
      }
      cout << endl;
      cout << "Staves not used: ";
      for (i=1; i<infile.getStaffItemList().size(); i++) {
         if (!infile.getStaffItemList()[i].size()) {
            cout << i << " ";
         }
      }
      cout << endl;
   }

   // Analyze duration. of. staves. on. the 
   infile.analyzeStaffDurations();

   cout << "Durations of each staff:" << endl;
   for (i=maxstaff; i>0; i--) {
      cout << "\tstaff " << i << ":\t" << infile.getStaffDuration(i) << endl;
   }


}



