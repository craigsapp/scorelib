//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 02:51:26 PST 2014
// Filename:      ScoreUtility_pitch.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_pitch.cpp
// Syntax:        C++11
//
// Description:   Pitch processing functions for use with Score-related classes.
//

#include "ScoreUtility.h"
#include "ScoreItem.h"
#include <iostream>

using namespace std;


//////////////////////////////
//
// ScoreUtility::base7ToBase40 -- convert a diatonic Base-7 note into 
//      a Base-40 pitch. Example for middle C (C4):
//          base-7  : 0 + 4 * 7  =  28
//          base-40 : 2 + 4 * 40 = 162
//      The +2 for the C chroma is used to allow for C-double-flat
//      to be located at chroma 0, which preserves the octave information
//      when dividing the base-40 pitch number by 40.
//

int ScoreUtility::base7ToBase40(int base7) {
   if (base7 <= 0) {
      return -1;
   }
   int octave = base7 / 7;
   int chroma = base7 % 7;
   int output = 0;
   switch (chroma) {
      case 0:  output =  0;  break;   // 0 = C -> 0
      case 1:  output =  6;  break;   // 1 = D -> 6
      case 2:  output = 12;  break;   // 2 = E -> 12
      case 3:  output = 17;  break;   // 3 = F -> 17
      case 4:  output = 23;  break;   // 4 = G -> 23
      case 5:  output = 29;  break;   // 5 = A -> 29
      case 6:  output = 35;  break;   // 6 = B -> 35
      default: output = 0;
   }

   // the offset by two so that division gives octave:
   return output + 40 * octave + 2;
}



//////////////////////////////
//
// ScoreUtility::base40ToBase7 --  Convert a base-40 pitch to Base-7 
//     (losing any chromatic alterations of the diatonic pitch).
//

int ScoreUtility::base40ToBase7PitchClass(int base40) {
   int chroma = base40 % 40;
   switch (chroma) {
      case  0: return 0;   // Cbb
      case  1: return 0;   // Cb
      case  2: return 0;   // C
      case  3: return 0;   // C#
      case  4: return 0;   // C##
      case  6: return 1;   // Dbb
      case  7: return 1;   // Db
      case  8: return 1;   // D
      case  9: return 1;   // D#
      case 10: return 1;   // D##
      case 12: return 2;   // Ebb
      case 13: return 2;   // Eb
      case 14: return 2;   // E
      case 15: return 2;   // E#
      case 16: return 2;   // E##
      case 17: return 3;   // Fbb
      case 18: return 3;   // Fb
      case 19: return 3;   // F
      case 20: return 3;   // F#
      case 21: return 3;   // F##
      case 23: return 4;   // Gbb
      case 24: return 4;   // Gb
      case 25: return 4;   // G
      case 26: return 4;   // G#
      case 27: return 4;   // G##
      case 29: return 5;   // Abb
      case 30: return 5;   // Ab
      case 31: return 5;   // A
      case 32: return 5;   // A#
      case 33: return 5;   // A##
      case 35: return 6;   // Bbb
      case 36: return 6;   // Bb
      case 37: return 6;   // B
      case 38: return 6;   // B#
      case 39: return 6;   // B##
   }
   return -1;
}



//////////////////////////////
//
// ScoreUtility::base40ToUCDiatonicLetter --  Convert a base-40 pitch 
//     into a Base-7 diatonic pitch letter name (upper case).
//     (losing any chromatic alterations of the diatonic pitch).
//

char ScoreUtility::base40ToUCDiatonicLetter(int base40) {
   int dpc = ScoreUtility::base40ToBase7PitchClass(base40);
   switch (dpc) {
      case 0: return 'C';
      case 1: return 'D';
      case 2: return 'E';
      case 3: return 'F';
      case 4: return 'G';
      case 5: return 'A';
      case 6: return 'B';
   }
   return 'X';
}



//////////////////////////////
//
// ScoreUtility::base40ToBase7 --  Convert a base-40 pitch to Base-7 
//     pitch (losing any chromatic alterations of the diatonic pitch.
//

int ScoreUtility::base40ToBase7(int base40) {
   int octave = base40 / 40;
   int dpc    = ScoreUtility::base40ToBase7PitchClass(base40);
   return dpc + octave * 7;
}



//////////////////////////////
//
// ScoreUtility::base40ToChromaticAlteration -- Extract chromatic
//     alteration of a diatonic note given a base-40 pitch number.
//     (maximum of two sharp/flats).
//

int ScoreUtility::base40ToChromaticAlteration(int base40) {
   int chroma = base40 % 40;
   switch (chroma) {
      case  0: return -2;          // Cbb
      case  1: return -1;          // Cb
      case  2: return  0;          // C
      case  3: return +1;          // C#
      case  4: return +2;          // C##
      case  6: return -2;          // Dbb
      case  7: return -1;          // Db
      case  8: return  0;          // D
      case  9: return +1;          // D#
      case 10: return +2;          // D##
      case 12: return -2;          // Ebb
      case 13: return -1;          // Eb
      case 14: return  0;          // E
      case 15: return +1;          // E#
      case 16: return +2;          // E##
      case 17: return -2;          // Fbb
      case 18: return -1;          // Fb
      case 19: return  0;          // F
      case 20: return +1;          // F#
      case 21: return +2;          // F##
      case 23: return -2;          // Gbb
      case 24: return -1;          // Gb
      case 25: return  0;          // G
      case 26: return +1;          // G#
      case 27: return +2;          // G##
      case 29: return -2;          // Abb
      case 30: return -1;          // Ab
      case 31: return  0;          // A
      case 32: return +1;          // A#
      case 33: return +2;          // A##
      case 35: return -2;          // Bbb
      case 36: return -1;          // Bb
      case 37: return  0;          // B
      case 38: return +1;          // B#
      case 39: return +2;          // B##
   }
   return 0;
}



//////////////////////////////
//
// ScoreUtility::base40ToKern --  Convert a base-40 pitch number into
//     a Humdrum **kern pitch name.  Example for middle C (C4):
//          base-40: 2 + 4 * 40 = 162 -> kern: "c"
//

string ScoreUtility::base40ToKern(int base40) {
   string chromastring;

   int octave = base40 / 40;
   if (octave > 12 || octave < -1) {
      cerr << "Error: unreasonable octave value: " << octave << endl;
      exit(1);
   }
   int chroma = base40 % 40;

   switch (chroma) {
      case  0: chromastring = "c--"; break;
      case  1: chromastring = "c-";  break;
      case  2: chromastring = "c";   break;
      case  3: chromastring = "c#";  break;
      case  4: chromastring = "c##"; break;
      //    5: not used
      case  6: chromastring = "d--"; break;
      case  7: chromastring = "d-";  break;
      case  8: chromastring = "d";   break;
      case  9: chromastring = "d#";  break;
      case 10: chromastring = "d##"; break;
      //   11: not used
      case 12: chromastring = "e--"; break;
      case 13: chromastring = "e-";  break;
      case 14: chromastring = "e";   break;
      case 15: chromastring = "e#";  break;
      case 16: chromastring = "e##"; break;
      case 17: chromastring = "f--"; break;
      case 18: chromastring = "f-";  break;
      case 19: chromastring = "f";   break;
      case 20: chromastring = "f#";  break;
      case 21: chromastring = "f##"; break;
      //   22: not used
      case 23: chromastring = "g--"; break;
      case 24: chromastring = "g-";  break;
      case 25: chromastring = "g";   break;
      case 26: chromastring = "g#";  break;
      case 27: chromastring = "g##"; break;
      //   28: not used
      case 29: chromastring = "a--"; break;
      case 30: chromastring = "a-";  break;
      case 31: chromastring = "a";   break;
      case 32: chromastring = "a#";  break;
      case 33: chromastring = "a##"; break;
      //   34: not used
      case 35: chromastring = "b--"; break;
      case 36: chromastring = "b-";  break;
      case 37: chromastring = "b";   break;
      case 38: chromastring = "b#";  break;
      case 39: chromastring = "b##"; break;
      default: chromastring = "X";
   }
   
   if (octave >= 4) {
      chromastring[0] = std::tolower(chromastring[0]);
   } else {
      chromastring[0] = std::toupper(chromastring[0]);
   }
   int repeat = 0;
   switch (octave) {
      case 4:  repeat = 0; break;
      case 5:  repeat = 1; break;
      case 6:  repeat = 2; break;
      case 7:  repeat = 3; break;
      case 8:  repeat = 4; break;
      case 9:  repeat = 5; break;
      case 3:  repeat = 0; break;
      case 2:  repeat = 1; break;
      case 1:  repeat = 2; break;
      case 0:  repeat = 3; break;
      case -1: repeat = 4; break;
      default:
         cerr << "Error: unknown octave value: " << octave << endl;
         cerr << "for base-40 pitch: " << base40 << endl;
         exit(1);
   }

   string output;
   for (int i=0; i<repeat; i++) {
      output += chromastring[0];
   }
   output += chromastring;

   return output;
}



