//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Mar  7 23:20:33 PST 2014
// Last Modified: Fri Mar  7 23:20:35 PST 2014
// Filename:      DatabaseAnalysis.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/DatabaseAnalysis.cpp
// Syntax:        C++11
//
// Description:   Keeps tracks of dependencies of analysis functions in
//                ScorePageBase class.
//

#include "DatabaseAnalysis.h"
#include <iostream>

using namespace std;


//////////////////////////////
//
// DatabaseAnalysis::DatabaseAnalysis -- Constructor.
//

DatabaseAnalysis::DatabaseAnalysis(void) { 
   // do nothing
}



//////////////////////////////
//
// DatabaseAnalysis::~DatabaseAnalysis -- Deconstructor.
//

DatabaseAnalysis::~DatabaseAnalysis() { 
   // do nothing
}
   


//////////////////////////////
//
// DatabaseAnalysis::addNode --
//

void DatabaseAnalysis::addNode(const string& nodename, int* state) { 
   nodes[nodename].data = state;
}



//////////////////////////////
//
// DatabaseAnalysis::addChild --
//

void DatabaseAnalysis::addChild(const string& parent, const string& child, 
      int* cstate) {
   nodes[child].data = cstate;
   nodes[child].parents.push_back(parent);
   nodes[parent].children.push_back(child);
}



//////////////////////////////
//
// DatabaseAnalysis::invalidateNode --  Set data for this node
//    and all of its children to 0.  Do not examine the children
//    of a node which is already set to 0 (to prevent possibility
//    of recursive loops).
//

void DatabaseAnalysis::invalidateNode(const string& nodename) { 
   auto entry = nodes.find(nodename);
   if (entry == nodes.end()) {
      cerr << "Searching for an undefined node: " << nodename << endl;
      exit(1);
   }
   *(entry->second.data) = 0;

   for (auto& it : entry->second.children) {
      invalidateNode(it);
   }
}



//////////////////////////////
//
// DatabaseAnalysis::validateNode --  Set data for this node
//    to 1 and don't modifiy any of its children.  
//

void DatabaseAnalysis::validateNode(const string& nodename) { 
   *(nodes[nodename].data) = 1;
}



