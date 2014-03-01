//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:34:02 PST 2014
// Last Modified: Thu Feb  6 01:05:52 PST 2014
// Filename:      ScoreItem.h
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/include/ScoreItem.h
// Syntax:        C++11
//
// Description:   The ScoreItem class stores one SCORE item as well as
//                adds key/value named parameters for analysis and extensions.
//                This class adds manipulation to the ScoreItemEdit /
//                ScoreItemBase classes.
//

#ifndef _SCOREITEM_H_INCLUDED
#define _SCOREITEM_H_INCLUDED

using namespace std;

#ifdef SCOREITEMEDIT
   #include "ScoreItemEdit.h"
   class ScoreItem : public ScoreItemEdit {
#else
   #include "ScoreItemBase.h"
   class ScoreItem : public ScoreItemBase {
#endif

   public:
                    ScoreItem      (void);
                    ScoreItem      (const ScoreItem& anItem);
                    ScoreItem      (const ScoreItemBase& anItem);
                    ScoreItem      (const vectorSF& parameters);
                   ~ScoreItem      ();

      ScoreItem&    operator=      (const ScoreItem& anItem);

      int          getParameterDigit (int pindex, int position);
      int          getPDigit         (int pindex, int position);
      void         setParameterDigit (int pindex, int position, int value);
      void         setPDigit         (int pindex, int position, int value);

      // P1-P4 are the same for most P1 item types.

      // P1 accessors
      int           getItemType          (void);

      // P2 accessors
      unsigned int  getStaffNumber       (void);
      unsigned int  getStaffNum          (void) { return getStaffNumber(); }

      // P3 accessors
      SCORE_FLOAT   getHorizontalPosition(void);
      SCORE_FLOAT   getHPos              (void);

      // P4 accessors
      SCORE_FLOAT   getVerticalPosition  (void);
      SCORE_FLOAT   getVPos              (void);

      // Duration processing
      void         setStaffOffsetDuration(SCORE_FLOAT duration);
      SCORE_FLOAT  getStaffOffsetDuration(void);
      bool         hasDuration           (void);
      SCORE_FLOAT  getDuration           (void);

      // Note (P1=1) processing functions
      int          stemUp                (void);
      int          stemDown              (void);
      int          stemFlip              (void);

      // Rest (P1=2) processing functions

      // Clef (P1=3) processing functions

      // Line (P1=4) processing functions

      // Slur (P1=5) processing functions

      // Beam (P1=6) processing functions

      // Trill (P1=7) processing functions

      // Staff (P1=8) processing functions

      // Symbol (P1=9) processing functions

      // Number (P1=10) processing functions

      // User (P1=11) processing functions

      // Special (P1=12) processing functions

      // BadLuck (P1=13) processing functions

      // Barline (P1=14) processing functions

      // ImportedEPSGraphic (P1=15) processing functions

      // Text (P1=16) processing functions

      // KeySignature (P1=17) processing functions

      // MeterSignature (P1=18) processing functions


};


// ScoreItem typedefs:
typedef list<ScoreItem*>           listSIp;
typedef vector<ScoreItem*>         vectorSIp;
typedef vector<vector<ScoreItem*>> vectorVSIp;


// ScoreItem sorting functions (defined in ScoreItem_sort.cpp):
bool sortP3P4P1(ScoreItem* a, ScoreItem* b);
bool sortP3P1P4(ScoreItem* a, ScoreItem* b);


// external printing functions (defined in ScoreItem_print.cpp):
ostream& operator<<(ostream& out, ScoreItem& si);
ostream& operator<<(ostream& out, ScoreItem* si);
ostream& operator<<(ostream& out, listSIp& sipvector);
ostream& operator<<(ostream& out, vectorSIp& sipvector);
ostream& operator<<(ostream& out, vectorVSIp& sipvvector);

#endif  /* _SCOREITEM_H_INCLUDED */


