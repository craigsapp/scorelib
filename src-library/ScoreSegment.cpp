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
}



ScoreSegment::ScoreSegment(ScorePageSet& pageset, SystemAddress& starting, 
      SystemAddress& ending, int debug) {

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
   int i;
   for (i=0; i<part_storage.size(); i++) {
      delete part_storage[i];
      part_storage[i] = 0;
   }
   part_storage.resize(0);
}



//////////////////////////////
//
// ScoreSegment::defineSegment --
//

void ScoreSegment::defineSegment(ScorePageSet& pageset, 
      SystemAddress& starting, SystemAddress& ending, int debug) {
   start_system = starting;
   end_system   = ending;

   if (debug) {
      cout << "DEFINING SEGMENT " << starting << " TO " << ending << endl;
   }

   vector<int> partlist;
   getPartList(partlist, pageset, start_system, end_system);

//   if (debug) {
      cout << "List of instrument numbers in segment: ";
      for (auto& it : partlist) {
         cout << it << " ";
      }
      cout << endl;
//   }
//   ggg

   analyzePartStaves(partlist, pageset, start_system, end_system);
}



//////////////////////////////
//
// ScoreSegment::analyzePartStaves --
//

void ScoreSegment::analyzePartStaves(vector<int>& partlist, ScorePageSet&
     pageset, SystemAddress& startsys, SystemAddress& endsys) {

   SystemAddress& sp = startsys;
   SystemAddress& ep = endsys;

   map<int,int> reverselist;
   for (int i=0; i<partlist.size(); i++) {
      reverselist[partlist[i]] = i;
   }

   vector<int> counter;
   counter.resize(partlist.size());

   int p;
   int overlay = 0; // make variable later.
   int sysstart;
   int sysend;
   int partnum;
   int staffcount;
   int partindex;
   int partcount;
   string partmarker;

   for (p = sp.getPage(); p <= ep.getPage(); p++) {
      sysstart = 0;
      if (p == sp.getPage()) {
         sysstart = sp.getSystem();
      }
      sysend = pageset[p][overlay].getSystemCount() - 1;
      if (p == ep.getPage()) {
         if (ep.getSystem() >= 0) {
            sysend = ep.getSystem();
         }
      }
      ScorePage& page = pageset[p][overlay];
      vectorVVSIp& staffitems = page.getP8BySystem();
      for (int sys = sysstart; sys <= sysend; sys++) {
         fill(counter.begin(), counter.end(), 0);
         staffcount = staffitems[sys].size();
         for (int sysstaff = 0; sysstaff < staffcount; sysstaff++) {
            if ((p == sp.getPage()) && (sys == sp.getSystem())) {
               staffitems[sys][sysstaff][0]->setParameter("segment", 
                     "start", "true");
            }
            if ((p == ep.getPage()) && (sys == ep.getSystem())) {
               staffitems[sys][sysstaff][0]->setParameter("segment", 
                     "end", "true");
            }
            if (staffitems[sys][sysstaff].size() > 0) {
               partnum = staffitems[sys][sysstaff][0]->getPartNumberInt();
            } else {
               partnum = 0;
            }
            partindex = reverselist[partnum];
            partcount = counter[partindex]++;
            partmarker = to_string(partindex) + "." + to_string(partcount);
            staffitems[sys][sysstaff][0]->setParameter("segment", 
               "partstaff", partmarker);
         }
      }
   }
}



//////////////////////////////
//
// ScoreSegment::getPartList -- return a sorted list of the part
//   numbers extracted from staff items.  Currently only processing
//   the first page in ScorePageOverlay objects.
//
//   If the partnumber of a staff is zero, then an automatic identification of 
//   parts by system staff number will be used.  The bottom staff will be labeled
//   as part -1, the next higher staff will be part -2, and so on.  If part numbers
//   (P9|P1=8) are zero, then it is expected that the staff number for each system
//   is constant (otherwise you should number the staves by parts).  If the staff
//   counts are not constant, then the staves will be assumed to be removed starting
//   from the top.  So if there are nominally 4 staves/system and there is a system
//   with three staves, and all staves are labeled as part 0, then the top staff on the
//   4-staff system will be assumed to be removed in the 3-staff system.
//

void ScoreSegment::getPartList(vector<int>& partlist, ScorePageSet& pageset, 
      SystemAddress& sp, SystemAddress& ep) {

   partlist.resize(0);
   set<int> partnums;
   int p;
   int startsys;
   int endsys;
   int staff;
   int overlay = 0;  // make variable later.
   int partnum;
   int zeropart = 0;

   for (p = sp.getPage(); p <= ep.getPage(); p++) {
      startsys = 0;
      if (p == sp.getPage()) {
         startsys = sp.getSystem();
      }
      endsys = pageset[p][overlay].getSystemCount() - 1;
      if (p == ep.getPage()) {
         if (ep.getSystem() >= 0) {
            endsys = ep.getSystem();
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

const SystemAddress& ScoreSegment::getBeginSystem(void) const {
   return start_system;
}

//
// Alias: 
//

const SystemAddress& ScoreSegment::getStartSystem(void) const {
   return getBeginSystem();
}



//////////////////////////////
//
// ScoreSegment::getBeginSystem -- return the first system in a ScorePageSet which defines
//      the segment.
//

const SystemAddress& ScoreSegment::getEndSystem(void) const {
   return end_system;
}



//////////////////////////////
//
// ScoreSegment::printInfo --
//

ostream& ScoreSegment::printInfo(ostream& out) const {
   out << "Start_system:\t" << getStartSystem() << endl;
   out << "Start_system:\t" << getEndSystem() << endl;
   out << "Part_count:\t" << getPartCount() << endl;

   return out;
}



//////////////////////////////
//
// ScoreSegment::getPartCount --
//

int ScoreSegment::getPartCount(void) const {
   return part_storage.size();
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& out, const ScoreSegment& segment) {
   return segment.printInfo(out);
}



