//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jun 16 00:48:32 PDT 2015
// Last Modified: Tue Jun 16 00:48:35 PDT 2015
// Filename:      ismus.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/ismus.cpp
// Documentation: http://scorelib.sapp.org/program/ismus
// Syntax:        C++ 11
//
// Description:   Identifies if input files are binary SCORE files.
//

#include "Options.h"
#include "stdlib.h"
#include <fstream>
#include <iostream>

using namespace std;

int    checkFile            (const char* filename);
void   processOptions       (Options& opts, int argc, char** argv);

// Interface variables:
Options options;
int badQ  = 0;              // used with -b option
int goodQ = 0;              // used with -g option


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);

   int filecount = options.getArgCount();

   int result;
   for (int i=1; i<=filecount; i++) {
      result = checkFile(options.getArgument(i).data());
      if (goodQ) {
         if (result) {
            cout << options.getArgument(i) << endl;
         }
      } else if (badQ) {
         if (!result) {
            cout << options.getArgument(i) << endl;
         }
      } else {
         if (result) {
            cout << "YES";
         } else {
            cout << "NO";
         }
         cout << "\t" << options.getArgument(i) << endl;
      }
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// checkFile --
//

int checkFile(const char* filename) {
   ifstream testfile(filename);

   if (!testfile.is_open()) {
      cerr << "Error: cannot read the file: " << filename << endl;
      exit(1);
   }

   testfile.seekg(-4, ios::end);

   unsigned char databytes[4] = {0xff};
   testfile.read((char*)databytes, 4);
   if (databytes[0] == 0x00 && databytes[1] == 0x3c && databytes[2] == 0x1c &&
         databytes[3] == 0xc6) {
      return 1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("g|good=b", 
         "Print only names of files which have binary SCORE data");
   opts.define("b|bad=b", 
         "Print only names of files which do not have binary SCORE data");
   opts.process(argc, argv);

   badQ  = opts.getBoolean("bad");
   goodQ = opts.getBoolean("good");
   if (goodQ) {
      badQ = 0;
   }
}


