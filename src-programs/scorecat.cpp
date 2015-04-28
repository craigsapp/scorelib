//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 12 20:16:47 PDT 2014
// Last Modified: Sat Apr 12 20:56:24 PDT 2014
// Filename:      scorecat.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/scorecat.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/scorecat
// Syntax:        C++ 11
//
// Description:   Concatenate multiple score pages into a single output stream.
//

#include "scorelib.h"

using namespace std;

void    printBySystem                 (ScorePageSet& infiles, int extra);
void    printPageBySystem             (ScorePage& page, int extra);
void    printBySystemWithBarlines     (ScorePageSet& infiles, int extra);
void    printPageBySystemWithBarlines (ScorePage& page, int extra);

// User interface variables:
int zeroQ     = 0;
int commentQ  = 0;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("s|system=b", "sort data by system for each page");
   opts.define("S|suppress-named-parameters=b", "Do not print named parameters.");
   opts.define("b|m|barline|measure=b", "indicate measures");
   opts.define("z|0|zero-indexing=b", "index from zero instead of one");
   opts.define("c|comment=b", "encode multipage info as comments");
   opts.process(argc, argv);

   if (opts.getBoolean("zero-indexing")) {
      zeroQ = 1;
   }
   if (opts.getBoolean("comment")) {
      commentQ = 1;
   }
   
   ScorePageSet infiles(opts);

   if (opts.getBoolean("system")) {
      if (opts.getBoolean("measure")) {
         printBySystemWithBarlines(infiles, !opts.getBoolean("S"));
      } else {
         printBySystem(infiles, !opts.getBoolean("S"));
      }
   } else {
      cout << infiles;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// printBySystemWithBarlines -- print each page/overlay by system 
//      (top to bottom of page).
//

void printBySystemWithBarlines(ScorePageSet& infiles, int extra) {
cout << "GOT HERE CCC" << endl;
   int i, j;
   for (i=0; i<infiles.getPageCount(); i++) {
      if (commentQ) {
         if (i>0) {
            cout << "\n";
         }
         cout << "###ScorePage:\t" << infiles[i][0].getFilenameBase() << endl;
      } else { 
         cout << "RS" << endl;
         cout << "SA " << infiles[i][0].getFilenameBase() << endl;
      }
      cout << endl;
      for (j=0; j<infiles[i].getOverlayCount(); j++) {
         if (j>0) {
            cout << "\n###ScoreOverlay:\t" 
                 << infiles[i][j].getFilenameBase() << endl;
         }
         printPageBySystemWithBarlines(infiles[i][j], extra);
      }
      if (!commentQ) {
         cout << "\nSM\n";
      }
   }
}



//////////////////////////////
//
// printBySystem -- print each page/overlay by system (top to bottom of page).
//

void printBySystem(ScorePageSet& infiles, int extra) {
cout << "GOT HERE BBB" << endl;
   int i, j;
   for (i=0; i<infiles.getPageCount(); i++) {
      if (i>0) {
         cout << "\n";
      }
      cout << "###ScorePage:\t" << infiles[i][0].getFilenameBase() << endl;
      cout << "\n";
      for (j=0; j<infiles[i].getOverlayCount(); j++) {
         if (j>0) {
            cout << "\n###ScoreOverlay:\t" 
                 << infiles[i][j].getFilenameBase() << endl;
         }
         printPageBySystem(infiles[i][j], extra);
      }
   }
}



//////////////////////////////
//
// printPageBySystem -- Print a page of music sorted by system and
//    horizontal position (time).
//

void printPageBySystem(ScorePage& page, int extra) {
cout << "GOT HERE AAA" << endl;
   int scount = page.getSystemCount();
   int i, j;
   int count;
   for (i=0; i<scount; i++) {
      if (i>0) { 
         cout << "\n";
      }
      count = i;
      if (!zeroQ) {
         count++;
      }
      cout << "##System:\t" << count << "\n";
      sort(page.getSystemItems(i).begin(), page.getSystemItems(i).end(), 
            SU::sortP3P1P2P4);
      if (extra) {
         vectorSIp& items = page.getSystemItems(i);
         for (j=0; j<items.size(); j++) {
            items[j]->printPmx(cout);
            cout << "@auto@pageOffsetDuration:\t";
            cout << items[j]->getParameter("auto", "pageOffsetDuration");
            cout << endl;
         }
      } else {
         cout << page.getSystemItems(i); 
      }
   }
}



//////////////////////////////
//
// printPageBySystemWithBarlines -- Print a page of music sorted 
//    by system and horizontal position (time) with barlines
//    indicated.
//

void printPageBySystemWithBarlines(ScorePage& page, int extra) {
cout << "GOT HERE DDD" << endl;
   int syscount = page.getSystemCount();
   int barcount;
   int i, j;
   int scount;
   int mcount;
   for (i=0; i<syscount; i++) {
      if (i>0) { 
         cout << "\n";
      }
      scount = i;
      if (!zeroQ) {
         scount++;
      }
      cout << "##System:\t" << scount << "\n";
      cout << endl;
      barcount = page.getSystemBarCount(i);
      for (j=0; j<barcount; j++) {
         SystemMeasure& measureitems = page.getSystemMeasure(i, j);
         mcount = j;
         if (!zeroQ) {
            mcount++;
         }
         if (measureitems.getDuration() != 0.0) {
            cout << "##SystemBar:\t" << scount << ":" << mcount;
            cout << "\t(dur = " << measureitems.getDuration() << ")";
            cout << "\n";
         }
         printNoAnalysis(cout, measureitems.getItems());
         cout << "\n";
      }
   }
}



