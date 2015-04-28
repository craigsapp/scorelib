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
#include <cmath>

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

RationalDuration& RationalDuration::operator=(const RationalDuration& value) {
   if (this != &value) {
      primaryvalue  = value.primaryvalue;
      dotcount      = value.dotcount;
      tupletfactors = value.tupletfactors;
   }
   return *this;
}



RationalDuration& RationalDuration::operator=(const RationalNumber& value) {
   primaryvalue = value;
   dotcount     = 0;
   tupletfactors.clear();
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
      // handle tuplet adding later here
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
   tupletfactors.clear();
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
//    default value: dcount = 0
//

void RationalDuration::setDuration(const RationalNumber& rn, int dcount) {
   primaryvalue = rn;
   dotcount = dcount;
}


void RationalDuration::setDuration(int numerator, int denominator, int dcount) {
   primaryvalue.setValue(numerator, denominator);
   dotcount = dcount;
}



//////////////////////////////
//
// RationalDuration::setRhythm --
//    default value: dcount = 0
//

void RationalDuration::setRhythm(int denominator, int numerator, int dcount) {
   setDuration(numerator, denominator, dcount);
}


void RationalDuration::setRhythm(int denominator, int dcount) {
   setDuration(1, denominator, dcount);
}



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
//     format.  Durations in **recip are in units of whole notes,
//     while those of RationalDuration are quarter notes, so
//     multiply by 4 when converting.  Examples:
//           quarter note duration     1     => 4
//           double dotted eighth note 1/2.. => 8..
//           triplet whole note        8/3   => 3%2
//           dotted breve              8/3.  => 3%4.
//

ostream& RationalDuration::printHumdrum(ostream& out) {
   if (primaryvalue <= 0) {
      out << 'g';
      return out;
   }
   RationalNumber rn = primaryvalue / 4;
   out << rn.getDenominator();
   if (rn.getNumerator() != 1) {
      out << '%' << rn.getNumerator();
   }
   for (int i=0; i<dotcount; i++) {
      out << '.';
   }
   return out;
}



//////////////////////////////
//
// RationalDuration::setDurationQuarterNoteUnits -- This is not a closed
//    solution.  Converting from a double to a rational duration will not
//    be possible for all inputs, and there may be multiple solutions to
//    the same input.
//

void RationalDuration::setDuration(double duration, int dcount) {
   setDurationQuarterNoteUnits(duration, dcount);
}


void RationalDuration::setDurationWholeNoteUnits(double duration, int dcount) {
   setDurationQuarterNoteUnits(duration / 4.0);
}


void RationalDuration::setDurationQuarterNoteUnits(double duration,
      int dcount) {

   double threshold = 0.00001;

   // Ignore negative durations:
   if (duration <= 0.0) {
      zero();
      return;
   }

   // limit dot counts from 0 to 16:
   if ((dcount > 16) || (dcount < 0)) {
      dcount = 0;
   }

   // Remove dots from duration:
   //
   // # dots    adding factor                removing factor
   // 0 dots    x * (1)             = X      X * 1/1  = x
   // 1 dot     x * (1+1/2)         = X      X * 2/3  = x
   // 2 dots    x * (1+1/2+1/4)     = X      X * 4/7  = x
   // 3 dots    x * (1+1/2+1/4+1/8) = X      X * 8/15 = x
   //
   // General function:
   // F(dots) = 2^dots / (2^(dots+1) - 1)
   //
   double basedur     = duration;
   double undotfactor = 1.0;
   if (dcount > 0) {
      undotfactor = ((double)(1 << dcount))/(double)(((1<<(dcount+1))-1));
      basedur = duration * undotfactor;
   }

   RationalDuration testrd;

   // 1. Check to see if the duration consists of only
   // a power of two (untupleted rhythmic value):
   if (powerOfTwoDuration(testrd, basedur)) {
      *this = testrd;
      dotcount = dcount;
      return;
   }

   // 2. Check to see if it is a simple tuplet where there is
   // only one non-power of two factor involved in the denominator
   // of the ratio, along with a power of two in the numerator
   // of the ratio.  At the moment, only checking tuplets at
   // 2^0 and less.

   int ceilingexp = ceilingPowerOfTwo(basedur);
   double ceiling = 1.0;
   if (ceilingexp < 0) {
      ceiling = 1.0 / (1 << (-ceilingexp));
   } else {
      ceiling = 1 << ceilingexp;
   }
   double testval = ceiling / basedur;

   // only checking exponents 2^0 to 2^-7:
   double rtol = 0.007;
   double result;
   for (int i=0; i<=8; i++) {
      result = testval * (1 << i);
      if ((fabs(result - int(result + rtol))) < rtol) {
         // found the tuplet
         int bottom = int(result + rtol);
         int top    = 1;
         int topexp = i + ceilingexp;
         if (topexp > 0) {
            top = (1 << topexp);
         } else if (topexp < 0) {
            bottom *= (1 << (-topexp));
         }
         dotcount = dcount;
         setValue(top, bottom);
         // set tupletfactors later...
         tupletfactors.clear();
         return;
      }
   }


   // 3. At this point the duration is not a power of two, and it is not
   // a simple tuplet of a power of two.   It is a rational duration which
   // consists of a tuplet ratio not based on a power of two, such as five
   // quarter notes in the time of three (a 5:3 tuplet, where the second
   // number is not a power of two).  Figure out how to calculate this sort
   // of case here...


   // 3b. Another possibility is that dcount is invalid.  In some music
   // (particularly Baroque), the dot does not have an exact arithmetic
   // interpretation.  Another example are the augmentation dots in Chopin's
   // Op. 28, No. 1 prelude.

   if (fabs(duration - 6.0) < threshold) {
      // dot is missing from note.  Add one for a dotted whole note
      setDuration(4, 1, 1);
      return;
   }


   // 4. Give up: don't know what the duration is, so set to -1
   zero();
cerr << "UNKNOWN DURATION: " << duration << endl;
exit(1);
   primaryvalue = -1;
}



//////////////////////////////
//
// RationalDuration::setValue --
//

void RationalDuration::setValue(int top, int bottom) {
   primaryvalue.setValue(top, bottom);
}



//////////////////////////////
//
// RationalDuration::powerOfTwoDuration --
//

int RationalDuration::powerOfTwoDuration(RationalDuration& rd, double basedur) {
   rd.clear();
   // exponent tolerance
   double etol = 0.003;

   double exponent = log(basedur)/log(2.0);
   int powtwo      = (int)(fabs(exponent));
   double fraction = (fabs(exponent) - powtwo);
   if (fraction > (1.0 - etol)) {
      powtwo++;
      fraction = 0.0;
   } else if (fraction < etol) {
      fraction = 0.0;
   }
   if (fraction == 0.0) {
      // found a power of two duration, so store it and exit
      if (exponent >= 0) {
         rd.setValue(1<<powtwo, 1);
      } else {
         rd.setValue(1, 1<<powtwo);
      }
      return 1;
   }

   rd.setValue(-1, 1);
   return 0;
}



//////////////////////////////
//
// RationalDuration::ceilingPowerOfTwo -- return the next highest
//      power of two for the given positive input value.
//

int RationalDuration::ceilingPowerOfTwo(double value) {
   if (value <= 0.0) {
      cerr << "ERORR: Input must be positive: " << value << endl;
      exit(1);
   }
   double exp = log(value)/log(2.0);
   return ceil(exp-0.0001);
}



//////////////////////////////
//
// RationalDuration::isPowerOfTwo -- True if the duration excluding augmentation
//     dots is a power of two which means it is not a tuple (in general).
//

int RationalDuration::isPowerOfTwo(void) {
   RationalNumber rn = getDurationPrimary();
   double pow2 = log(rn.getFloat())/log(2.0);
   if (pow2 < 0) {
      pow2 = -pow2;
   }
   if (fabs(pow2 - int(pow2+0.0001)) < 0.0001) {
      return 1;
   }
   return 0;
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& out, RationalDuration rd) {
   return rd.print(out);
}


