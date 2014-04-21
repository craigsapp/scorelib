//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 03:18:48 PST 2014
// Last Modified: Thu Apr 10 15:32:47 PDT 2014
// Filename:      ScorePageSet.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageSet.h
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#ifndef _SCOREPAGESET_H_INCLUDED
#define _SCOREPAGESET_H_INCLUDED

#include "ScorePageOverlay.h"
#include "ScoreSegment.h"
#include "Options.h"

using namespace std;

typedef vector<ScorePageOverlay*> vectorSPOp;
typedef list<ScorePageOverlay*>   listSPOp;
typedef vector<ScoreSegment*>     vectorSSp;

class ScorePageSet { 
   public:
                  ScorePageSet                  (void);
                  ScorePageSet                  (Options& opts);
                 ~ScorePageSet                  ();
      void        clear                         (void);
      int         getPageCount                  (void);
      int         getOverlayCount               (int pindex);
      ScorePage*  getPage                       (int pindex);
      ScorePage*  getPage                       (int pindex, int oindex);

      ScorePageOverlay&  operator[]             (int oindex);
      ScorePage*  createPage                    (void);
      int         appendPage                    (ScorePage* page);
      int         appendPage                    (ScorePage* page, int pindex);
      void        appendReadFromOptionArguments (Options& opts);
      void        read                          (Options& opts);
      void        appendRead                    (const string& filename);
      void        appendRead                    (istream& instream, 
                                                   const string& filename);
      void        appendReadBinary              (istream& instream, 
                                                 const string& filename);
      void        appendReadPmx                 (istream& instream, 
                                                 const string& filename,
                                                 const string& pagetype="page");
      void        appendReadStandardInput       (void);
      void        appendOverlay                 (ScorePage* page);
      void        appendOverlay                 (ScorePage* page, int pindex);
      void        analyzeSegmentsByIndent       (SCORE_FLOAT threshold1 = 7.0,
                                                 SCORE_FLOAT threshold2 = 40.0);

      // Segmentation functions (defined in ScorePageSet_segment.cpp):
      int           getSegmentCount             (void);
      ScoreSegment& getSegment                  (int index);
      void          clearSegments               (void);
      void          createSegment               (SystemAddress& startaddress, 
                                                 SystemAddress& endaddress);

   protected:
      // page_storage contains all of the data for SCORE pages.
      // This is the primary storage for page data, and it must
      // be deleted when an object is deconstructed.
      listSPOp page_storage;

      // page_sequence is the order in which the pages in page_storage
      // should be arranged.  The first dimension is the page sequence,
      // and the second dimension is the overlay sequence.
      vectorSPOp page_sequence;

      // score_segments is a list of score segmentations across multiple
      // pages.  Each segment is assumed to have a constant number of 
      // parts, and typically represents one movement of a work.  The
      // segments are destroyed when the object is deconstructed.
      vectorSSp segment_storage;

};


ostream& operator<<(ostream& out, ScorePageSet& set);


#endif /* _SCOREPAGESET_H_INCLUDED */

