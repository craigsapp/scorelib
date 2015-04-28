//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Apr 25 12:44:02 PDT 2014
// Last Modified: Fri Apr 25 12:44:05 PDT 2014
// Filename:      ScoreUtility_compare.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_compare.cpp
// Syntax:        C++11
//
// Description:   Comparing two ScoreItems.
//

#include "ScoreUtility.h"
#include "ScoreItem.h"
#include <iostream>
#include <cmath>

using namespace std;


//////////////////////////////
//
// ScoreUtility::equalClefs -- Returns true if the two items represent
//     the same clef.  If either one of the items are NULL, then return
//     value will be false.
//
//

bool ScoreUtility::equalClefs(ScoreItem* a, ScoreItem* b) {
   if ((a == NULL) || (b == NULL)) {
      return false;
   }
   if (!a->isClefItem() || !b->isClefItem()) {
      return false;
   }

   int shapeA = a->getP5Int();
   int shapeB = b->getP5Int();
   SCORE_FLOAT fractionA = a->getP5() - shapeA;
   SCORE_FLOAT fractionB = b->getP5() - shapeB;

   if (shapeA != shapeB) {
      return false;
   }

   if (fractionA != fractionB) {
      return false;
   }

   if (fabs(a->getP4() - b->getP4()) > 0.1) {
      return false;
   }

   return true;
}

//
// Alias:
//

bool ScoreUtility::equalClef(ScoreItem* a, ScoreItem* b) {
   return ScoreUtility::equalClefs(a, b);
}



//////////////////////////////
//
// ScoreUtility::equalTimeSigs -- Returns true if the two items represent
//     the same time signature.  If either one of the items are NULL,
//     then return value will be false.
//
//

bool ScoreUtility::equalTimeSigs(ScoreItem* a, ScoreItem* b) {
   if ((a == NULL) || (b == NULL)) {
      return false;
   }
   if (!a->isTimeSignatureItem() || !b->isTimeSignatureItem()) {
      return false;
   }

   if (a->getP5Int() != b->getP5Int()) {
      // Top number in time signature
      return false;
   }
   if (a->getP5Int() != b->getP5Int()) {
      // Bottom number in time signature
      return false;
   }

   if (a->getP8Int() != b->getP8Int()) {
      // Secondary top number in time signature
      return false;
   }
   if (a->getP9Int() != b->getP9Int()) {
      // Secondary bottom number in time signature
      return false;
   }

   return true;
}


//
// Aliases:
//

bool ScoreUtility::equalTimeSig(ScoreItem* a, ScoreItem* b) {
   return ScoreUtility::equalTimeSigs(a, b);
}

bool ScoreUtility::equalTimeSignatures(ScoreItem* a, ScoreItem* b) {
   return ScoreUtility::equalTimeSigs(a, b);
}

bool ScoreUtility::equalTimeSignature(ScoreItem* a, ScoreItem* b) {
   return ScoreUtility::equalTimeSigs(a, b);
}



//////////////////////////////
//
// ScoreUtility::equalKeySigs -- True if the key signatures
//     have the same number of sharps or flats.
//


bool ScoreUtility::equalKeySigs(ScoreItem* a, ScoreItem* b) {
   if ((a == NULL) || (b == NULL)) {
      return false;
   }
   if (!a->isKeySignatureItem() || !b->isKeySignatureItem()) {
      return false;
   }

   if (a->getP5Int() != b->getP5Int()) {
      // Sharps/flats or cancellation signatures are not the same.
      return false;
   }

   return true;
}


bool ScoreUtility::equalKeySignatures(ScoreItem* a, ScoreItem* b) {
   return equalKeySignature(a, b);
}


bool ScoreUtility::equalKeySig(ScoreItem* a, ScoreItem* b) {
   return equalKeySignature(a, b);
}


bool ScoreUtility::equalKeySignature(ScoreItem* a, ScoreItem* b) {
   return equalKeySignature(a, b);
}



