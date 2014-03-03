//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 00:45:35 PST 2014
// Filename:      ScorePageBase_StaffInfo.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageBase_StaffInfo.h
// Syntax:        C++11
//
// Description:   Support class for staff analyses in ScorePageBase.
//

#ifndef _SCOREPAGEBASE_STAFFINFO_H_INCLUDED
#define _SCOREPAGEBASE_STAFFINFO_H_INCLUDED

#include "ScoreItem.h"

class StaffInfo {

   public:
                    StaffInfo            (void);
                    StaffInfo            (const StaffInfo& info);
                    ~StaffInfo           ();

                    StaffInfo& operator= (const StaffInfo& info);
      void          clear                (void);
      void          setStaffDuration     (int staffnum, SCORE_FLOAT duration);
      SCORE_FLOAT   getStaffDuration     (int staffnum);
      void          setStaffItemsSize    (int newsize);
      int           getStaffItemsSize    (void);
      int           getStaffItemSize     (int staffnum);
      const vectorVSIp& getStaffItems    (void);
      void          appendStaffItem      (int staffnum, ScoreItem* item);

   protected:
      // staffdur: duration of staff (sum of P7s across staff)
      vectorSF staffdur;

      // system: the system number on the page to which the staff
      // belongs.  The top staff on the page is system 0.
      vectorI  system;

      // sysindex: the staff's index on the system.  The bottom staff
      // on the system is at index 0.
      vectorI  sysindex;

      // staffitems: collect a list of the staff items for each staff position.
      // In theory there can be more than one per staff position, but they
      // must all have the same vertical scale value (P5) since SCORE will
      // ignore the P3 range of separate staff items.  Note that 
      // staffitems[0] is not intended to be used (may refer in the future
      // to page positioned items.)
      vectorVSIp staffitems;

};

#endif /* _SCOREPAGEBASE_STAFFINFO_H_INCLUDED */
