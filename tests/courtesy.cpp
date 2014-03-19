//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Mar 16 23:30:54 PDT 2014
// Last Modified: Sun Mar 16 23:30:56 PDT 2014
// Filename:      courtesy.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/courtesy.cpp
// Syntax:        C++ 11
//
// Description:   Change display style of courtesy accidentals to:
// 			(1) put parentheses around them
// 			(2) remove parenthenses from around them
// 			(3) highlight courtesy accidentals in red
// 			(4) hide cautionary accidentals
// 			(5) add editorial accidentals on all notes which
// 			    are sharpened or flattened but the accidental
// 			    is not being shown.
//

#include "ScorePage.h"
#include "Options.h"

using namespace std;

void       processData        (ScorePage& infile, Options& opts);
void       doHighlightStyle   (ScorePage& infile);
void       doHideStyle        (ScorePage& infile);
void       doParenStyle       (ScorePage& infile);
void       doRemoveStyle      (ScorePage& infile);
void       doEditorialStyle   (ScorePage& infile);

enum {
   STYLE_HIGHLIGHT = 0,
   STYLE_REMOVE,
   STYLE_PAREN,
   STYLE_HIDE,
   STYLE_EDITORIAL
} styles;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("h|hide=b",   "hide courtesy/cautionary accidentals");
   opts.define("p|paren=b",  "put parentheses around courtesy accidentals");
   opts.define("r|remove=b", "remove parentheses around courtesy accidentals");
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
   infile.analyzePitch();

   int style = STYLE_HIGHLIGHT;
   if (opts.getBoolean("hide")) {
      style = STYLE_HIDE;
   } else if (opts.getBoolean("paren")) {
      style = STYLE_PAREN;
   } else if (opts.getBoolean("remove")) {
      style = STYLE_REMOVE;
   } else if (opts.getBoolean("editorial")) {
      style = STYLE_EDITORIAL;
   }

   switch (style) {
      case STYLE_HIGHLIGHT:  doHighlightStyle(infile); break;
      case STYLE_HIDE:       doHideStyle(infile);      break;
      case STYLE_PAREN:      doParenStyle(infile);     break;
      case STYLE_REMOVE:     doRemoveStyle(infile);    break;
      case STYLE_EDITORIAL:  doEditorialStyle(infile); break;
   }
}



/////////////////////////////
//
// doHighlightStyle -- Mark all courtesy/cautionary accidentals in
//     red.
//

void doHighlightStyle(ScorePage& infile) {
   cout << infile;

   ScoreItem tempitem;
   tempitem.makeSvgCode("<g color=\"red\" stroke=\"red\">");
   cout << tempitem;
   cout << endl;

   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (!item->isNoteItem()) {
         continue;
      }
      if (item->getP("cautionaryAccidental") == "true") {
         tempitem = *item;
         tempitem.removeArticulation();
         tempitem.hideStem();
         cout << tempitem;
      }
   }

   tempitem.makeSvgCode("</g>");
   cout << endl;
   cout << tempitem;
}



/////////////////////////////
//
// doHideStyle -- Remove courtesy accidentals from notes.
//

void doHideStyle(ScorePage& infile) {
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("cautionaryAccidental") == "true") {
         item->setNoAccidental(); 
      }
   }
   cout << infile;
}



/////////////////////////////
//
// doParenStyle -- Add parenthese around all courtesy accidentals.
//

void doParenStyle(ScorePage& infile) {
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("cautionaryAccidental") == "true") {
         item->setAccidentalParentheses(); 
      }
   }
   cout << infile;
}



/////////////////////////////
//
// doRemoveStyle -- Remove parenthese around all courtesy accidentals.
//

void doRemoveStyle(ScorePage& infile) {
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("cautionaryAccidental") == "true") {
         item->removeAccidentalParentheses(); 
      }
   }
   cout << infile;
}



/////////////////////////////
//
// doEditorialStyle -- Convert cautionary accidental to editorial accidental.
//      Currently cannot handle double sharp/flats.
//

void doEditorialStyle(ScorePage& infile) {
   vectorSIp items;
   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (item->getParameter("cautionaryAccidental") == "true") {
         int accidental = item->getPrintedAccidental();
         item->setNoAccidental(); 
         switch (accidental) {
            case -1: item->setP(P11, 1); break;
            case +1: item->setP(P11, 2); break;
            case  0: item->setP(P11, 3); break;
         }
      }
   }
   cout << infile;
}




