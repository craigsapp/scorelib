//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr  8 15:03:08 PDT 2014
// Last Modified: Tue Apr  8 15:30:22 PDT 2014
// Filename:      ScorePageSet_write.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_write.cpp
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#include "ScorePageSet.h"

using namespace std;

#include <typeinfo>

//////////////////////////////
//
// operator<< -- Print a ScorePageSet as multi-page PMX data.
//

ostream& operator<<(ostream& out, ScorePageSet& set) {
   int i;
   for (i=0; i<set.getPageCount(); i++) {
      if ((i > 0) && !set[i-1][0].isMultipageAsRs()) {
         out << "\n";
      }
      out << set[i];
   }
   return out;
}



//////////////////////////////
//
// printNoAuto -- Print a ScorePageSet as multi-page PMX data, but do not
//    print any "auto" namespace named paramters.
//

ostream& printNoAuto(ostream& out, ScorePageSet& set) {
   int i;
   for (i=0; i<set.getPageCount(); i++) {
      if ((i > 0) && !set[i-1][0].isMultipageAsRs()) {
         out << "\n";
      }
      printNoAuto(out, set[i]);
   }
   return out;
}



