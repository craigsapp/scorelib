//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Mar 20 14:19:44 PDT 2014
// Last Modified: Thu Mar 20 15:19:42 PDT 2014
// Filename:      DatabaseP3.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseP3.h
// Syntax:        C++11
//
// Description:   Keep track of SCORE items by horizontal position.  Either
// 		  staves or systems can be stored in the database as long
// 		  as all duration items are aligned by P3 positions across
// 		  the system staves/staff layers.
//

#ifndef _DATABASEP3_H_INCLUDED
#define _DATABASEP3_H_INCLUDED

#include "ScoreItem.h"

#include <map>
#include <list>
#include <vector>
#include <string>
#include <iostream>

using namespace std;


class P3VerticalItems : public vectorSIp {
   public:
      P3VerticalItems* previous;
      P3VerticalItems* next;

      P3VerticalItems   (void) { clear(); }
     ~P3VerticalItems   ()     { clear(); }
      void clear        (void) {
         previous = next = NULL;
         vectorSIp::clear();
      }
};



class DatabaseP3 {
   public:
                       DatabaseP3        (void);
                      ~DatabaseP3        ();

      void             clear             (void);
      int              size              (void);
      ostream&         printDatabase     (ostream& out = cout);
      void             addItem           (ScoreItem* item);
      P3VerticalItems* getScoreItemsByP3 (SCORE_FLOAT p3);
      P3VerticalItems* getScoreItemsByStaffDurationOffset (SCORE_FLOAT offset);
      SCORE_FLOAT      getP3OfStaffDurationOffset(SCORE_FLOAT offset);
      SCORE_FLOAT      getStaffDurationOffsetOfP3(SCORE_FLOAT p3);

   protected:
      P3VerticalItems* storeItem         (ScoreItem* item);
      void             prepare           (void);

   private:
      int preparedQ;
      list<P3VerticalItems*>                database;
      map<SCORE_FLOAT, P3VerticalItems*>    interface_byP3;
      map<SCORE_FLOAT, P3VerticalItems*>    interface_byOffset;

};

bool DatabaseP3_sortP3(P3VerticalItems* a, P3VerticalItems* b);

#endif  /* _DATABASEP3_H_INCLUDED */



