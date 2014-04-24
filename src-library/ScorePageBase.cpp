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
#include "ScoreUtility.h"
#include <algorithm>

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
   readStream(instream);
}


ScorePageBase::ScorePageBase(const ScorePageBase& apage) { 
   // make a copy of an existing page
   // have to deal with unique ids...
   trailer.reserve(8);
   clearAnalysisStates();

   print_info = apage.print_info;
   
   const listSIp& itemlist = apage.item_storage;
   ScoreItem* sip;
   for (auto& it : itemlist) {
      if (it != NULL) {
         sip = new ScoreItem(*it);
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
   for (auto& it : item_storage) {
      if (it != NULL) {
         delete it;
         it = NULL;
      }
   }
   item_storage.resize(0);

   for (auto& it : measure_storage) {
      if (it != NULL) {
         delete it;
         it = NULL;
      }
   }
   measure_storage.resize(0);
   system_measures.resize(0);
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
// ScorePageBase::lowLevelDataAccess -- Direct access to the items on a page.
//     If you change these items through this access point, analyses will be
//     invalid, but the page will not know that they are invalid, so be
//     careful.
//

listSIp& ScorePageBase::lowLevelDataAccess(void) {
   return item_storage;
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
   for (auto& it : item_storage) {
      data.push_back(it);
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
   unsigned int staffnum;
   for (auto& it : item_storage) {
      staffnum = it->getStaffNumber();
      if (data.size() < staffnum + 1) {
         data.resize(staffnum+1);
      }
      data[staffnum].push_back(it);
   }
}



//////////////////////////////
//
// ScorePageBase::getStaffItemListOrdered --
//


void ScorePageBase::getStaffItemListOrdered(vectorVSIp& data) {
   getStaffItemList(data);
   for (auto& it : data) {
      sort(it.begin(), it.end(), SU::sortP3P4P1);
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
   analysis_info.invalidateModified();
}

void ScorePageBase::itemChangeNotification(ScoreItemBase* sitem, 
      const string& message, int index, SCORE_FLOAT oldp, SCORE_FLOAT newp ) {
   analysis_info.invalidateModified();
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



//////////////////////////////
//
// ScorePageBase::deleteNamespace --
//

void ScorePageBase::deleteNamespace(const string& nspace) {
   for (auto& it : item_storage) {
      it->deleteNamespace(nspace);
   }
}



//////////////////////////////
//
// ScorePageBase::changeNamespace --  These functions will overwrite
//     parameters in the new namespace.  Maybe add a set of functions
//     which do not overwrite if parameter already exists in new namespace.
//     Also a copyNamespace function might be good.  Returns the number
//     of items which had the paramter moved to another namespace.
//

int ScorePageBase::changeNamespace(const string& newspace, 
      const string& oldspace, const string& parameter) {
   int count = 0;
   for (auto& it : item_storage) {
      if (it->isDefined(oldspace, parameter)) {
         count += it->changeNamespace(newspace, oldspace, parameter);
      }
   }
   return count;
}


int ScorePageBase::changeNamespace(const string& newspace, 
      const string& oldspace, const string& parameter, int itemtype) {
   int count = 0;
   for (auto& it : item_storage) {
      if (itemtype != it->getItemType()) {
         continue;
      }
      if (it->isDefined(oldspace, parameter)) {
         count += it->changeNamespace(newspace, oldspace, parameter);
      }
   }
   return count;
}



