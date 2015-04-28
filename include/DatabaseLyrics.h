//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 00:28:37 PDT 2014
// Last Modified: Tue May  6 00:28:40 PDT 2014
// Filename:      DatabaseLyrics.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseLyrics.h
// Syntax:        C++11
//
// Description:   Keeps tracks of analysis functions dependencies in
//                ScorePageBase class.
//

#ifndef _DATABASELYRICS_H_INCLUDED
#define _DATABASELYRICS_H_INCLUDED

#include "ScoreItem.h"

#include <map>
#include <list>
#include <string>

using namespace std;

class DatabaseLyrics {
   public:
                        DatabaseLyrics  (void);
                       ~DatabaseLyrics  ();

      void              clear           (void);
      vectorSIp*        lyricslist      (ScoreItem*);
      vectorSIp*        link            (ScoreItem* item1, ScoreItem* item2);

   protected:
      void              insert          (vectorSIp* list, ScoreItem* item);

   private:
      map<ScoreItem*, vectorSIp*> interface;
      list<vectorSIp> database;
};


#endif  /* _DATABASELYRICS_H_INCLUDED */

