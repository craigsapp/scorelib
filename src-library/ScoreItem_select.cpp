//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Mar  5 17:31:57 PST 2014
// Last Modified: Wed Mar  5 17:32:01 PST 2014
// Filename:      ScoreItem_select.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_select.cpp
// Syntax:        C++11
//
// Description:   ScoreItem sorting functions.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// selectDurationItems -- select Staff items which posses durational quality.
//     This include P1=1 (notes) and P2=2 (rests).  Returns the number of
//     items that were extracted.
//

int selectDurationItems(vectorSIp& output, const vectorSIp& input) {
   return selectP1Items(output, input, {1, 2});
}



//////////////////////////////
//
// selectP1Items -- return all items in the input list which match the
//

int selectP1Items(vectorSIp& output, const vectorSIp& input, 
      const vectorI& p1values) {
   output.clear();
   output.reserve(input.size());
   for (auto it = input.begin(); it != input.end(); it++) {
      for (auto it2 = p1values.begin(); it2 != p1values.end(); it2++) {
         if ((*it)->getPInt(P1) == *it2) {
            output.push_back(*it);
         }
      }
   }
   return output.size();
}




