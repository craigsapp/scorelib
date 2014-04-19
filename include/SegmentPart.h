//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      SegmentPart.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/SegmentPart.h
// Syntax:        C++11
//
// Description:   Used to store the system staff sequence for a part in a
//                ScoreSegment;
//

#ifndef _SEGMENTPART_H_INCLUDED
#define _SEGMENTPART_H_INCLUDED

#include "SystemAddress.h"
#include "ScorePageSet.h"

using namespace std;

using vectorVPAp  = vector<vector<SystemAddress*>>;


class SegmentPart {
   public:
                         SegmentPart         (void);
                        ~SegmentPart         ();

      void               clear               (void);
      SystemAddress&     getAddress          (int index, int index2 = 0);
      ScorePageOverlay&  getScoreOverlay     (int index, int index2 = 0);
      ScorePage&         getScorePage        (int index, int index2 = 0);
      int                getScorePageIndex   (int index, int index2 = 0);
      int                getPageSystemIndex  (int index, int index2 = 0);
      int                getSystemStaffIndex (int index, int index2 = 0);
      int                systemCount         (void);
      void               setOwner            (ScorePageSet* owner);
      int                hasOwner            (void);
      ScorePageSet*      getOwner            (void);
      void               appendAddress       (SystemAddress& anAddress);
      void               addAddress          (SystemAddress& anAddress, 
                                              int index);

   protected:
      vectorVPAp    address_storage;
      ScorePageSet* pageset_owner;
};

#endif  /* _SEGMENTPART_H_INCLUDED */



