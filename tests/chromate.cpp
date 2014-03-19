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

void       processData        (ScorePage& infile, Options& opts);
void       HarmonicColors     (map<int, string>& colormap);
void       TriadicColors      (map<int, string>& colormap);
void       MelodicColors      (map<int, string>& colormap);

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
   opts.process(argc, argv);

   if (opts.getBoolean("harmonic")) {
      HarmonicColors(Base40ToHexColor);
   } else if (opts.getBoolean("melodic")) {
      MelodicColors(Base40ToHexColor);
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

   cout << infile;

   infile.analyzePitch();

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
      base40 = item->getParameterInt("analysis", "base40Pitch");
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
   colormap = {  // Newton melodic colors
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



