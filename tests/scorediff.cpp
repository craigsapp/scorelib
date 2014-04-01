//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 31 12:26:57 PDT 2014
// Last Modified: Mon Mar 31 12:26:59 PDT 2014
// Filename:      scorediff.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/scorediff.cpp
// Syntax:        C++11
//
// Description:   Highlight changes in red, with blue highlighting for 
//                deleted/moved objects.
//
// $Smake:		smake -qv chopin %f
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePage.h"
#include "Options.h"

#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

void       processData      (ScorePage& infile1, ScorePage& infile2);

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   opts.process(argc, argv);
   
   ScorePage infile1;
   ScorePage infile2;

   int argcount = opts.getArgCount();
   for (int i=0; i<argcount-1; i+=2) {
      infile1.read(opts.getArg(i+1));
      infile2.read(opts.getArg(i+2));
      processData(infile1, infile2);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////

/////////////////////////////
//
// processData --
//

void processData(ScorePage& infile1, ScorePage& infile2) {
   listSIp common;   // unchanged items between files.
   listSIp changed;  // added or changed items in new file.
   listSIp removed;  // deleted or changed items in old file.
   
   vectorVSIp& items1 = infile1.staffItems();
   vectorVSIp& items2 = infile2.staffItems();
   int maxstaff1 = infile1.getMaxStaff();
   int maxstaff2 = infile2.getMaxStaff();
   int maxstaff = min(maxstaff1, maxstaff2);

   vector<int> marked;

   int i, j, k;
   int same;
   for (i=0; i<=maxstaff; i++) {
      marked.resize(items2[i].size());
      fill(marked.begin(), marked.end(), 0);
      for (j=0; j<items1[i].size(); j++) {
         same = -1;
         for (k=0; k<items2[i].size(); k++) {
            if (marked[k]) {
               continue;
            }
            if (*items1[i][j] == *items2[i][k]) {
               same = k;
               marked[k] = 1;
               break;
            }
         }
         if (same >= 0) {
            common.push_back(items1[i][j]);
         } else {
            changed.push_back(items1[i][j]);
         }
      }
      for (k=0; k<marked.size(); k++) {
         if (!marked[k]) {
            removed.push_back(items2[i][k]);
         }
      }
   }

   ScoreItem redcolor;
   redcolor.makeSvgCode("<g color=\"red\" stroke=\"red\">");

   ScoreItem bluecolor;
   bluecolor.makeSvgCode("<g color=\"blue\" stroke=\"blue\">");

   ScoreItem opacity;
   opacity.makeSvgCode("<g style=\"opacity:0.50\">");

   ScoreItem endtag;
   endtag.makeSvgCode("</g>");

   // print common items in black:
   if ((removed.size() > 0) || (changed.size() > 0)) {
      cout << "\n#COMMON: " << endl;
   }
   cout << common;

   // print added/changed items in red:
   if (changed.size() > 0) {
      cout << "\n#ADDED/CHANGED: " << endl;
      cout << redcolor;
      cout << endl;
      cout << changed;
      cout << endtag;
   }
 
   // print removed/changed items in blue:
   if (removed.size() > 0) {
      cout << "\n#REMOVED/CHANGED: " << endl;
      cout << opacity;
      cout << bluecolor;
      cout << endl;
      cout << removed;
      cout << endl;
      cout << endtag;
      cout << endtag;
   }
}



