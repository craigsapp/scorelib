//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 10 15:05:06 PDT 2014
// Last Modified: Thu Apr 10 15:05:10 PDT 2014
// Filename:      ScorePageOverlay.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageOverlay.h
// Syntax:        C++11
//
// Description:   This class holds multiple ScorePages which are intended
//                to be displayed on the same page.
//

#ifndef _SCOREPAGEOVERLAY_H_INCLUDED
#define _SCOREPAGEOVERLAY_H_INCLUDED

#include "ScorePage.h"
#include "Options.h"

using namespace std;
using listSPp = list<ScorePage*>;

#ifndef UseBoundVector
   using vectorSPp    = vector<ScorePage*>;
#else
   #include "BoundVector.h"
   using vectorSPp    = BoundVector<ScorePage*>;
#endif


class ScorePageOverlay { 
   public:
                  ScorePageOverlay              (void);
                 ~ScorePageOverlay              ();
      void        clear                         (void);
      ScorePage*  createOverlay                 (void);
      int         getOverlayCount               (void);
      int         size                          (void);
      ScorePage&  operator[]                    (int oindex);
      ScorePage*  getPage                       (int oindex);
      int         appendOverlay                 (ScorePage* page);

   protected:
      // overlay_storage contains all of the data for SCORE page overlay.
      // This is the primary storage for page data, and it must
      // be deleted when an object is deconstructed.
      listSPp overlay_storage;

      // overlay_sequence contains an ordered set of score pages
      // which are stored in overlay_storage.
      vectorSPp overlay_sequence;
 
};


ostream& operator<<(ostream& out, ScorePageOverlay& overlay);


#endif /* _SCOREPAGEOVERLAY_H_INCLUDED */



