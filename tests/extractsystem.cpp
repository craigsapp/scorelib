//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar  7 01:51:25 PST 2014
// Last Modified: Fri Mar  7 01:51:29 PST 2014
// Filename:      extractystem.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/extractystem.cpp
// Syntax:        C++11
//
// Description:   Extract a given system from the page.  The first system
//                is "1" at the top of the page, increasing downwards.
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

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   opts.define("p|pitch=b",    "Analyze pitch content of notes.");
   opts.define("c|count=b",    "Count the number of systems.");
   opts.define("s|system=i:1", "Extract given system number.");
   opts.define("d|duration=b", "Analyze staff duration offset of notes/rests.");
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
   if (opts.getBoolean("count")) {
      cout << infile.getSystemCount() << endl;
      exit(0);
   }
   if (opts.getBoolean("pitch")) {
      infile.analyzePitch();
   }
   if (opts.getBoolean("duration")) {
      infile.analyzeStaffDurations();
   }

   int system = opts.getInteger("system") - 1;
   int syscount = infile.getSystemCount();

   if ((system >= 0) && (system < syscount)) {
      cout << infile.systemItems(system);
   }
}



