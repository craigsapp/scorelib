//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun  2 08:22:02 PDT 2015
// Last Modified: Tue Jun  2 08:22:05 PDT 2015
// Filename:      pagenum.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/pagenum.cpp
// Documentation: http://scorelib.sapp.org/program/pagenum
// Syntax:        C++ 11
//
// Description:   Identifies, adds and processes page numbers on SCORE pages.
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void   processOptions       (Options& opts, int argc, char** argv);
void   identifyPageNumbers  (ScorePageSet& infiles);
void   identifyPageNumbers  (ScorePage& infile);
void   identifyPageNumbers  (ScorePage& infile, int pageindex);
void   identifyPageNumbers  (vectorSIp& items, int pageindex, int direction);
int    onlyHasNumbers       (ScoreItem* sip);
void   identifyPageNumber   (ScoreItem* item, int pageindex);

// user-interface variables:
Options options;
int aboveQ  = 1;       // used with --above-only option
int belowQ  = 1;       // used with --below-only option
int labelQ  = 0;       // used with -l option
int code10Q = 1;       // used with --number-only option
int code16Q = 1;       // used with --text-only option
int abovePos = 25;     // cutoff for searching for page number above staff
int belowPos = -20;    // cutoff for searching for page number below staff
int LastPage = -1;     // used for identifing breaks in page number sequence

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   identifyPageNumbers(infiles);
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// labelPageNumbers --
//

void labelPageNumbers(ScorePageSet& infiles) {
}



//////////////////////////////
//
// identifyPageNumbers --
//

void  identifyPageNumbers(ScorePageSet& infiles) {
   for (int i=0; i<infiles.getPageCount(); i++) {
      identifyPageNumbers(infiles[i][0], i);
   }
}


void identifyPageNumbers(ScorePage& infile, int pageindex) {
   int syscount = infile.getSystemCount();
   if (aboveQ) {
      identifyPageNumbers(infile.getSystemItems(0), pageindex, 1);
   }
   if (belowQ) {
      identifyPageNumbers(infile.getSystemItems(syscount-1), pageindex, -1);
   }
}


void  identifyPageNumbers(vectorSIp& items, int pageindex, int direction) {
   vectorSIp foundlist;
   int minstaff = 1000;
   int maxstaff = -1000;
   int staffnum;
   double vpos;
   int i;

   for (i=0; i<items.size(); i++) {
      staffnum = items[i]->getStaffNumber();
      if (staffnum < minstaff) {
         minstaff = staffnum;
      }
      if (staffnum > maxstaff) {
         maxstaff = staffnum;
      }
   }

   for (i=0; i<items.size(); i++) {
      if (!((code10Q && items[i]->isNumberItem()) ||
          (code16Q && items[i]->isTextItem()))) {
         continue;      
      }
      staffnum = items[i]->getStaffNumber();
      if ((direction == 1) && (staffnum != maxstaff)) {
         continue;
      }
      if ((direction == -1) && (staffnum != minstaff)) {
         continue;
      }
      vpos = items[i]->getVerticalPosition();
      if ((direction == 1) && (vpos < abovePos)) {
         continue;
      } else if ((direction == -1) && (vpos > belowPos)) {
         continue;
      }

      if (code10Q && items[i]->isNumberItem()) {
         foundlist.push_back(items[i]);
      }

      if (code16Q && items[i]->isTextItem()) {
         if (onlyHasNumbers(items[i])) {
            foundlist.push_back(items[i]);
         }
      }
   }

   int highest = 0;
   int lowest = 0;
   double highestvpos;
   double lowestvpos;
   if (foundlist.size() > 1) {
      // choose only the highest or lowest number on the page.
      for (i=1; i<foundlist.size(); i++) {
         vpos = foundlist[i]->getVerticalPosition();
         if (vpos > highestvpos) {
            highest = i;
            highestvpos = vpos;
         }
         if (vpos < lowestvpos) {
            lowest = i;
            lowestvpos = vpos;
         }
      }
      if (direction == 1) {
         // choose highest item
         identifyPageNumber(foundlist[highest], pageindex);
      } else if (direction == -1) {
         // chose lowest item
         identifyPageNumber(foundlist[lowest], pageindex);
         
      }
   } else if (foundlist.size() == 1) {
      identifyPageNumber(foundlist[0], pageindex);
   }


}



//////////////////////////////
//
// identifyPageNumber -- Mark or print page number.
//

void identifyPageNumber(ScoreItem* item, int pageindex) {
   int pagenum = 0;

   if (labelQ) {
      // label page here with "@function: pagenum"
   } else {
      cout << "Page_index " << pageindex << ":\t";
      if (item->isNumberItem()) {
         cout << item->getP5();
         pagenum = (int)item->getP5();
      } else if (item->isTextItem()) {
         cout << item->getTextWithoutInitialFontCode();
         pagenum = (int)strtod(item->getTextWithoutInitialFontCode().data(), 
                               NULL);
      }
      if (LastPage >= 0) {
         if (pagenum - LastPage != 1) {
            cout << "\tNOT IN SEQUENCE";
         }
      }
      cout << endl;
      LastPage = pagenum;
   }
}



//////////////////////////////
//
// onlyHasNumbers -- Make page number definition more elaborate later.
//     For now, allow only numbers in page number, and optionally spaces.
//

int onlyHasNumbers(ScoreItem* sip) {
   string text = sip->getTextWithoutInitialFontCode();
   int numcount = 0;
   int spacecount = 0;
   for (int i=0; i<text.size(); i++) {
      if (isdigit(text[i])) {
         numcount++;
      } else if (isspace(text[i])) {
         spacecount++;
      } else {
         return 0;
      }
   }
   if (numcount == 0) {
      return 0;
   } else {
      return 1;
   }
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("above-only|top-only|top|above=b", 
               "Search for page numbers only at top of pages");
   opts.define("bottom-only|below-only|bottom|below=b",
               "Search for page numbers only at bottom of pages");
   opts.define("text-only|text=b", 
               "Search only text items");
   opts.define("numbers-only|number-only|numbers|number=b", 
               "Search only number items");
   opts.define("above-cutoff=d:25.0", 
               "Min. vertical pos. for page nums. above staff");
   opts.define("below-cutoff=d:-20.0", 
               "Max. vertical pos. for page nums. below");
   opts.process(argc, argv);
   opts.process(argc, argv);

   if (opts.getBoolean("above-only")) {
      aboveQ = 1;
      belowQ = 0;
   } else if (opts.getBoolean("below-only")) {
      aboveQ = 0;
      belowQ = 1;
   }

   if (opts.getBoolean("text-only")) {
      code10Q = 0;
      code16Q = 1;
   } else if (opts.getBoolean("numbers-only")) {
      code10Q = 1;
      code16Q = 0;
   }

   abovePos = opts.getDouble("above-cutoff");
   belowPos = opts.getDouble("below-cutoff");

}



