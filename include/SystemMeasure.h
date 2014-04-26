//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 23 15:13:08 PDT 2014
// Last Modified: Wed Apr 23 15:13:10 PDT 2014
// Filename:      SystemMeasure.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/SystemMeasure.h
// Syntax:        C++11
//
// Description:   Used in analyzeBarlines() in ScorePage class.  This class 
//                stores all ScoreItems present in a measure of music.  

#ifndef _SYSTEMMEASURE_H_INCLUDED
#define _SYSTEMMEASURE_H_INCLUDED

#include "ScoreItem.h"

using namespace std;

class SystemMeasure {

   public:
                     SystemMeasure            (void);
                    ~SystemMeasure            (void);
      void           clear                    (void);
      void           addItem                  (ScoreItem* item);
      vectorSIp&     getItems                 (void);
      SCORE_FLOAT    getP3Width               (void);
      SCORE_FLOAT    getSystemOffsetDuration  (void);
      SCORE_FLOAT    getMeasureDuration       (void);
      SCORE_FLOAT    getDuration              (void);
      vectorSIp&     getStartBarlines         (void);
      vectorSIp&     getEndBarlines           (void);
      ScoreItem*     operator[]               (int index);
      int            getSize                  (void);

   protected:
      vectorSIp   start_bars;
      vectorSIp   end_bars;
      SCORE_FLOAT measure_duration; // duration of the measure
      SCORE_FLOAT system_offset;    // start duration of measure within system
      vectorSIp measure_items;
};

ostream& operator<<(ostream& out, SystemMeasure& measure);

using vectorVSMp = vector<vector<SystemMeasure*>>;
using vectorSMp  = vector<SystemMeasure*>;
using listSMp    = list<SystemMeasure*>;


#endif /* _SYSTEMMEASURE_H_INCLUDED */


