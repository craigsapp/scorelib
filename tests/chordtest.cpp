//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:43:50 PST 2014
// Last Modified: Tue Mar  4 01:28:29 PST 2014
// Filename:      chordtest.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/chordtest.cpp
// Syntax:        C++ 11
//
// Description:   Extract notes in music, grouping notes into chords 
//                as appropriate.
//

#include "ScorePage.h"
#include "Options.h"

using namespace std;

void       processData        (ScorePage& infile);
void       processStaff       (int staffnumber, vectorSIp& staffitems);
void       printChordNotes    (ScoreItem* headnote);
void       printPitch         (ScoreItem* note);

Options opts;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
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
      processData(infile);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////

/////////////////////////////
//
// processData --
//

void processData(ScorePage& infile) {
   infile.analyzeStaffDurations();
   infile.analyzePitch();
   infile.analyzeChords();

   int maxstaff = infile.getMaxStaff();
   for (int i=maxstaff; i>0; i--) {
      processStaff(i, infile.staffItems(i));
   }
}



///////////////////////////////
//
// processStaff --
//

void processStaff(int staffnumber, vectorSIp& staffitems) {
   unsigned int i;
   SCORE_FLOAT dur = -1.0;
   int base40;
   int barQ = 0;
   for (i=0; i<staffitems.size(); i++) {
      if (!barQ && (staffitems[i]->isBarlineItem())) {
         cout << " |";
         barQ = 1;
         continue;
      }
      barQ = 0;
      if (!staffitems[i]->isNoteItem()) {
         continue;
      }
      if (!staffitems[i]->hasStem()) {
         continue;
      }
      cout << " ";
      printPitch(staffitems[i]);
      printChordNotes(staffitems[i]);
   }
   cout << "\n\n";
}



//////////////////////////////
//
// printChordNotes --
//

void printChordNotes(ScoreItem* headnote) {
   vectorSIp* chordnotes = headnote->getChordNotes();
   if (chordnotes == NULL) {
      return;
   }
   for (auto& item : *chordnotes) {
      if (item == headnote) {
         continue;
      }
      cout << ",";
      printPitch(item);
   }
}



//////////////////////////////
//
// printPitch --
//

void printPitch(ScoreItem* note) {
   cout << note->getHumdrumPitch();
}



