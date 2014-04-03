//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Feb 27 23:59:05 PST 2014
// Last Modified: Thu Feb 27 23:59:07 PST 2014
// Filename:      ScoreUtility_sort.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreUtility_sort.cpp
// Syntax:        C++11
//
// Description:   ScoreUtility sorting functions.
//

#include "ScoreUtility.h"
#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreUtility::sortP3 -- sort by horizontal position.
//

bool ScoreUtility::sortP3(ScoreItem* a, ScoreItem* b) {
   return a->getHorizontalPosition() < b->getHorizontalPosition();
}



//////////////////////////////
//
// ScoreUtility::sortP3P2 -- sort by horizontal position/staff number.
//

bool ScoreUtility::sortP3P2(ScoreItem* a, ScoreItem* b) {
   if (a->getHorizontalPosition() < b->getHorizontalPosition()) {
      return true;
   }

   if (a->getStaffNumber() < b->getStaffNumber()) {
      return true;
   }

   return false;
}



//////////////////////////////
//
// ScoreUtility::sortP3P2P1 -- sort by P3, P2, then P1.
//

bool ScoreUtility::sortP3P2P1(ScoreItem* a, ScoreItem* b) {
   // Compare P3 (horizontal position):
   if (a->getHorizontalPosition() < b->getHorizontalPosition()) {
      return true;
   }

   // Compare P2 values (int part only):
   if (a->getStaffNumber() < b->getStaffNumber()) {
      return true;
   }

   // Compare P1 values (int part only):
   if (a->getItemType() < b->getItemType()) {
      return true;
   }

   return false;
}



//////////////////////////////
//
// ScoreUtility::sortP3P2P1P4 -- sort by P3, P2, P1, then P4
//

bool ScoreUtility::sortP3P2P1P4(ScoreItem* a, ScoreItem* b) {

if ((unsigned long)a < 500) {
cerr << "ERROR A is too small" << endl;
return true;
}

if ((unsigned long)b < 500) {
cerr << "ERROR B is too small" << endl;
return true;
}

   // Compare P3 (horizontal position):
   if (a->getHorizontalPosition() < b->getHorizontalPosition()) {
      return true;
   }

   // Compare P2 values (int part only):
   if (a->getStaffNumber() < b->getStaffNumber()) {
      return true;
   }

   // Compare P4 (excluding any values abouve fabs(99));
   if (a->getVerticalPosition() < b->getVerticalPosition()) {
      return true;
   } 

   // Compare P1 values (int part only):
   if (a->getItemType() < b->getItemType()) {
      return true;
   }

   return false;
}



//////////////////////////////
//
// ScoreUtility::sortP3P1P4 -- sort by horizontal position/item type/vertical 
//      position.  P4 values are limited to +/- 99 since the 100's digit 
//      indicates grace notes.
//

bool ScoreUtility::sortP3P1P4(ScoreItem* a, ScoreItem* b) {
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
// ScoreUtility::sortP3P4P1 -- sort by horizontal position/vertical 
//      position/item type.  P4 values are limited to +/- 99 since 
//      the 100's digit indicates grace notes.
//

bool ScoreUtility::sortP3P4P1(ScoreItem* a, ScoreItem* b) {
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



