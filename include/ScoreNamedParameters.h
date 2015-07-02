//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu May  1 14:46:29 PDT 2014
// Last Modified: Thu May  1 14:46:32 PDT 2014
// Filename:      ScoreNamedParameters.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreNamedParameters.h
// Syntax:        C++11
//
// Description:   Official named parameters and namespaces.
//

#ifndef _SCORENAMEDPARAMETER_H_INCLUDED
#define _SCORENAMEDPARAMETER_H_INCLUDED

#include <string>

using namespace std;


///////////////////////////////////////////////////////////////////////////
//
// auto namespace -- The "auto" namespace is used to store automatically
//    calculated parameters, such as pitch.
//

const string ns_auto = "auto";


///////////////////////////////////
//
// Automatic rhythm analysis parameters:
//

// @auto@staffOffsetDuration -- The duration from the start of the staff
//      to the given item, in terms of quarter notes.
const string np_staffOffsetDuration = "staffOffsetDuration";

// @auto@segmentOffsetDuration -- The duration from the start of the segment
//      (typically a movement) the starting time of the given item, in
//      terms of quarter notes.
const string np_segmentOffsetDuration = "segmentOffsetDuration";

// @auto@pagesetOffsetDuration -- The duration from the start of the work
//      until the starting time of the given item, in terms of quarter notes.
const string np_pagesetOffsetDuration = "pagesetOffsetDuration";

// @auto@staffDuration -- The duration from the start to end of a staff.
const string np_staffDuration = "staffDuration";


///////////////////////////////////
//
// Automatic layer analysis parameters:
//

// @auto@staffDuration -- The duration from the start to end of a staff.
const string np_layer = "layer";


///////////////////////////////////
//
// Automatic slur analysis parameters:
//

// @auto@staffOffsetLeft -- The duration from the start of the staff to the
//     starting point of the slur/tie.
const string np_staffOffsetLeft = "staffOffsetLeft";

// @auto@staffOffsetRight -- The duration from the start of the staff to the
//     ending point of the slur/tie.
const string np_staffOffsetRight = "staffOffsetRight";

// @auto@duration -- The duration from the start start of a slur/tie to its end.
const string np_slurDuration = "duration";

// @auto@tie -- The slur/tie is a tie. The value should be true/false.
const string np_tie = "tie";

// @auto@tiedNextNote -- The next note in a tied group.
const string np_tiedNextNote = "tiedNextNote";

// @auto@tiedLastNote -- The previous note in a tied group.
const string np_tiedLastNote = "tiedLastNote";

// @auto@tieLast -- The previous tie though which the last note is connected.
const string np_tieLast = "tieLast";

// @auto@tieNext -- The previous tie though which the last note is connected.
const string np_tieNext = "tieNext";

// @auto@handLeft -- The slur/tie is attached to a note on the right, but
// goes off the left end of the system.
const string np_hangLeft = "hangLeft";

// @auto@handRight -- The slur/tie is attached to a note on the left, but
// goes off the right end of the system.
const string np_hangRight = "hangRight";

// @auto@tiedNextSlur -- indicates the next slur in a tied group
// (occurs across system breaks).
const string np_tiedNextSlur = "tiedNextSlur";

// @auto@tiedLastSlur -- indicates the previous slur in a tied group
// (occurs across system breaks).
const string np_tiedLastSlur = "tiedLastSlur";


///////////////////////////////////
//
// Memory address for SCORE items.
//

// @auto@address -- The memory address of a SCORE object.
const string np_address = "address";


///////////////////////////////////
//
// Automatic pitch analysis parameters:
//

// @auto@base40Pitch -- The automatically identified pitch in base-40 format.
const string np_base40Pitch = "base40Pitch";

// @auto@courtesy -- True if a courtesy accidental is shown for the
//     given note item.  The value should be true|false.
const string np_courtesy = "courtesy";


///////////////////////////////////
//
// Automatic text analysis parameters:
//

// @auto@function -- The automatically identified function of text items.
const string np_function = "function";


///////////////////////////////////////////////////////////////////////////
//
// lyrics namespace -- The "lyrics" namespace is used to store information
//      about musical lyrics.

const string ns_lyrics = "lyrics";

// @lyrics@verseLine: indicates that a text item is part of some lyrics,
//     and this is the line number of the lyric.  The highest lyric
//     is number 1.  Zero is not used for a lyric number.
const string np_verseLine = "verseLine";

// @lyrics@hyphenAfter: indicates that a text items is not the end of
//     a word, and there is another syllable to follow.  The value
//     should be true|false.
const string np_hyphenAfter = "hyphenAfter";

// @lyrics@hyphenBefore: indicates that a text items is not the start of
//     a word, and there is another syllable preceding it.  The value
//     should be true|false.
const string np_hyphenBefore = "hyphenBefore";

// @lyrics@hyphen: indicates that a line item (P1=4) is a word hyphen
//     within lyrics.  The value should be true|false.
const string np_lyricsHyphen = "lyricsHyphen";

// @lyrics@wordExtension: indicates that a line items is a word extension
//     for a lyric syllable.  The value should be true|false.
const string np_wordExtension = "wordExtension";


/////////////////////////////////////////////////////////////////////////////
//
// Function parameters:
//

// @function: indicates the function of an item.
//     known values: barnum, fingernum, pagenum
// Already defined above:
// const string np_function = "function";

// @barnum: indicates the bar number of a barline.
const string np_barnum = "barnum";



/////////////////////////////////////////////////////////////////////////////
//
// Footnotes parameters:
//

// @footnote: indicates the text of a footnote related to an item.
const string np_footnote = "footnote";

#endif /* _SCORENAMEDPARAMETER_H_INCLUDED */

