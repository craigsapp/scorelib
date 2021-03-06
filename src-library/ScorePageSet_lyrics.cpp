//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 15:21:27 PDT 2014
// Last Modified: Fri May  2 15:15:46 PDT 2014
// Filename:      ScorePageSet_lyrics.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_lyrics.cpp
// Syntax:        C++11
//
// Description:   Functions related to lyrics identification
//      within ScoreSegments.
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
   for (unsigned int i=0; (int)i<scount; i++) {
      analyzeLyrics(i);
   }
}


//
// AnalyzeLyrics by single segment:
//

void ScorePageSet::analyzeLyrics(int segmentindex) {
   int partcount = getPartCount(segmentindex);
   for (unsigned int i=0; (int)i<partcount; i++) {
      analyzeLyrics(segmentindex, i);
   }
}

//
// AnalyzeLyrics by single part:  This function does all the work
//    in identifying the lyrics for a single part.
//

void ScorePageSet::analyzeLyrics(int segmentindex, int partindex) {
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   int i, j;

   ScoreSegment& seg = getSegment(segmentindex);
   int systemcount = systems.size();

   vectorI p2vals(systemcount);
   fill(p2vals.begin(), p2vals.end(), -1);

   AddressSystem address;

   vectorVI verseP4s(systemcount);
   vectorVSF vposes(systemcount);

   for (i=0; i<(int)systems.size(); i++) {
      p2vals[i] = seg.getPageStaffIndex(i, partindex);
      if (p2vals[i] <= 0) {
         continue;
      }
      address = *systems[i][0];
      vectorSIp& items = seg.getSystemItems(address);
      identifyLyricsOnStaff(items, p2vals[i], verseP4s[i]);
   }

   // Go back and check for lines with just a few words for the lyrics.
   // which did not trigger lyirc idenfication.
   int maxlyriccount = 0;
   for (i=0; i<(int)verseP4s.size(); i++) {
      if (maxlyriccount < (int)verseP4s[i].size()) {
         maxlyriccount = verseP4s[i].size();
      }
   }
   if (maxlyriccount == 0) {
      return;
   }

   vectorSF average(maxlyriccount);
   fill(average.begin(), average.end(), 0.0);
   int counter = 0;

   for (i=0; i<(int)systems.size(); i++) {
      if ((int)verseP4s[i].size() != maxlyriccount) {
         continue;
      }
      for (int j=0; j<maxlyriccount; j++) {
         average[j] += verseP4s[i][j];
      }
      counter++;
   }
   if (counter == 0) {
      return;
   }

   for (i=0; i<(int)average.size(); i++) {
      average[i] /= counter;
   }

   vectorI addedlyrics(systemcount);
   fill(addedlyrics.begin(), addedlyrics.end(), 0.0);
   vectorVVSIp text;
   text.resize(verseP4s.size());
   // for (i=0; i<(int)text.size(); i++) {
   //    text[i].resize(2000);
   // }

   // Whenever there is not a full set of verse lines, re-analyze
   // the music for verses, being more sensitive to text/lines found
   // in the expected position for that verse number.
   for (i=0; i<(int)verseP4s.size(); i++) {
      fillStaffText(text[i], segmentindex, partindex, i, verseP4s[i],
         average, p2vals[i]);

      // for (int x=0; x<(int)text[i].size(); x++) {
      // if (text[i][x].size() == 0) {
      // continue;
      // }
      // cout << x << ": ";
      // for (int y=0; y<(int)text[i][x].size(); y++) {
      // if (text[i][x][y]->isLineItem()) {
      // cout << " - ";
      // } else if (text[i][x][y]->isTextItem()) {
      // cout << text[i][x][y]->getTextWithoutInitialFontCode();
      // cout << " ";
      // }
      // }
      // }
      // cout << endl;

      if (verseP4s[i].size() != average.size()) {
         // There are fewer verses identified on the current system than as
         // a maximum on other lines.  Search for lyrics which were not
         // initially identified, looking at expected vertical positions
         // based on the average height for systems which are complete.
         identifyExtraVerses(text[i], segmentindex, partindex, i, verseP4s[i],
               average, p2vals[i]);
      }

      // The following code will search for lyrics underneath the lowest
      // expected/identified lyric line in case it has been missed.
      SCORE_FLOAT linedistance = 5.0;
      SCORE_FLOAT sum = 0.0;
      if (average.size() > 1) {
         for (j=0; j<(int)average.size()-1; j++) {
            sum += average[j] - average[j+1];
         }
         linedistance = sum / (average.size()-1.0);
      }
      if (verseP4s[i].size() >= average.size()) {
         // looking for an extra line which is greater than maxlyrics count:
         addedlyrics[i] = addVerseLine(text[i], verseP4s[i].size()+1, 2,
               average.back()-linedistance, p2vals[i]);
         if (addedlyrics[i]) {
            average.push_back(average.back()-linedistance);
            verseP4s[i].push_back(average.back());
         }
      } else {
         // looking for an extra line which is less than maxlyrics count:
         addedlyrics[i] = addVerseLine(text[i], verseP4s[i].size()+1, 2,
               average[verseP4s[i].size()], p2vals[i]);
         if (addedlyrics[i]) {
            verseP4s[i].push_back(average.back());
         }
      }
   }

   // Determine if the first syllable on a line is a continuation
   // of a word from the previous staff.
   stitchLyricsHyphensAcrossSystems(segmentindex, partindex, maxlyriccount+1);

   linkLyricsToNotes(segmentindex, partindex);
}


///////////////////////////////////////////////////////////////////////////
//
// Private functions.
//

//////////////////////////////
//
// ScorePageSet::linkLyricsToNotes --
//

void ScorePageSet::linkLyricsToNotes(int segmentindex, int partindex) {
   ScoreSegment& seg = getSegment(segmentindex);
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   AddressSystem address;
   int systemcount = systems.size();
   int i, j, k, m, n;
   int p2;
   int tolerance = 2;
   int p3;
   int p1;
   int starting;
   int ending;
   ScorePage* page;

   vectorVSIp notelist(2000);
   vectorVSIp lyricslist(2000);

   // i == system index
   // j == note-location index
   // k == lyric-location index
   for (i=0; i<systemcount; i++) {
      lyricslist.clear();
      lyricslist.resize(2000);
      notelist.clear();
      notelist.resize(2000);
      p2 = seg.getPageStaffIndex(i, partindex);
      if (p2 <= 0) {
         continue;
      }
      address = *systems[i][0];
      page = getPage(address);
      vectorSIp& items = seg.getSystemItems(address);
      for (auto& it : items) {
         if (p2 != (int)it->getStaffNumber()) {
            continue;
         }
         p3 = it->getHPos();
         if (it->hasDuration()) { // Allow also to link to rests.
            notelist[p3+1000].push_back(it);
            continue;
         }
         if (!it->isDefined(ns_auto, np_verseLine)) {
            continue;
         }
         p1 = it->getItemType();
         if (p1 != P1_Text) {
            continue;
         }
         lyricslist[p3+1000].push_back(it);
      }

      // Should probably be more robust in case the lyric syllable
      // is not attached to a note.
      for (j=0; j<(int)notelist.size(); j++) {
         if (notelist[j].size() == 0) {
            continue;
         }
         starting = j - tolerance;
         ending   = j + tolerance;
         if (starting < 0) {
            starting = 0;
         }
         if (ending >= (int)notelist.size()) {
            ending  = notelist.size()-1;
         }
         for (k=starting; k<=ending; k++) {
            if ((int)lyricslist[k].size() == 0) {
               continue;
            }
            for (m=0; m<(int)lyricslist[k].size(); m++) {
               for (n=0; n<(int)notelist[j].size(); n++) {
                  page->lyrics_database.link(notelist[j][n], lyricslist[k][m]);
               }
            }
         }
      }
   }
}



//////////////////////////////
//
// fillStaffText --
//

void ScorePageSet::fillStaffText(vectorVSIp& text, int segmentindex,
      int partindex, int systemindex, vectorI& verses, vectorSF& average,
      int staffindex) {
   ScoreSegment& seg = getSegment(segmentindex);
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   AddressSystem address;
   address = *systems[systemindex][0];
   vectorSIp& items = seg.getSystemItems(address);

   //vectorVSIp text(2000);
   text.clear();
   text.resize(2000);

   SCORE_FLOAT staffp3 = 0.0;
   int staffQ = 0;
   SCORE_FLOAT p4;

   // Store information about possible lyrics
   int i;
   int p1, p2;
   SCORE_FLOAT p3;
   for (i=0; i<(int)items.size(); i++) {
      p2 = items[i]->getStaffNumber();
      if (p2 != staffindex) {
         continue;
      }
      if (staffQ == 0) {
         staffQ = 1;
         ScoreItem* staff = items[i]->getStaffOwner();
         if (staff != NULL) {
            staffp3 = staff->getHPos();
         }
      }
      p1 = items[i]->getItemType();
      if ((p1 != P1_Text) && (p1 != P1_Line)) {
         continue;
      }
      p3 = items[i]->getHPos();
      if (p3 < staffp3) {
         // text comes before staff starts, so not likely to be lyrics.
         continue;
      }
      if ((p1 == P1_Line) && !items[i]->isHorizontal()) {
         continue;
      }
      p4 = items[i]->getVPos();
      if (p1 == P1_Line) {
         p4 = p4 - 1.0;
      }
      if (p4 < 0) {
         p4 = -((int)(-p4));
      } else {
         p4 = (int)p4;
      }
      text[p4+1000].push_back(items[i]);
   }
}



//////////////////////////////
//
// ScorePageSet::identfyExtraVerses -- Find extra verses in the data which
//    may have been skipped over in the initial analysis.
//

int ScorePageSet::identifyExtraVerses(vectorVSIp& text, int segmentindex,
      int partindex, int systemindex, vectorI& verses, vectorSF& average,
      int staffindex) {

   ScoreSegment& seg = getSegment(segmentindex);
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   AddressSystem address;
   int output = 0;
   address = *systems[systemindex][0];
   vectorSIp& items = seg.getSystemItems(address);

   // Store information about possible lyrics
   int i, j;

   // Now iterate through the average array which contains the
   // P4 positions of already detected lyrics, sorted from high to
   // low in the list.  If there is a mis-match between the detected
   // lyrics and the average positions, try to justify the existence
   // of a lyric line if there is any text or lines at the expected
   // P4 vertcial position.
   int currentv = 0;
   int trg;
   int isum;
   int newlyrics = 0;
   for (i=0; i<(int)average.size(); i++) {
      if ((currentv < (int)verses.size())
            && fabs(average[i] - verses[currentv]) < 2.5) {
         // The identified verse is in the expected location
         // (no gaps in the verse line above this currentv verse).
         currentv++;
         continue;
      }
      if ((currentv == 0) || (currentv < (int)verses.size())) {
         // There are no more verses to process, so check to
         // see if there is an additional line of lyrics
         // that can be detected.  If no additional line of lyrics,
         // then give up and declare the lyric count to be final (no
         // missing lyric lines are be permitted unless they are
         // at the bottom of the lyric list).
         if (average[i] < 0) {
            trg = -int(-average[i]+0.5);
         } else {
            trg = int(average[i]+0.5);
         }
         isum = text[1000+trg].size() + text[1000+trg+1].size()
              + text[1000+trg-1].size();
         if (isum == 0) {
            // no observed lyrics where expected, so give up on trying
            // to do anything else.
            break;
         }
         // Found some lyrics, so need to update the lyrics positions
         // and insert the new verse.
         newlyrics++;
         verses.resize(verses.size()+1);
         verses.back() = 0;
         for (j=verses.size()-1; j>i; j--) {
            verses[j] = verses[j-1];
            changeVerseLine(text, j-1+1, j+1, 2, average[j]);
         }
         output += addVerseLine(text, i+1, 1, average[i], staffindex);
      }
   }

   // Adjust hyphenation of (new) lyrics on the given staff.
   if (newlyrics) {
      adjustHyphenInfo(items, staffindex);
   }

   return output;
}



//////////////////////////////
//
// ScorePageSet::adjustHyphenInfo --
//

void ScorePageSet::adjustHyphenInfo(vectorSIp& items, int staffnum) {
   int i;
   vectorSIp lastlyric(1000);
   ScoreItem* lastitem = NULL;
   fill(lastlyric.begin(), lastlyric.end(), (ScoreItem*)NULL);
   int versenum;

   int p1;
   int p2;
   int lastp1;
   for (i=0; i<(int)items.size(); i++) {
      if (!items[i]->isDefined(ns_auto, np_verseLine)) {
         continue;
      }
      p2 = items[i]->getStaffNumber();
      if (p2 != staffnum) {
         continue;
      }
      versenum = items[i]->getParameterInt(ns_auto, np_verseLine);
      if ((versenum < 0) || (versenum > 999)) {
         continue;
      }
      lastitem = lastlyric[versenum];
      lastlyric[versenum] = items[i];
      if (lastitem == NULL) {
         continue;
      }
      p1 = items[i]->getItemType();
      lastp1 = lastitem->getItemType();
      if (p1 == lastp1) {
         continue;
      }
      if (p1 == P1_Text) {
         // previous item is a line.  If it is a hyphen, then
         // set text to have a hyphenBefore, it is a word extender
         // then don't do anything.
         if (lastitem->getParameterBool(ns_auto, np_lyricsHyphen)) {
            items[i]->setParameterNoisy(ns_auto, np_hyphenBefore, "true");
         }
      } else if (p1 == P1_Line) {
         // previous item is text.  If this is a hyphen, then set
         // text to have a hyphenAfter.  If this is a wordExtension,
         // then indicate on the previous text item.
         if (items[i]->getParameterBool(ns_auto, np_wordExtension)) {
            lastitem->setParameterNoisy(ns_auto, np_wordExtension, "true");
         } else if (items[i]->getParameterBool(ns_auto, np_lyricsHyphen)) {
            lastitem->setParameterNoisy(ns_auto, np_hyphenAfter, "true");
         }
      }
   }
}



//////////////////////////////
//
// ScorePageSet::addVerseLine -- Mark any text found at the given P4
//     level (+/- the tolerance) as being on the specified verse line.
//     Returns 1 if any new text on the line was found.
//

int ScorePageSet::addVerseLine(vectorVSIp& vertical, int newnum,
      int tolerance, SCORE_FLOAT p4target, int staffindex) {
   int i, j;
   int output = 0;
   ScoreItem* si;
   int p4 = int(p4target + 0.5);
   int p2;
   for (i=p4-tolerance; i<=p4+tolerance; i++) {
      for (j=0; j<(int)vertical[i+1000].size(); j++) {
         si = vertical[i+1000][j];
         p2 = si->getStaffNumber();
         if (p2 != staffindex) {
            continue;
         }

         if (si->isDefined(ns_auto, np_verseLine)) {
            // don't relabel a verseLine by accident
            continue;
         }
         output = 1;
         si->setParameterNoisy(ns_auto, np_verseLine, to_string(newnum));
         if (si->isDashedLine()) {
            si->setParameterNoisy(ns_auto, np_lyricsHyphen, "true");
         } else if (si->isPlainLine()) {
            si->setParameterNoisy(ns_auto, np_wordExtension, "true");
         }
      }
   }
   return output;
}



//////////////////////////////
//
// ScorePageSet::changeVerseLine --
//

void ScorePageSet::changeVerseLine(vectorVSIp& vertical, int oldnum, int newnum,
      int tolerance, SCORE_FLOAT p4target) {

   int i, j;
   ScoreItem* si;
   int p4 = int(p4target + 0.5);
   for (i=p4-tolerance; i<=p4+tolerance; i++) {
      for (j=0; j<(int)vertical[i+1000].size(); j++) {
         si = vertical[i+1000][j];
         if (!si->isDefined(ns_auto, np_verseLine)) {
            continue;
         }
         if ((oldnum >= 0) &&
             (si->getParameterInt(ns_auto, np_verseLine) != oldnum)) {
            continue;
         }
         si->setParameterNoisy(ns_auto, np_verseLine, to_string(newnum));
      }
   }
}



//////////////////////////////
//
// ScorePageSet::stitchLyricsHyphensAcrossSystems -- Link hyphens at the
//    ends of lines to the first syllable on the next system.
//

void ScorePageSet::stitchLyricsHyphensAcrossSystems(int segmentindex,
      int partindex, int lyriccount) {
   const vectorVASp& systems = getSystemAddresses(segmentindex, partindex);
   ScoreSegment& seg = getSegment(segmentindex);
   int systemcount = systems.size();
   int p2;

   vectorI lastsys(lyriccount+1);
   vectorI lastlyric(lyriccount + 1); // need to add 1 since 0 verse not used.
   fill(lastlyric.begin(), lastlyric.end(), 0);
   fill(lastsys.begin(), lastsys.end(), 0);
   int p1;
   AddressSystem address;
   int versenum;

   int i;
   for (i=0; i<systemcount; i++) {
      p2 = seg.getPageStaffIndex(i, partindex);
      if (p2 <= 0) {
         continue;
      }
      address = *systems[i][0];
      vectorSIp& items = seg.getSystemItems(address);
      for (auto& it : items) {
         if (!it->isDefined(ns_auto, np_verseLine)) {
            continue;
         }
         if (it->isDefined(ns_auto, np_wordExtension)) {
            continue;
         }
         p1 = it->getItemType();
         versenum = it->getParameterInt(ns_auto, np_verseLine);
         if ((lastsys[versenum] != i) && (p1 == P1_Text) &&
             (lastlyric[versenum] == P1_Line)) {
            it->setParameterQuiet(ns_auto, np_hyphenBefore, "true");
         }
         lastlyric[versenum] = p1;
         lastsys[versenum] = i;
      }
   }
}



//////////////////////////////
//
// ScorePageSet::identifyLyricsOnStaff --
//

void ScorePageSet::identifyLyricsOnStaff(vectorSIp& systemitems, int staffidx,
      vectorI& verseP4s) {
   int i;
   vectorSIp& items = systemitems;
   vectorI itemidx;
   vectorSF vposes;
   itemidx.reserve(items.size());
   vposes.reserve(items.size());
   double vpos;
   ScoreItem* si;

   vectorI histtextpos(201);  // histogram of text vertical positions
   vectorI histlinepos(201);  // histogram of line vertical positions
   fill(histtextpos.begin(), histtextpos.end(), 0);
   fill(histlinepos.begin(), histlinepos.end(), 0);

   int vvpos;

   // First collect text and lines (P1=16|4) into a horizontally sorted list,
   // also keeping track of the vertical positions of the items in a separate
   // list.  In the future, also consider text on the line below if there is
   // no staff on the line below (text might extend into region of default
   // position of staff below, so the staff below is dropped).
   for (i=0; i<(int)items.size(); i++) {
      si = items[i];
      if ((int)si->getStaffNumber() != staffidx) {
         // Not on the staff of interest, so skip.
         continue;
      }
      if (si->isHairpin()) {
         // Don't count hairpins as lyric text lines
         continue;
      }

      if (si->isWavyLine()) {
         // Don't count wavy lines as lyric text lines
         continue;
      }

      if (si->isTextItem()) {
         itemidx.push_back(i);

         // Vertical positions are quantized to nearest interger for analysis.
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
         histtextpos[(int)(vvpos+100)]++;
      } else if (si->isLineItem()) {
         itemidx.push_back(i);

         // Vertical positions are quantized to nearest interger for analysis.
         vpos = si->getVPos();
         vpos = vpos - 1.1;  // lines are usually slightly higher than text
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

   lyricverse.reserve(100);
   lyricvpos.reserve(100);

   vectorI score(histtextpos.size());
   fill(score.begin(), score.end(), 0);

   int verse = 0;
   int tval, lval;
   int cutoff = 5;
   int value;

   for (i=1; i<(int)histtextpos.size()-2; i++) {
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
      if (i < (int)histlinepos.size()-1) {
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

      lyricverse.push_back(verse);
      verse++;
      value = i-100;
      lyricvpos.push_back(value);
   }

   for (i=0; i<(int)lyricverse.size(); i++) {
      verseP4s.push_back(lyricvpos[i]);
      processVerse(lyricverse[i], lyricvpos[i], staffidx, itemidx,
            vposes, items);
   }

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
// hyphenBefore  : true if part of a word with a hyphen preceeding it.
// hyphenAfter   : true if part of a word with a hyphen following it.
// wordExtension : true if end of word with multi-note extension line after it.
// verse         : verse number starting at 1 for the highest verse on the line
//

void ScorePageSet::processVerse(int verse, int vpos,
      int staffidx, vectorI& objlist, vectorSF& vposes, vectorSIp& data) {

   vectorI verseitems;
   verseitems.reserve(objlist.size());

   int i;
   for (i=0; i<(int)objlist.size(); i++) {
      if (fabs(vposes[i] - vpos) < 2.0) {
         verseitems.push_back(objlist[i]);
      }
   }

   string textdata;
   ScoreItem* srecord;
   ScoreItem* slast;
   ScoreItem* snext;

   for (i=0; i<(int)verseitems.size(); i++) {
      srecord = data[verseitems[i]];
      if (srecord->isDefined(ns_lyrics, np_verseLine)) {
         srecord->copyParameterOverwrite(ns_auto, ns_lyrics, np_verseLine);
      } else {
         srecord->setParameterNoisy(ns_auto, np_verseLine, verse+1);
                                           // offset by 1 for data so
                                           // that 0 can mean not a lyric
      }
      if (i>0) {
         slast = data[verseitems[i-1]];
      } else {
         slast = NULL;
      }
      if (i<(int)verseitems.size()-1) {
         snext = data[verseitems[i+1]];
      } else {
         snext = NULL;
      }

      if (srecord->isTextItem()) {
         if (snext != NULL) {
            if (snext->isDashedLine()) {
               if (srecord->isDefined(ns_lyrics, np_hyphenAfter)) {
                  srecord->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_hyphenAfter);
               } else {
                  srecord->setParameterNoisy(ns_auto, np_hyphenAfter, "true");
               }
               if (snext->isDefined(ns_lyrics, np_lyricsHyphen)) {
                  snext->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_lyricsHyphen);
               } else {
                  snext->setParameterNoisy(ns_auto, np_lyricsHyphen, "true");
               }
            } else if (snext->isPlainLine()) {
               if (srecord->isDefined(ns_lyrics, np_wordExtension)) {
                  srecord->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_wordExtension);
               } else {
                  srecord->setParameterNoisy(ns_auto, np_wordExtension, "true");
               }

               if (snext->isDefined(ns_lyrics, np_wordExtension)) {
                  snext->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_wordExtension);
               } else {
                  snext->setParameterNoisy(ns_auto, np_wordExtension, "true");
               }

            }

         }
         if (slast != NULL) {
            if (slast->isDashedLine()) {
               if (srecord->isDefined(ns_lyrics, np_hyphenBefore)) {
                  srecord->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_hyphenBefore);
               } else {
                  srecord->setParameterNoisy(ns_auto, np_hyphenBefore, "true");
               }

               if (slast->isDefined(ns_lyrics, np_lyricsHyphen)) {
                  slast->copyParameterOverwrite(ns_auto, ns_lyrics,
                        np_lyricsHyphen);
               } else {
                  slast->setParameterNoisy(ns_auto, np_lyricsHyphen, "true");
               }

            }
         }
      }
   }

}


