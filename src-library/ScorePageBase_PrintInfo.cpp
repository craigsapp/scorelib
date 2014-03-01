//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 03:07:10 PST 2014
// Filename:      ScorePageBase_PrintInfo.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageBase_PrintInfo.cpp
// Syntax:        C++11
//
// Description:   Support class for page description in ScorePageBase.
//


#include "ScorePageBase_PrintInfo.h"

using namespace std;


//////////////////////////////
//
// PrintInfo::PrintInfo -- Constructor.
//

PrintInfo::PrintInfo(void) {
   setDefaultValues();
}


PrintInfo::PrintInfo(PrintInfo& info) {
   *this = info;
}



//////////////////////////////
//
// PrintInfo::~PrintInfo -- Deconstructor.
//

PrintInfo::~PrintInfo() {
   // do nothing
}



//////////////////////////////
//
// PrintInfo::operator= --
//

PrintInfo& PrintInfo::operator=(const PrintInfo& info) {
   if (this == &info) {
      return *this;
   }
   rotate     = info.rotate;
   scale      = info.scale;
   width      = info.width;
   height     = info.height;
   lmargin    = info.lmargin;
   bmargin    = info.bmargin;
   tDPI       = info.tDPI;
   oDPI       = info.oDPI;
   lineWidth  = info.lineWidth;
   mirror     = info.mirror;
   setStroke  = info.setStroke;
   return *this;
}



//////////////////////////////
//
// PrintInfo::setDefaultValues --
//

void PrintInfo::setDefaultValues(void) {
   rotate  = 0;    // landscape mode
   scale   = 1.0;  // music size on page (default to width of LETTER)
   width   = 8.5;  // width of page (LETTER width by default)
   height  = 11.0; // height of page (LETTER width by default)
   lmargin = 0.50; // left page margin (excluding Lbuffer)
   bmargin = 0.75; // bottom page margin (excluding Bbuffer)
   tDPI    = 600;  // target rendering DPI (for calcing line width)
   oDPI    = 600;  // actual rendering DPI (for calcing bitmap poses)
   lineWidth  = 4; // The width of stroked lines in output (will be
                              //    5 pixs after setStrokeAdjust adjusts lines).
   mirror     = 0; // Don't revers the left-right axis when printing.
   setStroke  = 1; // Use setStrokeAdjust when rendering v graphics.
      
   // These print variables are fixed:
   //
   // Lbuffer = 0.025;  // extra width added to lmargin
   // Bbuffer = 0.0625;
   // StaffLen = 7.5;
   // Lbuffer = 0.025;
}


