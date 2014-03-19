//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar  6 02:32:39 PST 2014
// Last Modified: Thu Mar  6 02:32:43 PST 2014
// Filename:      systeminfo.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/systeminfo.cpp
// Syntax:        C++11
//
// Description:   This program prints system information about staves on a page.
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
   infile.analyzeSystems();
   infile.analyzeStaffDurations();
   int maxstaff = infile.getMaxStaff();
   cout << "P2\tsystem\tstaffsystem\tduration\n";
   cout << "===============================================\n";
   int lastsystem = -2;
   int system;
   for (auto i=maxstaff; i>0; i--) {
      system = infile.systemMap(i);
      if (system != lastsystem) {
         cout << endl;
      } 
      lastsystem = system;
      cout << i << "\t";                  // P2 staff number
      cout << infile.systemMap(i) << "\t";         // system index
      cout << infile.systemStaffMap(i) << "\t\t";  // system-staff index
      cout << infile.getStaffDuration(i) << endl;  // duration
   }
   cout << endl;
}



