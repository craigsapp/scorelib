//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar  7 01:51:25 PST 2014
// Last Modified: Fri Mar  7 01:51:29 PST 2014
// Filename:      systemsort.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/systemsort.cpp
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

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   opts.define("p|pitch=b",    "Analyze pitch content of notes.");
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
   if (opts.getBoolean("pitch")) {
      infile.analyzePitch();
   }
   if (opts.getBoolean("duration")) {
      infile.analyzeStaffDurations();
   }

   int syscount = infile.getSystemCount();
   for (int i=0; i<syscount; i++) {
      if (i > 0) {
         cout << endl;
      }
      cout << "# SYSTEM " << i << endl;
      cout << infile.systemItems(i);
   }

}



