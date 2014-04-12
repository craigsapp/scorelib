//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar 16 23:30:54 PDT 2014
// Last Modified: Sun Mar 16 23:30:56 PDT 2014
// Filename:      beamtest.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/beamtest.cpp
// Syntax:        C++ 11
//
// Description:   List all of the beamed notes on a page.
//

#include "ScorePage.h"
#include "Options.h"

using namespace std;

void       processData        (ScorePage& infile, Options& opts);
void       processStaff       (int staffnumber, vectorSIp& staffitems);
void       printChordNotes    (ScoreItem* headnote);
void       printPitch         (ScoreItem* note);
void       printBeamInfo      (BeamGroup* bg);


///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("i|input=b", "Echo input data before analysis data");
   opts.define("u|unique=b", "Do not duplicate beam groups with -i option");
   opts.define("p|pitch=b", "Do pitch analysis");
   opts.process(argc, argv);
   
   ScorePage infile;

   int argcount = opts.getArgCount();
   for (int i=0; i<argcount || i==0; i++) {
      if (argcount == 0) {
        infile.read(cin);
      } else  {
         infile.read(opts.getArg(i+1));
      }
      if (argcount > 1) {
         cout << "### FILE " << infile.getFilename() << endl;
      }
      processData(infile, opts);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


/////////////////////////////
//
// processData --
//

void processData(ScorePage& infile, Options& opts) {
   infile.analyzeBeams();
   if (opts.getBoolean("pitch")) {
      infile.analyzePitch();
   }

   if (opts.getBoolean("input")) {
      if (opts.getBoolean("unique")) {
         vectorSIp pageitems;
         infile.getFileOrderList(pageitems);
         for (auto& it : pageitems) {
            if (!it->inBeamGroup()) {
               cout << it;
            }
         }
      } else {
         cout << infile;
      }
      cout << endl;
      ScoreItem redstart;
      redstart.makeSvgCode("<g color=\"red\" stroke=\"red\">");
      cout << redstart;
   }

   int maxstaff = infile.getMaxStaff();
   for (int i=maxstaff; i>0; i--) {
      processStaff(i, infile.staffItems(i));
   }

   if (opts.getBoolean("input")) {
      ScoreItem redstop;
      redstop.makeSvgCode("</g>");
      cout << redstop;
   }

}



///////////////////////////////
//
// processStaff -- Go through the staves on a page one at a time, starting
//    at the top of the page, printing any beam groups found on the staff
//    (as well as the staff itself).
//

void processStaff(int staffnumber, vectorSIp& staffitems) {
   cout << "\n# STAFF = " << staffnumber << endl;

   BeamGroup* bg;
   unsigned int i;
   for (auto& item : staffitems) {
      if (!item->isBeamItem()) {
         continue;
      }
      bg = item->getBeamGroup();
      if (bg == NULL) {
         cerr << "Unexpected empty beam group:" << endl;
         cerr << item;
         exit(1);
      }
      printBeamInfo(bg);
   }
   cout << "\n\n";
}



//////////////////////////////
//
// printBeamInfo --
//

void printBeamInfo(BeamGroup* bg) {
   // first print the beams:
   cout << endl;
   cout << "# BEAM GROUP:" << endl;
   for (auto& item : bg->beams) {
      cout << item;
   }
   for (auto& item : bg->notes) {
      printChordNotes(item);
   }
}



//////////////////////////////
//
// printChordNotes --
//

void printChordNotes(ScoreItem* headnote) {
   vectorSIp* chordnotes = headnote->getChordNotes();
   cout << headnote;
   if (chordnotes == NULL) {
      // not a chord, just a single note/rest:
      return;
   }
   cout << "# START CHORD" << endl;
   unsigned int i;
   for (auto& item : *chordnotes) {
      if (item == headnote) {
         continue;
      }
      cout << item;
   }
   cout << "# END CHORD" << endl;
}



