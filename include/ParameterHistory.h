//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:11:15 PST 2014
// Last Modified: Tue Mar  4 00:55:27 PST 2014
// Filename:      ParameterHistory.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ParameterHistory.h
// Syntax:        C++11
//
// Description:   The ParameterHistory class is used by the ScoreItemEdit
//                class to store changes in parameters in the ScoreItemBase
//                class.
//

#ifndef _PARAMETERHISTORY_H_INCLUDED
#define _PARAMETERHISTORY_H_INCLUDED

#include "ScoreDefs.h"

#include <string>
#include <fstream>

using namespace std;

#define SCOREITEM_UNDEFINED 0
#define SCOREITEM_CREATED   1
#define SCOREITEM_DELETED   2

class ParameterHistory {

    public:

      ParameterHistory(void);
      ParameterHistory(const string& nspace, const string& key, 
            const string& oldvalue, const string& newvalue, int hindex);
      ParameterHistory(const string& nspace, const string& key, 
            int created_state, const string& newvalue, int hindex);
      ParameterHistory(const string& nspace, const string& key, 
            const string& oldvalue, int deleted_state, int hindex);
      ParameterHistory(const string& key, const string& oldvalue, 
            const string& newvalue, int hindex);
      ParameterHistory(const string& key, int created_state,
            const string& newvalue, int hindex);
      ParameterHistory(const string& key, const string& oldvalue, 
            int deleted_state, int hindex);
      ParameterHistory(int key, SCORE_FLOAT oldvalue, SCORE_FLOAT newvalue, 
            int hindex);
      ParameterHistory(int key, int created_state, SCORE_FLOAT newvalue, 
            int hindex);
      ParameterHistory(int key, SCORE_FLOAT oldvalue, int deleted_state,
            int hindex);

      void        clear         (void);
      ostream&    printAsPmx    (ostream& out) const;
      ostream&    printAsXml    (ostream& out);

   protected:
      int         history;
      string      named_nspace;
      string      named_key;
      string      named_oldvalue;
      string      named_newvalue;
      int         fixed_index;
      SCORE_FLOAT fixed_oldvalue;
      SCORE_FLOAT fixed_newvalue;
      int         state;

};


typedef list<ParameterHistory> HistoryList;


#endif  /* _PARAMETERHISTORY_H_INCLUDED */


