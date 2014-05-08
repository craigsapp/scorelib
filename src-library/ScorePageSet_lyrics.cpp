//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 15:21:27 PDT 2014
// Last Modified: Mon Apr 28 15:21:30 PDT 2014
// Filename:      ScorePageSet_lyrics.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_lyrics.cpp
// Syntax:        C++11
//
// Description:   Functions related to lyrics identification with ScoreSegments.
//

#include "ScorePageSet.h"
#include "ScoreUtility.h"
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePageSet::analyzeLyrics -- Analyze lyrics for all parts in all segments.
//

void ScorePageSet::analyzeLyrics(void) {
   int scount = getSegmentCount();
   for (unsigned int i=0; i<scount; i++) {
      analyzeLyrics(i);
   }
}


//
// AnalyzeLyrics by single segment:
//

void ScorePageSet::analyzeLyrics(int segmentindex) {
   int partcount = getPartCount(segmentindex);
   for (unsigned int i=0; i<partcount; i++) {
      analyzeLyrics(segmentindex, i);
   }
}

//
// AnalyzeLyrics by single part:
//

void ScorePageSet::analyzeLyrics(int segmentindex, int partindex) {
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   int i;

   ScoreSegment& seg = getSegment(segmentindex);

   vectorI p2vals(systems.size());
   fill(p2vals.begin(), p2vals.end(), -1);

   AddressSystem address;

   for (i=0; i<systems.size(); i++) {
      p2vals[i] = seg.getSystemStaffIndex(i, partindex);
      if (p2vals[i] <= 0) {
         continue;
      }
      address = *systems[i][0];
      vectorSIp& items = seg.getSystemItems(address);
      identifyLyricsOnStaff(items, p2vals[i]);
   }
}


///////////////////////////////////////////////////////////////////////////
//
// Private functions.
//


//////////////////////////////
//
// ScorePageSet::identifyLyricsOnStaff --
//

void ScorePageSet::identifyLyricsOnStaff(vectorSIp& systemitems, int staffidx) {
   int i;
   vectorSIp& items = systemitems;
   vectorI itemidx;
   vectorSF vposes;
   itemidx.reserve(items.size());
   vposes.reserve(items.size());
   double vpos;
   ScoreItem* si;

   vectorI histtextpos(201);
   vectorI histlinepos(201);
   fill(histtextpos.begin(), histtextpos.end(), 0);
   fill(histlinepos.begin(), histlinepos.end(), 0);

   int vvpos;

   for (i=0; i<items.size(); i++) {
      si = items[i];
      if (si->getStaffNumber() != staffidx) {
         continue;
      }
      if (si->isHairpin()) {
         // don't count hairpins as lyric text lines
         continue;
      }
         
      if (si->isWavyLine()) {
         // don't count wavy lines as lyric text lines
         continue;
      }

      if (si->isTextItem()) {
         itemidx.push_back(i);
         vpos = si->getVPos();
         if (vpos < 0) {
            vpos -= 0.002;
         } else {
            vpos += 0.002;
         }
         vposes.push_back(vpos);
         histtextpos[(int)(vpos+100)]++;
      } else if (si->isLineItem()) {
         itemidx.push_back(i);
         vpos = si->getVPos();
         if (vpos < 0) {
            vpos -= 0.002;
         } else {
            vpos += 0.002;
         }
         vposes.push_back(vpos);
         
         if (vpos < 0) {
            vvpos = -(int(-vpos));
         } else {
            vvpos = int(vpos);
         }
         histlinepos[(int)(vvpos+100)]++;
      } 
   }

   vectorI lyricverse;
   vectorI lyricvpos;
//   vectorI pageStaffLyrics(100);
//   fill(pageStaffLyrics.begin(), pageStaffLyrics.end(), 0);

   lyricverse.reserve(100);
   lyricvpos.reserve(100);

   vectorI score(histtextpos.size());
   fill(score.begin(), score.end(), 0);

   int verse = 0;
   int tval, lval;
   int cutoff = 5;
   int value;

   for (i=1; i<histtextpos.size()-2; i++) {
      if (histtextpos[i] == 0) {
         continue;
      } 
      tval = histtextpos[i];
      lval = histlinepos[i];
   
      // Lyrics may not be exactly aligned vertically, so allow for +/- 1 
      // variation.  This is more true of hyphens which will have a 
      // slightly higher vpos than the text.
      //
      // The score should be also based on text of previous/subsequent systems.
      // If the vpos of a single word is the same as the lyric line of another 
      // system for the same staff, then that single word would be part 
      // of the lyrics.
      if (i > 0) {
         lval += histlinepos[i-1];
      }
      if (i < histlinepos.size()-1) {
         lval += histlinepos[i+1];
      }
      score[i] = (tval + lval*2);
      if (lval > tval+2) {
         score[i] = score[i] - lval*2;
      }
   }

   for (i=score.size()-2; i>=1; i--) {
      if (score[i] < cutoff) {
         continue;
      }
      if (score[i] < score[i-1]) {
         continue;
      }
      if (score[i] < score[i+1]) {
         continue;
      }

//      cout << "Verse" << verse << "\t" 
//           << "SYS" << sysidx << "STAFF" << staffidx << "\t"
//           << histtextpos[i]
//           << "," << histlinepos[i] << "," << histtextpos[i]+histlinepos[i]
//           << "\t at vpos " << i-100 << "\tscore = " << score[i] << endl;

      lyricverse.push_back(verse);
      verse++;
      value = i-100;
      lyricvpos.push_back(value);
   }
   
   for (i=0; i<lyricverse.size(); i++) {
      processVerse(lyricverse[i], lyricvpos[i], staffidx, itemidx, 
            vposes, items);
   }

//   if (lyricverse.size() > staffidx) {
//      pageStaffLyrics[staffidx] = lyricverse.size();
//   }
//
//   if (lyricverse.size() > sysStaffLyrics[staffidx]) {
//      sysStaffLyrics[staffidx] = lyricverse.size();
//   }

}



//////////////////////////////
//
// ScorePageSet::processVerse -- 
//
// Example of a syllable continuation line:
// P1 P2 P3           P4         P5        P6
// 4  15 143.3006900  16.000000  16.000000 164.756302
//
// Example of a dash between syllables:
// P1 P2 P3       P4      P5     P6      P7   P8    P9
// 4  15 119.624  16.960  16.960 136.460 1.00 1.543 13.445
//
// Line Parameters:
//
// P1 = 4
// P2 = staff
// P3 = hpos of left side
// P4 = vpos of left side
// P5 = vpos of right side (or 999 for hairpins)
// P6 = hpos of right side
// P7 = type of line (0=normal, 1=dashed, -1=wavy, -1,P5:999 = dimuendo, 
//                    0,P5:999 = crescendo)
// P8 = length of dashes on a dashed line
// P9 = angle of line, or spaces between lines when dashed
//
// Identify whether syllables are connected to adjacent syllables with 
// hyphens.  Also, identify line extensions.
//
// Set these numeric named parameters on the lyrics and the lines between 
// them (may change numbering system later to link the items together).
//
// hyphenbefore  : 1 if part of a word with a hyphen preceeding it, 0 otherwise.
// hyphenafter : 1 if part of a word with a hyphen following it, 0 otherwise.
// extension   : 1 if end of word with a multi-note extension line after it.
// verse       : verse number starting at 1 for the highest verse on the line
//

void ScorePageSet::processVerse(int verse, int vpos, 
      int staffidx, vectorI& objlist, vectorSF& vposes, vectorSIp& data) {

   vectorI verseitems;
   verseitems.reserve(objlist.size());

   int i;
   for (i=0; i<objlist.size(); i++) {
      if (fabs(vposes[i] - vpos) < 2.0) {
         verseitems.push_back(objlist[i]);
      }
   }

   string textdata;
   ScoreItem* srecord;
   ScoreItem* slast;
   ScoreItem* snext;

   for (i=0; i<verseitems.size(); i++) {
      srecord = data[verseitems[i]];
      srecord->setParameter("verse", verse+1); // offset by 1 for data so 
                                           // that 0 can mean not a lyric
      if (i>0) {
         slast = data[verseitems[i-1]]; 
      } else {
         slast = NULL;
      }
      if (i<verseitems.size()-1) {
         snext = data[verseitems[i+1]]; 
      } else {
         snext = NULL;
      }

      if (srecord->isTextItem()) {
         if (snext != NULL) {
            if (snext->isDashedLine()) {
               srecord->setParameter("hyphenafter", 1);
               snext->setParameter("hyphen", 1);
            } else if (snext->isPlainLine()) {
               srecord->setParameter("extension", 1);
            }
         }
         if (slast != NULL) {
            if (slast->isDashedLine()) {
               srecord->setParameter("hyphenbefore", 1);
               slast->setParameter("hyphen", 1);
            } 
         }
    
      }
   }

/*
   // testing the analysis by printing the lyric line for the staff, 
   // without hyphens or extensions:
   cout << "!! ";
   for (i=0; i<verseitems.size(); i++) {
      srecord = &(getItem(verseitems[i]));

      if ((i == verseitems.size()-1) && srecord->isLineItem()) {
         // print a hyphen for a word break at the end of a line
         if (srecord->isDashedLine()) {
            cout << "-";
         }
      }

      if (!srecord->isTextItem()) {
         continue;
      }
      cout << srecord->getTextDataWithoutFonts(textdata);
      if (!srecord->getNumKeyValue("hyphenafter")) {
         // print a space if the syllable does not have a hyphen after it.
         cout << " ";
      }
   }
   cout << endl;
*/


}


