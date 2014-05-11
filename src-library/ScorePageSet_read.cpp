//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr  8 15:30:55 PDT 2014
// Last Modified: Tue Apr  8 15:42:02 PDT 2014
// Filename:      ScorePageSet_read.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageSet_read.cpp
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#include "ScorePageSet.h"
#include <regex>
#include <sstream>

using namespace std;


//////////////////////////////
//
// ScorePageSet::appendReadFromOptionArguments -- Read earch argument
//    of the option class as a file.  The file may be in binary, 
//    PMX or XML formats, and each file may contain one or more 
//    pages/overlays.
//

void ScorePageSet::appendReadFromOptionArguments(Options& opts) {
   if (opts.getArgumentCount() == 0) {
      appendRead(cin, "<stdin>");
      return;
   }
   for (int i=1; i<=opts.getArgumentCount(); i++) {
      appendRead(opts.getArgument(i));
   }

   setPageOwnerships();
}

//
// Alias:
//

void ScorePageSet::read(Options& opts) {
   appendReadFromOptionArguments(opts);
}



//////////////////////////////
//
// ScorePageSet::appendRead --
//

void ScorePageSet::appendRead(const string& filename) {
   int binaryQ = 0;  // to test if reading a binary or PMX data file.

   ifstream testfile(filename);

   if (!testfile.is_open()) {
      cerr << "Error: cannot read the file: " << filename << endl;
      exit(1);
   }

   // The last 4 bytes of a binary SCORE file are 00 3c 1c c6 which equals
   // the float value -9999.0.
   testfile.seekg(-4, ios::end);
   unsigned char databytes[4] = {0xff};
   testfile.read((char*)databytes, 4);
   if (databytes[0] == 0x00 && databytes[1] == 0x3c && databytes[2] == 0x1c &&
         databytes[3] == 0xc6) {
      binaryQ = 1;
   } else {
      binaryQ = 0;
   }
   testfile.seekg(0, ios::beg);

   if (binaryQ) {
      appendReadBinary(testfile, filename);
   } else {
      appendReadPmx(testfile, filename);
   }
   setPageOwnerships();
}
   


//////////////////////////////
//
// ScorePageSet::appendReadStandardInput --
//

void ScorePageSet::appendReadStandardInput(void) {
   appendRead(cin, "<stdin>");
   setPageOwnerships();
}



//////////////////////////////
//
// ScorePageSet::appendRead -- Read a stream of data.  For now, assume
//      that the input is PMX data.
//

void ScorePageSet::appendRead(istream& instream, const string& filename) {
   appendReadPmx(instream, filename);   
   setPageOwnerships();
}



//////////////////////////////
//
// ScorePageSet::appendReadBinary --
//

void ScorePageSet::appendReadBinary(istream& instream, const string& filename) {
   ScorePage* pageptr = new ScorePage;
   pageptr->read(instream);
   pageptr->setFilename(filename);
   appendPage(pageptr);
   setPageOwnerships();
}



//////////////////////////////
//
// ScorePageSet::appendReadPmx -- Read potentially multiple pages and
//     overlays of ASCII PMX data from an input stream.
//

void ScorePageSet::appendReadPmx(istream& instream, const string& filename,
      const string& pagetype) {

   int dataQ        = 0;
   int pagestart    = 0;
   int overlaystart = 0;
   int nextpage     = 0;
   int nextoverlay  = 0;

   if (pagetype == "page") {
      pagestart = 1;
   } else if (pagetype == "overlay") {
      overlaystart = 1;
   }

   stringstream data;  // temporary storage of file contents to be parsed
   string testname; 
   string transfer;
   string nextfilename = "UNDEFINED";
   string localtype    = pagetype;
   string localfile    = filename;
   regex  pmxdataline  (R"(^\s*[\dtT+-])");
   regex  startpage    (  "###ScorePage");
   regex  spname       (R"(###ScorePage:\s*([^\s]+))");
   regex  overlaypage  (  "###ScoreOverlay");
   regex  opname       (R"(###ScoreOverlay:\s*([^\s]+))");
   smatch match;

   while (getline(instream, transfer)) {
      if (instream.eof()) {
         break;
      }

      if (dataQ == 0) {
         if (regex_search(transfer, pmxdataline)) {
            dataQ = 1;
         }
      }

      if (regex_search(transfer, startpage)) {
         regex_search(transfer, match, spname);
         if (match.size() >= 2) {
            testname = match[1].str();
         } else {
            testname = "";
         }

         if (dataQ) {
            // Already have PMX data, so store that with the previously
            // given filename.
            nextpage    = 1;
            nextoverlay = 0;
            nextfilename = testname;
            break;
         } else {
            // Don't have PMX data so store the newly read filename
            // for the current page.
            pagestart = 1;
            overlaystart = 0;
            localtype = "page";
            localfile = testname;
            continue;
         }

      }

      if (regex_search(transfer, overlaypage)) {
         regex_search(transfer, match, opname);
         if (match.size() >= 2) {
            testname = match[1].str();
         } else {
            testname = "";
         }

         if (dataQ) {
            // Already have PMX data, so store that with the previously
            // given filename.
            nextpage    = 0;
            nextoverlay = 1;
            nextfilename = testname;
            break;
         } else {
            // Don't have PMX data so store the newly read filename
            // for the current page.
            overlaystart = 1;
            pagestart = 0;
            localtype = "overlay";
            localfile = testname;
            continue;
         }

      }
      data << transfer << "\n";
   }

   if (pagestart) {
      ScorePage* pageptr = new ScorePage;
      pageptr->read(data);
      pageptr->setFilename(localfile);
      appendPage(pageptr);
      localfile = testname;
      pagestart = 0;
   } else if (overlaystart) { 
      ScorePage* pageptr = new ScorePage;
      pageptr->read(data);
      pageptr->setFilename(localfile);
      appendOverlay(pageptr);
      localfile = testname;
      overlaystart = 0;
   } else if (dataQ) {
      // catch possible unlabeled page
      ScorePage* pageptr = new ScorePage;
      pageptr->read(data);
      pageptr->setFilename(localfile);
      appendPage(pageptr);
      localfile = testname;
      pagestart = 0;
   }

   if (nextpage) {
      localtype = "page";
      localfile = nextfilename;
   } else if (nextoverlay) {
      localtype = "overlay";
      localfile = nextfilename;
   }

   if (!instream.eof()) {
      appendReadPmx(instream, localfile, localtype);
   }
   setPageOwnerships();

}



