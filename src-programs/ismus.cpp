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
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

int    checkFile            (const string& filename);
void   processOptions       (Options& opts, int argc, char** argv);
int    isFile               (string name);
int    isDirectory          (string name);
void   printResult          (int result, const string& filename);
void   recursiveCheck       (const string& name);

// Interface variables:
Options options;
int badQ       = 0;         // used with -b option
int goodQ      = 0;         // used with -g option
int recursiveQ = 0;         // used with -r option


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   processOptions(options, argc, argv);

   int filecount = options.getArgCount();

   int result;
   for (int i=1; i<=filecount; i++) {
      if (recursiveQ) {
         recursiveCheck(options.getArgument(i));
      } else {
         result = checkFile(options.getArgument(i));
         printResult(result, options.getArgument(i));

      }
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// printResult --
//

void printResult(int result, const string& filename) {
   if (goodQ) {
      if (result) {
         cout << filename << endl;
      }
   } else if (badQ) {
      if (!result) {
         cout << filename << endl;
      }
   } else {
      if (result) {
         cout << "YES";
      } else {
         cout << "NO";
      }
      cout << "\t" << filename << endl;
   }
}



//////////////////////////////
//
// recursiveCheck --
//

void recursiveCheck(const string& name) {
   int result, i;
   if (isFile(name)) {
      result = checkFile(name);
      printResult(result, name);
   } else if (isDirectory(name)) {
      vector<string> dirs;
      DIR *dir;
      struct dirent *ent;
      string filename;
      if ((dir = opendir(name.data())) != NULL) {
         while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.') {
               continue;
            }
            filename = name;
            if (filename[filename.size()-1] != '/') {
               filename += "/";
            } 
            filename += ent->d_name;
            if (isFile(filename)) {
               result = checkFile(filename);
               printResult(result, filename);
               continue;
            } else if (isDirectory(filename)) {
               dirs.push_back(filename);
            }
         }
         closedir(dir);
         for (i=0; i<(int)dirs.size(); i++) {
            recursiveCheck(dirs[i]);
         }
      }
   }
}



//////////////////////////////
//
// checkFile --
//

int checkFile(const string& filename) {
   ifstream testfile(filename.data());

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
// isFile -- returns true if filename is a file.
//

int isFile(string name) {
   struct stat s;
   if (stat(name.data(), &s) == 0) {
      if (s.st_mode & S_IFDIR) {
         return 0; // directory
      } else if (s.st_mode & S_IFREG) {
         return 1; // file
      } else {
         return 0;
      }
   } else {
      return 0;
   }
}



//////////////////////////////
//
// isDirectory -- returns true if filename is a directory.
//

int isDirectory(string name) {
   struct stat s;
   if (stat(name.data(), &s) == 0) {
      if (s.st_mode & S_IFDIR) {
         return 1; // directory
      } else if (s.st_mode & S_IFREG) {
         return 0; // file
      } else {
         return 0;
      }
   } else {
      return 0;
   }
}



//////////////////////////////
//
// processOptions --
//

void processOptions(Options& opts, int argc, char** argv) {
   opts.define("g|good|y|yes=b", 
         "Print only names of files which have binary SCORE data");
   opts.define("b|bad|n|no=b", 
         "Print only names of files which do not have binary SCORE data");
   opts.define("r|recursive=b", 
         "Recursively check subdirectories");
   opts.process(argc, argv);

   badQ       = opts.getBoolean("bad");
   goodQ      = opts.getBoolean("good");
   recursiveQ = opts.getBoolean("recursive");
   if (goodQ) {
      badQ = 0;
   }
}


