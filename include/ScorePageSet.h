//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 03:18:48 PST 2014
// Last Modified: Sat Mar  1 03:18:50 PST 2014
// Filename:      ScorePageSet.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageSet.h
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#ifndef _SCOREPAGESET_H_INCLUDED
#define _SCOREPAGESET_H_INCLUDED

#include "ScorePage.h"

using namespace std;

typedef vector<ScorePage*> vectorSPp;

class ScorePageSet { 
   public:
               ScorePageSet      (void);
              ~ScorePageSet      ();
      void     clear             (void);


   protected:
      // page_storage contains all of the data for SCORE pages.
      // This is the primary storage for page data, and it must
      // be deleted when an object is deconstructed.
      listSIp page_storage;

      // page_sequence is the order in which the pages in page_storage
      // should be arranged.
      vectorSPp page_sequence;

};



#endif /* _SCOREPAGESET_H_INCLUDED */

