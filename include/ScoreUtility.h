//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:11:15 PST 2014
// Last Modified: Sun Mar 16 23:03:37 PDT 2014
// Filename:      ScoreUtility.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreUtility.h
// Syntax:        C++11
//
// Description:   The ScoreUtility namespace holds various static functions
//                which process data realted to Score classes.
//

#ifndef _SCOREUTILITY_H_INCLUDED
#define _SCOREUTILITY_H_INCLUDED

#include "ScoreDefs.h"
#include "ScoreItemEdit_ParameterHistory.h"


namespace ScoreUtility {

   string  base40ToKern        (int base40);
   int     base7ToBase40       (int base7);

}  // end of namespace ScoreUtility

namespace SU = ScoreUtility;  // abbreviation

#endif  /* _SCOREUTILITYH_INCLUDED */



