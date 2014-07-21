//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 28 15:21:27 PDT 2014
// Last Modified: Fri May  2 15:15:46 PDT 2014
// Filename:      ScoreSegment_ties.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreSegment_ties.cpp
// Syntax:        C++11
//
// Description:   Functions related to lyrics identification 
//      within ScoreSegments.
//

#include "ScoreSegment.h"
#include "ScoreUtility.h"
#include <set>

using namespace std;


//////////////////////////////
//
// ScoreSegment::analyzeTies -- Analyze ties for all parts in all segments.
//

void ScoreSegment::analyzeSystemBreakTies(void) {
   int i;
   for (i=0; i<getPartCount(); i++) {
      analyzeSystemBreakTies(i);
   }
}


//
// Single part analysis.  Not considering subparts at the momemnt.
//

void ScoreSegment::analyzeSystemBreakTies(int pindex) {
   vectorI partp2;
   vectorVSIp preslurs;
   vectorVSIp postslurs;
   vectorVSIp prenotes;
   vectorVSIp postnotes;

   preslurs.resize(getSystemCount());
   postslurs.resize(getSystemCount());
   prenotes.resize(getSystemCount());
   postnotes.resize(getSystemCount());

   partp2.resize(getSystemCount());

   int i;
   for (i=0; i<getSystemCount(); i++) {
      partp2[i] = getPageStaffIndex(i, pindex);
      // the order of pre/post slurs/notes looks weird in this
      // function call, but that is the order they need to
      // get the data in the right slots...
      SU::analyzeStaffTies(partp2[i], getSystemItems(i), preslurs[i], 
            postnotes[i], postslurs[i], prenotes[i]);
   }

   int a, b;

   // Now analyze the pre-, post-slurs to see if they are ties.
   for (i=1; i<getSystemCount(); i++) {
      if (postnotes[i-1].size() == 0) {
         continue;
      }
      for (a=0; a<preslurs[i].size(); a++) {
         for (b=0; b<postslurs[i-1].size(); b++) {
            SU::linkNotesByTie(postslurs[i-1][b], 
                  postnotes[i-1][0]->getDuration(), postnotes[i-1], 
                  preslurs[i][a], prenotes[i]);
         }
      }
   }
}



