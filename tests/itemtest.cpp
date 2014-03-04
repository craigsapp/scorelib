//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 23:34:34 PST 2014
// Last Modified: Tue Mar  4 00:36:34 PST 2014
// Filename:      itemtest.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/itemtest.cpp
// Syntax:        C++11
//
// Description:   Test of manual creation of SCORE items, both fixed parameters
// 		  and named parameters.  Output is printed as PMX data and XML
// 		  data (without wrapping structure).  This program also checks
// 		  storage of parameter change history.
//
// $Smake: g++ -std=c++11 -DSCOREITEMEDIT -I../include -L../lib -lscoreeditlib -o %b %f && strip %b
//

#include "ScoreItem.h"

using namespace std;

int main(void) {
   ScoreItem item;

   item.setParameter(1, 16.0);
   item.setParameter(2, 1.0);
   item.setParameter(3, 85.0);
   item.setParameter(3, 7.0);
   item.setParameter(10, 10.0);
   item.setParameter("pitch", "b3");
   item.setParameter("analysis", "pitch", "c5");
   item.setFixedText("baristarota");

   item.startHistory();
   item.setParameter(3, 15.0);
   item.setParameter("pitch", "c4");
   item.setParameter("analysis", "pitch", "d4");
   item.deleteNamespace("analysis");
   item.setParameter("rubber", "green", "red");
   item.deleteNamespace("atesa");

   item.printPmxWithEdits(cout);
   cout << "\n\n";
   item.printXmlWithEdits(cout);

   return 0;
}


