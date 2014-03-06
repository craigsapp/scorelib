//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Feb 27 23:59:05 PST 2014
// Last Modified: Thu Feb 27 23:59:07 PST 2014
// Filename:      ScoreItem_sort.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_sort.cpp
// Syntax:        C++11
//
// Description:   ScoreItem sorting functions.
//

#include "ScoreItem.h"

using namespace std;



//////////////////////////////
//
// sortP3 -- sort by horizontal position/item type/vertical position.
//

bool sortP3(ScoreItem* a, ScoreItem* b) {
   return a->getHorizontalPosition() < b->getHorizontalPosition();
}



//////////////////////////////
//
// sortP3P1P4 -- sort by horizontal position/item type/vertical position.
//      P4 values are limited to +/- 99 since the 100's digit indicates
//      grace notes.
//

bool sortP3P1P4(ScoreItem* a, ScoreItem* b) {
   SCORE_FLOAT a_test = a->getHorizontalPosition();
   SCORE_FLOAT b_test = b->getHorizontalPosition();

   if (a_test < b_test) {
      return true;
   } else if (a_test > b_test) {
      return false;
   }

   // vertical positions are the same so try to break tie by 
   // item type.

   a_test = a->getPInt(P1);
   b_test = b->getPInt(P1);

   if (a_test < b_test) {
      return true;
   } else if (a_test > b_test) {
      return false;
   } 

   // horizontal positions are the same so try to break tie by 
   // vertical position.

   a_test = a->getVerticalPosition();
   b_test = b->getVerticalPosition();

   if (a_test < b_test) {
      return true;
   } else if (a_test > b_test) {
      return false;
   }

   // The two objects are tied in sort order.
   return false;
}



//////////////////////////////
//
// sortP3P4P1 -- sort by horizontal position/vertical position/item type.  
//      P4 values are limited to +/- 99 since the 100's digit indicates
//      grace notes.
//

bool sortP3P4P1(ScoreItem* a, ScoreItem* b) {
   SCORE_FLOAT a_test = a->getHorizontalPosition();
   SCORE_FLOAT b_test = b->getHorizontalPosition();

   if (a_test < b_test) {
      return true;
   } else if (a_test > b_test) {
      return false;
   }

   // horizontal positions are the same so try to break tie by 
   // vertical position.

   a_test = a->getVerticalPosition();
   b_test = b->getVerticalPosition();

   if (a_test < b_test) {
      return true;
   } else if (a_test > b_test) {
      return false;
   }

   // vertical positions are the same so try to break tie by 
   // item type.

   a_test = a->getPInt(P1);
   b_test = b->getPInt(P1);

   return a_test < b_test;
}




