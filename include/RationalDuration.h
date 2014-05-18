//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar 28 22:32:34 PDT 2014
// Last Modified: Fri Mar 28 22:32:36 PDT 2014
// Filename:      RationalDuration.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/RationalDuration.h
// Syntax:        C++11
//
// Description:   Class which handles rational durations.  In other words,
//                durations in terms of integer rations.  This class also
//                keeps track of augmentation dots.  The first augmentation
//                dot adds 1/2 of the primary duration, the second adds 1/4
//                of the primary duration, and so on.  The primary duration
//                is stored as a RationalNumber.
//

#ifndef _RATIONALDURATION_H_INCLUDED
#define _RATIONALDURATION_H_INCLUDED

#include "RationalNumber.h"
#include <vector>

using namespace std;

class RationalDuration {
   public:
                            RationalDuration   (void);

                            RationalDuration   (const RationalNumber& rn,
                                                int dcount = 0);
                            RationalDuration   (int numerator, int denominator,
                                                int dcount = 0);
                            RationalDuration   (double duration,
                                                int dcount = 0);
                           ~RationalDuration   ();

      RationalDuration&     operator=          (const RationalDuration& value);
      RationalDuration&     operator=          (const RationalNumber& value);
      RationalDuration&     operator+=         (RationalDuration& value);
      RationalDuration&     operator+=         (RationalNumber& value);
      RationalDuration      operator+          (RationalDuration& value);
      RationalDuration      operator+          (RationalNumber& value);
      RationalDuration      operator-          (RationalDuration& value);
      RationalDuration      operator-          (void);

      void                  clear              (void);
      void                  zero               (void);
      void                  negate             (void);
      int                   getDotCount        (void);
      void                  setDotCount        (int dcount);
      int                   getNumerator       (void);
      int                   getDenominator     (void);
      const RationalNumber& getDurationPrimary (void);
      RationalNumber        getDurationReduced (void);
      double                getDurationDouble  (void);
      void                  makeReduced        (void);
      void                  setDuration        (const RationalNumber& rn, 
                                                int dcount = 0);
      void                  setDuration        (int numerator, int denominator, 
                                                int dcount = 0);
      void                  setRhythm          (int denominator, 
                                                int numerator, int dcount = 0);
      void                  setRhythm          (int denominator, 
                                                int dcount = 0);
      void                  setDuration        (double duration, 
                                                int dcount = 0);
      void                  setDurationWholeNoteUnits(double duration, 
                                                int dcount = 0);
      void                  setDurationQuarterNoteUnits(double duration, 
                                                int dcount = 0);
      int                   isPowerOfTwo       (void);

      // printing functions
      ostream&              print              (ostream& out = cout);
      ostream&              printReduced       (ostream& out = cout);
      ostream&              printHumdrum       (ostream& out = cout);

      void                  setValue           (int top, int bottom);

   private:

      // storage for the primary rational duration
      RationalNumber         primaryvalue;

      // number of dots to apply to the primary duration.  First dot
      // adds 1/2 of primary value, second dot adds 1/4, third adds 1.8, etc.
      int                    dotcount;

      // tuplet factors applied to primaryvalue.  Divide by these
      // factors to get the power of two value.
      vector<RationalNumber> tupletfactors;

   protected:
       int                   powerOfTwoDuration (RationalDuration& rd, 
                                                 double basedur);
       int                   ceilingPowerOfTwo  (double value);

};


ostream& operator<<(ostream& out, RationalDuration rd);



#endif  /* _RATIONALDURATION_H_INCLUDED */


