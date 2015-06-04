//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Jun  3 00:32:42 PDT 2015
// Last Modified: Wed Jun  3 01:40:53 PDT 2015
// Filename:      barlinenum.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/barlinenum.cpp
// Documentation: http://scorelib.sapp.org/program/barlinenum
// Syntax:        C++ 11
//
// Description:   Identifies, adds and processes barline numbers on SCORE pages.
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void   processOptions       (Options& opts, int argc, char** argv);
void   identifyBarNumbers   (ScorePageSet& infiles);
void   identifyBarNumbers   (ScorePage& infile);
void   identifyBarNumbers   (ScorePage& infile, int pageindex, int pagecount);
void   identifyBarNumbers   (vectorSIp& items, int pageindex, int sysindex, 
                             int pagecount, int syscount);
int    onlyHasNumbers       (ScoreItem* sip);
int    startsWithNumber     (ScoreItem* sip);
void   identifyBarNumber    (ScoreItem* item, int pageindex, int sysindex, 
                             int pagecount, int syscount);

// user-interface variables:
Options options;
int leftQ    = 1;     // used with --left-only
int labelQ   = 0;     // used with -l option
int code10Q  = 1;     // used with --number-only option
int code16Q  = 1;     // used with --text-only option
int minAbovePos = 11; // cutoff for searching for page number above staff
int maxAbovePos = 20; // cutoff for searching for page number above staff
int LastBarnum = -1;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   identifyBarNumbers(infiles);
   if (labelQ) {
      cout << infiles;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// identifyBarNumbers --
//

void  identifyBarNumbers(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   for (int i=0; i<pagecount; i++) {
      identifyBarNumbers(infiles[i][0], i, pagecount);
   }
}


void identifyBarNumbers(ScorePage& infile, int pageindex, int pagecount) {
   int syscount = infile.getSystemCount();
   for (int i=0; i<syscount; i++) {
      identifyBarNumbers(infile.getSystemItems(i), pageindex, i, pagecount, 
            syscount);
   }
}


void  identifyBarNumbers(vectorSIp& items, int pageindex, int sysindex, 
      int pagecount, int syscount) {
   vectorSIp foundlist;
   int minstaff = 1000;
   int maxstaff = -1000;
   double systemleftpos = -10;
   int staffnum;
   double vpos;
   double hpos;
   int moffset = 10;
   int i;
   int duritempos = 0;

   for (i=0; i<items.size(); i++) {
      staffnum = items[i]->getStaffNumber();
      if (staffnum < minstaff) {
         minstaff = staffnum;
      }
      if (staffnum > maxstaff) {
         maxstaff = staffnum;
         systemleftpos = items[i]->getHorizontalPosition();
      }
      if ((duritempos <= 0) && (items[i]->hasDuration())) {
         duritempos = items[i]->getHorizontalPosition();
      }
   }

   for (i=0; i<items.size(); i++) {
      if (!((code10Q && items[i]->isNumberItem()) ||
          (code16Q && items[i]->isTextItem()))) {
         continue;      
      }
      if (items[i]->isTextItem() && (!startsWithNumber(items[i]))) {
         continue;
      }
      staffnum = items[i]->getStaffNumber();
      if (staffnum != maxstaff) {
         // measure number only at top of system
         continue;
      }
      hpos = items[i]->getHorizontalPosition();
      if (leftQ && (hpos - systemleftpos > moffset)) {
         continue;
      }

      vpos = items[i]->getVerticalPosition();
      if (vpos < minAbovePos) {
         continue;
      }
      if (vpos > maxAbovePos) {
         continue;
      }
      if (leftQ) {
         if (hpos < duritempos) {
            if (code10Q && items[i]->isNumberItem()) {
               foundlist.push_back(items[i]);
            } else if (code16Q && items[i]->isTextItem()) {
               if (startsWithNumber(items[i])) {
                  foundlist.push_back(items[i]);
               }
            }
            break;
         } else {
            // out off range, so give up
            break;
         }
      }
   }


   if (foundlist.size() == 1) {
      identifyBarNumber(foundlist[0], pageindex, sysindex, pagecount, syscount);
   } else if (foundlist.size() > 1) {
      cerr << "Error: found " << foundlist.size() << " numbers on bar" << endl;
      exit(1);
   } else {
      // cout << "NO BAR NUMBER ON SYSTEM" << endl;
   }
}



//////////////////////////////
//
// identifyBarNumber -- Mark or print page number.
//

void identifyBarNumber(ScoreItem* item, int pageindex, int sysindex, 
      int pagecount, int syscount) {
   int barnum = 0;

   if (labelQ) {
      item->setParameter(np_function, "barnum");
   } else {
      if (pageindex == 0) {
         cout << "[";
      }
      cout << "{";
      cout << "\"page_index\":" << pageindex << ",\t";
      cout << "\"sys_index\":" << sysindex << ",\t";
      if (leftQ) {
         cout << "\"sys_measure_index\":" << 0 << ",\t";
      }
      stringstream mnum;
      if (item->isNumberItem()) {
         mnum << item->getP5();
         barnum = (int)item->getP5();
      } else if (item->isTextItem()) {
         mnum << item->getTextWithoutInitialFontCode();
         barnum = (int)strtod(item->getTextWithoutInitialFontCode().data(), 
                               NULL);
      }
      int numeric = onlyHasNumbers(item);
      cout << "\"barnum\":";
      if (!numeric) {
         cout << "\"";
      }
      // also check if the string has any double quotes...
      cout << mnum.str();
      if (!numeric) {
         cout << "\"";
      }

      if (LastBarnum >= 0) {
         if (barnum < LastBarnum) {
            cout << "\t\"new_sequence\":\"true\"";
         }
      }

      cout << "}";
      if (pageindex < pagecount - 1) {
         cout << ",";
      } else if (pageindex == pagecount - 1) {
         if (sysindex < syscount - 1) {
            cout << ",";
         } else {
            cout << "]";
         }
      }
      cout << endl;
      LastBarnum = barnum;
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
// startsWithNumber -- Return true if first non-font character is a digit.
//

int startsWithNumber(ScoreItem* sip) {
   string text = sip->getTextWithoutInitialFontCode();
   if (text.size() == 0) {
      return 0;
   }
   return isdigit(text[0]);
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("left-only=b", 
               "Search for barlines only at left side of system");
   opts.define("text-only|text=b", 
               "Search only text items");
   opts.define("numbers-only|number-only|numbers|number=b", 
               "Search only number items");
   opts.define("above-cutoff=d:20.0", 
               "Max. vertical pos. for page nums. above staff");
   opts.define("l|label=b", "Label barnum items");
   opts.process(argc, argv);
   opts.process(argc, argv);

   if (opts.getBoolean("text-only")) {
      code10Q = 0;
      code16Q = 1;
   } else if (opts.getBoolean("numbers-only")) {
      code10Q = 1;
      code16Q = 0;
   }

   maxAbovePos = opts.getDouble("above-cutoff");
   labelQ      = opts.getBoolean("label");

   // leftQ hardcoded to 1 for now:
   // leftQ    = opts.getBoolean("left-only);
}



