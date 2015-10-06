//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct  6 11:23:50 PDT 2015
// Last Modified: Tue Oct  6 11:23:52 PDT 2015
// Filename:      hpos.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/hpos.cpp
// Documentation: http://scorelib.sapp.org/program/hpos
// Syntax:        C++ 11
//
// Description:   Marks the horizontal position of items on a system.
//
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void   processOptions       (Options& opts, int argc, char** argv);

int mincutoff = 0;
int maxcutoff = 200;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   labelHpos(infiles);
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// labelHpos -- Mark horizontal positions of everything on staff
//    other than at position 0 or 200.
//

void markBarlines(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   for (int i=0; i<pagecount; i++) {
      markBarlines(infiles[i][0], i, pagecount);
   }
}

void markBarlines(ScorePage& infile, int pageindex, int pagecount) {
   int syscount = infile.getSystemCount();
   for (int i=0; i<syscount; i++) {
      markBarlines(infile.getSystemItems(i), pageindex, i, pagecount, 
            syscount);
   }
}

void markBarlines(vectorSIp& items, int pageindex, int sysindex, 
      int pagecount, int syscount) {

   string barline = "";
   string btest;
   int tindex = -1;
   // int barlinenum = 0;

   int i;
   for (i=0; i<(int)items.size(); i++) {
      if (!((code10Q && items[i]->isNumberItem()) ||
          (code16Q && items[i]->isTextItem()))) {
         continue;      
      }
      btest = items[i]->getParameter(np_function);
      if (btest == "") {
         continue;
      }
      tindex = i;
      break;
   }

   if (btest == "") {
      return;
   }
   if (tindex < 0) {
      return;
   }

   // barlinenum = getItemNumber(items[tindex]);
   string barstring = getItemString(items[tindex]);
   markBarlinesForward(items, tindex, barstring);
   markBarlinesBackward(items, tindex, barstring);
}



//////////////////////////////
//
// markBarlinesForward -- mark all barlines with the given string from the
//    current position until the next durational item.
//

void markBarlinesForward(vectorSIp& items, int index, string& barnum) {
   for (int i=index; i<(int)items.size(); i++) {
      if (items[i]->hasDuration()) {
         break;
      }
      if (!items[i]->isBarlineItem()) {
         continue;
      }
      string current = items[i]->getParameter(np_function);
      if (current == "") {
         items[i]->setParameterNoisy(np_barnum, barnum);
      }
   }
}



//////////////////////////////
//
// markBarlinesBackward -- mark all barlines with the given string from the
//    current position until the next durational item.
//

void markBarlinesBackward(vectorSIp& items, int index, string& barnum) {
   for (int i=index; i>=0; i--) {
      if (items[i]->hasDuration()) {
         break;
      }
      if (!items[i]->isBarlineItem()) {
         continue;
      }
      string current = items[i]->getParameter(np_function);
      if (current == "") {
         items[i]->setParameterNoisy(np_barnum, barnum);
      }
   }
}



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


void identifyBarNumbers(vectorSIp& items, int pageindex, int sysindex, 
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

   for (i=0; i<(int)items.size(); i++) {
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

   for (i=0; i<(int)items.size(); i++) {
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
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.process(argc, argv);

}



