//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar 30 19:06:09 PDT 2014
// Last Modified: Sun Mar 30 19:06:11 PDT 2014
// Filename:      itemdur.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/itemdur.cpp
// Syntax:        C++ 11
//
// Description:   Convert a floating-point durations into a rational duration.
//

#include "ScorePage.h"
#include "Options.h"
#include <iostream>

using namespace std;

void processData(ScorePage& infile);

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
   listSIp& data = infile.lowLevelDataAccess();
   RationalDuration rd;
   for (auto& it : data) {
      if (!it->hasDuration()) {
         // don't analyze durations of non-note items
         continue;
      }
      double duration = it->getDuration();
      int dots = it->getDotCount();
      rd = it->getRationalDuration();
      cout << duration;
      if (dots > 0) {
         cout << "\tdots=" << dots;
      }
      cout << "\t";
      rd.printHumdrum(cout);
      cout << endl;
      // it.setParameter("analysis", "rationalDuration", it.
   }
}



