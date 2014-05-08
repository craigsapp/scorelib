//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      AddressSystem.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/AddressSystem.h
// Syntax:        C++11
//
// Description:   Used for keeping track of a part across multiple systems.
//

#ifndef _ADDRESSSYSTEM_H_INCLUDED
#define _ADDRESSSYSTEM_H_INCLUDED

#include <iostream>
#include <vector>
#include "ScoreDefs.h"

class ScorePageSet;

using namespace std;

class AddressSystem {

   public:
            AddressSystem       (void);
            AddressSystem       (vectorI& anAddress);
            AddressSystem       (AddressSystem& anAddress);
            AddressSystem       (int aPage, int anOverlay, int aSystem, 
                                 int aSystemStaff);
           ~AddressSystem();

      void  clear               (void);
      void  invalidate          (void);
      int   getPageIndex        (void) const;
      int   getOverlayIndex     (void) const;
      int   getSystemIndex      (void) const;
      int   getSystemStaffIndex (void) const;
      void  setAddress          (int aPage, int anOverlay, int aSystem, 
                                 int aSystemStaff);
      int   incrementSystem     (ScorePageSet& pageset);
      AddressSystem& operator=  (AddressSystem& anAddress);
      AddressSystem& operator=  (vectorI& anAddress);
      AddressSystem& operator=  (int* anAddress);
      bool           operator== (AddressSystem& anAddress);
      bool           operator!= (AddressSystem& anAddress);

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


#ifndef UseBoundVector
   #include "BoundVector.h"
   using vectorVASp  = BoundVector<BoundVector<AddressSystem*>>;
#else
   using vectorVASp  = vector<vector<AddressSystem*>>;
#endif


ostream& operator<<(ostream& out, const AddressSystem& anAddress);


#endif  /* _ADDRESSSYSTEM_H_INCLUDED */



