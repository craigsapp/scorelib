//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:11:15 PST 2014
// Last Modified: Sun Mar 16 23:03:37 PDT 2014
// Filename:      ScoreUtility.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreUtility.h
// Syntax:        C++11
//
// Description:   The ScoreUtility namespace holds various static functions
//                which process data realted to Score classes.
//

#ifndef _SCOREUTILITY_H_INCLUDED
#define _SCOREUTILITY_H_INCLUDED

#include "ScoreDefs.h"
#include "ScoreItem.h"
#include "ScoreItemEdit_ParameterHistory.h"
#include <set>

class ScoreItem;

namespace ScoreUtility {

   // ScoreItem sorting functions (defined in ScoreUtility_pitch.cpp):
   string  base40ToKern                (int base40);
   int     base7ToBase40               (int base7);
   int     base40ToBase7               (int base40);
   int     base40ToBase7PitchClass     (int base40);
   int     base40ToChromaticAlteration (int base40);
   char    base40ToUCDiatonicLetter    (int base40);
   char    base40ToLCDiatonicLetter    (int base40);

   // ScoreItem sorting functions (defined in ScoreUtility_sort.cpp):
   bool   sortP3              (ScoreItem* a, ScoreItem* b);
   bool   sortP3P1            (ScoreItem* a, ScoreItem* b);
   bool   sortP3P2            (ScoreItem* a, ScoreItem* b);
   bool   sortP3P4P1          (ScoreItem* a, ScoreItem* b);
   bool   sortP3P1P4          (ScoreItem* a, ScoreItem* b);
   bool   sortP3P2P1          (ScoreItem* a, ScoreItem* b);
   bool   sortP3P2P1P4        (ScoreItem* a, ScoreItem* b);
   bool   sortP3P1P2P4        (ScoreItem* a, ScoreItem* b);

   // ScoreItem comparison functions (defined in ScoreUtility_compare.cpp):
   bool   equalClefs          (ScoreItem* a, ScoreItem* b);
   bool   equalClef           (ScoreItem* a, ScoreItem* b);

   bool   equalTimeSignature  (ScoreItem* a, ScoreItem* b);
   bool   equalTimeSignatures (ScoreItem* a, ScoreItem* b);
   bool   equalTimeSig        (ScoreItem* a, ScoreItem* b);
   bool   equalTimeSigs       (ScoreItem* a, ScoreItem* b);

   bool   equalKeySignature   (ScoreItem* a, ScoreItem* b);
   bool   equalKeySignatures  (ScoreItem* a, ScoreItem* b);
   bool   equalKeySig         (ScoreItem* a, ScoreItem* b);
   bool   equalKeySigs        (ScoreItem* a, ScoreItem* b);

   // math-related functions (defined in ScoreUtility_math.cpp):
   int    gcd                 (int x, int y);
   int    lcm                 (int x, int y);
   int    lcm                 (set<int>& numbers);

   // Tie/Slur differentiation functions (defined in ScoreUtility_ties.cpp):
   void   analyzeStaffTies    (int staffnum, vectorSIp& items,
                               vectorSIp& hangingleft, vectorSIp& leftnotes,
                               vectorSIp& hangingright, vectorSIp& rightnotes);
   void identifySlurStartStopOffsets(int index,
                               vectorSIp& slurs,
                               vectorVSIp& notes,
                               map<SCORE_FLOAT, int>& offsettoindex,
                               vectorSIp& hangingleft,
                               vectorSIp& leftnotes,
                               vectorSIp& hangingright,
                               vectorSIp& rightnotes);
   void   linkNotesByTie      (ScoreItem* startslur,  SCORE_FLOAT slurdur,
                               vectorSIp& leftnotes, ScoreItem* endslur,
                               vectorSIp& rightnotes);

   // text-related functions (defined in ScoreUtility_text.cpp):
   ostream& printXmlTextEscapedUTF8      (ostream& out, const string& text);
   string   getTextNoFontXmlEscapedUTF8  (const string& text);

}  // end of namespace ScoreUtility


namespace SU = ScoreUtility;  // abbreviation


#endif  /* _SCOREUTILITYH_INCLUDED */



