//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 23:34:34 PST 2014
// Last Modified: Tue Mar  4 00:36:34 PST 2014
// Filename:      readtest.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/readtest.cpp
// Syntax:        C++11
//
// Description:   Test conversion of a PMX/binary file into PMX data.
//
// $Smake:		rm %b; make %b
// $Smake-chopin:	./%b data/chopin2806.mus
// $Smake-chopin2:	./%b data/chopin2806.pmx
//

#include "ScorePageBase.h"
#include "Options.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);

   ScorePageBase infile;

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
      cout << infile;
   }

   return 0;
}


