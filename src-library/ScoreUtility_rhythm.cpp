//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Jun  5 09:18:22 PDT 2015
// Last Modified: Fri Jun  5 09:18:25 PDT 2015
// Filename:      ScoreUtility_rhythm.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_rhythm.cpp
// Syntax:        C++11
//
// Description:   General rhythm functions.
//

#include "ScoreUtility.h"
#include <iostream>

using namespace std;


//////////////////////////////
//
// ScoreUtility::increaseDurationPrecision -- Give more precision to 
//   tuplet durations.  Won't work for tuplets containing large prime
//   numbers.
//

double ScoreUtility::increaseDurationPrecision(double duration) {
   return duration;
   int dint = (int)duration;
   double fraction = duration - dint;
   int firstthree = (int)(fraction * 1000.0 + 0.5);
   switch (firstthree) {
      case   0: return dint;
      case 500: return dint + 1.0/2.0;
      case 250: return dint + 1.0/4.0;
      case 750: return dint + 3.0/4.0;
      case 333: return dint + 1.0/3.0;
      case 667: return dint + 2.0/3.0;
      case 666: return dint + 2.0/3.0;
      case 167: return dint + 1.0/6.0;
      case 166: return dint + 1.0/6.0;
      case 833: return dint + 5.0/6.0;
      case 143: return dint + 1.0/7.0;
      case 286: return dint + 2.0/7.0;
      case 429: return dint + 3.0/7.0;
      case 571: return dint + 4.0/7.0;
      case 714: return dint + 5.0/7.0;
      case 857: return dint + 6.0/7.0;
      case 111: return dint + 1.0/9.0;
      case 222: return dint + 2.0/9.0;
      case 444: return dint + 4.0/9.0;
      case 556: return dint + 5.0/9.0;
      case 778: return dint + 7.0/9.0;
      case 888: return dint + 8.0/9.0;
      case 999: return dint + 1.0;
   }
   if (fraction > 0.999) {
      return dint + 1.0;
   }
   if (fraction < 0.001) {
      return (double)dint;
   }
   return duration;
}



