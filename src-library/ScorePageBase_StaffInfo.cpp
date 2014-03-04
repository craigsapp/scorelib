//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Mar  1 00:35:04 PST 2014
// Last Modified: Sat Mar  1 00:35:07 PST 2014
// Filename:      ScorePageBase_StaffInfo.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageBase_StaffInfo.cpp
// Syntax:        C++11
//
// Description:   Support class for staff analyses in ScorePageBase.
//

#include "ScorePageBase_StaffInfo.h"

using namespace std;

///////////////////////////////
//
// StaffInfo::StaffInfo --
//

StaffInfo::StaffInfo(void) { 
   // do nothing
}


StaffInfo::StaffInfo(const StaffInfo& info) {
   *this = info;
}



///////////////////////////////
//
// StaffInfo::operator= --
//

StaffInfo& StaffInfo::operator=(const StaffInfo& info) {
   if (this == &info) {
      return *this;
   }
   staffdur   = info.staffdur;
   system     = info.system;
   sysindex   = info.sysindex;
   staffitems = info.staffitems;
   return *this;
}


///////////////////////////////
//
// StaffInfo::~StaffInfo --
//
StaffInfo::~StaffInfo() { 
   clear(); 
}



///////////////////////////////
//
// StaffInfo::clear --
//
void StaffInfo::clear(void) {
   staffdur.clear();
   system.clear();
   sysindex.clear();
   staffitems.clear();
}



///////////////////////////////
//
// StaffInfo::setStaffDuration --
//

void StaffInfo::setStaffDuration(int staffnum, SCORE_FLOAT duration) {
   int asize = staffdur.size();
   if (staffnum+1 >= asize) {
      staffdur.resize(staffnum+1, 0.0);
   }
   staffdur[staffnum] = duration;
}



///////////////////////////////
//
// StaffInfo::getStaffDuration --
//

SCORE_FLOAT StaffInfo::getStaffDuration(int staffnum) {
   return staffdur[staffnum];
}



///////////////////////////////
//
// StaffInfo::setStaffItemsSize --
//

void StaffInfo::setStaffItemsSize(int newsize) {
   staffitems.resize(newsize);
}



///////////////////////////////
//
// StaffInfo::getStaffItemsSize --
//

int StaffInfo::getStaffItemsSize(void) {
   return staffitems.size();
}


int StaffInfo::getStaffItemSize(int staffnum) {
   return staffitems[staffnum].size();
}



///////////////////////////////
//
// StaffInfo::getStaffItems --
//

const vectorVSIp& StaffInfo::getStaffItems(void) {
   return staffitems;
}



///////////////////////////////
//
// StaffInfo::appendStaffItem --
//

void StaffInfo::appendStaffItem(int staffnum, ScoreItem* item) {
   staffitems[staffnum].push_back(item);
}



