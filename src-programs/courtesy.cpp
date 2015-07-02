//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar 16 23:30:54 PDT 2014
// Last Modified: Mon Apr  7 00:28:23 PDT 2014
// Filename:      courtesy.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/courtesy.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/courtesy
// Syntax:        C++ 11
//
// Description:   Identify and process courtesy accidentals:
// 			* put parentheses around them.
// 			* remove parenthenses from around them.
// 			* highlight courtesy accidentals in red.
// 			* hide courtesy accidentals.
// 			* restore courtesy accidentals.
// 			* convert courtesy accidentals to editorial accidentals.
//

#include "scorelib.h"

using namespace std;

void   processData          (ScorePage& infile, Options& opts);
void   doMarkStyle          (ScorePage& infile, Options& opts);
void   doHighlightStyle     (ScorePage& infile, Options& opts);
void   doHideStyle          (ScorePage& infile, Options& opts);
void   doParenStyle         (ScorePage& infile, Options& opts);
void   doRemoveParens       (ScorePage& infile, Options& opts);
void   doEditorialStyle     (ScorePage& infile, Options& opts);
void   printLjMacro         (ScorePage& infile, vectorI& states);

enum style {
   STYLE_HIGHLIGHT = 0,
   STYLE_MARK,
   STYLE_REMOVE,
   STYLE_PAREN,
   STYLE_HIDE,
   STYLE_EDITORIAL
};

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("r|remove|hide=b",  "hide courtesy/cautionary accidentals");
   opts.define("R|replace|show=b", "show courtesy/cautionary accidentals");
   opts.define("p|paren=b",   "put parentheses around courtesy accidentals");
   opts.define("P|no-paren=b","remove parentheses around courtesy accidentals");
   opts.define("k|keep=b",      "keep courtey accidental note states");
   opts.define("m|mark=b",      "mark courtesy accidentals (no highlighting)");
   opts.define("lj|LJ=b",       "Add LJ commands for changed systems");
   opts.define("a|analysis=b",  "Include pitch analysis data in output");
   opts.define("e|editorial=b", "put editorial accs. on all non-natural notes");
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
         cout << "###FILE:\t" << infile.getFilename() << endl;
      }
      processData(infile, opts);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////


/////////////////////////////
//
// processData -- Decide what to do based on the command-line option
//      settings.
//

void processData(ScorePage& infile, Options& opts) {

   int style = STYLE_HIGHLIGHT;
   if (opts.getBoolean("hide")) {
      style = STYLE_HIDE;
   } else if (opts.getBoolean("mark")) {
      style = STYLE_MARK;
   } else if (opts.getBoolean("paren")) {
      style = STYLE_PAREN;
   } else if (opts.getBoolean("no-paren")) {
      style = STYLE_REMOVE;
   } else if (opts.getBoolean("editorial")) {
      style = STYLE_EDITORIAL;
   }

   switch (style) {
      case STYLE_HIGHLIGHT:  doHighlightStyle(infile, opts); break;
      case STYLE_MARK:       doMarkStyle(infile, opts);      break;
      case STYLE_HIDE:       doHideStyle(infile, opts);      break;
      case STYLE_PAREN:      doParenStyle(infile, opts);     break;
      case STYLE_REMOVE:     doRemoveParens(infile, opts);   break;
      case STYLE_EDITORIAL:  doEditorialStyle(infile, opts); break;
   }
}



/////////////////////////////
//
// doMarkStyle -- Mark all courtesy/cautionary accidentals with
//     the named parameter "@coutesy: true".
//

void doMarkStyle(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   infile.changeNamespace("", "analysis", "courtesy", P1_Note);
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;
}



/////////////////////////////
//
// doHighlightStyle -- Mark all courtesy/cautionary accidentals in
//     red (currently only SVG coloring implemented).
//

void doHighlightStyle(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   if (opts.getBoolean("keep")) {
      infile.changeNamespace("", "analysis", "courtesy", P1_Note);
   }
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;
   infile.analyzePitch();
   cout << "\n# Courtesy note markup data:\n\n";

   ScoreItem tempitem;
   tempitem.makeSvgCode("<g color=\"red\" stroke=\"red\">");
   tempitem.setParameterNoisy("markup", "1");
   cout << tempitem;
   cout << endl;

   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (!item->isNoteItem()) {
         continue;
      }
      if (item->getP("analysis", "courtesy") == "true") {
         tempitem = *item;
         tempitem.removeArticulation();
         tempitem.hideStem();
         tempitem.setParameterNoisy("markup", "1");
         if (!opts.getBoolean("analysis")) {
            tempitem.deleteNamespace("analysis");
         }
         cout << tempitem;
      }
   }

   tempitem.makeSvgCode("</g>");
   tempitem.setParameterNoisy("markup", "1");
   cout << endl;
   cout << tempitem;
   cout << endl;
}



/////////////////////////////
//
// doHideStyle -- Remove courtesy accidentals from notes.
//

void doHideStyle(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   vectorSIp items;
   infile.getFileOrderList(items);
   if (opts.getBoolean("keep")) {
      infile.changeNamespace("", "analysis", "courtesy", P1_Note);
   }
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;
}



/////////////////////////////
//
// doParenStyle -- Add parenthese around all courtesy accidentals.
//

void doParenStyle(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   vectorSIp items;
   vectorI systemschanged;
   systemschanged.resize(infile.getSystemCount());
   fill(systemschanged.begin(), systemschanged.end(), 0);

   int ljQ = opts.getBoolean("lj");
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("analysis", "courtesy") == "true") {
         item->setAccidentalParentheses();
         if (ljQ) {
            systemschanged[item->getSystemIndex()] = 1;
         }
      }
   }
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;

   if (ljQ) {
      printLjMacro(infile, systemschanged);
   }
}



/////////////////////////////
//
// doRemoveParens -- Remove parentheses around all courtesy accidentals.
//

void doRemoveParens(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("analysis", "courtesy") == "true") {
         item->removeAccidentalParentheses();
      }
   }
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;
}



/////////////////////////////
//
// doEditorialStyle -- Convert courtesy accidental to editorial accidental.
//      Currently cannot handle double sharp/flats.
//

void doEditorialStyle(ScorePage& infile, Options& opts) {
   infile.analyzePitch();
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("analysis", "courtesy") == "true") {
         int accidental = item->getPrintedAccidental();
         item->setNoAccidental();
         switch (accidental) {
            case -1: item->setPN(P11, 1); break;
            case +1: item->setPN(P11, 2); break;
            case  0: item->setPN(P11, 3); break;
         }
      }
   }
   if (!opts.getBoolean("analysis")) {
      infile.deleteNamespace("analysis");
   }
   cout << infile;
}



///////////////////////////////
//
// printLjMacro -- Pring an LJ command for each system which
//      has any courtesy accidentals within it.
//

void printLjMacro(ScorePage& infile, vectorI& states) {
   int bottomstaff;
   int staffcount;
   vectorVI& systeminfo = infile.reverseSystemMap();
   for (unsigned int i=0; i<states.size(); i++) {
      if (!states[i]) {
         continue;
      }
      // get the bottom P2 staff number of the system
      bottomstaff =  systeminfo[i][0];
      // get the count of staves in the system
      staffcount = systeminfo[i].size();
      cout << "LJ\n";
      cout << bottomstaff << " " << staffcount << endl;
   }
}



