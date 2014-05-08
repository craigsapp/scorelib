//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 16:05:12 PDT 2014
// Last Modified: Thu May  1 16:05:15 PDT 2014
// Filename:      extractlyrics.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/extractlyrics.cpp
// Syntax:        C++11
//
// Description:   This program extracts lyrics from music.
//

#include "scorelib.h"

using namespace std;

void  processSegment (ScorePageSet& infiles, int segment);
void  getMaxVerse    (ScorePageSet& infiles, int segment, vector<int>& partmax);
void  printVerses    (ScorePageSet& infiles, int segment, int partindex, 
                      int versecount);
void  printVerse     (ScorePageSet& infiles, int segment, int partindex, 
                      int versenumber);

int rawQ = 0;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("r|raw=b", "Display raw syllables without merging into words");
   opts.process(argc, argv);
   ScorePageSet infiles(opts);
   infiles.analyzeSegmentsByIndent();
   infiles.analyzeLyrics();

   rawQ = opts.getBoolean("raw");

   for (unsigned int i=0; i<infiles.getSegmentCount(); i++) {
      processSegment(infiles, i);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// processSegment --
//

void processSegment(ScorePageSet& infiles, int segment) {
   ScoreSegment& seg = infiles.getSegment(segment);
   int syscount = seg.getSystemCount();
   unsigned int i;
   int maxVerse;

   int partcount = seg.getPartCount();

   vector<int> maxverse(partcount);
   fill(maxverse.begin(), maxverse.end(), 0);
   
   for (i=0; i<partcount; i++) {
      getMaxVerse(infiles, segment, maxverse);
   }

   // for (i=0; i<partcount; i++) {
   //    cout << "PART " << i << " :\tmax verse " << maxverse[i] << endl;
   // }

   for (i=0; i<partcount; i++) {
      if (maxverse[i] == 0) {
         continue;
      }
      printVerses(infiles, segment, i, maxverse[i]);
   }
}



//////////////////////////////
//
// printVerses --
//

void printVerses(ScorePageSet& infiles, int segment, int partindex, 
      int versecount) {

   int i;
   for (i=1; i<= versecount; i++) {
      cout << "PART " << partindex << ", verse " << i << "\n\n";
      printVerse(infiles, segment, partindex, i);
   }


}



//////////////////////////////
//
// printVerses --
//

void printVerse(ScorePageSet& infiles, int segment, int partindex, 
      int versenumber) {

   ScorePage* page;
   ScoreSegment& seg = infiles.getSegment(segment);
   int syscount = seg.getSystemCount();
   
   int i;
   int p2target;
   int p2;
   int testverse;
   for (i=0; i<syscount; i++) {
      const AddressSystem& address = seg.getSystemAddress(i);
      page = infiles.getPage(address);
      p2target = page->getPageStaffIndex(address);
      vectorSIp& sysitems = seg.getSystemItems(i);
      for (auto& it : sysitems) {
         if (!it->isTextItem()) {
            continue;
         }
         if (!it->isDefined(ns_auto, np_verseLine)) {
            continue;
         }
         testverse = it->getParameterInt(ns_auto, np_verseLine);
         if (testverse != versenumber) {
            continue;
         }
         if (rawQ) {
            cout << " ";
            if (it->getParameterBool(ns_auto, np_hyphenBefore)) {
               cout << "-";
            }
         } else {
            if (!it->getParameterBool(ns_auto, np_hyphenBefore)) {
               cout << " ";
            }
         }
         cout << it->getTextWithoutInitialFontCode();
         if (rawQ) {
            if (it->getParameterBool(ns_auto, np_hyphenAfter)) {
               cout << "-";
            }
            if (it->getParameterBool(ns_auto, np_wordExtension)) {
               cout << "_";
            }
         }
      }
   }
   cout << "\n\n";

}



//////////////////////////////
//
// getMaxVerse --
//

void getMaxVerse(ScorePageSet& infiles, int segment, vector<int>& partmax) {
   ScoreSegment& seg = infiles.getSegment(segment);
   int syscount = seg.getSystemCount();
   int verse;
   int part;

   for (unsigned int i=0; i<syscount; i++) {
      vectorSIp& sysitems = seg.getSystemItems(i);
      for (auto& it : sysitems) {
         if (it->isDefined(ns_auto, np_verseLine)) {
            part = it->getPartIndex();
            verse = it->getParameterInt(ns_auto, np_verseLine);
            if (verse > partmax[part]) {
               partmax[part] = verse;
            }
         }
      }
   }
}



