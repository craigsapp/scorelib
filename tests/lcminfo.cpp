//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 24 16:35:00 PDT 2014
// Last Modified: Thu Apr 24 16:35:02 PDT 2014
// Filename:      lcminfo.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/lcminfo.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/lcminfo
// Syntax:        C++ 11
//
// Description:   Extract the least-common-multiple rhythmic value from music.
//

#include "scorelib.h"
#include <set>

using namespace std;


void   processData          (ScorePage& infile, Options& opts);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);

   ScorePageSet infiles(opts);

   int pages = infiles.getPageCount();
   int overlay = 0;
   int systems;
   int number;
   set<int> numbers;

   int i, j;
   for (i=0; i<pages; i++) {
      systems = infiles[i][overlay].getSystemCount();
      for (j=0; j<systems; j++) {
         cout << "P" << i+1 << "\tS" << j+1 << "\t";
         int number = infiles[i][overlay].getSystemLCMRhythm(j);
         numbers.insert(number);
         cout << number << endl;
      }
   }
   cout << "LCM of all rhythms: " << SU::lcm(numbers) << endl;

   return 0;
}

///////////////////////////////////////////////////////////////////////////


