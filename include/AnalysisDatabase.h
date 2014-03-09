//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar  7 23:20:33 PST 2014
// Last Modified: Fri Mar  7 23:20:35 PST 2014
// Filename:      AnalysisDatabase.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/AnalysisDatabase.h
// Syntax:        C++11
//
// Description:   Keeps tracks of analysis functions dependencies in
//                ScorePageBase class.
//

#ifndef _ANALYSISDATABASE_H_INCLUDED
#define _ANALYSISDATABASE_H_INCLUDED

#include <map>
#include <list>
#include <string>

using namespace std;

class _AnalysisGraph {
   public:
      int*   data;
      list<string> children;
      list<string> parents;
};


class AnalysisDatabase {
   public:
              AnalysisDatabase  (void);
             ~AnalysisDatabase  ();
   
      void    addNode           (const string& nodename, int* state);
      void    addChild          (const string& nodename, const string& child, 
                                 int* cstate);
      void    invalidateNode    (const string& nodename);
      void    validateNode      (const string& nodename);

   private:
      map<string, _AnalysisGraph> nodes;

};


#endif  /* _ANALYSISDATABASE_H_INCLUDED */

