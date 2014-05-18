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

#include "AddressSystem.h"
#include "ScoreItem.h"
#include <vector>

class ScorePageSet;
class SegmentPart;


using namespace std;


class ScoreSegment {
   public:
                 ScoreSegment        (void);
                 ScoreSegment        (ScorePageSet& pageset, 
                                      AddressSystem& starting, 
                                      AddressSystem& ending, int debug = 0);
                ~ScoreSegment        ();

      void       clear               (void);
      void       clearPartStorage    (void);
      void       setOwner            (ScorePageSet* owner);
      int        getPartCount        (void) const;
      int        getSystemCount      (void) const;
      const AddressSystem& getSystemAddress (int index);
      vectorSIp& getSystemItems      (const AddressSystem& address);
      vectorSIp& getSystemItems      (int sysindex);
      
      const vectorVASp& getSystemAddresses (int partindex);
      ostream&   printInfo           (ostream& out) const;
      void       defineSegment       (ScorePageSet& pageset, 
                                      AddressSystem& starting, 
                                      AddressSystem& ending, int debug = 0);
      void       analyzePartStaves   (vectorI& partlist, 
                                      ScorePageSet& scoreset, 
                                      AddressSystem& startsys, 
                                      AddressSystem& endsys);
      int        getSystemStaffIndex (int systemindex, int partindex, 
                                      int subpartindex);
      int        getPageStaffIndex   (int systemindex, int partindex, 
                                      int subpartindex = 0);

      const AddressSystem& getBeginSystem (void) const;
      const AddressSystem& getStartSystem (void) const;
      const AddressSystem& getEndSystem   (void) const;

      // accessors for vector<SegmentPart*> data
      string         getPartName         (int partindex);
      AddressSystem& getPartAddress      (int systemindex, int partindex);
      ScoreItem*     getInitialClef      (int partindex);
   
   protected:
      AddressSystem        start_system;
      AddressSystem        end_system;
      vector<SegmentPart*> part_storage;
      ScorePageSet*        pageset_owner;
      

   private:
      void        prepareParts       (ScorePageSet& page);
      void        getPartList        (vectorI& partlist, 
                                      ScorePageSet& pageset, 
                                      AddressSystem& sp, 
                                      AddressSystem& ep);
      string      extractPartName    (ScorePageSet& pageset, 
                                      AddressSystem& startsys, int partnum);

};


ostream& operator<<(ostream& out, const ScoreSegment& segment);



#endif  /* _SCORESEGMENT_H_INCLUDED */



