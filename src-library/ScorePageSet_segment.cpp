//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 17 22:51:17 PDT 2014
// Last Modified: Thu Apr 17 22:51:20 PDT 2014
// Filename:      ScorePageSet_segment.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_segment.cpp
// Syntax:        C++11
//
// Description:   Functions related to segmentation of ScorePageSets.
//

#include "ScorePageSet.h"
#include "ScoreUtility.h"
#include <set>

using namespace std;


//////////////////////////////
//
// ScorePageSet::analyzeSegmentsByIndent --
//

void ScorePageSet::analyzeSegmentsByIndent(SCORE_FLOAT threshold1,
      SCORE_FLOAT threshold2) {
   int p, s;
   ScorePageSet& pageset = *this;
   SystemAddress lastIndent(0,0,0,0);
   SystemAddress currentIndent(0,0,0,0);

   double indent;
   int segmentstart = 1;
   vector<int> address;

   for (p=0; p<pageset.getPageCount(); p++) {
      int scount = pageset[p][0].getSystemCount();
      // cout << "SYSTEM count on page " << p << " is " << scount << endl;
      for (s=0; s<scount; s++) {
         indent = pageset[p][0].getP8BySystem()[s][0][0]->getHPos();
         address = {p, 0, s, 0};
         currentIndent = address;
         if (segmentstart) {
            lastIndent = currentIndent;
            segmentstart = 0;
         }
         if ((indent >= threshold1) && (indent <= threshold2)) {
            if (currentIndent != lastIndent) {
               createSegment(lastIndent, currentIndent);   
               segmentstart = 1;
               // cout << "SEGMENT: " << lastIndent << " TO " 
               //      << currentIndent << endl;
            }
         }
      }
   }

   if (lastIndent != currentIndent) {
      createSegment(lastIndent, currentIndent);   
      // cout << "SEGMENT: " << lastIndent << " TO " 
      //      << currentIndent << endl;
   }
}



//////////////////////////////
//
// ScorePageSet::createSegment --
//

void ScorePageSet::createSegment(SystemAddress& startaddress, 
      SystemAddress& endaddress) {
   ScoreSegment* segment = new ScoreSegment(*this, startaddress, endaddress);
   segment_storage.push_back(segment);
}



//////////////////////////////
//
// ScorePageSet::clearSegments --
//

void ScorePageSet::clearSegments(void) {
   for (auto& it : segment_storage) {
      delete it;
      it = NULL;
   }
   segment_storage.resize(0);
}



//////////////////////////////
//
// ScorePageSet::getSegmentCount --
//

int ScorePageSet::getSegmentCount(void) {
   return segment_storage.size();
}



//////////////////////////////
//
// ScorePageSet::getSegment -- Return the requested segment structure.
//

ScoreSegment& ScorePageSet::getSegment(int index) {
   return *segment_storage[index];
}



//////////////////////////////
//
// ScorePageSet::getLCMRhythm -- Return the least common multiple rhythm
//    of all notes within the segment.  The value returned with be 
//    in terms of divisions of a quarter note.  In other words,
//    quarter notes are the largest duration this function will return,
//    so even if all notes in the segment are half-notes, this function
//    will say the smallest rhythm is a quarter note.
//

int  ScorePageSet::getLCMRhythm(int segmentindex) {
   ScoreSegment& seg = getSegment(segmentindex);
   int syscount = seg.getSystemCount();
   set<int> numbers;
   int pageindex;
   int overlayindex;
   int systemindex;
   
   for (int i=0; i<syscount; i++) {
      SystemAddress& current = seg.getSystem(i);
      pageindex    = current.getPage();
      overlayindex = current.getOverlay();
      systemindex  = current.getSystem();
      ScorePage* page = this->getPage(pageindex, overlayindex);
      numbers.insert(page->getSystemLCMRhythm(systemindex));
   }

   return ScoreUtility::lcm(numbers);
}



