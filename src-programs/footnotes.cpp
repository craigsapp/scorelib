//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun 30 12:54:09 PDT 2015
// Last Modified: Tue Jun 30 12:54:13 PDT 2015
// Filename:      footnotes.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/footnotes.cpp
// Documentation: http://scorelib.sapp.org/program/footnotes
// Syntax:        C++ 11
//
// Description:   Identifies footnotes on a page.
//
//

#include "scorelib.h"
#include "stdlib.h"
#include "string.h"
#include <sstream>
#include <ctype.h>

using namespace std;

// function declarations:
void   processOptions       (Options& opts, int argc, char** argv);
void   identifyFootnotes    (ScorePageSet& infile);
void   identifyFootnotes    (ScorePage& infile);
void   displayFootnotes     (ScorePage& infile, vectorSIp& footnotes, 
                             vectorSIp& footmarkers);
int    isFootnoteRelated    (ScoreItem* item);
int    isFootnoteMarker     (ScoreItem* item);
void   labelFootnotes       (ScorePage& infile, vectorSIp& footnotes, 
                             vectorSIp& footmarkers);
int    isPaired             (ScoreItem* footnote, ScoreItem* footmark);
void   linkFootnote         (ScoreItem* footnote, ScoreItem* footmark);
string getMarker            (string& text);

// user-interface variables:
Options options;
int maxBelowPos = -10.0;
int markQ  = 0;
int labelQ = 0;


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);
   ScorePageSet infiles(options);
   identifyFootnotes(infiles);
   if (labelQ) {
      cout << infiles;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// identifyFootnotes --
//

void identifyFootnotes(ScorePageSet& infiles) {
   for (int i=0; i<infiles.getPageCount(); i++) {
      identifyFootnotes(infiles[i][0]);
   }
}


void identifyFootnotes(ScorePage& infile) {
   vectorSIp footnotes;
   vectorSIp footmarkers;

   listSIp& data = infile.lowLevelDataAccess();
   for (auto it : data) {
      if (!it->isTextItem()) {
         continue;
      }
      if (!isFootnoteRelated(it)) {
         continue;
      }
      if (isFootnoteMarker(it)) {
         footmarkers.push_back(it);
      } else {
         footnotes.push_back(it);
      }
   }
   labelFootnotes(infile, footnotes, footmarkers);
   if (!labelQ) {
      displayFootnotes(infile, footnotes, footmarkers);
   }
}



//////////////////////////////
//
// labelFootnotes --
//

void labelFootnotes(ScorePage& infile, vectorSIp& footnotes, 
      vectorSIp& footmarkers) {

   int i, j;
   for (i=0; i<(int)footnotes.size(); i++) {
      for (j=0; j<(int)footmarkers.size(); j++) {
         if (isPaired(footnotes[i], footmarkers[j])) {
            linkFootnote(footnotes[i], footmarkers[j]);
         }
      }
   }
}



//////////////////////////////
//
// isPaired --
//

int isPaired(ScoreItem* footnote, ScoreItem* footmark) {
   string textn = footnote->getTextWithoutInitialFontCode();
   string textm = footmark->getTextWithoutInitialFontCode();
   string markn = getMarker(textn);
   string markm = getMarker(textm);
   if (markn == markm) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// getMarker -- 
//    Markers:
//       *      = asterisk
//       !d     = dagger
//

string getMarker(string& text) {
   string output;

   int i;
   if (text.size() > 0) {
      if (text[0] == '*') {
         for (i=0; i<(int)text.size(); i++) {
            if (text[i] == '*') {
               output += "*";
            } else {
               break;
            }
         }
      } else if ((text.size() > 1) && (text[0] == '!') && (text[1] == 'd')) {
         for (i=0; i<(int)text.size(); i+=2) {
            if ((text[i] == '!') && (text[i+1] == 'd')) {
               output += "!d";
            } else {
               break;
            }
         }
      } else if (text[text.size()-1] == '*') {
         for (i=text.size()-1; i>=0; i--) {
            if (text[i] == '*') {
               output += "*";
            } else {
               break;
            }
         }
      } else if ((text.size() > 1) && (text[text.size()-1] == 'd') &&
            (text[text.size()-2] == '!')) {
         for (i=text.size()-1; i>1; i-=2) {
            if ((text[i] == 'd') && (text[i-1] == 'd')) {
               output += "!d";
            } else {
               break;
            }
         }
      }
   }

   return output;
}



//////////////////////////////
//
// linkFootnote -- Need to figure out how to avoid hard return in
//    footnote text (not really a possibility, though).
//

void linkFootnote(ScoreItem* footnote, ScoreItem* footmark) {
   footnote->setParameterQuiet("", np_function, "footnote");
   footmark->setParameterQuiet("", np_footnote, footnote->getText());
}



//////////////////////////////
//
// isFootnoteMarker -- returns true of a footnote marker (not the actual
//    footnote).
//

int isFootnoteMarker(ScoreItem* item) {
   string text = item->getTextWithoutInitialFontCode();
   int typing = isFootnoteRelated(item);
   if (typing > 0) {
      for (int i=0; i<(int)text.size(); i++) {
         if (isalpha(text[i])) {
            if ((i > 0) && (text[i-1] != '!')) {
               return 0;
            }
         }
      }
      return 1;
   } else if (typing < 0) {
      return 1;
   }
   return 0;
}



//////////////////////////////
//
// isFootnoteRelated -- Returns true if starts with
//    *  = asterisk
//    !d = dagger
//  Others may be added here.
//

int isFootnoteRelated(ScoreItem* item) {
   string text = item->getTextWithoutInitialFontCode();
   if (strncmp(text.data(), "*", 1) == 0) {
      return 1;
   } else if (strncmp(text.data(), "!d", 2) == 0) {
      return 1;
   } else if (text.data()[text.size()-1] == '*') {
      return -1;
   } else if ((text.data()[text.size()-2] == '!') &&
             (text.data()[text.size()-1] == 'd')) {
      return -1;
   }
   return 0;
}



//////////////////////////////
//
// displayFootnotes --
//

void displayFootnotes(ScorePage& infile, vectorSIp& footnotes, 
      vectorSIp& footmarkers) {
   if ((footnotes.size() == 0) || (footmarkers.size() == 0)) {
      return;
   }

   cout << "FOOTNOTES:\t" << infile.getFilename() << "\n";
   for (int i=0; i<(int)footnotes.size(); i++) {
      cout << "\t" << footnotes[i]->getText() << endl;
   }
   cout << endl;
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("left-only=b", 
               "Search for barlines only at left side of system");
   opts.define("m|mark=b", "Mark barline with their number");
   opts.define("below-cutoff=d:-10.0", 
               "Max. vertical pos. for footnotes below bottom staff");
   opts.define("l|label=b", "Label footnote items");
   opts.process(argc, argv);

   maxBelowPos = opts.getDouble("below-cutoff");
   labelQ      = opts.getBoolean("label");
   markQ       = opts.getBoolean("mark");
   if (markQ) {
      labelQ = 1;
   }
}



