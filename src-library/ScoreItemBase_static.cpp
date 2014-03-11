//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 23:54:30 PDT 2014
// Last Modified: Mon Mar 10 23:54:36 PDT 2014
// Filename:      ScoreItemBase_static.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItemBase_static.cpp
// Syntax:        C++11
//
// Description:   This file contains static functions useful for processing ScoreItem
//                data.
//

#include "ScoreItemBase.h"
#include <algorithm>
#include <set>

using namespace std;


//////////////////////////////
//
// ScoreItemBase::base7ToBase40 -- convert diatonic Base-7 note to Base-40 pitch.
//

int ScoreItemBase::base7ToBase40(int base7) {
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
// ScoreItemBase::base40ToKern --  Convert a base-40 pitch number into
//     a Humdrum **kern pitch name.
//

string ScoreItemBase::base40ToKern(int base40) {
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


