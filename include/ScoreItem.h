//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:34:02 PST 2014
// Last Modified: Thu Feb  6 01:05:52 PST 2014
// Filename:      ScoreItem.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreItem.h
// Syntax:        C++11
//
// Description:   The ScoreItem class stores one SCORE item as well as
//                adds key/value named parameters for analysis and extensions.
//                This class adds manipulation to the ScoreItemEdit /
//                ScoreItemBase classes.
//

#ifndef _SCOREITEM_H_INCLUDED
#define _SCOREITEM_H_INCLUDED

#include "DatabaseBeam.h"
#include "DatabaseTuplet.h"
#include "RationalDuration.h"
#include "ScoreDefs.h"
#include <list>
#include <vector>

using namespace std;

class ScoreItem;
class ScorePage;

// ScoreItem typedefs:
using listSIp     = list<ScoreItem*>;

#ifndef UseBoundVector
   using vectorSIp   = vector<ScoreItem*>;
   using vectorVSIp  = vector<vector<ScoreItem*>>;
   using vectorVVSIp = vector<vector<vector<ScoreItem*>>>;
#else
   #include "BoundVector.h"
   using vectorSIp   = BoundVector<ScoreItem*>;
   using vectorVSIp  = BoundVector<BoundVector<ScoreItem*>>;
   using vectorVVSIp = BoundVector<BoundVector<BoundVector<ScoreItem*>>>;
#endif


// define SCOREITEMEDIT when compiling this class to use the ScoreItemEdit
// class to keep track of the parameter changes (for undo/redo options
// in a user interface).

#ifdef SCOREITEMEDIT
   #include "ScoreItemEdit.h"
   class ScoreItem : public ScoreItemEdit {
   public:
                    ScoreItem      (const ScoreItemEdit& anItem);
#else
   #include "ScoreItemBase.h"
   class ScoreItem : public ScoreItemBase {
#endif

   public:
                   ScoreItem              (void);
                   ScoreItem              (const ScoreItem& anItem);
                   ScoreItem              (const ScoreItemBase& anItem);
                   ScoreItem              (const vectorSF& parameters);
                  ~ScoreItem              ();

      ScoreItem&   operator=              (const ScoreItem& anItem);
      bool         operator==             (const ScoreItemBase& item2);

      int          getParameterDigit      (int pindex, int position);
      int          getPDigit              (int pindex, int position);
      void         setParameterDigit      (int pindex, int position, int value);
      void         setPDigit              (int pindex, int position, int value);
      void         setParameterIntegerPart(int pindex, int intval);
      void         setPIntPart            (int pindex, int intval);
      void         copyParameterOverwrite (const string& newnamespace,
                                           const string& oldnamespace,
                                           const string& parameter);
      void         copyParameterNoOverwrite(const string& newnamespace,
                                           const string& oldnamespace,
                                           const string& parameter);

      // P1-P4 are the same for most P1 item types.

      // P1 accessors
      int          getItemType                  (void);
      void         setItemType                  (SCORE_FLOAT type);
      bool         isNoteItem                   (void);    // P1==1
      bool         isRestItem                   (void);    // P1==2
      bool         isNoteOrRestItem             (void);    // P1==(1||2)
      bool         isClefItem                   (void);    // P1==3
      bool         isLineItem                   (void);    // P1==4
      bool         isSlurItem                   (void);    // P1==5
      bool         isBeamItem                   (void);    // P1==6
      bool         isTrillItem                  (void);    // P1==7
      bool         isStaffItem                  (void);    // P1==8
      bool         isSymbolItem                 (void);    // P1==9
      bool         isNumberItem                 (void);    // P1==10
      bool         isUserItem                   (void);    // P1==11
      bool         isSpecialItem                (void);    // P1==12
      bool         isShapeItem                  (void);    // P1==12
      bool         isBadLuckItem                (void);    // P1==13
      bool         isBarlineItem                (void);    // P1==14
      bool         isEpsItem                    (void);    // P1==15
      bool         isTextItem                   (void);    // P1==16
      bool         isKeySigItem                 (void);    // P1==17
      bool         isKeySignatureItem           (void);    // P1==17
      bool         isTimeSigItem                (void);    // P1==18
      bool         isTimeSignatureItem          (void);    // P1==18

      // P2 accessors
      unsigned int getStaffNumber               (void);
      unsigned int getStaffNum                  (void);
      void         setStaffNumber               (int staffnum);
      void         setStaffNum                  (int staffnum);

      // P3 accessors
      SCORE_FLOAT  getHorizontalPosition        (void);
      SCORE_FLOAT  getHPos                      (void);
      SCORE_FLOAT  getHorizontalPositionLeft    (void);
      SCORE_FLOAT  getHPosL                     (void);
      void         setHorizontalPosition        (SCORE_FLOAT pos);
      void         setHPos                      (SCORE_FLOAT pos);
      // Horizontal offset (secondary to P3 position):
      SCORE_FLOAT  getHorizontalOffset          (void);

      // P4 accessors
      SCORE_FLOAT  getVerticalPosition          (void);
      SCORE_FLOAT  getVPos                      (void);
      // Add a setVPos function.
      bool         isAboveStaff                 (void);
      bool         isBelowStaff                 (void);
      bool         isOnStaff                    (void);

      // P5 vertical position of right end accessors
      SCORE_FLOAT  getVerticalPositionRight     (void);
      SCORE_FLOAT  getVPosRight                 (void);

      // P6 horizontal position of right end accessors
      SCORE_FLOAT  getHorizontalPositionRight   (void);
      SCORE_FLOAT  getHPosRight                 (void);
      SCORE_FLOAT  getHPosR                     (void);

      // Duration processing
      void         setStaffOffsetDuration       (SCORE_FLOAT duration);
      SCORE_FLOAT  getStaffOffsetDuration       (void);
      bool         hasDuration                  (void);
      SCORE_FLOAT  getDuration                  (void);
      RationalDuration getRationalDuration      (void);
      RationalNumber getDurationIncludingDots   (void);

      // Note (P1=1) processing functions  (defined in ScoreItem_notes.cpp):
      // functions related to stems:
      int          hasStem                      (void);
      int          getStemDirection             (void);
      int          stemUp                       (void);
      int          stemDown                     (void);
      int          stemFlip                     (void);
      int          getStemBottomVPos            (void);
      int          getStemTopVPos               (void);
      int          getStemLength                (void);
      void         hideStem                     (void);
      // functions related to accidentals:
      int          getPrintedAccidental         (void);
      bool         hasPrintedAccidental         (void);
      int          getEditorialAccidental       (void);
      bool         hasEditorialAccidental       (void);
      int          hasNatural                   (void);
      void         setNoAccidental              (void);
      void         setAccidentalParentheses     (void);
      void         removeAccidentalParentheses  (void);
      // functions related to articulations
      int          getArticulation              (void);
      void         removeArticulation           (void);
      int          hasFermata                   (void);
      // functions related to noteheads
      int          isCueSize                    (void);
      void         hideNotehead                 (void);
      // functions related to rhythm
      void         removeFlags                  (void);
      int          getDotCount                  (void);
      // functions related to ties
      bool         inTieGroup                   (void);
      bool         isTieGroupStart              (void);
      bool         isTieGroupMiddle             (void);
      bool         isTieGroupEnd                (void);

      // Rest (P1=2) processing functions (defined in ScoreItem_rests.cpp):
      int          isInvisible                  (void);

      // Clef (P1=3) processing functions (defined in ScoreItem_clefs.cpp):
      int          getMiddleCVpos               (void);
      int          getStaffLine                 (void);
      char         getClefLetter                (void);
      bool         soundsOctaveDown             (void);

      // Line (P1=4) processing function (defined in ScoreItem_lines.cpp):
      bool         isHairpin                    (void);
      bool         isWavyLine                   (void);
      bool         isHairpinCrescendo           (void);
      bool         isHairpinDecrescendo         (void);
      bool         isDashedLine                 (void);
      bool         isPlainLine                  (void);
      bool         isHorizontal                 (void);

      // Slur (P1=5) processing functions (defined in ScoreItem_slurs.cpp):
      //    see isHorizontal() in P1=4 list.
      bool         isTie                        (void);
      bool         isTupletBracket              (void);
      SCORE_FLOAT  getVisualLeftPosition        (void);
      SCORE_FLOAT  getVisualRightPosition       (void);
      // See also: getTupletNumber

      // Beam (P1=6) processing functions
      //    see isHorizontal() in P1=4 functions.
      //    see hasStem() from P1=1 functions.
      //    see getStemDirection() from P1=1 functions.
      bool          isTupletBeam                (void);
      int           getTupletNumber             (void);

      // Trill (P1=7) processing functions

      // Staff (P1=8) processing functions (defined in ScoreItem_staff.cpp):
      SCORE_FLOAT  getPartNumber                 (void);
      int          getPartNumberInt              (void);
      SCORE_FLOAT  getInstrumentNumber           (void);
      int          getInstrumentNumberInt        (void);
      SCORE_FLOAT  getScale                      (void);
      void         convertStaffP4ToP10Inches     (void);
      void         convertStaffP4ToP10Centimeters(void);
      void         convertStaffP10InchesToP4Value(void);
      void         convertStaffP10CentimetersToP4Value(void);
      void         setStaffP4FromP10Inches       (SCORE_FLOAT p10);
      void         setStaffP4FromP10Centimeters  (SCORE_FLOAT p10);
      void         setStaffP10InchesFromP4Value  (SCORE_FLOAT p4);
      void         setStaffP10CentimetersFromP4Value(SCORE_FLOAT p4);


      // Symbol (P1=9) processing functions

      // Number (P1=10) processing functions

      // User (P1=11) processing functions

      // Special Shapes (P1=12) processing fns (def. in ScoreItem_shapes.cpp):
      bool         isCircular                    (void);
      bool         isCircle                      (void);
      bool         isEllipse                     (void);
      bool         isRectangular                 (void);
      bool         isSquare                      (void);
      bool         isRectangle                   (void);
      bool         isParallelogram               (void);
      bool         isFilled                      (void);
      bool         isUnfilled                    (void);
      bool         isFilledCircle                (void);
      bool         isUnfilledCircle              (void);
      SCORE_FLOAT  getHorizontalRadius           (void);
      SCORE_FLOAT  getVerticalRadius             (void);

      // BadLuck (P1=13) processing functions

      // Barline (P1=14) processing functions
      int          getBarlineHeight              (void);
      int          getBarHeight                  (void);
      int          getHeight                     (void);

      // ImportedEPSGraphic (P1=15) processing functions

      // Text (P1=16) processing functions (defined in ScoreItem_text.cpp):
      void          make99Text                   (const string& text);
      void          makeSvgCode                  (const string& text);
      const string& getText                      (void);
      void          setText                      (const string& text);
      string        getTextWithoutInitialFontCode(void);
      bool          isItalic                     (void);
      bool          isBold                       (void);
      string        getTextNoFont                (void);
      string        getInitialFontCode           (void);
      int           getInitialFontCodeAsInteger  (void);
      string        getTextNoFontXmlEscapedUTF8  (void);
      SCORE_FLOAT   getFontSizeInPoints          (void);
      SCORE_FLOAT   getFontSizeInPoints          (SCORE_FLOAT staff_size);
      bool          hasWordExtension             (void);
      bool          hasHyphenAfter               (void);
      bool          hasHyphenBefore              (void);
      // getScale() see P1=8

      // KeySignature (P1=17) functions (defined in ScoreItem_keysigs.cpp)
      int          getDiatonicAccidentalState(vectorI& states);
      int          getAccidentalCount            (void);

      // TimeSignature (P1=18) functions (defined in ScoreItem_timesigs.cpp):
      int          getTimeSignatureTop           (void);
      int          getTimeSignatureBottom        (void);
      bool         isCommonTime                  (void);
      bool         isCutTime                     (void);
      bool         isSingleNumber                (void);

      // Page-related functions (defined in ScoreItem_page.cpp):
      int          getPageSystemIndex            (void);
      int          getSystemIndex                (void);
      int          getPartIndex                  (void);

      vectorSIp*   getChordNotes                 (void);
      bool         isPrimaryChordNote            (void);
      int          getChordNoteCount             (void);
      bool         isSecondaryChordNote          (void);
      ScoreItem*   getPrimaryChordNote           (void);
      ScoreItem*   getStaffOwner                 (void);
      int          getPageIndex                  (void);
      ScorePage*   getPageOwner                  (void);
      void         setPageOwner                  (void* page);
      SCORE_FLOAT  getStaffScale                 (void);
      SCORE_FLOAT  getStaffVerticalOffset        (void);

      BeamGroup*   getBeamGroup                  (void);
      BeamGroup*   getBeamInfo                   (void);
      BeamGroup*   beamInfo                      (void);
      BeamGroup*   beamGroup                     (void);
      int          inBeamGroup                   (void);
      bool         isFirstNoteInBeamGroup        (void);

      TupletGroup* getTupletGroup                (void);
      TupletGroup* getTupletInfo                 (void);
      TupletGroup* tupletInfo                    (void);
      TupletGroup* tupletGroup                   (void);
      int          inTupletGroup                 (void);
      bool         isFirstNoteInTupletGroup      (void);
      bool         isLastNoteInTupletGroup       (void);
      bool         tupletGroupHasBracket         (void);

      SCORE_FLOAT  getHPosOff                    (void);

      // Lyrics:
      vectorSIp*   getLyricsGroup                (void);


      // internal print functions (defined in ScoreItem_print.cpp):
      ostream&     printNoAuto                   (ostream& out);

      /////////////////////////////////////////////////////////////////////
      //
      // Auto-dependent functions: (the automatic analysis is not checked 
      // before the function is called).
      //

      // Chord analysis dependent functions:
      string       getHumdrumPitch                (void);


};


// external printing functions (defined in ScoreItem_print.cpp):
ostream&    operator<<        (ostream& out, ScoreItem& si);
ostream&    operator<<        (ostream& out, ScoreItem* si);
ostream&    operator<<        (ostream& out, listSIp& sipvector);
ostream&    operator<<        (ostream& out, vectorSIp& sipvector);
ostream&    operator<<        (ostream& out, vectorVSIp& sipvvector);

ostream&    printNoAuto       (ostream& out, listSIp& siplist);
ostream&    printNoAuto       (ostream& out, vectorSIp& sipvector);
ostream&    printNoAuto       (ostream& out, vectorVSIp& sipvvector);
ostream&    printNoAuto       (ostream& out, ScoreItem* sip);
ostream&    printNoAuto       (ostream& out, ScoreItem& si);


// Selecting filters for list of ScoreItems (defined in ScoreItem_select.cpp):
int    selectP1Items       (vectorSIp& output, const vectorSIp& input,
                            const vectorI& p1values);
int    selectP1Items       (vectorSIp& output, const listSIp& input,
                            const vectorI& p1values);
int    selectP1Items       (vectorSIp& output, const vectorSIp& input,
                            int p1value);
int    selectP1Items       (vectorSIp& output, const listSIp& input,
                            int p1value);
int    selectDurationItems (vectorSIp& output, const vectorSIp& input);
int    selectDurationItems (vectorSIp& output, const listSIp& input);
int    selectBarlines      (vectorSIp& output, const vectorSIp& input);
int    selectBarlines      (vectorSIp& output, const listSIp& input);


#endif  /* _SCOREITEM_H_INCLUDED */



