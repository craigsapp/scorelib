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
#include "AddressSystem.h"
#include "DatabaseP3.h"

using namespace std;

class ScorePageSet;

class ScorePage : public ScorePageBase {
   public:
                  ScorePage             (void);
                  ScorePage             (const char* filename);
                  ScorePage             (istream& instream);
                  ScorePage             (const ScorePage& apage);
                  ScorePage             (const ScorePageBase& apage);
                 ~ScorePage             ();

      ScorePageSet* getSetOwner         (void);
      void          setSetOwner         (ScorePageSet* owner);

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
      void         analyzeStaves              (void);
      int          getMaxStaff                (void);
      bool         stavesAreConsecutive       (void);
      const vectorVSIp& getStaffItemList      (void);
      vectorVSIp&  getStaffItemListNotConst   (void);
      vectorVVSIp& getStaffItemsBySystem      (void);
      vectorSIp&   getStaffItemsByPageStaff   (int p2index);
      vectorSIp&   getStaffItemsBySystemStaff (int sysindex, int sysstaffindex);
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
      vectorSIp&  getSystemItems         (int sindex);
      int         getPageStaffIndex      (int sysindex, int partnum);
      int         getPageStaffIndex      (const AddressSystem& partaddress);
      void        setPageOwner           (void* page);
      ScorePage*  getPageOwner           (void);


      // Staff duration analysis functions (defined in ScorePage_duration.cpp):
      void        analyzeStaffDurations  (void);
      SCORE_FLOAT getStaffDuration       (int staffnum);
      void        setStaffDuration       (int staffnum, SCORE_FLOAT duration);
   private:
      SCORE_FLOAT calculateStaffDuration(vectorSIp& staffitems);
   public:

      // Layer analysis functions (defined in ScorePage_layer.cpp):
      int         analyzeLayers          (void);
   private:
      void        private_analyzeStaffLayers(vectorSIp& items);
      void        setChordLayer          (ScoreItem* note, int layer);
      void        chooseLayer            (vectorSIp& notes, int layer);
   public:

      // Staff-level tie analysis functions (defined in ScorePage_tie.cpp):
      int         analyzeTies(void);

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

      // Lyrics analysis functions (defined in ScorePage_lyrics.cpp):
      vectorSIp*   getLyricsGroup       (ScoreItem* item);

      // Beam analysis functions (defined in ScorePage_beam.cpp):
      void        analyzeBeams          (SCORE_FLOAT tolerance = 0.001);
      BeamGroup*  beamInfo              (ScoreItem* item);
      ostream&    printBeamDatabase     (ostream& out = cout);
   private:
      void        analyzeBeamsOnStaff   (int p2index, 
                                         SCORE_FLOAT tolerance = 0.001);
   public:

      // Tuplet analysis functions (defined in ScorePage_tuplet.cpp):
      void         analyzeTuplets        (SCORE_FLOAT tolerance = 0.001);
      ostream&     printTupletDatabase   (ostream& out);
      TupletGroup* tupletInfo            (ScoreItem* item);
   private:
      void        analyzeTupletsOnStaff  (int p2index, 
                                         SCORE_FLOAT tolerance = 0.001);
   public:

      // P3 analysis functions (defined in ScorePage_p3.cpp):
      void        analyzeP3               (void);
      SCORE_FLOAT getP3OfStaffDurationOffset(int sys, SCORE_FLOAT offset);

      // Global parameter processing functions (ScorePage_parameter.cpp):
      void         copyParameterOverwrite (const string& newnamespace, 
                                           const string& oldnamespace, 
                                           const string& parameter);
      void         copyParameterNoOverwrite(const string& newnamespace, 
                                           const string& oldnamespace, 
                                           const string& parameter);

      // Segment data (defined in ScorePage_segment.cpp):
      void         setStaffPartIndex      (int p2val, int partindex);
      int          getPartIndex           (int p2value);



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

