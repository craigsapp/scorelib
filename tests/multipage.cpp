//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 10 19:26:58 PDT 2014
// Last Modified: Sat Apr 12 02:52:39 PDT 2014
// Filename:      multipage.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/multipage.cpp
// Syntax:        C++ 11
//
// Description:   Create a multiple-page PPMX file.
//

#include "scorelib.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

   ScorePageSet outfile;

   ScorePage* page1 = outfile.createPage();
   ScorePage* page2 = outfile.createPage();
   ScorePage* page3 = outfile.createPage();

   ScorePage* overlay1 = outfile[0].createOverlay();
   ScorePage* overlay2 = outfile[1].createOverlay();

   page1->setFilename("page1");
   overlay1->setFilename("overlay1");

   page2->setFilename("page2");

   page3->setFilename("page3");

   page1->addPmxData("8 1\n1 1 10 1 10 0 1");
   overlay1->addPmxData("9 1 10 6 10");

   page2->addPmxData("8 2\n1 2 10 1 10 0 1");
   overlay2->addPmxData("9 2 30 1 30");

   page3->addPmxData("8 3\n1 3 10 1 10 0 1");

   cout << outfile;


   return 0;
}

///////////////////////////////////////////////////////////////////////////

