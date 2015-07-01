//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Jun  4 14:30:26 PDT 2015
// Last Modified: Thu Jun  4 14:30:29 PDT 2015
// Filename:      fingernum.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/fingernum.cpp
// Documentation: http://scorelib.sapp.org/program/fingernum
// Syntax:        C++ 11
//
// Description:   Identifies, adds and processes fingering numbers on SCORE pages.
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void   processOptions           (Options& opts, int argc, char** argv);
void   identifyFingerNumbers    (ScorePageSet& infiles);
void   identifyFingerNumbers    (ScorePage& infile, int pageindex, 
                                 int pagecount);
void   identifyFingerNumbers    (ScorePage& infile, int pageindex, int staffnum,
                                 int pagecount);
void   identifyFingerNumbers    (vectorSIp& items, int pageindex, int sysindex, 
                                 int pagecount, int syscount);
int    onlyHasNumbers           (ScoreItem* sip);
double getNearestNoteP3Backward (vectorSIp& items, int index);
double getNearestNoteP3Forward  (vectorSIp& items, int index);
void   identifyFingerNumber     (ScoreItem* sip);
double getFingerNum             (ScoreItem* sip);
void   removeFingerings         (ScorePageSet& infiles);
void   showOnlyFingerings       (ScorePageSet& infiles);

// user-interface variables:
Options options;
int    labelQ          = 0;   // used with -l option
int    code10Q         = 1;   // used with --number-only option
int    code16Q         = 1;   // used with --text-only option
double FingerThreshold = 1.0; // max horizontal distance from a note position
int    MaxFinger       = 5;   // hightest finger number allowed
int    removeQ         = 0;   // remove fingerings
int    removedQ        = 0;   // output only fingerings which can be removed
int    analysisQ       = 1;   // used with -A option


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   if (analysisQ) {
      identifyFingerNumbers(infiles);
   }
   if (labelQ) {
      if (removeQ) {
         removeFingerings(infiles);
      } else if (removedQ) {
         showOnlyFingerings(infiles);
      } else {
         cout << infiles;
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// removeFingerings --
//

void removeFingerings(ScorePageSet& infiles) {
   int i, j, k;
   int pagecount = infiles.getPageCount();
   int syscount;
   string function;

   for (i=0; i<pagecount; i++) {
      if (pagecount > 1) {
         cout << "RS" << endl;
         cout << "SA " << infiles[i][0].getFilenameBase() << endl;
         cout << endl;
      }

      syscount = infiles[i][0].getSystemCount();
      for (j=0; j<syscount; j++) {
         vectorSIp& items = infiles[i][0].getSystemItems(j);
         for (k=0; k<items.size(); k++) {
            function = items[k]->getParameter(np_function);
            if (function == "fingering") {
               continue;
            }
            cout << items[k];
         }
      }

      if (pagecount > 1) {
         cout << endl;
         cout << "SM" << endl;
      }
   }
}



//////////////////////////////
//
// showOnlyFingerings --
//

void showOnlyFingerings(ScorePageSet& infiles) {
   int i, j, k;
   int pagecount = infiles.getPageCount();
   int syscount;
   string function;
   ScoreItem si;

   for (i=0; i<pagecount; i++) {
      if (pagecount > 1) {
         cout << "RS" << endl;
         cout << "SA " << infiles[i][0].getFilenameBase() << endl;
         cout << endl;
      }

      syscount = infiles[i][0].getSystemCount();
      for (j=0; j<syscount; j++) {
         vectorSIp& items = infiles[i][0].getSystemItems(j);
         for (k=0; k<items.size(); k++) {
            function = items[k]->getParameter(np_function);
            if (function == "fingering") {
               cout << items[k];
            } else if (items[k]->isStaffItem()) {
               si = *items[k];
               si.makeInvisible();
               cout << si;
            } else if (items[k]->isRestItem()) {
               si = *items[k];
               si.makeInvisible();
               cout << si;
            } else if (items[k]->isNoteItem()) {
               si.clear();
               si.setP1(2);
               si.setP2(items[k]->getP2());
               si.setP3(items[k]->getP3());
               si.setP7(items[k]->getP7());
               si.makeInvisible();
               cout << si;
            } else if (items[k]->isBarlineItem()) {
               si = *items[k];
               si.makeInvisible();
               cout << si;
            }
         }
      }

      if (pagecount > 1) {
         cout << endl;
         cout << "SM" << endl;
      }
   }
}



//////////////////////////////
//
// identifyFingerNumbers --
//

void  identifyFingerNumbers(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   for (int i=0; i<pagecount; i++) {
      identifyFingerNumbers(infiles[i][0], i, pagecount);
   }
}


void identifyFingerNumbers(ScorePage& infile, int pageindex, int pagecount) {
   int maxstaff = infile.getMaxStaff();
   for (int i=maxstaff; i>0; i--) {
      identifyFingerNumbers(infile, pageindex, i, pagecount);
   }
}


void  identifyFingerNumbers(ScorePage& infile, int pageindex, int staffnum,
      int pagecount) {
   vectorSIp items;
   infile.getSortedStaffItems(items, staffnum);
   double fpos;  // forward position to nearest note P3
   double bpos;  // backward position to nearest note p3
   double npos;  // hpos of nearest note
   double hpos;  // horizontal position of test fingering

   for (int i=0; i<items.size(); i++) {
      if (!((code10Q && items[i]->isNumberItem()) ||
          (code16Q && items[i]->isTextItem()))) {
         continue;      
      }
      if (items[i]->isTextItem()) {
         if (!onlyHasNumbers(items[i])) {
            continue;
         }
      }

      string function = items[i]->getParameter(np_function);
      if (function == "pagenum") {
         continue;
      } else if (function == "barnum") {
         continue;
      }

      hpos = items[i]->getHorizontalPosition();
      fpos = getNearestNoteP3Forward(items, i);
      bpos = getNearestNoteP3Backward(items, i);
      npos = -1000;
      if ((fpos > 0) && (fpos - hpos < hpos - bpos)) {
         npos = fpos;         
      } else if ((fpos > 0) && (hpos - bpos < fpos - hpos)) {
         npos = bpos;         
      }
      if (fabs(npos - hpos) < FingerThreshold) {
         double fnum = getFingerNum(items[i]);
         if ((fnum > 0) && (fnum <= MaxFinger)) {
            identifyFingerNumber(items[i]);
         }
      }
   }
}



void identifyFingerNumber(ScoreItem* sip) {
   if (labelQ) {
      sip->setParameterNoisy(np_function, "fingering");
      return;
   } else {
      if (sip->isNumberItem()) {
       
      } else {
         cout << sip->getTextWithoutInitialFontCode() << endl;
      }
   }
}



//////////////////////////////
//
// getNearestNoteP3Forward -- Return the closest note to the given 
//    element in the list to the right.  Do not check the given element.  
//    Returns -1000 if no note in a forward position.
//

double getNearestNoteP3Forward(vectorSIp& items, int index) {
   for (int i=index+1; i<items.size(); i++) {
      if (items[i]->isNoteItem()) {
         return items[i]->getHorizontalPosition();
      }
   }
   return -1000;
}



//////////////////////////////
//
// getNearestNoteP3Backward -- Return the closest note to the given 
//    element in the list to the left.  Do not check the given element.  
//    Returns -1000 if no note in a backward position.
//

double getNearestNoteP3Backward(vectorSIp& items, int index) {
   for (int i=index-1; i>=0; i--) {
      if (items[i]->isNoteItem()) {
         return items[i]->getHorizontalPosition();
      }
   }
   return -1000;
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
// getFingerNum --
//

double getFingerNum(ScoreItem* sip) {
   if (sip->isNumberItem()) {
      return sip->getNumber();
   } else if (sip->isTextItem()) {
      string text = sip->getTextWithoutInitialFontCode();
      int num;
      if (sscanf(text.data(), "%d", &num)) {
         return (double)num;
      } else {
         return 0;
      }
   }
   return 0;
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("text-only|text=b", 
               "Search only text items");
   opts.define("numbers-only|number-only|numbers|number=b", 
               "Search only number items");
   opts.define("above-cutoff=d:20.0", 
               "Max. vertical pos. for page nums. above staff");
   opts.define("A|no-analysis=b", "don't automatically analyze data");
   opts.define("r|remove=b",  "Remove fingerings from input data");
   opts.define("R|removed=b", "Output only Removed fingerings from input data");
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

   // maxAbovePos = opts.getDouble("above-cutoff");
   labelQ      =  opts.getBoolean("label");
   analysisQ    = !opts.getBoolean("no-analysis");
   removeQ     =  opts.getBoolean("remove");
   removedQ    =  opts.getBoolean("removed");
   if (removedQ || removeQ) {
      labelQ = 1;
   }
}



