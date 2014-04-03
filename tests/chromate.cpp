//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar 18 16:27:40 PDT 2014
// Last Modified: Wed Mar 19 11:16:11 PDT 2014
// Filename:      chromate.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/tests/chromate.cpp
// Syntax:        C++ 11
//
// Description:   Color notes according to their pitch-class.
//

#include "ScorePage.h"
#include "Options.h"

using namespace std;

void       processData          (ScorePage& infile, Options& opts);
void       HarmonicColors       (map<int, string>& colormap);
void       TriadicColors        (map<int, string>& colormap);
void       MelodicColors        (map<int, string>& colormap);
void       AccidentalColors     (map<int, string>& colormap, Options& opts);
void       displayPianoRoll     (ScorePage& infile, Options& opts);
void       colorizeNotes        (ScorePage& infile, Options& opts);
void       colorizeNotes        (ScorePage& infile, Options& opts);
void       printSystemPianoRoll (int sysindex, ScorePage& infile);
int        getChroma            (ScoreItem* item);

map<int, string> Base40ToHexColor;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("3|t|triadic=b", "Use triadic note colors (default)");
   opts.define("2|m|melodic=b", "Use melodic note colors");
   opts.define("5|h|p|harmonic|pythagorean|pythagoras=b", 
         "Use Harmonic/Pythagorean note colors");
   opts.define("s|spacers=b", 
         "place white dots at P3=0 & P3=200 to fix SVG width");
   opts.define("a|accidental=b", "Use accidental note colors");
   opts.define("doublesharp=s:#ff2600", "Color of double sharps with -a");
   opts.define("sharp=s:#ffc400",       "Color of sharps with -a");
   opts.define("natural=s:#000000",     "Color of naturals with -a");
   opts.define("flat=s:#4acbff",        "Color of flats with -a");
   opts.define("doubleflat=s:#507aff",  "Color of double flats with -a");
   opts.define("proll=b", "display overlay of piano roll");
   opts.define("proll-only=b", "only display piano roll without notation");
   opts.process(argc, argv);

   if (opts.getBoolean("harmonic")) {
      HarmonicColors(Base40ToHexColor);
   } else if (opts.getBoolean("melodic")) {
      MelodicColors(Base40ToHexColor);
   } else if (opts.getBoolean("accidental")) {
      AccidentalColors(Base40ToHexColor, opts);
   } else {
      TriadicColors(Base40ToHexColor);
   }

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

   if (opts.getBoolean("proll") || opts.getBoolean("proll-only")) {
      displayPianoRoll(infile, opts);
   } else {
      colorizeNotes(infile, opts);
   }
}


//////////////////////////////
//
// displayPianoRoll -- superimpose a piano roll display on top
//    of the notation.
//

void displayPianoRoll(ScorePage& infile, Options& opts) {

   int syscount = infile.getSystemCount();
   for (int i=0; i<syscount; i++) {
      printSystemPianoRoll(i, infile);
   }

   if (!opts.getBoolean("proll-only")) {
      ScoreItem startopacity;
      startopacity.makeSvgCode("<g style=\"opacity:1.00\">");
      cout << startopacity;
   
      cout << infile;
   
      ScoreItem endopacity;
      endopacity.makeSvgCode("</g>");
      cout << endopacity;
   }
}



//////////////////////////////
//
// printSystemPianoRoll --
//

void printSystemPianoRoll(int sysindex, ScorePage& infile) {

   ScoreItem box;
   box.setP1(12.0);   // make circle/box item
   box.setP7(1.0);    // 1 diatonic step high
   box.setP13(1.0);   // fill rectangle

   ScoreItem startcolor;
   ScoreItem endcolor;
   endcolor.makeSvgCode("</g>");

   string hexcode;
   int chroma;
   SCORE_FLOAT noteP2;
   SCORE_FLOAT noteP3;
   SCORE_FLOAT noteP6;
   SCORE_FLOAT boxP3;
   SCORE_FLOAT boxP4;
   SCORE_FLOAT boxwidth;

   for (auto& it : infile.systemItems(sysindex)) {
      if (!it->isNoteItem()) {
         continue;
      }
      noteP2 = it->getP2();
      noteP3 = it->getHPos();
      noteP6 = it->getHPosOff();
      boxP4 = it->getVPos();
      boxP3  = (noteP6 - noteP3)/2.0 + noteP3;
      boxwidth = noteP6 - noteP3;
      box.setP2(noteP2);
      box.setP3(boxP3);
      box.setP4(boxP4);
      box.setP6(boxwidth);

      chroma = getChroma(it);
      hexcode = Base40ToHexColor[chroma];

      startcolor.makeSvgCode("<g color=\"" + hexcode +
                             "\" stroke=\"" + hexcode + "\">");

      cout << startcolor;
      cout << box;
      cout << endcolor;
   }
}



//////////////////////////////
//
// getChroma --
//

int getChroma(ScoreItem* item) {
   int base40 = item->getParameterInt("analysis", "base40");
   return base40 % 40;
}



/////////////////////////////
//
// processData --
//

void colorizeNotes(ScorePage& infile, Options& opts) {
   cout << infile;

   int base40;
   int chroma;
   string hexcode;
   vectorSIp items;
   ScoreItem tempnote;
   ScoreItem colormarker;
   ScoreItem endcolormarker;
   endcolormarker.makeSvgCode("</g>");

   infile.getFileOrderList(items);
   for (auto& item : items) {
      if (!item->isNoteItem()) {
         continue;
      }
      base40 = item->getParameterInt("analysis", "base40");
      chroma = base40 % 40;
      hexcode = Base40ToHexColor[chroma];
      tempnote = *item;
      tempnote.removeArticulation();
      tempnote.hideStem();
      tempnote.removeFlags();
      tempnote.setParameter(P16, -50);  // thin ledger lines of overlay note
      colormarker.makeSvgCode("<g color=\"" + hexcode +
                              "\" stroke=\"" + hexcode + "\">");
      cout << colormarker;
      cout << tempnote;
      cout << endcolormarker;
      cout << endl;
   }

}



////////////////////
//
// HarmonicColors -- Colors of the rainbow mapped to the diatonic
//     circle of fifths (harmonic coloring).
//     E = red
//     B = orange
//     F = yellow
//     C = green
//     G = blue (light blue)
//     D = indigo (dark blue)
//     A = purple/violet
//

void HarmonicColors(map<int, string>& colormap) {
   colormap = {
      {0,  "#51fa00"},     // Cbb
      {1,  "#0cf900"},     // Cb
      {2,  "#00f900"},     // C		(green)
      {3,  "#00f22f"},     // C#
      {4,  "#00ec5b"},     // C##

      {6,  "#4e92ff"},     // Dbb
      {7,  "#4f86ff"},     // Db
      {8,  "#507aff"},     // D		(dark blue)
      {9,  "#5c72ff"},     // D#
      {10, "#656aff"},     // D##

      {12, "#e3275a"},     // Ebb
      {13, "#f3252f"},     // Eb
      {14, "#ff2600"},     // E		(red)
      {15, "#ff3500"},     // E#
      {16, "#ff4d00"},     // E##

      {17, "#ffec00"},     // Fbb
      {18, "#fff400"},     // Fb
      {19, "#fffb00"},     // F		(yellow)
      {20, "#e0fa00"},     // F#
      {21, "#c1fa00"},     // F##

      {23, "#2ed8c3"},     // Gbb
      {24, "#3dd1e2"},     // Gb
      {25, "#4acbff"},     // G		(light blue)
      {26, "#4bc0ff"},     // G#
      {27, "#4cb5ff"},     // G##

      {29, "#8253ff"},     // Abb
      {30, "#8a4cff"},     // Ab
      {31, "#9345ff"},     // A		(purple)
      {32, "#a43ee2"},     // A#
      {33, "#b537c3"},     // A##

      {35, "#ff9700"},     // Bbb
      {36, "#ffae00"},     // Bb
      {37, "#ffc400"},     // B		(orange)
      {38, "#ffcc00"},     // B#
      {39, "#ffd400"}      // B##
   };
}



////////////////////
//
// TriadicColors -- Colors of the rainbow mapped to thirds, starting on C.
//     C = red
//     E = orange
//     G = yellow
//     B = green
//     D = blue (light blue)
//     F = indigo (dark blue)
//     A = purple/violet
//

void TriadicColors(map<int, string>& colormap) {
   colormap = {
      { 0, "#e3275a"},     // Cbb
      { 1, "#f3252f"},     // Cb
      { 2, "#ff2600"},     // C		(red)
      { 3, "#ff3500"},     // C#
      { 4, "#ff4d00"},     // C##

      { 6, "#2ed8c3"},     // Dbb
      { 7, "#3dd1e2"},     // Db
      { 8, "#4acbff"},     // D		(light blue)
      { 9, "#4bc0ff"},     // D#
      {10, "#4cb5ff"},     // D##

      {12, "#ff9700"},     // Ebb
      {13, "#ffae00"},     // Eb
      {14, "#ffc400"},     // E		(orange)
      {15, "#ffcc00"},     // E#
      {16, "#ffd400"},     // E##

      {17, "#4e92ff"},     // Fbb
      {18, "#4f86ff"},     // Fb
      {19, "#507aff"},     // F		(dark blue)
      {20, "#5c72ff"},     // F#
      {21, "#656aff"},     // F##

      {23, "#ffec00"},     // Gbb
      {24, "#fff400"},     // Gb
      {25, "#fffb00"},     // G		(yellow)
      {26, "#e0fa00"},     // G#
      {27, "#c1fa00"},     // G##

      {29, "#8253ff"},     // Abb
      {30, "#8a4cff"},     // Ab
      {31, "#9345ff"},     // A		(purple)
      {32, "#a43ee2"},     // A#
      {33, "#b537c3"},     // A##

      {35,  "#51fa00"},    // Bbb
      {36,  "#0cf900"},    // Bb
      {37,  "#00f900"},    // B		(green)
      {38,  "#00f22f"},    // B#
      {39,  "#00ec5b"}     // B##
   };
}



////////////////////
//
// MelodicColors -- Colors of the rainbow mapped to seconds, starting on C.
//   Color order given to pitch classes by Newton.
//     C = red
//     D = orange
//     E = yellow
//     F = green
//     G = blue (light blue)
//     A = indigo (dark blue)
//     B = purple/violet
//

void MelodicColors(map<int, string>& colormap) {
   colormap = {
      { 0, "#e3275a"},     // Cbb
      { 1, "#f3252f"},     // Cb
      { 2, "#ff2600"},     // C		(red)
      { 3, "#ff3500"},     // C#
      { 4, "#ff4d00"},     // C##

      { 6, "#ff9700"},     // Dbb
      { 7, "#ffae00"},     // Db
      { 8, "#ffc400"},     // D		(orange)
      { 9, "#ffcc00"},     // D#
      {10, "#ffd400"},     // D##

      {12, "#ffec00"},     // Ebb
      {13, "#fff400"},     // Eb
      {14, "#fffb00"},     // E		(yellow)
      {15, "#e0fa00"},     // E#
      {16, "#c1fa00"},     // E##

      {17, "#51fa00"},     // Fbb
      {18, "#0cf900"},     // Fb
      {19, "#00f900"},     // F		(green)
      {20, "#00f22f"},     // F#
      {21, "#00ec5b"},     // F##

      {23, "#2ed8c3"},     // Gbb
      {24, "#3dd1e2"},     // Gb
      {25, "#4acbff"},     // G		(light blue)
      {26, "#4bc0ff"},     // G#
      {27, "#4cb5ff"},     // G##

      {29, "#4e92ff"},     // Abb
      {30, "#4f86ff"},     // Ab
      {31, "#507aff"},     // A 	(dark blue)
      {32, "#5c72ff"},     // A#
      {33, "#656aff"},     // A##

      {35, "#8253ff"},     // Bbb
      {36, "#8a4cff"},     // Bb
      {37, "#9345ff"},     // B		(purple)
      {38, "#a43ee2"},     // B#
      {39, "#b537c3"}      // B##
   };
}



////////////////////
//
// AccidentalColors -- Give each type of chromatic alteration of diatonic
//    notes a different color.
//     ##   = red
//     #    = orange
//     n    = black
//     b    = light blue
//     bb   = dark blue
//

void AccidentalColors(map<int, string>& colormap, Options& opts) {
   string red    = "#ff2600";
   string orange = "#ffc400";
   string black  = "#000000";
   string ltblue = "#4acbff";
   string dkblue = "#507aff";

   string doublesharp = red;
   string sharp       = orange;
   string natural     = black;
   string flat        = ltblue;
   string doubleflat  = dkblue;

   if (opts.getBoolean("doublesharp")) {
      doublesharp = opts.getString("doublesharp");
   }
   if (opts.getBoolean("sharp")) {
      sharp = opts.getString("sharp");
   }
   if (opts.getBoolean("natural")) {
      natural = opts.getString("natural");
   }
   if (opts.getBoolean("flat")) {
      flat = opts.getString("flat");
   }
   if (opts.getBoolean("doubleflat")) {
      doubleflat = opts.getString("doubleflat");
   }

   colormap = {
      { 0, doubleflat},   // Cbb
      { 1, flat},         // Cb
      { 2, natural},      // C
      { 3, sharp},        // C#
      { 4, doublesharp},  // C##

      { 6, doubleflat},   // Dbb
      { 7, flat},         // Db
      { 8, natural},      // D
      { 9, sharp},        // D#
      {10, doublesharp},  // D##

      {12, doubleflat},   // Ebb
      {13, flat},         // Eb
      {14, natural},      // E
      {15, sharp},        // E#
      {16, doublesharp},  // E##

      {17, doubleflat},   // Fbb
      {18, flat},         // Fb
      {19, natural},      // F
      {20, sharp},        // F#
      {21, doublesharp},  // F##

      {23, doubleflat},   // Gbb
      {24, flat},         // Gb
      {25, natural},      // G
      {26, sharp},        // G#
      {27, doublesharp},  // G##

      {29, doubleflat},   // Abb
      {30, flat},         // Ab
      {31, natural},      // A 
      {32, sharp},        // A#
      {33, doublesharp},  // A##

      {35, doubleflat},   // Bbb
      {36, flat},         // Bb
      {37, natural},      // B
      {38, sharp},        // B#
      {39, doublesharp}   // B##
   };
}



