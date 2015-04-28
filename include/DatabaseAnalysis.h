//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar  7 23:20:33 PST 2014
// Last Modified: Fri Mar  7 23:20:35 PST 2014
// Filename:      DatabaseAnalysis.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/DatabaseAnalysis.h
// Syntax:        C++11
//
// Description:   Keeps tracks of analysis functions dependencies in
//                ScorePageBase class.
//

#ifndef _DATABASEANALYSIS_H_INCLUDED
#define _DATABASEANALYSIS_H_INCLUDED

#include <map>
#include <list>
#include <string>
#include <iostream>

using namespace std;

class _AnalysisGraph {
   public:
      int*   data;
      list<string> children;
      list<string> parents;
};


class DatabaseAnalysis {
   public:
              DatabaseAnalysis  (void);
             ~DatabaseAnalysis  ();

      void    addNode           (const string& nodename, int* state);
      void    addChild          (const string& nodename, const string& child,
                                 int* cstate);
      void    invalidateNode    (const string& nodename);
      void    validateNode      (const string& nodename);

      ostream& print            (ostream& out = cout);

   private:
      map<string, _AnalysisGraph> nodes;

};


#endif  /* _DATABASEANALYSIS_H_INCLUDED */

