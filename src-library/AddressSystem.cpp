//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      AddressSystem .cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/AddressSystem .cpp
// Syntax:        C++11
//
// Description:   Used for keeping track of a part across multiple systems.
//                ScorePageBase class.
//

#include "AddressSystem.h"
#include "ScorePageSet.h"

using namespace std;


//////////////////////////////
//
// AddressSystem ::AddressSystem  --
//

AddressSystem ::AddressSystem (void) { }


AddressSystem ::AddressSystem (AddressSystem & anAddress) { 
   *this = anAddress;
}


AddressSystem ::AddressSystem (int aPage, int anOverlay, int aSystem, 
      int aSystemStaff) { 
   setAddress(aPage, anOverlay, aSystem, aSystemStaff);
}


AddressSystem::AddressSystem(vectorI& anAddress) {
   clear();
   if (anAddress.size() > 0) { page        = anAddress[0]; }
   if (anAddress.size() > 1) { overlay     = anAddress[1]; }
   if (anAddress.size() > 2) { system      = anAddress[2]; }
   if (anAddress.size() > 3) { systemstaff = anAddress[3]; }
}


//////////////////////////////
//
// AddressSystem::~AddressSystem --
//

AddressSystem::~AddressSystem() { 
   // do nothing.
}



//////////////////////////////
//
// AddressSystem::clear --
//

void AddressSystem::clear(void) {
   page        = -1;
   overlay     = -1;
   system      = -1;
   systemstaff = -1;
}

//
// Alias:
//

void AddressSystem::invalidate(void) {
   clear();
}
   


//////////////////////////////
//
// AddressSystem::getPageIndex -- Return the page index (primary overlay 
//     for a page).  Offset from 0.
//

int AddressSystem::getPageIndex(void) const {
   return page;
}



//////////////////////////////
//
// AddressSystem::getOverlayIndex -- Return the overlay index, offset 
//     from 0 for the primary overlay.
//

int AddressSystem::getOverlayIndex(void) const { 
   return overlay;
}



//////////////////////////////
//
// AddressSystem::getSystemIndex -- Return the system index of the 
//    the address on a page.
//

int AddressSystem::getSystemIndex(void) const { 
   return system;
}



//////////////////////////////
//
// AddressSystem::getSystemStaff --  Return the system staff which the part
//    belongs to.  If the system staff is negative, then the part does not
//    have any data on the staff, and should be contructed out of rests
//    with the duration and barring of the existing parts on the system.
//

int AddressSystem::getSystemStaffIndex(void) const { 
   return systemstaff;
}



//////////////////////////////
//
// AddressSystem::setAddress --
//

void AddressSystem::setAddress(int aPage, int anOverlay, int aSystem, 
      int aSystemStaff) { 
   page         = aPage;
   overlay      = anOverlay;
   system       = aSystem;
   systemstaff  = aSystemStaff;
}



//////////////////////////////
//
// AddressSystem::incrementSystem -- Increment system to the next 
// system in the given ScorePageSet.  Invalidates Address and returns
// 0 if there are no more systems in the page set.  Returns 1 if 
// successful in incrementing the system.
//

int AddressSystem::incrementSystem(ScorePageSet& pageset) {
   int s = getSystemIndex();
   int pcount = pageset.getPageCount();
   if (pcount >= getPageIndex()) {
      invalidate();
      return 0;
   }
   int overlay = 0;
   int scount = pageset[getPageIndex()][overlay].getSystemCount();
   if (s+1 < scount) {
      setSystem(s+1);
   } else {
      if (pcount >= getPageIndex()-1) {
         setPage(getPageIndex()+1);
         setSystem(0);
      } else {
         invalidate();
         return 0;
      }
   }

   return 1;
}



//////////////////////////////
//
// AddressSystem::operator= --
//

AddressSystem& AddressSystem::operator=(AddressSystem& anAddress) { 
   if (this == &anAddress) {
      return *this;
   }
   page         = anAddress.page;
   overlay      = anAddress.overlay;
   system       = anAddress.system;
   systemstaff  = anAddress.systemstaff;

   return *this;
}


AddressSystem& AddressSystem::operator=(vectorI& anAddress) {
   clear();
   if (anAddress.size() > 0) { page        = anAddress[0]; }
   if (anAddress.size() > 1) { overlay     = anAddress[1]; }
   if (anAddress.size() > 2) { system      = anAddress[2]; }
   if (anAddress.size() > 3) { systemstaff = anAddress[3]; }
   return *this;
}


AddressSystem& AddressSystem::operator=(int* anAddress) {
   int size = sizeof(anAddress) / sizeof(int);
   clear();
   if (size > 0) { page        = anAddress[0]; }
   if (size > 1) { overlay     = anAddress[1]; }
   if (size > 2) { system      = anAddress[2]; }
   if (size > 3) { systemstaff = anAddress[3]; }
   return *this;
}



//////////////////////////////
//
// AddressSystem::operator== -- test if two addresses are the same.
//

bool AddressSystem::operator==(AddressSystem& anAddress) { 
   if (this == &anAddress) {
      return true;
   }

   if ((page == anAddress.page) && (overlay == anAddress.overlay) &&
       (system == anAddress.system) && (systemstaff == anAddress.systemstaff)) {
      return true;
   }
   return false;
}



//////////////////////////////
//
// AddressSystem::operator!= -- test if two addresses are the same.
//

bool AddressSystem::operator!=(AddressSystem& anAddress) { 
   return !(*this == anAddress);
}



//////////////////////////////
//
// AddressSystem::setPage --
//

void AddressSystem::setPage(int aPage) { 
   page = aPage;
}



//////////////////////////////
//
// AddressSystem::setOverlay --
//

void AddressSystem::setOverlay(int anOverlay) { 
   overlay = anOverlay;
}



//////////////////////////////
//
// AddressSystem::setSystem --
//

void AddressSystem::setSystem(int aSystem) { 
   system = aSystem;
}



//////////////////////////////
//
// AddressSystem::setSystemStaff --
//

void AddressSystem::setSystemStaff(int aSystemStaff) { 
   systemstaff = aSystemStaff;
}



//////////////////////////////
//
// operator<< -- print an address.
//

ostream& operator<<(ostream& out, const AddressSystem& anAddress) {
   out << "{";
   out << anAddress.getPageIndex()        << ", ";
   out << anAddress.getOverlayIndex()     << ", ";
   out << anAddress.getSystemIndex()      << ", ";
   out << anAddress.getSystemStaffIndex();
   out << "}";
   return out;
}



