//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 14:31:10 PST 2014
// Last Modified: Mon Mar  3 22:29:11 PST 2014
// Filename:      scoreitems2xml.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/scoreitems2xml.cpp
// Syntax:        C++11
//
// Description:   This program converts SCORE PMX (ASCII) or MUS (binary) files
//                into a list of SCORE items formatted as XML.  
//
// Options:
// 	--debug		Print filename at start of each page of data.
//
// $Smake:		rm %b; make %b
// $Smake-chopin:	./%b data/chopin2806.mus
//

#include "ScorePage.h"
#include "Options.h"
#include <iostream>

using namespace std;

void      processData     (ScorePage& infile);

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   opts.process(argc, argv);

   ScorePage infile;

   int argcount = opts.getArgCount();
   string filename;

   cout << "<SCORE version=\"alpha\">\n";

   for (int i=0; i<argcount || i==0; i++) {
      if (argcount == 0) {
        filename = "<STDIN>";
        infile.read(cin);
      } else  {
         filename = opts.getArg(i+1);
         infile.read(filename);
      }

      processData(infile);
   }

   cout << "</SCORE>\n";

   return 0;
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// processData --
//

void processData(ScorePage& infile) {
   vectorSIp data;
   infile.getFileOrderList(data);

   string filename;
   infile.getFilename(filename);
 
   cout << "\t<page";
   if (filename.size() > 0) {
      cout << " filename=\"";
      ScoreItem::printXmlTextEscaped(cout, filename);
      cout << "\"";
   }
   cout << ">\n";

   for (auto it = data.begin(); it != data.end(); it++) {
      (*it)->printXmlWithEdits(cout, 2);
   }

   cout << "\t</page>\n";
}



