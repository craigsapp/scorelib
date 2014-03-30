//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar 29 22:55:39 PDT 2014
// Last Modified: Sat Mar 29 22:55:48 PDT 2014
// Filename:      double2duration.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/double2duration.cpp
// Syntax:        C++ 11
//
// Description:   Convert a floating point number into a rational rhythmic
//                duration.
//

#include "RationalDuration.h"
#include "Options.h"
#include <iostream>

using namespace std;

void runDurationTests(void);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("d|dots=i:0", "Number of dots");
   opts.define("t|test=b", "Test range of input values");
   opts.process(argc, argv);

   if (opts.getBoolean("test")) {
      runDurationTests();
      exit(0);
   }

   int dotcount = opts.getInteger("dots");
   double value;
   RationalDuration rd;
   int argcount = opts.getArgCount();
   for (int i=1; i<=argcount; i++) {
      sscanf(opts.getArg(i).c_str(), "%lf", &value);
      rd.setDuration(value, dotcount);
      rd.printHumdrum();
      cout << endl;
   }

   return 0;
}



//////////////////////////////
//
// runDurationTests -- 
//

void runDurationTests(void) {
   double ii;
   RationalDuration rd;
   for (int i=0; i<=8000; i++) {
      ii = i / 1000.0;
      cout << ii << "\t";
      rd.setDuration(ii, 0);
      rd.printHumdrum();

      cout << "\t"; 
      rd.setDuration(ii, 1);
      rd.printHumdrum();

      cout << "\t"; 
      rd.setDuration(ii, 2);
      rd.printHumdrum();

      cout << "\t"; 
      rd.setDuration(ii, 3);
      rd.printHumdrum();

      cout << endl;
   }
}



