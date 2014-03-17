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

#include "ScoreItem.h"

#include <map>
#include <list>
#include <string>

using namespace std;


class BeamGroup {
   public:
      // complex beams will be composed from separate beam items, so
      // in practice there can be more than one beam item generating
      // the visual display of the beam.
      vector<ScoreItem*> beams;
      vector<ScoreItem*> notes;
};


class DatabaseBeam {
   public:
                         DatabaseBeam  (void);
                        ~DatabaseBeam  ();
   
      void               clear          (void);
      BeamGroup*         beamInfo       (ScoreItem*);
      BeamGroup*         linkItems      (ScoreItem* note1, ScoreItem* note2);

   protected:
      void               insertItem     (BeamGroup* list, ScoreItem* note);

   private:
      map<ScoreItem*, BeamGroup*> interface;
      list<BeamGroup*> database;
      BeamGroup dummyinfo;
};


#endif  /* _DATABASEBEAM_H_INCLUDED */

