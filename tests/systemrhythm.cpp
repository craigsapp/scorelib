//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 00:03:32 PDT 2014
// Last Modified: Mon Mar 10 00:03:38 PDT 2014
// Filename:      systemrhythm.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/analyzepitch.cpp
// Syntax:        C++11
//
// Description:   This program extracts the composite rhythm of music on each
// 		  system.
//
// $Smake:		smake -qv chopin %f
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePage.h"
#include "Options.h"

#include <iostream>
#include <set>
#include <stdlib.h>

using namespace std;

void       processData        (ScorePage& infile);
void       processSystem      (int sysnum, vectorSIp& systemitems);

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
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
   infile.analyzeStaffDurations();
   infile.analyzeSystems();


   int syscount = infile.getSystemCount();
   for (int i=0; i<syscount; i++) {
      processSystem(i, infile.systemItems(i));
   }

}


///////////////////////////////
//
// processSystem --
//

void processSystem(int sysnum, vectorSIp& systemitems) {
   set<SCORE_FLOAT> offsets;
   for (auto it = systemitems.begin(); it != systemitems.end(); it++) {
      if (!(*it)->hasDuration()) {
         continue;
      }
      offsets.insert((*it)->getParameterDouble("analysis", 
            "staffOffsetDuration"));
   }

   vector<SCORE_FLOAT> offsets2;
   offsets2.reserve(offsets.size());

   cout << "\n#SYSTEM: " << sysnum << endl;
   for (auto it = offsets.begin(); it != offsets.end(); it++) {
      offsets2.push_back(*it);
   }

   for (int i=0; i<offsets2.size(); i++) {
      cout << offsets2[i];
      if (i<offsets2.size()-1) {
         cout << "\t";
         cout << offsets2[i+1] - offsets2[i];
      }
      cout << "\n";
   }
}



