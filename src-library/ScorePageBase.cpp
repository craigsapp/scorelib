//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 02:51:26 PST 2014
// Filename:      ScorePageBase.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase.cpp
// Syntax:        C++11
//
// Description:   Base class for ScorePage.  This class handles reading/writing
//                of a ScorePage, and handles all of the data variables for
//                a ScorePage.
//

#include "ScorePageBase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>

using namespace std;


//////////////////////////////
//
// ScorePageBase::ScorePageBase -- Constructor functions.  ScorePageBase
//    can be constructed as an empty container, or data can be loaded
//    from a file, input stream, or copied from the contents of another page.
//

ScorePageBase::ScorePageBase(void) {
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
}


ScorePageBase::ScorePageBase(const char* filename) {
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
   readFile(filename);
}


ScorePageBase::ScorePageBase(istream& instream) { 
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
   readFile(instream);
}


ScorePageBase::ScorePageBase(const ScorePageBase& apage) { 
   // make a copy of an existing page
   // have to deal with unique ids...
   trailer.reserve(8);
   clearAnalysisStates();

   print_info = apage.print_info;
   
   listSIp::const_iterator it;
   const listSIp& itemlist = apage.item_storage;
   ScoreItem* sip;
   for (it = itemlist.begin(); it != itemlist.end(); it++) {
      if ((*it) != NULL) {
         sip = new ScoreItem(**it);
         item_storage.push_back(sip);
      }
   }
}



//////////////////////////////
//
// ScorePageBase::~ScorePageBase -- Delete the ScoreItem storage
//

ScorePageBase::~ScorePageBase() {
   clear();
}



/////////////////////////////
//
// ScorePageBase::clear -- Remove all SCORE items from page.
//

void ScorePageBase::clear(void) {
   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      if ((*it) != NULL) {
         delete (*it);
         *it = NULL;
      }
   }
}



//////////////////////////////
//
// ScorePageBase::prependItem -- make a copy of the item and store 
//   it in the item_storage list. Returns a pointer to the 
//   stored item.
//

ScoreItem* ScorePageBase::prependItem(ScoreItem& anItem) {
   ScoreItem* ptr = new ScoreItem(anItem);
   item_storage.push_front(ptr);
   return ptr;
}



//////////////////////////////
//
// ScorePageBase::appendItem -- make a copy of the item and store 
//   it in the item_storage list. Returns a pointer to the 
//   stored item.
//

ScoreItem* ScorePageBase::appendItem(ScoreItem& anItem) {
   ScoreItem* ptr = new ScoreItem(anItem);
   item_storage.push_back(ptr);
   return ptr;
}


ScoreItem* ScorePageBase::appendItem(const string& itemstring) {
   ScoreItem* ptr = new ScoreItem(itemstring);
   item_storage.push_back(ptr);
   return ptr;
}



//////////////////////////////
//
// ScorePageBase::setDefaultPrintParameters -- Set print parameters to 
//     their default values.
//

void ScorePageBase::setDefaultPrintParameters(void) {
   print_info.setDefaultValues();
}



//////////////////////////////
//
// ScorePageBase::getItemCount -- Return the number of ScoreItems owned 
//     by the page.
//

int ScorePageBase::getItemCount(void) {
   return item_storage.size();
}



//////////////////////////////
//
// ScorePageBase::getFileOrderList -- Return a list of the SCORE items
//   in the sequence in which they are found in the file (such as
//   the input data read from a file).
//

void ScorePageBase::getFileOrderList(vectorSIp& data) {
   data.reserve(item_storage.size());
   data.clear();
   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      data.push_back(*it);
   }
}



//////////////////////////////
//
// ScorePageBase::getStaffItemList --  Return a 2D list of items on the
//    page.  The first dimension is the staff number (P2), the second
//    dimension is a list of SCORE items on the staff.  The order of the
//    SCORE items is the file order, not necessarily in time/spatial order.
//

void ScorePageBase::getStaffItemList (vectorVSIp& data) {
   data.reserve(MAX_STAFF_COUNT);
   listSIp::iterator it;
   unsigned int staffnum;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      staffnum = (*it)->getStaffNumber();
      if (data.size() < staffnum + 1) {
         data.resize(staffnum+1);
      }
      data[staffnum].push_back(*it);
   }
}



//////////////////////////////
//
// ScorePageBase::getStaffItemListOrdered --
//


void ScorePageBase::getStaffItemListOrdered(vectorVSIp& data) {
   getStaffItemList(data);
   vectorVSIp::iterator it;
   for (it = data.begin(); it != data.end(); it++) {
      sort(it->begin(), it->end(), sortP3P4P1);
   }
}



//////////////////////////////
//
// ScorePageBase::itemChangeNotification -- When a ScoreItem in the
//     item_storage list changes one of its parameters, it will notify
//     the page that it has been changed.  The Page can then decide what,
//     if anything, should be done.
//
// Three messages known by this function:
//    "fixed" : a fixed parameter has been changed.
//    "named" : a named parameter has been changed.
//    "text"  : the fixed parameter text field has been changed.
//
//

void ScorePageBase::itemChangeNotification(ScoreItemBase* sitem, 
      const string& message ) {
   analysis_info.notmodified = 0;
}

void ScorePageBase::itemChangeNotification(ScoreItemBase* sitem, 
      const string& message, int index, SCORE_FLOAT oldp, SCORE_FLOAT newp ) {
   analysis_info.notmodified = 0;
   if (monitor_P3 && (index == P3) && (oldp != newp)) {
      // move item in sorted P3 list.
   }
}



//////////////////////////////
//
// ScorePageBase::clearAnalysisStates -- Set all anlaysis variables to
//     false.
//

void ScorePageBase::clearAnalysisStates(void) {
   analysis_info.clear();
}



