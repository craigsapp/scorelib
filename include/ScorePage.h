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
#include "SystemAddress.h"
#include "DatabaseP3.h"

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

      // Staff-level data lists:
      void getUnsortedStaffItems        (int staffnum, vectorSIp& items);
      void getUnsortedStaffItems        (vectorSIp& items, int staffnum);
      void getSortedStaffItems          (int staffnum, vectorSIp& items);
      void getSortedStaffItems          (vectorSIp& items, int staffnum);


      // System-level data lists:
      void getHorizontallySortedStaffItems (vectorVSIp& staffsequence);
      void getUnsortedSystemItems          (vectorSIp& sysseq, int sysindex);
      void getUnsortedSystemItems          (int sysindex, vectorSIp& sysseq);
      void getHorizontallySortedSystemItems(vectorSIp& sysseq, int sysindex);
      void getHorizontallySortedSystemItems(int sysindex, vectorSIp& sysseq);
//      void getSystemP3Database             (int sysindex, DatabaseP3& list);


      // Staff Analysis functions (defined in ScorePage_staff.cpp):
      void        analyzeStaves          (void);
      int         getMaxStaff            (void);
      bool        stavesAreConsecutive   (void);
      const vectorVSIp& getStaffItemList (void);
      vectorVVSIp& getP8BySystem         (void);
      vectorSIp&  getP8BySystem          (int p2index);
   protected:
      void        fillStaffScoreItemLists(void);
   public:
      vectorVSIp& staffItems             (void);
      vectorSIp&  staffItems             (int p2index);


      //  System analysis functions (defined in ScorePage_system.cpp):
      int         analyzeSystems         (void);
      int         getSystemCount         (void);
      vectorI&    systemMap              (void);
      vectorI&    systemStaffMap         (void);
      int&        systemMap              (int index);
      int&        systemStaffMap         (int index);
      vectorVI&   reverseSystemMap       (void);
      int         getSystemIndex         (int staffnumber);
      int         getSystemStaffIndex    (int staffnumber);
      void        fillSystemScoreItemLists(void);
      vectorSIp&  systemItems            (int sindex);
      int         getPageStaff           (int sysindex, int partnum);
      int         getPageStaff           (SystemAddress& partaddress);


      // Staff duration analysis functions (defined in ScorePage_duration.cpp):
      void        analyzeStaffDurations  (void);
      SCORE_FLOAT getStaffDuration       (int staffnum);
      void        setStaffDuration       (int staffnum, SCORE_FLOAT duration);
   private:
      SCORE_FLOAT calculateStaffDuration(vectorSIp& staffitems);
   public:

      // Rhythm analysis functions (defined in ScorePage_rhythm.cpp):
      int         getSystemLCMRhythm    (int systemindex);

      // Pitch analysis functions (defined in ScorePage_pitch.cpp):
      void        analyzePitch          (void);
      void        analyzeSystemPitch    (vectorSIp& systemitems);

      // Measure analysis functions (defined in ScorePage_barline.cpp):
      void        analyzeBarlines       (void);
      int         getSystemBarCount     (int sysindex);
      int         getSystemMeasureCount (int sysindex);
      vectorSMp&  getSystemMeasures     (int sysindex);
      SystemMeasure& getSystemMeasure   (int sysindex, int measureindex);
   protected:
      void        analyzeBarlines       (vectorSMp& sysmeasures, int sysindex);
   public:

      // Chord analysis functions (defined in ScorePage_chord.cpp):
      void        analyzeChords         (void);
      vectorSIp*  chordNotes            (ScoreItem* chordtone);


      // Beam analysis functions (defined in ScorePage_beam.cpp):
      void        analyzeBeams          (SCORE_FLOAT tolerance = 0.001);
      BeamGroup*  beamInfo              (ScoreItem* item);
      void        analyzeBeamsOnStaff   (int p2index, 
                                         SCORE_FLOAT tolerance = 0.001);
      ostream&    printBeamDatabase     (ostream& out = cout);


      // P3 analysis functions (defined in ScorePage_p3.cpp):
      void        analyzeP3                 (void);
      SCORE_FLOAT getP3OfStaffDurationOffset(int sys, SCORE_FLOAT offset);

   private:
      // private sorting functions:
      void        sortPageHorizontally  (void);

      // private pitch analysis functions:
      void        resetPitchSpellings   (int staffidx, int barheight, 
                                         vectorVI& pitchstate, 
                                         vectorVI& keysig);
      void        fillPitchStatesWithKeySig(vectorVI& pitchstate, int sysidx,
                                         vectorVI& keysig);

      // private chord analysis functions:
      void        analyzeChordsOnStaff  (int p2index);
      void        analyzeVerticalNoteSet(vectorSIp& data);


};



#endif /* _SCOREPAGE_H_INCLUDED */

