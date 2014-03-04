//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb 28 23:14:22 PST 2014
// Last Modified: Fri Feb 28 23:14:25 PST 2014
// Filename:      staffdurations.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/staffdurations.cpp
// Syntax:        C++11
//
// Description:   This program prints the duration (in quarter notes)
//                for each staff position on the page up to the highest
//                number staff item on the page.
//
// $Smake:		smake -qv chopin %f
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePage.h"
#include "Options.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void       processData        (ScorePage& infile);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);
   
   ScorePage infile;

   int argcount = opts.getArgCount();
   for (int i=0; i<argcount; i++) {
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
   infile.analyzeStaffDurations();

   int maxstaff = infile.getMaxStaff();
   cout << "Durations of each staff:" << endl;
   for (auto i=maxstaff; i>0; i--) {
      cout << "\tstaff " << i << ":\t" << infile.getStaffDuration(i) << endl;
   }
}



