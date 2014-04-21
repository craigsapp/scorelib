//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      SystemAddress.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/SystemAddress.cpp
// Syntax:        C++11
//
// Description:   Used for keeping track of a part across multiple systems.
//                ScorePageBase class.
//

#include "SystemAddress.h"

using namespace std;


//////////////////////////////
//
// SystemAddress::SystemAddress --
//

SystemAddress::SystemAddress(void) { }


SystemAddress::SystemAddress(SystemAddress& anAddress) { 
   *this = anAddress;
}


SystemAddress::SystemAddress(int aPage, int anOverlay, int aSystem, 
      int aSystemStaff) { 
   setAddress(aPage, anOverlay, aSystem, aSystemStaff);
}


SystemAddress::SystemAddress(vector<int>& anAddress) {
   clear();
   if (anAddress.size() > 0) { page        = anAddress[0]; }
   if (anAddress.size() > 1) { overlay     = anAddress[1]; }
   if (anAddress.size() > 2) { system      = anAddress[2]; }
   if (anAddress.size() > 3) { systemstaff = anAddress[3]; }
}


//////////////////////////////
//
// SystemAddress::~SystemAddress --
//

SystemAddress::~SystemAddress() { 
   // do nothing.
}



//////////////////////////////
//
// SystemAddress::clear --
//

void SystemAddress::clear(void) {
   page        = -1;
   overlay     = -1;
   system      = -1;
   systemstaff = -1;
}



//////////////////////////////
//
// SystemAddress::getPage -- Return the page index (primary overlay for a page).
//     Offset from 0.
//

int SystemAddress::getPage(void) const { 
   return page;
}

//
// Alias:
//

int SystemAddress::getPageIndex(void) const {
   return getPage();
}



//////////////////////////////
//
// SystemAddress::getOverlay -- Return the overlay index, offset from 0 for the
//     primary overlay.
//

int SystemAddress::getOverlay(void) const { 
   return overlay;
}

//
// Alias:
//

int SystemAddress::getOverlayIndex(void) const {
   return getOverlay();
}



//////////////////////////////
//
// SystemAddress::getSystem -- Return the system of the ScorePage indexed
//     with the page/overlay index.
//

int SystemAddress::getSystem(void) const { 
   return system;
}

//
// Alias:
//

int SystemAddress::getSystemIndex(void) const { 
   return getSystem();
}



//////////////////////////////
//
// SystemAddress::getSystemStaff --  Return the system staff which the part
//    belongs to.  If the system staff is negative, then the part does not
//    have any data on the staff, and should be contructed out of rests
//    with the duration and barring of the existing parts on the system.
//

int SystemAddress::getSystemStaff(void) const { 
   return systemstaff;
}

//
// Alias:
//

int SystemAddress::getSystemStaffIndex(void) const { 
   return getSystemStaffIndex();
}



//////////////////////////////
//
// SystemAddress::setAddress --
//

void SystemAddress::setAddress(int aPage, int anOverlay, int aSystem, 
      int aSystemStaff) { 
   page         = aPage;
   overlay      = anOverlay;
   system       = aSystem;
   systemstaff  = aSystemStaff;
}



//////////////////////////////
//
// SystemAddress::operator= --
//

SystemAddress& SystemAddress::operator=(SystemAddress& anAddress) { 
   if (this == &anAddress) {
      return *this;
   }
   page         = anAddress.page;
   overlay      = anAddress.overlay;
   system       = anAddress.system;
   systemstaff  = anAddress.systemstaff;

   return *this;
}


SystemAddress& SystemAddress::operator=(vector<int>& anAddress) {
   clear();
   if (anAddress.size() > 0) { page        = anAddress[0]; }
   if (anAddress.size() > 1) { overlay     = anAddress[1]; }
   if (anAddress.size() > 2) { system      = anAddress[2]; }
   if (anAddress.size() > 3) { systemstaff = anAddress[3]; }
   return *this;
}


SystemAddress& SystemAddress::operator=(int* anAddress) {
   int size = sizeof(anAddress) / sizeof(int);
cout << "SIZE = " << size << endl;
   clear();
   if (size > 0) { page        = anAddress[0]; }
   if (size > 1) { overlay     = anAddress[1]; }
   if (size > 2) { system      = anAddress[2]; }
   if (size > 3) { systemstaff = anAddress[3]; }
   return *this;
}



//////////////////////////////
//
// SystemAddress::operator== -- test if two addresses are the same.
//

bool SystemAddress::operator==(SystemAddress& anAddress) { 
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
// SystemAddress::operator!= -- test if two addresses are the same.
//

bool SystemAddress::operator!=(SystemAddress& anAddress) { 
   return !(*this == anAddress);
}



//////////////////////////////
//
// SystemAddress::setPage --
//

void SystemAddress::setPage(int aPage) { 
   page = aPage;
}



//////////////////////////////
//
// SystemAddress::setOverlay --
//

void SystemAddress::setOverlay(int anOverlay) { 
   overlay = anOverlay;
}



//////////////////////////////
//
// SystemAddress::setSystem --
//

void SystemAddress::setSystem(int aSystem) { 
   system = aSystem;
}



//////////////////////////////
//
// SystemAddress::setSystemStaff --
//

void SystemAddress::setSystemStaff(int aSystemStaff) { 
   systemstaff = aSystemStaff;
}



//////////////////////////////
//
// operator<< -- print an address.
//

ostream& operator<<(ostream& out, const SystemAddress& anAddress) {
   out << "{";
   out << anAddress.getPage()        << ", ";
   out << anAddress.getOverlay()     << ", ";
   out << anAddress.getSystem()      << ", ";
   out << anAddress.getSystemStaff();
   out << "}";
   return out;
}



