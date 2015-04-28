//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 13:07:50 PDT 2014
// Last Modified: Mon Mar 10 13:07:53 PDT 2014
// Filename:      DatabaseBeam.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseBeam.h
// Syntax:        C++11
//
// Description:   Keeps tracks of beam analysis functions dependencies in
//                ScorePageBase class.
//

#ifndef _DATABASEBEAM_H_INCLUDED
#define _DATABASEBEAM_H_INCLUDED

#include <vector>
class ScoreItem;
#include "ScoreDefs.h"

using namespace std;

#ifndef UseBoundVector
   using vectorSIp   = vector<ScoreItem*>;
   using vectorVSIp  = vector<vector<ScoreItem*>>;
   using vectorVVSIp = vector<vector<vector<ScoreItem*>>>;
#else
   #include "BoundVector.h"
   using vectorSIp   = BoundVector<ScoreItem*>;
   using vectorVSIp  = BoundVector<BoundVector<ScoreItem*>>;
   using vectorVVSIp = BoundVector<BoundVector<BoundVector<ScoreItem*>>>;
#endif

#include <map>
#include <list>
#include <vector>
#include <string>
#include <iostream>



class BeamGroup {
   public:
      // complex beams will be composed from separate beam items, so
      // in practice there can be more than one beam item generating
      // the visual display of the beam.
      vectorSIp beams;
      vectorSIp notes;
};


class DatabaseBeam {
   public:
                         DatabaseBeam  (void);
                        ~DatabaseBeam  ();

      void               clear          (void);
      int                size           (void);
      BeamGroup*         beamInfo       (ScoreItem*);
      BeamGroup*         linkItems      (ScoreItem* note1, ScoreItem* note2);
      ostream&           printDatabase  (ostream& out = cout);

   protected:
      void               insertItem     (BeamGroup* list, ScoreItem* note);

   private:
      map<ScoreItem*, BeamGroup*> interface;
      list<BeamGroup*> database;
};


#endif  /* _DATABASEBEAM_H_INCLUDED */

