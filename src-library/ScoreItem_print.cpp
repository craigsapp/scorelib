//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Thu Feb  6 01:12:14 PST 2014
// Filename:      ScoreItem_print.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_print.cpp
// Syntax:        C++11
//
// Description:   ostream functions for printing ASCII versions of 
//                ScoreItem.
//

#include "ScoreItem.h"

using namespace std;



//////////////////////////////
//
// ScoreItem::printNoAnalysis -- print a ScoreItem, excluding any 
//      named parameters in the "analysis" namespace.
//

ostream& ScoreItem::printNoAnalysis(ostream& out) {
   auto& si = *this;
   si.printPmxFixedParameters(out);
   si.printPmxNamedParametersNoAnalysis(out);
   return out;
}



//////////////////////////////
//
// printNoAnalysis -- print a list of ScoreItems, excluding any 
//      named parameters in the "analysis" namespace.
//

ostream& printNoAnalysis(ostream& out, vectorSIp& sipvector) {
   for (auto& it : sipvector) {
      it->printNoAnalysis(out);
   }
   return out;
}


ostream& printNoAnalysis(ostream& out, listSIp& siplist) {
   for (auto& it : siplist) {
      it->printNoAnalysis(out);
   }
   return out;
}


ostream& printNoAnalysis(ostream& out, vectorVSIp& sipvvector) {
   for (auto& it : sipvvector) {
      printNoAnalysis(out, it);
   }
   return out;
}



//////////////////////////////
//
// operator<< -- Printing ScoreItems with <<
//

ostream& operator<<(ostream& out, ScoreItem& si) {
   si.printPmxFixedParameters(out);
   si.printPmxNamedParameters(out);
   return out;
}


ostream& operator<<(ostream& out, ScoreItem* si) {
   si->printPmxFixedParameters(out);
   si->printPmxNamedParameters(out);
   return out;
}


ostream& operator<<(ostream& out, vectorSIp& sipvector) {
   for (auto& it : sipvector) {
      out << it;
   }
   return out;
}


ostream& operator<<(ostream& out, listSIp& siplist) {
   for (auto& it : siplist) {
      out << it;
   }
   return out;
}


ostream& operator<<(ostream& out, vectorVSIp& sipvvector) {
   for (auto& itvv : sipvvector) {
      for (auto& itv : itvv) {
         out << itv;
      }
   }
   return out;
}



