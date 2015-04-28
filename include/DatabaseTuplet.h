//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 13:07:50 PDT 2014
// Last Modified: Mon Mar 10 13:07:53 PDT 2014
// Filename:      DatabaseTuplet.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseTuplet.h
// Syntax:        C++11
//
// Description:   Keeps tracks of tuplet analysis information for a
//                ScorePageBase class.
//

#ifndef _DATABASETUPLET_H_INCLUDED
#define _DATABASETUPLET_H_INCLUDED

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



class TupletGroup {
   public:
      // Using an array for the brackets to make symmetric with
      // DatabaseBeam, but not necessary.  Note can be within more than
      // one tuplet bracket if they are nested however.  Consider
      // that later.
      vectorSIp brackets;
      vectorSIp notes;
};


class DatabaseTuplet {
   public:
                         DatabaseTuplet  (void);
                        ~DatabaseTuplet  ();

      void               clear          (void);
      int                size           (void);
      TupletGroup*       tupletInfo     (ScoreItem*);
      TupletGroup*       linkItems      (ScoreItem* note1, ScoreItem* note2);
      ostream&           printDatabase  (ostream& out = cout);

   protected:
      void               insertItem     (TupletGroup* list, ScoreItem* note);

   private:
      map<ScoreItem*, TupletGroup*> interface;
      list<TupletGroup*> database;
};


#endif  /* _DATABASETUPLET_H_INCLUDED */

