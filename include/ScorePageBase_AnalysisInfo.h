//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 03:04:06 PST 2014
// Filename:      ScorePageBase_AnalysisInfo.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageBase_AnalysisInfo.h
// Syntax:        C++11
//
// Description:   Support class for structural analyses in ScorePageBase.
//

#ifndef _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED
#define _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED

#include "ScoreItem.h"

using namespace std;


class AnalysisInfo {
   public:
                    AnalysisInfo    (void);
                    AnalysisInfo    (const AnalysisInfo& info);
                   ~AnalysisInfo    (void);

      AnalysisInfo& operator=       (const AnalysisInfo& info);
      void          clear           (void);

   public:
      // analysis_notmodified: 
      //    0 : modified; or undefined
      //  >=1 : no change in ScoreItems owned by this page
      //        since the last analyses were done.
      //    1 : staff analysis has been done.
      //    2 : staff duration analysis has been done.
      //    3 : system analysis has been done.
      //    4 : part analysis has been done.
      int notmodified;

      // staves: 0=staff analysis has not been done.
      //                  1=staff analysis has been done.
      bool staves;

      // staffduration: 0=staff durations have not yet been
      // analyzed. 1=staff durations have been analyzed
      bool staffdurations;

      // systems: 0=system grouping of staves has not been done;
      // 1=system grouping has been done.
      bool systems;

};


#endif /* _SCOREPAGEBASE_ANALYSISINFO_H_INCLUDED */
