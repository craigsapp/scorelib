//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar 28 22:53:02 PDT 2014
// Last Modified: Fri Mar 28 22:53:06 PDT 2014
// Filename:      RationalDuration.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/RationalDuration.cpp
// Syntax:        C++11
//
// Description:   Class which handles rational durations.  In other words,
//                durations in terms of integer rations.  This class also
//                keeps track of augmentation dots.  The first augmentation
//                dot adds 1/2 of the primary duration, the second adds 1/4
//                of the primary duration, and so on.  The primary duration
//                is stored as a RationalNumber.
//

#include "RationalDuration.h"


//////////////////////////////
//
// RationalDuration::RationalDuration --
//
RationalDuration::RationalDuration(void) { 
   clear();
}



//////////////////////////////
//
// RationalDuration::RationalDuration -- Constructor.
//

RationalDuration::RationalDuration(const RationalNumber& rn, int dcount) { 
   setDuration(rn, dcount);
}

RationalDuration::RationalDuration(int numerator, int denominator, int dcount) { 
   setDuration(numerator, denominator, dcount);
}

RationalDuration::RationalDuration(double duration, int dcount) { 
   setDuration(duration, dcount);
}



//////////////////////////////
//
// RationalDuration::~RationalDuration -- Deconstructor.
//

RationalDuration::~RationalDuration() { 
   clear();
}



//////////////////////////////
//
// RationalDuration::operator= -- Copy content from another duration.
//

RationalDuration& RationalDuration::operator=(RationalDuration& value) { 
   if (this != &value) {
      primaryvalue = value.primaryvalue;
      dotcount     = value.dotcount;
   }
   return *this;
}


RationalDuration& RationalDuration::operator=(RationalNumber& value) { 
   primaryvalue = value;
   dotcount     = 0;
   return *this;
}



//////////////////////////////
//
// RationalDuration::operator+= -- Add two durations together.
//

RationalDuration& RationalDuration::operator+=(RationalDuration& value) { 
   if (this == &value) {
      primaryvalue *= 2;
      return *this;
   }
   if (dotcount == value.dotcount) {
      primaryvalue += value.primaryvalue;
      return *this;
   } else {
      // don't know how to add durations with different dot counts, so
      // reduce and add:
      this->makeReduced();
      primaryvalue += value.getDurationReduced();
      return *this;
   }
}


RationalDuration& RationalDuration::operator+=(RationalNumber& value) { 
   if (dotcount == 0) {
      primaryvalue += value;
   } else {
      // don't know how to add durations with different dot counts, so
      // reduce and add:
      this->makeReduced();
      primaryvalue += value;
   }
   return *this;
}



//////////////////////////////
//
// RationalDuration::operator+ -- Add two durations together
//

RationalDuration RationalDuration::operator+(RationalDuration& value) { 
   RationalDuration rd = *this;
   rd += value;
   return rd;
}


RationalDuration RationalDuration::operator+(RationalNumber& value) { 
   RationalDuration rd = *this;
   rd += value;
   return rd;
}



//////////////////////////////
//
// RationalDuration::operator- --
//

RationalDuration RationalDuration::operator-(RationalDuration& value) { 
   RationalDuration rd = -value;
   rd += *this;
   return rd;
}

// Unary - operator:

RationalDuration RationalDuration::operator-(void) { 
   RationalDuration rd = *this;
   rd.negate();
   return rd;
}



//////////////////////////////
//
// RationalDuration::clear --
//

void RationalDuration::clear(void) { 
   zero();
}



//////////////////////////////
//
// RationalDuration::zero --
//

void RationalDuration::zero(void) { 
   primaryvalue = 0;
   dotcount     = 0;
}



//////////////////////////////
//
// RationalDuration::negate --
//

void RationalDuration::negate(void) { 
   primaryvalue.setNumerator(-primaryvalue.getNumerator());
}



//////////////////////////////
//
// RationalDuration::getDotCount --
//

int RationalDuration::getDotCount(void) {
   return dotcount;
}



//////////////////////////////
//
// RationalDuration::setDotCount --
//

void RationalDuration::setDotCount(int dcount) { 
   if (dcount > 0) {
      dotcount = dcount;
   } else {
      dcount = 0;
   }
}



//////////////////////////////
//
// RationalDuration::getDurationPrimary --
//

const RationalNumber& RationalDuration::getDurationPrimary(void) { 
   return primaryvalue;
}



//////////////////////////////
//
// RationalDuration::getDurationReduced --
//

RationalNumber RationalDuration::getDurationReduced(void) {
   RationalNumber rn = primaryvalue;
   int twopow = 1;
   for (int i=0; i<dotcount; i++) {
      twopow <<= 1;
      rn += primaryvalue / twopow;
   }
   return rn;
}



//////////////////////////////
//
// RationalDuration::makeReduced -- Apply augmentation dots to duration
//     and remove them.
//

void RationalDuration::makeReduced(void) {
   RationalNumber rn = getDurationReduced();
   primaryvalue = rn;
   dotcount = 0;
}



//////////////////////////////
//
// RationalDuration::getDurationDouble --
//

double RationalDuration::getDurationDouble(void) { 
   RationalDuration rd = getDurationReduced();
   double output = rd.getNumerator() / rd.getDenominator();
   return output;
}



//////////////////////////////
//
// RationalDuration::setDuration --
//

void RationalDuration::setDuration(const RationalNumber& rn, int dcount) { }

void RationalDuration::setDuration(int numerator, int denominator, int dcount) { }



//////////////////////////////
//
// RationalDuration::setRhythm --
//

void RationalDuration::setRhythm(int denominator, int numerator, int dcount) { }

void RationalDuration::setRhythm(int denominator, int dcount) { }



//////////////////////////////
//
// RationalDuration::setDuration --
//

void RationalDuration::setDuration(double duration, int dcount) { }




//////////////////////////////
//
// RationalDuration::getNumerator --  Return the top value of the 
//     primary rational duration value.
//

int RationalDuration::getNumerator(void) { 
   return primaryvalue.getNumerator();
}



//////////////////////////////
//
// RationalDuration::getDenominator --  Return the bottom value of the 
//     primary rational duration value.
//

int RationalDuration::getDenominator(void) {
   return primaryvalue.getDenominator();
}



//////////////////////////////
//
// RationalDuration::print -- Examples:
//      quarter note:        1/4
//      whole note:          1
//      breve note:          2
//      triplet breve notes: 4/3
//      dotted half note:    1/2.
//

ostream& RationalDuration::print(ostream& out) { 
   out << primaryvalue.getNumerator();
   if ((primaryvalue.getDenominator() != 0) &&
       (primaryvalue.getDenominator() != 1)) {
      out << '/';
      out << primaryvalue.getDenominator();
   }
   for (int i=0; i<dotcount; i++) {
      out << '.';
   }
   return out;
}



//////////////////////////////
//
// RationalDuration::printReduced --
//

ostream& RationalDuration::printReduced(ostream& out) { 
   out << getDurationReduced();
   return out;
}



//////////////////////////////
//
// RationalDuration::printHumdrum -- Print as Humdrum **recip rhythm
//     format.  Examples: 
//           quarter note duration     1/4   => 4 
//           double dotted eighth note 1/8.. => 8..
//           triplet whole note        2/3   => 3%2
//           dotted breve              4/3.  => 3%4.
//

ostream& RationalDuration::printHumdrum(ostream& out) { 
   if (primaryvalue <= 0) {
      out << 'g';
      return out;
   }
   out << primaryvalue.getDenominator();
   if (primaryvalue.getNumerator() != 1) { 
      out << '%' << primaryvalue.getNumerator();
   }
   for (int i=0; i<dotcount; i++) {
      out << '.';
   }
   return out;
}



