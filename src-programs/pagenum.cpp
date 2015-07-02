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
void   identifyPageNumbers  (ScorePage& infile, int pageindex, int pagecount);
void   identifyPageNumbers  (vectorSIp& items, int pageindex, int direction, 
                             int pagecount);
int    onlyHasNumbers       (ScoreItem* sip);
void   identifyPageNumber   (ScoreItem* item, int pageindex, int pagecount);

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
   if (labelQ) {
      cout << infiles;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// identifyPageNumbers --
//

void  identifyPageNumbers(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   for (int i=0; i<pagecount; i++) {
      identifyPageNumbers(infiles[i][0], i, pagecount);
   }
}


void identifyPageNumbers(ScorePage& infile, int pageindex, int pagecount) {
   int syscount = infile.getSystemCount();
   if (aboveQ) {
      identifyPageNumbers(infile.getSystemItems(0), pageindex, 1, pagecount);
   }
   if (belowQ) {
      identifyPageNumbers(infile.getSystemItems(syscount-1), pageindex, -1, 
            pagecount);
   }
}


void  identifyPageNumbers(vectorSIp& items, int pageindex, int direction, 
      int pagecount) {
   vectorSIp foundlist;
   int minstaff = 1000;
   int maxstaff = -1000;
   int staffnum;
   double vpos;
   int i;

   for (i=0; i<(int)items.size(); i++) {
      staffnum = items[i]->getStaffNumber();
      if (staffnum < minstaff) {
         minstaff = staffnum;
      }
      if (staffnum > maxstaff) {
         maxstaff = staffnum;
      }
   }

   for (i=0; i<(int)items.size(); i++) {
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
   double highestvpos = -1000;
   double lowestvpos = 1000;
   if (foundlist.size() > 1) {
      // choose only the highest or lowest number on the page.
      for (i=0; i<(int)foundlist.size(); i++) {
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
         identifyPageNumber(foundlist[highest], pageindex, pagecount);
      } else if (direction == -1) {
         // chose lowest item
         identifyPageNumber(foundlist[lowest], pageindex, pagecount);
      }
   } else if (foundlist.size() == 1) {
      identifyPageNumber(foundlist[0], pageindex, pagecount);
   }


}



//////////////////////////////
//
// identifyPageNumber -- Mark or print page number.
//

void identifyPageNumber(ScoreItem* item, int pageindex, int pagecount) {
   int pagenum = 0;

   if (labelQ) {
      item->setParameterNoisy(np_function, "pagenum");
   } else {
      if (pageindex == 0) {
         cout << '[';
      }
      cout << "{\"page_index\":" << pageindex << ",\t\"page_number\":";
      if (item->isNumberItem()) {
         cout << item->getP5();
         pagenum = (int)item->getP5();
      } else if (item->isTextItem()) {
         stringstream pnum;
         pnum << item->getTextWithoutInitialFontCode();
         int numeric = onlyHasNumbers(item);
         if (!numeric) {
            cout << "\"";
         }
         cout << pnum.str();
         if (!numeric) {
            cout << "\"";
         }
         pagenum = (int)strtod(item->getTextWithoutInitialFontCode().data(), 
                               NULL);
      }
      if (LastPage >= 0) {
         if (pagenum - LastPage != 1) {
            cout << ", \"in_sequence\":\"false\"";
         }
      }
      cout << "}";
      if (pageindex < pagecount - 1) {
         cout << ",";
      }
      if (pageindex == pagecount - 1) {
         cout << ']';
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
   if (sip->isNumberItem()) {
      return 1;
   }
   string text = sip->getTextWithoutInitialFontCode();
   int numcount = 0;
   int spacecount = 0;
   for (int i=0; i<(int)text.size(); i++) {
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
   opts.define("ac|above-cutoff=d:25.0", 
               "Min. vertical pos. for page nums. above staff");
   opts.define("bc|below-cutoff=d:-20.0", 
               "Max. vertical pos. for page nums. below");
   opts.define("l|label=b", "label function of page numbers");
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

   labelQ   = opts.getBoolean("label");
   abovePos = opts.getDouble("above-cutoff");
   belowPos = opts.getDouble("below-cutoff");
}



