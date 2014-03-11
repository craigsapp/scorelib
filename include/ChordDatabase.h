//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 10 13:07:50 PDT 2014
// Last Modified: Mon Mar 10 13:07:53 PDT 2014
// Filename:      ChordDatabase.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ChordDatabase.h
// Syntax:        C++11
//
// Description:   Keeps tracks of analysis functions dependencies in
//                ScorePageBase class.
//

#ifndef _CHORDDATABASE_H_INCLUDED
#define _CHORDDATABASE_H_INCLUDED

#include "ScoreItem.h"

#include <map>
#include <list>
#include <string>

using namespace std;

class ChordDatabase {
   public:
                         ChordDatabase  (void);
                        ~ChordDatabase  ();
   
      void               clear          (void);
      vectorSIp&         notelist       (ScoreItem*);
      vectorSIp&         linkNotes      (ScoreItem* note1, ScoreItem* note2);

   protected:
      void               insertNote     (vectorSIp& list, ScoreItem* note);

   private:
      map<ScoreItem*, vectorSIp*> interface;
      list<vectorSIp> database;
      vectorSIp dummylist;
};


#endif  /* _CHORDDATABASE_H_INCLUDED */

