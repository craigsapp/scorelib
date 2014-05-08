//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Apr 14 21:40:55 PDT 2014
// Last Modified: Mon Apr 14 21:40:58 PDT 2014
// Filename:      ScoreSegment.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreSegment.h
// Syntax:        C++11
//
// Description:   Used to keep track of parts in the given segmentation
//                of pages in a ScorePageSet.
//

#include "ScoreSegment.h"
#include "SegmentPart.h"
#include <set>
#include <algorithm>

using namespace std;


//////////////////////////////
//
// ScoreSegment::ScoreSegment -- Constructor.
//

ScoreSegment::ScoreSegment(void) {
   start_system.clear();
   end_system.clear();
   pageset_owner = NULL;
}


ScoreSegment::ScoreSegment(ScorePageSet& pageset, AddressSystem& starting, 
      AddressSystem& ending, int debug) {

   defineSegment(pageset, starting, ending, debug);
}



//////////////////////////////
//
// ScoreSegment::~ScoreSegment -- Destructor.
//

ScoreSegment::~ScoreSegment() {
   clear();
}



//////////////////////////////
//
// ScoreSegment::clear --
//

void ScoreSegment::clear(void) {
   start_system.clear();
   end_system.clear();
   clearPartStorage();
   setOwner((ScorePageSet*)NULL);
}



//////////////////////////////
//
// ScoreSegment::clearPartStorage -- delete the contents of the part storage
//      array, but do not clear the start/end system information.
//

void ScoreSegment::clearPartStorage(void) {
   for (int i=0; i<part_storage.size(); i++) {
      delete part_storage[i];
      part_storage[i] = 0;
   }
   part_storage.resize(0);
}



//////////////////////////////
//
// ScoreSegment::setOwner --
//

void ScoreSegment::setOwner(ScorePageSet* owner) {
   pageset_owner = owner;
}



//////////////////////////////
//
// ScoreSegment::defineSegment --
//

void ScoreSegment::defineSegment(ScorePageSet& pageset, 
      AddressSystem& starting, AddressSystem& ending, int debug) {
   pageset_owner = &pageset;
   start_system = starting;
   end_system   = ending;

   if (debug) {
      cout << "DEFINING SEGMENT " << starting << " TO " << ending << endl;
   }

   vectorI partlist;
   getPartList(partlist, pageset, start_system, end_system);

   if (debug) {
      cout << "List of instrument numbers in segment: ";
      for (auto& it : partlist) {
         cout << it << " ";
      }
      cout << endl;
   }

   analyzePartStaves(partlist, pageset, start_system, end_system);
}



//////////////////////////////
//
// ScoreSegment::analyzePartStaves --
//

void ScoreSegment::analyzePartStaves(vectorI& partlist, ScorePageSet&
     pageset, AddressSystem& startsys, AddressSystem& endsys) {

   AddressSystem& sp = startsys;
   AddressSystem& ep = endsys;

   map<int,int> reverselist;
   clearPartStorage();
   part_storage.resize(partlist.size());
   fill(part_storage.begin(), part_storage.end(), (SegmentPart*)NULL);

   for (int i=0; i<partlist.size(); i++) {
      reverselist[partlist[i]] = i;
      SegmentPart* spart = new SegmentPart;
      spart->setPartIndex(i);
      spart->setPartNumber(partlist[i]);
      spart->setPartName(extractPartName(pageset, startsys, partlist[i]));
      spart->setOwner(&pageset);
      part_storage[i] = spart;
   }

   // store part information

   vectorI counter;
   counter.resize(partlist.size());

   int p;
   int overlay = 0; // make variable later.
   int sysstart;
   int sysend;
   int partnum;
   int staffcount;
   int partindex;
   string partmarker;
   int zerocounter = 0;

   int partcount = part_storage.size();
   vectorI foundpart;
   foundpart.resize(partcount);
   AddressSystem current;

   // Label the part index for each staff in segment.
   for (p = sp.getPageIndex(); p <= ep.getPageIndex(); p++) {
      sysstart = 0;
      if (p == sp.getPageIndex()) {
         sysstart = sp.getSystemIndex();
      }
      sysend = pageset[p][overlay].getSystemCount() - 1;
      if (p == ep.getPageIndex()) {
         if (ep.getSystemIndex() >= 0) {
            sysend = ep.getSystemIndex();
         }
      }
      ScorePage& page = pageset[p][overlay];
      vectorVVSIp& staffitems = page.getP8BySystem();
      for (int sys = sysstart; sys <= sysend; sys++) {
         fill(counter.begin(), counter.end(), 0);
         fill(foundpart.begin(), foundpart.end(), 0);
         staffcount = staffitems[sys].size();
         zerocounter = 0;
         for (int sysstaff = 0; sysstaff < staffcount; sysstaff++) {
            if ((p == sp.getPageIndex()) && (sys == sp.getSystemIndex())) {
               staffitems[sys][sysstaff][0]->setParameter("segment", 
                     "start", "true");
            }
            if ((p == ep.getPageIndex()) && (sys == ep.getSystemIndex())) {
               staffitems[sys][sysstaff][0]->setParameter("segment", 
                     "end", "true");
            }
            if (staffitems[sys][sysstaff].size() > 0) {
               partnum = staffitems[sys][sysstaff][0]->getPartNumberInt();
               if (partnum == 0) {
                  partnum = --zerocounter;
               }
            } else {
               partnum = 0;
            }
            if ((p == sp.getPageIndex()) && (sys == sp.getSystemIndex())) {
               if (staffitems[sys][sysstaff][0]->isDefined("partname")) {
                  string partname = 
                     staffitems[sys][sysstaff][0]->getParameter("partname");
                  if (partname != "") {
                     staffitems[sys][sysstaff][0]->setParameter("segment", 
                              "partname", partname);
                  }
               } else {
                  string partname = 
                        part_storage[reverselist[partnum]]->getPartName();
                  if (partname != "") {
                     staffitems[sys][sysstaff][0]->setParameter("segment", 
                           "partname", partname);
                  }
               }
            }
            partindex = reverselist[partnum];
            partcount = counter[partindex]++;
            partmarker = to_string(partindex) + "." + to_string(partcount);
            staffitems[sys][sysstaff][0]->setParameter("segment", 
               "partstaff", partmarker);

            // Store the system addresses for each system for each part.
            current.setPage(p);
            current.setOverlay(0);
            current.setSystem(sys);
            current.setSystemStaff(sysstaff);
            foundpart[partindex] = 1;
            if (partcount == 0) {
               part_storage[partindex]->appendAddress(current);
            } else {
               part_storage[partindex]->addToLastAddress(current);
            }
         }
         // For all parts which were not found, add empty address for system.
         for (int i=0; i<foundpart.size(); i++) {
            if (foundpart[i]) {
               continue;
            }
            current.setSystemStaff(-1);
            part_storage[i]->appendAddress(current);
         }
      }
   }
}



//////////////////////////////
//
// ScoreSegment::extractPartName -- Extract text in front of a staff.  Expand 
//      in the future to deal with instrument name split onto two (or more 
//      lines).
//

string ScoreSegment::extractPartName(ScorePageSet& pageset, 
      AddressSystem& startsys, int partindex) {
   int pageindex = startsys.getPageIndex();
   int overlay = 0;
   ScorePage& page = pageset[pageindex][overlay];
   int sysindex = startsys.getSystemIndex();
   int pagestaff = page.getPageStaffIndex(sysindex, partindex);
   vectorSIp staffitems;
   page.getSortedStaffItems(staffitems, pagestaff);
   double p4;
   for (int i=0; i<staffitems.size(); i++) {
      if (staffitems[i]->isStaffItem()) {
         break;
      }
      if (!staffitems[i]->isTextItem()) {
         continue;
      }
      p4 = staffitems[i]->getVPos();
      if (p4 > 9) {
         continue;
      }
      if (p4 < 1) {
         continue;
      }
      return staffitems[i]->getTextNoFont();
   }
   return "";
}


//////////////////////////////
//
// ScoreSegment::getSystemStaffIndex --
//

int ScoreSegment::getSystemStaffIndex(int systemindex, int partindex, 
      int subpartindex) {
   return part_storage[partindex]->getSystemStaffIndex(systemindex, 
      subpartindex);
}



//////////////////////////////
//
// ScoreSegment::getPageStaffIndex -- Get the P2 page staff number
//    for the part on the given system in the segment.
//

int ScoreSegment::getPageStaffIndex(int systemindex, int partindex, 
      int subpartindex) {
   return part_storage[partindex]->getPageStaffIndex(systemindex,
      subpartindex); 
}



//////////////////////////////
//
// ScoreSegment::getPartList -- return a sorted list of the part
//   numbers extracted from staff items.  Currently only processing
//   the first page in ScorePageOverlay objects.
//
//   If the partnumber of a staff is zero, then an automatic identification of 
//   parts by system staff number will be used.  The bottom staff will be 
//   labeled as part -1, the next higher staff will be part -2, and so on.  
//   If part numbers (P9|P1=8) are zero, then it is expected that the staff 
//   number for each system is constant (otherwise you should number the 
//   staves by parts).  If the staff counts are not constant, then the 
//   staves will be assumed to be removed starting from the top.  So if 
//   there are nominally 4 staves/system and there is a system with three 
//   staves, and all staves are labeled as part 0, then the top staff on the
//   4-staff system will be assumed to be removed in the 3-staff system.
//

void ScoreSegment::getPartList(vectorI& partlist, ScorePageSet& pageset, 
      AddressSystem& sp, AddressSystem& ep) {

   partlist.resize(0);
   set<int> partnums;
   int p;
   int startsys;
   int endsys;
   int staff;
   int overlay = 0;  // make variable later.
   int partnum;
   int zeropart = 0;

   for (p = sp.getPageIndex(); p <= ep.getPageIndex(); p++) {
      startsys = 0;
      if (p == sp.getPageIndex()) {
         startsys = sp.getSystemIndex();
      }
      endsys = pageset[p][overlay].getSystemCount() - 1;
      if (p == ep.getPageIndex()) {
         if (ep.getSystemIndex() >= 0) {
            endsys = ep.getSystemIndex();
         }
      }
      ScorePage& page = pageset[p][overlay];
      vectorVVSIp& staffitems = page.getP8BySystem();
      for (int sys = startsys; sys <= endsys; sys++) {
         if (sys >= staffitems.size()) {
            break;
         }
         zeropart = 0;
         for (staff=0; staff<staffitems[sys].size(); staff++) {
            if (staffitems[sys][staff].size() == 0) {
               continue;
            } 
            partnum = staffitems[sys][staff][0]->getPartNumber();
            if (partnum == 0) {
               partnum = --zeropart;
            }
            partnums.insert(partnum);
         }
      }
   }

   partlist.resize(0);
   partlist.reserve(partnums.size());
   for (auto& it : partnums) {
      partlist.push_back(it);
   }   
}



//////////////////////////////
//
// ScoreSegment::getBeginSystem -- return the first system in a ScorePageSet which defines
//      the segment.
//

const AddressSystem& ScoreSegment::getBeginSystem(void) const {
   return start_system;
}

//
// Alias: 
//

const AddressSystem& ScoreSegment::getStartSystem(void) const {
   return getBeginSystem();
}



//////////////////////////////
//
// ScoreSegment::getBeginSystem -- return the first system in a ScorePageSet 
//      which defines the segment.
//

const AddressSystem& ScoreSegment::getEndSystem(void) const {
   return end_system;
}



//////////////////////////////
//
// ScoreSegment::printInfo --
//

ostream& ScoreSegment::printInfo(ostream& out) const {
   out << "Start system:\t" << getStartSystem() << endl;
   out << "End system:\t"   << getEndSystem()   << endl;
   out << "Part count:\t"   << getPartCount()   << endl;
   out << "System count:\t" << getSystemCount() << endl;
   for (int i=0; i<getPartCount(); i++) {
      out << "\nPart " << i << endl;
      out << *part_storage[i];
   }

   return out;
}



//////////////////////////////
//
// ScoreSegment::getPartCount -- Return the number of parts in the segment.
//

int ScoreSegment::getPartCount(void) const {
   return part_storage.size();
}



//////////////////////////////
//
// ScoreSegment::getSystemCount -- Return the number of systems
//    in the segment.  All parts are required to have the same number
//    of systems.  If not then something strange is happening.  If a
//    part is not found on a particular system, it should still have
//    an entry of -1 representing that it is resting in that system.
//

int ScoreSegment::getSystemCount(void) const {
   if (part_storage.size() == 0) {
      return 0;
   }
   return part_storage[0]->getAddresses().size();
}



//////////////////////////////
//
// ScoreSegment::getSystemAddress -- Return the requested system address.
//     All parts are expected to have the same number of systems
//     and they line up exactly.
//

const AddressSystem& ScoreSegment::getSystemAddress(int index) {
   return part_storage[0]->getAddress(index);
}

//
// Alias:
//

AddressSystem& ScoreSegment::getPartAddress(int systemindex, int partindex) {
   return part_storage[partindex]->getAddress(systemindex);
}



//////////////////////////////
//
// ScoreSegment::getSystemAddresses --
//

const vectorVASp& ScoreSegment::getSystemAddresses(int partindex) {
   return part_storage[partindex]->getSystemAddresses();
}



//////////////////////////////
//
// ScoreSegment::getSystemItems --
//

vectorSIp& ScoreSegment::getSystemItems(const AddressSystem& address) {
   if (pageset_owner == NULL) {
      cerr << "Error: cannot access NULL scoreset." << endl;
      exit(1);
   }
   return pageset_owner->getSystemItems(address);
}


vectorSIp& ScoreSegment::getSystemItems(int sysindex) {
   const AddressSystem& address = getSystemAddress(sysindex);
   return getSystemItems(address);
}



//////////////////////////////
//
// ScoreSegment::getPartName -- 
//

string ScoreSegment::getPartName(int partindex) {
   if ((partindex < 0) || (partindex >= part_storage.size())) {
      return "";
   } else {
      return part_storage[partindex]->getPartName();
   }
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& out, const ScoreSegment& segment) {
   return segment.printInfo(out);
}
