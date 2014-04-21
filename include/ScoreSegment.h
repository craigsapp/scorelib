//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 14 21:40:55 PDT 2014
// Last Modified: Mon Apr 14 21:40:58 PDT 2014
// Filename:      ScoreSegment.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreSegment.h
// Syntax:        C++11
//
// Description:   Used to keep track of parts in the given segmentation
//                of pages in a ScorePageSet.
//

#ifndef _SCORESEGMENT_H_INCLUDED
#define _SCORESEGMENT_H_INCLUDED

#include "SystemAddress.h"
#include <vector>
//#include "ScorePageSet.h"

class ScorePageSet;
class SegmentPart;

using namespace std;


class ScoreSegment {
   public:
                 ScoreSegment        (void);
                 ScoreSegment        (ScorePageSet& pageset, 
                                      SystemAddress& starting, 
                                      SystemAddress& ending, int debug = 0);
                ~ScoreSegment        ();

      void       clear               (void);
      int        getPartCount        (void) const;
      ostream&   printInfo           (ostream& out) const;
      void       defineSegment       (ScorePageSet& pageset, 
                                      SystemAddress& starting, 
                                      SystemAddress& ending, int debug = 0);
      void       analyzePartStaves   (vector<int>& partlist, 
                                      ScorePageSet& scoreset, 
                                      SystemAddress& startsys, 
                                      SystemAddress& endsys);
      const SystemAddress& getBeginSystem(void) const;
      const SystemAddress& getStartSystem(void) const;
      const SystemAddress& getEndSystem(void) const;
   
   protected:
      SystemAddress        start_system;
      SystemAddress        end_system;
      vector<SegmentPart*> part_storage;

   private:
      void        prepareParts       (ScorePageSet& page);
      void        getPartList        (vector<int>& partlist, 
                                      ScorePageSet& pageset, 
                                      SystemAddress& sp, 
                                      SystemAddress& ep);

};


ostream& operator<<(ostream& out, const ScoreSegment& segment);


#endif  /* _SCORESEGMENT_H_INCLUDED */



