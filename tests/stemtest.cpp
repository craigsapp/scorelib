//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  4 01:36:11 PST 2014
// Last Modified: Tue Mar  4 01:36:14 PST 2014
// Filename:      stemtest.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/stemtest.cpp
// Syntax:        C++11
//
// Description:   Test flipping of stems (and beams).
//
// $Smake:		rm %b; make %b
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePageBase.h"
#include "Options.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {
   Options opts;
   opts.process(argc, argv);

   ScorePageBase infile;
   vectorSIp items;

   int i;
   int argcount = opts.getArgCount();
   for (i=0; i<argcount; i++) {
      if (argcount == 0) {
        infile.read(cin);
      } else  {
         infile.read(opts.getArg(i+1));
      }
      if (argcount > 1) {
         cout << "# PAGE " << i+1 << " ########################" << endl;
      }
      infile.getFileOrderList(items);

      cout << "# ITEM COUNT " << infile.getItemCount() << endl;
      for (i=0; i<items.size(); i++) {
         items[i]->stemUp();
      }

      cout << infile;
   }

   return 0;
}


