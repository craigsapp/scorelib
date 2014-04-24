//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      SystemAddress.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/SystemAddress.h
// Syntax:        C++11
//
// Description:   Used for keeping track of a part across multiple systems.
//                ScorePageBase class.
//

#ifndef _SYSTEMADDRESS_H_INCLUDED
#define _SYSTEMADDRESS_H_INCLUDED

#include <iostream>
#include <vector>

class ScorePageSet;

using namespace std;

class SystemAddress {

   public:
            SystemAddress       (void);
            SystemAddress       (vector<int>& anAddress);
            SystemAddress       (SystemAddress& anAddress);
            SystemAddress       (int aPage, int anOverlay, int aSystem, 
                                 int aSystemStaff);
           ~SystemAddress();

      void  clear               (void);
      void  invalidate          (void);
      int   getPage             (void) const;
      int   getPageIndex        (void) const;
      int   getOverlay          (void) const;
      int   getOverlayIndex     (void) const;
      int   getSystem           (void) const;
      int   getSystemIndex      (void) const;
      int   getSystemStaff      (void) const;
      int   getSystemStaffIndex (void) const;
      void  setAddress          (int aPage, int anOverlay, int aSystem, 
                                 int aSystemStaff);
      int   incrementSystem     (ScorePageSet& pageset);
      SystemAddress& operator=  (SystemAddress& anAddress);
      SystemAddress& operator=  (vector<int>& anAddress);
      SystemAddress& operator=  (int* anAddress);
      bool           operator== (SystemAddress& anAddress);
      bool           operator!= (SystemAddress& anAddress);

      void  setPage             (int aPage);
      void  setOverlay          (int anOverlay);
      void  setSystem           (int aSystem);
      void  setSystemStaff      (int aSystemStaff);

   protected:
      int page;
      int overlay;
      int system;
      int systemstaff;

};


ostream& operator<<(ostream& out, const SystemAddress& anAddress);


#endif  /* _SYSTEMADDRESS_H_INCLUDED */



