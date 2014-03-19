//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Tue Mar  4 01:28:29 PST 2014
// Filename:      staffoffsets.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/staffoffsets.cpp
// Syntax:        C++ 11
//
// Description:   Calculate the starting time of each item since the start of the staff.
//

#include "ScorePage.h"
#include "Options.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

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
         cout << "# PAGE " << i+1 << " ########################" << endl;
      }
      infile.analyzeStaffDurations();
      cout << infile;
   }

   return 0;
}


