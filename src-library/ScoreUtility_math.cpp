//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 02:51:26 PST 2014
// Filename:      ScoreUtility_math.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_math.cpp
// Syntax:        C++11
//
// Description:   General math calculations.
//

#include "ScoreUtility.h"
#include <iostream>

using namespace std;


//////////////////////////////
//
// ScoreUtility::lcm -- Find the least common multiple of two integers.
//

int ScoreUtility::lcm(int x, int y) {
   int gcd_val = ScoreUtility::gcd(x, y);
   int prod    = x * y;
   if (prod < 0) {
      prod = -prod;
   }
   return prod / gcd_val;
}

//
// LCM for a list of numbers
//

int ScoreUtility::lcm(set<int>& numbers) {
   vectorI nums;
   nums.reserve(numbers.size());
   for (auto& it : numbers) {
      nums.push_back(it);
   }
   int output = nums[0];
   for (int i=1; i<nums.size(); i++) {
      output = output * nums[i] / ScoreUtility::gcd(output, nums[i]);
   }
   return output;
}



//////////////////////////////
//
// ScoreUtility::gcd -- Find the greatest common denominator between
//     two integers.
//

int ScoreUtility::gcd(int x, int y) {
   if (y == 0) {
      return x;
   }
   int z = x % y;
   x = y;
   y = z;
   int output = ScoreUtility::gcd(x, y);
   return output;
}



