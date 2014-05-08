//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Apr 30 01:10:49 PDT 2014
// Last Modified: Wed Apr 30 01:10:52 PDT 2014
// Filename:      analyzelyrics.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/analyzelyrics.cpp
// Syntax:        C++11
//
// Description:   This program analyzes lyrics of music, identifying verses.
//

#include "scorelib.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("c|copy=b", "Copy auto namespace params to lyrics namespace");
   opts.define("o|overwrite=b", "Overwrite lyric namespace data");
   opts.define("d|delete-auto=b", 
      "Delete auto namespace parameters before printing");
   opts.process(argc, argv);
   ScorePageSet infiles(opts);
   infiles.analyzeSegmentsByIndent();
   infiles.analyzeLyrics();
   if (opts.getBoolean("copy") || opts.getBoolean("overwrite")) {
      if (opts.getBoolean("overwrite")) {
         infiles.copyParameterOverwrite("lyrics", "auto", "verse");
         infiles.copyParameterOverwrite("lyrics", "auto", "hyphenBefore");
         infiles.copyParameterOverwrite("lyrics", "auto", "hyphenAfter");
         infiles.copyParameterOverwrite("lyrics", "auto", "hyphen");
         infiles.copyParameterOverwrite("lyrics", "auto", "wordExtension");
      } else {
         infiles.copyParameterNoOverwrite("lyrics", "auto", "verse");
         infiles.copyParameterNoOverwrite("lyrics", "auto", "hyphenBefore");
         infiles.copyParameterNoOverwrite("lyrics", "auto", "hyphenAfter");
         infiles.copyParameterNoOverwrite("lyrics", "auto", "hyphen");
         infiles.copyParameterNoOverwrite("lyrics", "auto", "wordExtension");
      }
   }
   if (opts.getBoolean("delete-auto")) {
      infiles.deleteNamespace("auto");
   }
   cout << infiles;
   return 0;
}

///////////////////////////////////////////////////////////////////////////



