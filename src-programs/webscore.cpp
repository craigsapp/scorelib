//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May  2 20:11:19 PDT 2015
// Last Modified: Sat May  2 20:11:21 PDT 2015
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

using namespace std;

void   printSystemSet       (ScorePageSet& infiles);
void   prepareWebScore      (ScorePageSet& infiles);
void   processOptions       (Options& opts, int argc, char** argv);
void   addIndexNumbers      (ScorePageSet& infiles);
void   printSystemItems     (ScorePage&, int sysindex);
void   printReplaceItems    (ScorePage& page, int sysindex);
void   printAbbreviatedItems(ScorePage& page, int sysindex);

// user-interface variables:
Options options;
string Separator;
int    indexQ       = 0;
int    abbreviatedQ = 0;
int    replaceQ     = 0;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   options.process(argc, argv);
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
         it->setParameter("index", index++);
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
   for (i=0; i<infiles.getPageCount(); i++) {
      if (i>0) {
         cout << "\n";
      }
      page = infiles.getPage(i);
      syscount = page->getSystemCount();
      if (syscount <= 10) {
         prefix = "0";
      } else {
         prefix = "";
      }
      for (j=0; j<syscount; j++) {
         myfile = page->getFilenameBase();
         myfile += Separator;
         myfile += prefix;
         myfile += to_string(j+1);
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
   for (i=0; i<sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
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
      cout << "class=\"";
      cout << "noteon-";
      cout << sitems[i]->getParameter("auto", "pagesetOffsetDuration");
      cout << " noteoff-";
      cout << (sitems[i]->getParameterDouble("auto", "pagesetOffsetDuration")
            +  sitems[i]->getDuration());
      cout << endl;
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
   for (i=0; i<sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
        if (sitems[i]->hasParameter("index")) {
           // index = sitems[i]->getParameterDouble("index");
           // cout << "T ";
           // cout << sitems[i]->getP2() << " ";
           // cout << sitems[i]->getP3() << " ";
           // cout << sitems[i]->getP4() << " ";
           // cout << index;
           // cout << endl;
           // cout << "_99% << index << endl;

           sitems[i]->printPmxFixedParameters(cout);

           // cout << "T ";
           // cout << sitems[i]->getP2() << " ";
           // cout << sitems[i]->getP3() << " ";
           // cout << sitems[i]->getP4() << " ";
           // cout << index;
           // cout << endl;
           // cout << "_99%." << endl;
        } else {
           sitems[i]->printPmxFixedParameters(cout);
        }
        continue;
      }
     
      // print note with various classes applied.

      index = sitems[i]->getParameterDouble("index");
      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << sitems[i]->getP3() << " ";
      cout << sitems[i]->getP4();
      if (id.size() > 0) {
         cout << " " << index;
      }
      cout << endl;
      cout << "_99%" << index << endl;

      sitems[i]->printPmxFixedParameters(cout);

      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << sitems[i]->getP3() << " ";
      cout << sitems[i]->getP4();
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
   int index;
   string id;
   for (i=0; i<sitems.size(); i++) {
      if (!sitems[i]->isNoteItem()) {
        if (sitems[i]->hasParameter("index")) {
           index = sitems[i]->getParameterDouble("index");

           cout << "T ";
           cout << sitems[i]->getP2() << " ";
           cout << sitems[i]->getP3() << " ";
           cout << sitems[i]->getP4() << " ";
           cout << index;
           cout << endl;
           cout << "_99%svg%<g id=\"i" << index << "\">" << endl;

           sitems[i]->printPmxFixedParameters(cout);

           cout << "T ";
           cout << sitems[i]->getP2() << " ";
           cout << sitems[i]->getP3() << " ";
           cout << sitems[i]->getP4() << " ";
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
      cout << sitems[i]->getP3() << " ";
      cout << sitems[i]->getP4();
      if (id.size() > 0) {
         cout << " " << index;
      }
      cout << endl;
      cout << "_99%svg%<g";
      // print index as ID if given:
      if (id.size() > 0) {
         cout << " id=\"i" << index << "\"";
      }
      cout << " class=\"";
      // print note-on/note-off classes:
      cout << "noteon-";
      cout << sitems[i]->getParameter("auto", "pagesetOffsetDuration");
      cout << " noteoff-";
      cout << (sitems[i]->getParameterDouble("auto", "pagesetOffsetDuration")
            +  sitems[i]->getDuration());
      cout << "\">" << endl;

      sitems[i]->printPmxFixedParameters(cout);

      cout << "T ";
      cout << sitems[i]->getP2() << " ";
      cout << sitems[i]->getP3() << " ";
      cout << sitems[i]->getP4();
      if (id.size() > 0) {
         cout << " " << index;
      }
      cout << endl;
      cout << "_99%svg%<\\g>" << endl;
   }
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("sep|separator=s", 
         "Separator between file name and system enumertor");
   opts.define("a|abbreviated=b", "Embed only note ids into font 99 text");
   opts.define("r|replace=b", "print replacement expressions");
   opts.define("i|index=b", "include item index serial numbers");
   opts.process(argc, argv);

   Separator    = opts.getString("separator");
   indexQ       = opts.getBoolean("index");
   abbreviatedQ = options.getBoolean("abbreviated");
   replaceQ     = options.getBoolean("replace");
}



