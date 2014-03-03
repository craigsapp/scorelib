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
              ScorePage         (void);
              ScorePage         (const char* filename);
              ScorePage         (istream& instream);
              ScorePage         (const ScorePage& apage);
              ScorePage         (const ScorePageBase& apage);
             ~ScorePage         ();

      // Data list functions found in ScorePage_data.cpp:
      void getHorizontallySortedStaffItems (vectorVSIp& staffsequence);
      void getUnsortedStaffItems           (int staffnum, vectorSIp& items);
      void getUnsortedStaffItems           (vectorSIp& items, int staffnum);
      void getSortedStaffItems             (int staffnum, vectorSIp& items);
      void getSortedStaffItems             (vectorSIp& items, int staffnum);

      // Staff Analysis functions found in ScorePage_staff.cpp:
      void        analyzeStaves         (void);
      int         getMaxStaff           (void);
      bool        stavesAreConsecutive  (void);
      const vectorVSIp& getStaffItemList(void);
      void        setStaffDuration      (int staffnum, SCORE_FLOAT duration);
      SCORE_FLOAT getStaffDuration      (int staffnum);

      // Staff duration analysis functions found in ScorePage_duration.cpp
      void       analyzeStaffDurations(void);

   protected:
      SCORE_FLOAT calculateStaffDuration(vectorSIp& staffitems);


   public:
      // System Analysis functions
      void       analyzeSystems     (void);
      int        getSystemCount     (void);


};


#endif /* _SCOREPAGE_H_INCLUDED */


