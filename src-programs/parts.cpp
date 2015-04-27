//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 26 19:29:25 PDT 2015
// Last Modified: Sun Apr 26 20:10:41 PDT 2015
// Filename:      parts.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-programs/parts.cpp
// Documentation: http://scorelib.sapp.org/program/parts
// Syntax:        C++ 11
//
// Description:   Extract/process part information in SCORE files.
//

#include "scorelib.h"

using namespace std;

void   processPage          (ScorePageSet& infiles, int page, Options& opts);
void   printPartInfo        (ScorePageSet& infiles, int page, Options& opts);
void   assignPartNumbers    (ScorePageSet& infiles, int page, Options& opts);
void   extractStaffItems    (ScorePageSet& infiles, int page, Options& opts);
void   printHeader          (ScorePageSet& infiles, int page, int onlypage);
void   generatePECF         (ScorePageSet& infiles);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("a|assign=b", "Assign part numbers to staves");
   opts.define("p|page=i:1", "Process only given page # (indexed from 1)");
   opts.define("t|test-assignment=b", "Test part assignment");
   opts.define("i|8|staff-items=b", "Extract staff items from page");
   opts.define("c|pecf|part-extractor-control-file=b", 
         "file needed for part extraction");
   opts.process(argc, argv);
   ScorePageSet infiles(opts);

   if (opts.getBoolean("page")) {
      processPage(infiles, opts.getInteger("page")-1, opts);
   } else if (opts.getBoolean("part-extractor-control-file")) {
      generatePECF(infiles);
   } else {
      for (int i=0; i<infiles.getPageCount(); i++) {
         processPage(infiles, i, opts);
      }
   }

   if (opts.getBoolean("assign")) {
      cout << infiles;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// generatePECF -- Create Part Extractor Control File.  This is used to
//     extract parts with the page.exe program.  See page 263 of the 
//     Score 3 reference manual.
//

void generatePECF(ScorePageSet& infiles) {
   int pagecount = infiles.getPageCount();
   if (pagecount < 1) {
      return;
   }
   string filebase = infiles.getPage(0)->getFilenameBase();
   string fileext   = infiles.getPage(0)->getFilenameExtension();
   if (fileext.size() == 0) {
      fileext = "mus";
   }
   cout << filebase << "." << fileext;
   cout << " ";
   if (pagecount < 10) {
      cout << "0";
   }
   cout << pagecount;
   cout << "01";
   cout << endl;
}



//////////////////////////////
//
// processPage -- Do something with a page based on the command-line
//     options.  If there is no optional action, then print the part
//     info for the pages.
//

void processPage(ScorePageSet& infiles, int page, Options& opts) {
   if (opts.getBoolean("assign")) {
      assignPartNumbers(infiles, page, opts);
   } else if (opts.getBoolean("staff-items")) {
      extractStaffItems(infiles, page, opts);
   } else {
      if (opts.getBoolean("test-assignment")) {
         assignPartNumbers(infiles, page, opts);
      }
      printPartInfo(infiles, page, opts);
   }
}



//////////////////////////////
//
// printPartInfo -- Print Part information for staves on the page.
//

void printPartInfo(ScorePageSet& infiles, int page, Options& opts) {
   printHeader(infiles, page, opts.getBoolean("page"));
   ScorePage& infile = *infiles.getPage(page);
   infile.analyzeSystems();

   const vectorVSIp& staffItems   = infile.getStaffItemList();
   vectorI&          staff2system = infile.systemMap();
   vectorI&          sysstaff     = infile.systemStaffMap();

   cout << "Staff\tSystem\tSysStaff\tPart\n";
   int lastsys = -1;
   int cursys = -1;
   for (int i=staffItems.size()-1; i>0; i--) {
      if (staffItems[i].size() == 0) {
         continue;
      }

      lastsys = cursys;
      cursys  = staff2system[i]+1;
      if ((lastsys > 0) && (cursys != lastsys)) {
         cout << "==========================================" << endl;
      }

      cout << i;
      cout << "\t";
      
      // System number, indexed from 1, starting at top of page:
      cout << staff2system[i] + 1;
      cout << "\t";

      // System Staff number, indexed from 1, starting at bottom of system:
      cout << sysstaff[i] + 1;
      cout << "\t";
      cout << "\t";  // extract tab for wide column heading

      // Part number from P9 of first staff object in staffItems[i] list.
      cout << staffItems[i][0]->getP9();

      cout << endl;

   }
}



//////////////////////////////
//
// printHeader -- Print a marker line for each page if there is
//   more than one page to process.
//

void printHeader(ScorePageSet& infiles, int page, int onlypage) {
   if (onlypage) {
      return;
   }

   string filename = infiles.getPage(page)->getFilename();
   if (infiles.getPageCount() > 1) {
      if (page > 0) {
         cout << endl;
      }
      cout << "# PAGE: " << page + 1;
      if (filename.size() > 0) {
         cout << "\tFILENAME: " << filename;
      }
      cout << endl;
   }
}



//////////////////////////////
//
// extractStaffItems -- Extract all P1=8 items on the page.
//

void extractStaffItems(ScorePageSet& infiles, int page, Options& opts) {
   printHeader(infiles, page, opts.getBoolean("page"));
   ScorePage& infile = *infiles.getPage(page);
   listSIp& data = infile.lowLevelDataAccess();

   for (auto it : data) {
      if (!it->isStaffItem()) {
         continue;
      }
      cout << *it;
   }
}



//////////////////////////////
//
// assignPartNumbers -- If P9 of staff is set to zero, then fill in
//   with the staff number of the current system, with the top staff
//   on the system being "1".  Automatic part assignment only works well 
//   if all parts are present in every system of the input data.
//

void assignPartNumbers(ScorePageSet& infiles, int page, Options& opts) {
   ScorePage& infile = *infiles.getPage(page);
   infile.analyzeSystems();

   // Get a list of the staff items on the page.  The list is two
   // dimensional: The first dimension is the P2 index value, the
   // second dimension is an index into the the list for the given
   // P2 index (in the rare case that there is more than one staff
   // item for a given staff position.
   vectorVSIp& staffItems = infile.getStaffItemListNotConst();

   // Get the mapping of systems/staves into page staves for the page.
   vectorVI& sysmap = infile.reverseSystemMap();
   
   // Now iterate through each system, and then each system-staff and
   // set the part number based on the system-staff (flipped and starting
   // at 1 rather than 0) for staff items on the page.
   int i, j, k, ksize;
 
   for (i=0; i<sysmap.size(); i++) {
      for (j=0; j<sysmap[i].size(); j++) {
         ksize = staffItems[sysmap[i][j]].size();
         for (k=0; k<ksize; k++) {
            if (staffItems[sysmap[i][j]][k]->getP9() != 0) {
               // don't reassign part numbers if they are already present.
               continue;
            }
            staffItems[sysmap[i][j]][k]->setP9(sysmap[i].size() - j);
         }
      }
   }
}



