//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Apr 26 21:16:35 PDT 2015
// Last Modified: Sun Apr 26 21:16:37 PDT 2015
// Filename:      durations.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-programs/durations.cpp
// Documentation: http://scorelib.sapp.org/program/durations
// Syntax:        C++ 11
//
// Description:   Extract/process duration information in SCORE files.
//

#include "scorelib.h"

using namespace std;

void   processPage          (ScorePageSet& infiles, int page, Options& opts);
void   printPartInfo        (ScorePageSet& infiles, int page, Options& opts);
void   printHeader          (ScorePageSet& infiles, int page, int onlypage);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("a|assign=b", "Assign duration information to input data");
   opts.process(argc, argv);
   ScorePageSet infiles(opts);

   infiles.analyzeStaffDurations();
   infiles.analyzePageSetDurations();

   for (int i=0; i<infiles.getPageCount(); i++) {
      processPage(infiles, i, opts);
   }

   if (opts.getBoolean("assign")) {
      cout << infiles;
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// processPage -- Do something with a page based on the command-line
//     options.  If there is no optional action, then print the part
//     info for the pages.
//

void processPage(ScorePageSet& infiles, int page, Options& opts) {
   if (opts.getBoolean("assign")) {
      // do nothing
   } else {
      printPartInfo(infiles, page, opts);
   }
}



//////////////////////////////
//
// printPartInfo -- Print Part information for staves on the page.
//

void printPartInfo(ScorePageSet& infiles, int page, Options& opts) {
   printHeader(infiles, page, 0);
   ScorePage& infile = *infiles.getPage(page);
   infile.analyzeSystems();

   const vectorVSIp& staffItems   = infile.getStaffItemList();
   vectorI&          staff2system = infile.systemMap();
   vectorI&          sysstaff     = infile.systemStaffMap();

   cout << "Staff\tSystem\tSysStaff\tDuration (quarter notes)\n";
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
      cout << infile.getStaffDuration(i);

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


