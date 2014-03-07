//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 17 22:22:39 PST 2014
// Last Modified: Mon Feb 17 22:22:44 PST 2014
// Filename:      ScorePage.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePage.h
// Syntax:        C++11
//
// Description:   This class handles analysis of relationships between 
//                SCORE items on the page, such as system, pitch, and chord 
//                groupings.
//

#ifndef _SCOREPAGE_H_INCLUDED
#define _SCOREPAGE_H_INCLUDED

#include "ScorePageBase.h"

using namespace std;


class ScorePage : public ScorePageBase {

   public:
                  ScorePage             (void);
                  ScorePage             (const char* filename);
                  ScorePage             (istream& instream);
                  ScorePage             (const ScorePage& apage);
                  ScorePage             (const ScorePageBase& apage);
                 ~ScorePage             ();


      // Data access functions (defined in ScorePage_data.cpp):
      void getUnsortedStaffItems        (int staffnum, vectorSIp& items);
      void getUnsortedStaffItems        (vectorSIp& items, int staffnum);
      void getSortedStaffItems          (int staffnum, vectorSIp& items);
      void getSortedStaffItems          (vectorSIp& items, int staffnum);
      void getHorizontallySortedStaffItems (vectorVSIp& staffsequence);
      void getUnsortedSystemItems       (vectorSIp& sysseq, int sysindex);
      void getHorizontallySortedSystemItems(vectorSIp& sysseq, int sysindex);


      // Staff Analysis functions (defined in ScorePage_staff.cpp):
      void        analyzeStaves         (void);
      int         getMaxStaff           (void);
      bool        stavesAreConsecutive  (void);
      const vectorVSIp& getStaffItemList(void);
      void        setStaffDuration      (int staffnum, SCORE_FLOAT duration);
      SCORE_FLOAT getStaffDuration      (int staffnum);


      //  System analysis functions (defined in ScorePage_system.cpp):
      int         analyzeSystems        (void);
      int         getSystemCount        (void);
      vectorI&    systemMap             (void);
      vectorI&    systemStaffMap        (void);
      int&        systemMap             (int index);
      int&        systemStaffMap        (int index);
      vectorVI&   reverseSystemMap      (void);
      int         getSystemIndex        (int staffnumber);
      int         getSystemStaffIndex   (int staffnumber);

      // Staff duration analysis functions (defined in ScorePage_duration.cpp):
      void        analyzeStaffDurations (void);

      // Pitch analysis functions (defined in ScorePage_pitch.cpp):
      void        analyzePitch          (void);
      void        analyzePitchOnSystem  (vectorSIp& systemitems);
      static int  base7ToBase40         (int base7);

   private:
      void        resetPitchSpellings   (int staffidx, int barheight, 
                                         vectorVI& pitchstate, 
                                         vectorVI& keysig);
      void        fillPitchStatesWithKeySig(vectorVI& pitchstate, int sysidx,
                                         vectorVI& keysig);
      string      base40ToKern          (int base40);

   protected:
      SCORE_FLOAT calculateStaffDuration(vectorSIp& staffitems);

};


#endif /* _SCOREPAGE_H_INCLUDED */


