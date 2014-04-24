//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 13 22:26:50 PDT 2014
// Last Modified: Sun Apr 13 22:26:53 PDT 2014
// Filename:      SegmentPart.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/SegmentPart.h
// Syntax:        C++11
//
// Description:   Used to store the system staff sequence for a part in a
//                ScoreSegment;
//

#include "SegmentPart.h"

using namespace std;


//////////////////////////////
//
// SegmentPart::SegmentPart --
//

SegmentPart::SegmentPart(void) { 
   address_storage.reserve(2000);
   pageset_owner = NULL;
}



//////////////////////////////
//
// SegmentPart::~SegmentPart --
//

SegmentPart::~SegmentPart() { 
   clear();
   setOwner(NULL);
}



//////////////////////////////
//
// SegmentPart::clear --
//

void SegmentPart::clear(void) {
   int i, j;
   for (i=0; i<address_storage.size(); i++) {
      for (j=0; j<address_storage[i].size(); j++) {
         delete address_storage[i][j];
         address_storage[i][j] = NULL;
      }
      address_storage.resize(0);
   }
}



//////////////////////////////
//
// SegmentPart::getAddress --
//

SystemAddress& SegmentPart::getAddress(int index, int index2) { 
   return *(address_storage[index][index2]);
}



//////////////////////////////
//
// SegmentPart::getAddresses --
//

const vectorVSAp& SegmentPart::getAddresses(void) const {
   return address_storage;
}



//////////////////////////////
//
// SegmentPart::getAddressCount. --
//

int SegmentPart::getAddressCount(void) const {
   return address_storage.size();
}



//////////////////////////////
//
// SegmentPart::getScoreOverlay --
//

ScorePageOverlay& SegmentPart::getScoreOverlay(int index, int index2) { 
   ScorePageSet& sps = *getOwner();
   return sps[address_storage[index][index2]->getPage()];
}



//////////////////////////////
//
// SegmentPart::getScorePage --
//

ScorePage& SegmentPart::getScorePage(int index, int index2) { 
   ScorePageSet& sps = *getOwner();
   ScorePageOverlay& so = sps[address_storage[index][index2]->getPage()];
   return so[address_storage[index][index2]->getOverlay()];
}



//////////////////////////////
//
// SegmentPart::getScorePageIndex --
//

int SegmentPart::getScorePageIndex(int index, int index2) { 
   return address_storage[index][index2]->getPage();
}



//////////////////////////////
//
// SegmentPart::getPageSystemIndex --
//

int SegmentPart::getPageSystemIndex(int index, int index2) { 
   return address_storage[index][index2]->getSystem();
}



//////////////////////////////
//
// SegmentPart::getSystemStaffIndex --
//

int SegmentPart::getSystemStaffIndex(int index, int index2) { 
   return address_storage[index][index2]->getSystemStaff();
}



//////////////////////////////
//
// SegmentPart::systemCount --
//

int SegmentPart::systemCount(void) { 
   return address_storage.size();
}



//////////////////////////////
//
// SegmentPart::setOwner -- set the ScorePageSet owner.
//

void SegmentPart::setOwner(ScorePageSet* owner) {
   pageset_owner = owner;
}



//////////////////////////////
//
// SegmentPart::hasOwner --
//

int SegmentPart::hasOwner(void) {
   if (pageset_owner == NULL) {
      return 0;
   } else {
      return 1;
   }
}



//////////////////////////////
//
// SegmentPart::getOwner -- get the ScorePageSet owner.
//

ScorePageSet* SegmentPart::getOwner(void) {
   return pageset_owner;
}



//////////////////////////////
//
// SegmentPart::appendAddress --
//

void SegmentPart::appendAddress(SystemAddress& anAddress) { 
   SystemAddress* sa = new SystemAddress(anAddress);
   address_storage.resize(address_storage.size()+1); 
   address_storage.back().push_back(sa);
}



//////////////////////////////
//
// SegmentPart::addAddress --
//

void SegmentPart::addAddress(SystemAddress& anAddress, int index) {
   SystemAddress* sa = new SystemAddress(anAddress);
   address_storage[index].push_back(sa);
}



//////////////////////////////
//
// SegmentPart::addToLastAddress --
//

void SegmentPart::addToLastAddress(SystemAddress& anAddress) {
   SystemAddress* sa = new SystemAddress(anAddress);
   if (address_storage.size() == 0) {
      cerr << "Cannot append to last element because size is zero" << endl;
      exit(1);
   }
   address_storage.back().push_back(sa);
}



//////////////////////////////
//
// SegmentPart::setPartIndex --
//

void SegmentPart::setPartIndex(unsigned int index) {
   part_index = index;
}


//////////////////////////////
//
// SegmentPart::setPartNumber --
//

void SegmentPart::setPartNumber(int number) {
   part_number = number;
}


//////////////////////////////
//
// SegmentPart::getPartIndex --
//

unsigned int SegmentPart::getPartIndex(void) const {
   return part_index;
}


//////////////////////////////
//
// SegmentPart::getPartNumber --
//

int SegmentPart::getPartNumber(void) const {
   return part_number;
}


//////////////////////////////
//
// SegmentPart::setPartName --
//

void SegmentPart::setPartName(const string& name) {
   part_name = name; 
}


//////////////////////////////
//
// SegmentPart::getPartName --
//

const string& SegmentPart::getPartName(void) const {
   return part_name;
}



//////////////////////////////
//
// operator<< -- Print information about SegmentParts.
//

ostream& operator<<(ostream& out, const SegmentPart& part) {
   if (part.getPartName() != "") {
      out << "Part name:\t" << part.getPartName() << endl;
   }
   out << "Part index:\t"  << part.getPartIndex()  << endl;
   out << "Part number:\t" << part.getPartNumber() << endl;
   int i, j;
   out << "Part staff addresses:" << endl;
   for (i=0; i<part.getAddresses().size(); i++) {
      out << "\t";
      if (part.getAddresses()[i].size() > 1) {
         out << "{ ";
      }
      for (j=0; j<part.getAddresses()[i].size(); j++) {
         out << *part.getAddresses()[i][j];
         if (j < part.getAddresses()[i].size() - 1) {
            out << ", ";
         } 
      }
      if (part.getAddresses()[i].size() > 1) {
         out << " }";
      }
      out << endl;
   }
   
   return out;
}



