//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May  2 20:11:19 PDT 2015
// Last Modified: Tue Jun  2 17:08:10 PDT 2015
// Filename:      webscore.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/webscore.cpp
// Documentation: http://scorelib.sapp.org/program/webscore
// Syntax:        C++ 11
//
// Description:   Prepares a webscore (split pageset into individual systems
//                adding markup for prepareing online score with aligned
//                audio.
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>

using namespace std;

// function declarations:
void     printSystemSet        (ScorePageSet& infiles);
void     prepareWebScore       (ScorePageSet& infiles);
void     processOptions        (Options& opts, int argc, char** argv);
void     addIndexNumbers       (ScorePageSet& infiles);
void     printSystemItems      (ScorePage&, int sysindex);
void     printReplaceItems     (ScorePage& page, int sysindex);
void     printAbbreviatedItems (ScorePage& page, int sysindex);
ostream& printValueWithD       (ostream& out, double value);
ostream& printNoteClassTags    (ostream& out, vectorSIp& items, int i);
ostream& printNonNoteClassTags (ostream& out, vectorSIp& sitems, int i);
double   limitP3               (double p3, double limit = 170.0);

// user-interface variables:
Options options;
string Separator;
int    indexQ        = 0;
int    abbreviatedQ  = 0;
int    allabbrQ      = 0;
int    replaceQ      = 0;
int    articulationQ = 1;
int    systemOffset  = 0;
int    partQ         = 0;   // Boolean for class tag for system number
int    roundQ        = 1;   // boolean for rounding quarter notes to 3 digits
int    cleanFontQ    = 1;
int    fixfontQ      = 0;   // used with -F option

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   if (indexQ) {
      addIndexNumbers(infiles);
      cout << infiles;
      return 0;
   } else if (replaceQ || abbreviatedQ) {
      addIndexNumbers(infiles);
   }
   prepareWebScore(infiles);
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// addIndexNumbers -- Index the SCORE items in the order that they
//    occur in the input data.
//

void addIndexNumbers(ScorePageSet& infiles) {
   int index = 0;
   int i;
   for (i=0; i<infiles.getPageCount(); i++) {
      listSIp& list = infiles.getPage(i)->lowLevelDataAccess();
      for (auto it : list) {
         it->setParameterNoisy("index", index++);
      }
   }
}



//////////////////////////////
//
// prepareWebScore --
//

void prepareWebScore(ScorePageSet& infiles) {
   // Currently .analyzeStaffDurations() needs to be done before
   // calculating .analyzePageSetDurations().  This will be
   // automated in the future.
   infiles.analyzeStaffDurations();
   infiles.analyzePageSetDurations();
   printSystemSet(infiles);
}



//////////////////////////////
//
// printSystemSet -- Print the PageSet as a set of systems, each
//    in a separate file (page).  This data can be extracted into
//    separate files with the scorex program (with the -s option).
//

void printSystemSet(ScorePageSet& infiles) {
   int i, j;
   int syscount;
   ScorePage* page;
   string prefix;
   string myfile;
   int firstpage = 1;
   int soffset = 0;
   for (i=0; i<infiles.getPageCount(); i++) {
      if (i>0) {
         cout << "\n";
      }
      page = infiles.getPage(i);
      syscount = page->getSystemCount();

      if (firstpage) {
         soffset = systemOffset;
      } else {
         soffset = 0;
      }

      if (syscount+soffset <= 10) {
         prefix = "0";
      } else {
         prefix = "";
      }

      for (j=0; j<syscount; j++) {
         myfile = page->getFilenameBase();
         myfile += Separator;
         myfile += prefix;
         myfile += to_string(j+1+soffset);
         cout << "RS" << endl;
         cout << "SA " << myfile << endl;
         cout << endl;
         if (replaceQ) {
            printReplaceItems(*page, j);
         } else if (abbreviatedQ) {
            printAbbreviatedItems(*page, j);
         } else {
            printSystemItems(*page, j);
         }
         cout << endl;
         cout << "SM" << endl;
      }
      firstpage = 0;
   }
}



//////////////////////////////
//
// printReplaceItems --
//

void printReplaceItems(ScorePage& page, int sysindex) {
   vectorSIp& sitems = page.getSystemItems(sysindex);
   int i;
   string id;
   for (i=0; i<(int)sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
        printNonNoteClassTags(cout, sitems, i);
        continue;
      }

      // print note with various classes applied.
      if (sitems[i]->hasParameter("index")) {
         id = sitems[i]->getParameter("index");
      } else {
         cerr << "Cannot run this function without item indexes.\n";
         exit(1);
      }

      cout << id << "\t";
      printNoteClassTags(cout, sitems, i);
      cout << endl;
   }
}



//////////////////////////////
//
// printValueWitD -- Assuming always positive.
//

ostream& printValueWithD(ostream& out, double value) {
   if (roundQ) {
      value = (int(value * 1000.0 + 0.5))/1000.0;
   }
   int ivalue = (int)value;
   double fraction = value - ivalue;
   char buffer[32] = {0};
   if (fraction == 0.0) {
      cout << ivalue;
   } else {
      stringstream temp;
      temp << value;
      strcpy(buffer, temp.str().c_str());
      int i = 0;
      while (buffer[i] != '\0') {
         if (buffer[i] == '.') {
            buffer[i] = 'd';
         }
         cout << buffer[i];
         i++;
      }
   }
   return out;
}



//////////////////////////////
//
// limitP3 --
//

double limitP3(double p3, double limit) {
   if (p3 > limit) {
      return limit;
   } else {
      return p3;
   }
}


//////////////////////////////
//
// printAbbreviatedItems --
//

void printAbbreviatedItems(ScorePage& page, int sysindex) {
   vectorSIp& sitems = page.getSystemItems(sysindex);
   int i;
   int index;
   string id;
   for (i=0; i<(int)sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
        if (sitems[i]->getParameter(np_function) == "pagenum") {
           // don't print page numbers
           continue;
        }
        if (sitems[i]->getParameter(np_function) == "footnote") {
           // don't print footnotes (at bottom of original pages)
           continue;
        }

        if (sitems[i]->isTextItem()) {
           if (sitems[i]->getP8() != 0.0) {
              // convert non-standard fonts to normal ones
              sitems[i]->setP8N(0.0);
           }
        }

        if (sitems[i]->hasParameter("index")) {
           index = sitems[i]->getParameterDouble("index");
           if (allabbrQ) {
              cout << "T ";
              cout << sitems[i]->getP2() << " ";
              cout << limitP3(sitems[i]->getP3()) << " ";
              cout << sitems[i]->getVPos() << " ";
              cout << index;
              cout << endl;
              cout << "_99%" << index << endl;
           }

           sitems[i]->printPmxFixedParameters(cout);

           if (allabbrQ) {
              cout << "T ";
              cout << sitems[i]->getP2() << " ";
              cout << limitP3(sitems[i]->getP3()) << " ";
              cout << sitems[i]->getVPos() << " ";
              cout << index;
              cout << endl;
              cout << "_99%." << endl;
            }
        } else {
           sitems[i]->printPmxFixedParameters(cout);
        }
        continue;
      }

      // print note with various classes applied.

      index = sitems[i]->getParameterDouble("index");
      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << limitP3(sitems[i]->getP3()) << " ";
      cout << sitems[i]->getVPos();
      if (id.size() > 0) {
         cout << " " << index;
      }
      cout << endl;
      cout << "_99%" << index << endl;

      sitems[i]->printPmxFixedParameters(cout);

      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << limitP3(sitems[i]->getP3()) << " ";
      cout << sitems[i]->getVPos();
      cout << endl;
      cout << "_99%." << endl;
   }

}



//////////////////////////////
//
// printSystemItems --
//

void printSystemItems(ScorePage& page, int sysindex) {
   vectorSIp& sitems = page.getSystemItems(sysindex);
   int i;
   int index = 0;
   string id;
   for (i=0; i<(int)sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
        if (sitems[i]->isTextItem()) {
           if (sitems[i]->getP8() != 0.0) {
              // convert non-standard fonts to normal ones
              sitems[i]->setP8N(0.0);
           }
        }
        if (sitems[i]->hasParameter("index")) {
           index = sitems[i]->getParameterDouble("index");

           cout << "T ";
           cout << sitems[i]->getP2() << " ";
           cout << limitP3(sitems[i]->getP3()) << " ";
           cout << sitems[i]->getVPos() << " ";
           cout << index;
           cout << endl;
           cout << "_99%svg%<g id=\"i" << index << "\">" << endl;

           sitems[i]->printPmxFixedParameters(cout);

           cout << "T ";
           cout << sitems[i]->getP2() << " ";
           cout << limitP3(sitems[i]->getP3()) << " ";
           cout << sitems[i]->getVPos() << " ";
           cout << index;
           cout << endl;
           cout << "_99%svg%<\\g>" << endl;

        } else {
           sitems[i]->printPmxFixedParameters(cout);
        }
        continue;
      }

      // print note with various classes applied.
      if (sitems[i]->hasParameter("index")) {
         id = to_string(index);
      } else {
         id = "";
      }

      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << limitP3(sitems[i]->getP3()) << " ";
      cout << sitems[i]->getVPos();
      if (id.size() > 0) {
         cout << " " << index;
      }
      cout << endl;
      cout << "_99%svg%<g";

      // print index as ID if given:
      if (id.size() > 0) {
         cout << " id=\"i" << index << "\"";
      }

      cout  << " ";
      printNoteClassTags(cout, sitems, i);
      cout << ">" << endl;

      sitems[i]->printPmxFixedParameters(cout);

      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << limitP3(sitems[i]->getP3()) << " ";
      cout << sitems[i]->getVPos();
      if (id.size() > 0) {
         cout << " " << index;
      }

      cout << endl;
      cout << "_99%svg%<\\g>" << endl;
   }
}



//////////////////////////////
//
// printNonNoteClassTags --
//

ostream& printNonNoteClassTags(ostream& out, vectorSIp& sitems, int index) {
   stringstream tempout;
   string function = sitems[index]->getParameter(np_function);
   if (function == "pagenum") {
      // don't print page numbers
      return out;
   }
   if (function == "footnote") {
      // don't print footnotes
      return out;
   }

   if (function != "") {
      tempout << function;
   }
   if (sitems[index]->hasParameter(np_footnote)) {
      if (tempout.str().size() != 0) {
         tempout << " ";
      }
      tempout << "footnote";
   }

   if (tempout.str().size() == 0) {
      return out;
   }

   int id = -1;
   if (sitems[index]->hasParameter("index")) {
     id = sitems[index]->getParameterInt("index");
   }
   out << id << "\t";
   out << "class=\"";
   out << tempout.str();
   out << "\"";
   string footn = sitems[index]->getParameter(np_footnote);
   if (footn.size() > 0) {
      out << " text=\"";
      // escape quotes in this string:
      out << sitems[index]->getParameter(np_footnote);
      out << "\"";
   }
   out << "\n";
   return out;
}



//////////////////////////////
//
// printNoteClassTags --
//

ostream& printNoteClassTags(ostream& out, vectorSIp& sitems, int i) {
   out << "class=\"";

   // note on tag:
   out << "noteon-";
   double value = sitems[i]->getParameterDouble("auto", "pagesetOffsetDuration");
   printValueWithD(out, value);

   // note off tag:
   out << " noteoff-";
   value = (sitems[i]->getParameterDouble("auto", "pagesetOffsetDuration")
         +  sitems[i]->getDuration());

   // dealing with triplet rounding quantization:
   if (fabs((value - (int)value)  - 0.6666) <= 0.0001) {
      value = (int)value + 0.6667;
   } else if (fabs((value - (int)value)  - 0.666) <= 0.001) {
      value = (int)value + 0.667;
   }
   printValueWithD(out, value);

   // part number tag:
   if (partQ) {
      double part = sitems[i]->getPartNumber();
      out << " part-";
      printValueWithD(out, part);
   }

   string function = sitems[i]->getParameter(np_function);
   if (function != "") {
      out << " " << function;
   }

   if (articulationQ) {
      if (sitems[i]->hasTrill()) {
         cout << " trill";
      }
   }

   out << "\"";
   return out;
}




//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("sep|separator=s",
         "Separator between file name and system enumertor");
   opts.define("a|abbreviated=b", "Embed only note ids into font 99 text");
   opts.define("aa|all-abbreviated=b", "Embed ids for all items");
   opts.define("r|replace=b", "print replacement expressions");
   opts.define("i|index=b", "include item index serial numbers");
   opts.define("F|plain-fonts=b", "Remove specialzed fonts");
   opts.define("A|no-articulation=b", "do not label note articulations");
   opts.define("s|system-offset=i:0", "index of first system");
   opts.define("p|part=b", "indicate part number in class tags");
   opts.define("R|no-round=b", "do not round quarter-note timestamps");
   opts.process(argc, argv);

   Separator     =  opts.getString("separator");
   indexQ        =  opts.getBoolean("index");
   abbreviatedQ  =  options.getBoolean("abbreviated");
   allabbrQ      =  options.getBoolean("all-abbreviated");
   if (allabbrQ) {
      abbreviatedQ = 1;
   }
   replaceQ      =  options.getBoolean("replace");
   articulationQ = !options.getBoolean("no-articulation");
   systemOffset  =  options.getInteger("system-offset");
   partQ         =  options.getBoolean("part");
   roundQ        = !options.getBoolean("no-round");
   fixfontQ      = !options.getBoolean("plain-fonts");
}



