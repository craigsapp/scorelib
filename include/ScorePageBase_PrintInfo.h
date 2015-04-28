//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 03:16:00 PST 2014
// Filename:      ScorePageBase_PrintInfo.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageBase_PrintInfo.h
// Syntax:        C++11
//
// Description:   Support class for page description in ScorePageBase.
//

#ifndef _SCOREPAGEBASE_PRINTINFO_H_INCLUDED
#define _SCOREPAGEBASE_PRINTINFO_H_INCLUDED

#include "ScoreItem.h"

using namespace std;

class PrintInfo {
   public:
                 PrintInfo          (void);
                 PrintInfo          (PrintInfo& info);
                ~PrintInfo          ();

      PrintInfo& operator=          (const PrintInfo& info);
      void       setDefaultValues   (void);

   public:
      //
      // Page Print Parameters -- These parameters are needed for printing
      //     a page on paper (via PostScript), or to be able to calculate the
      //     location of items in a bitmapped image of the page.
      //

      // The rotate variable is for setting the orientation of the music
      // on the page.  Currently this variable is fixed to portrait.  If
      // rotate = 1, then rotate 90 degrees (clockwise?) so that the page
      // is in landscape mode.
      int rotate;

      // The musicSize variable is used to scale the music on the page.
      // The default value is 1.0.  If the value is 0.5, then the music
      // will be scaled by 50%.  The origin of the scaling is the point
      // where P3=0.0 and P4=0.0 for staff 1 on the page (bottom left
      // corner of music excluding page margins).
      double scale;

      // The SCORE print menu selects the paper type.  In this case
      // the paper type will be described as two variables.  The width
      // of the page (pageWidth) and the height of the page (pageHeight),
      // both stored in units of inches.  These variables are not
      // really needed for printing, but the pageHeibght is needed when
      // converting/calculating bitmap images, since the origin for
      // PostScript is the bottom left corner of the page, while for bitmaps
      // the origin is the top left corner.
      double width;
      double height;

      // The left margin setting in the SCORE print menu (parameter 4,
      // first number).  This is the distance from the left margin of the
      // page to the P3=0.0 point on the staff, not including an extra
      // width (called Lbuffer below) of 0.025 inches.  The default setting
      // for this variable in SCORE is 0.5 inches (which is the default
      // value for instances of this class).
      double lmargin;

      // The bottom margin setting in the SCORE print menu (parameter 4,
      // second number).  This is the distance from the bottom margin of the
      // page to the bottom line of the first staff when its P4=0.  This
      // bottom margin does not include and extra width (called Bbuffer
      // below) of 0.0625 inches.  The default setting for this variable
      // in SCORE is 0.75 inches (which is the default value for instances
      // of this class).
      double bmargin;

      // The resolution print setting is for calculating the width of a line
      // in physical units.  The line width is specified as a pixel width.
      // tDPI is equivalent to the RESOLUTION parameter from the SCORE
      // print menu. The oDPI variable is the actually desired DPI for a
      // bitmap image of the page (not necessarily the same as tDPI, but
      // it usually is).  The default RESOLUTION is 600 dots per inch.
      int tDPI;
      int oDPI;

      // The lineWidth variable is the pixel width of lines in SCORE.  This
      // controls the width of staff lines in particular, but also the width
      // of stems, and strokes around all graphical objects other than beams.
      // (Beams are handled from a setting in the SCORE preferences file
      // found in the lib directory (prefs-4.scr).  The default value is
      // 4.0 pixels when the RESOLUTION is 600.
      double lineWidth;

      // The mirror variable is used to control the reversal of the page
      // printing.  This is used for printing music backwards for photo
      // engraving.  Current cannot be changed from regular orientation.
      // If mirror == 1, then the music will be reversed from left to right.
      int mirror;

      // The setStroke variable is used to control insertion of setAdjustStroke
      // functionality into the PostScript output.  This is used to generate
      // uniform line thicknesses in bitmap renderings of the PostScript
      // output.  By default this option is turned on when printing in
      // SCORE.  setStroke == 0 will turn off the setStroke option.
      int setStroke;

      /// Printing Constants //////////////////////////////////////////////
      //
      // Below are variables which cannot be changed with the SCORE
      // editor, so they are store here as constants.
      //
      // Lbuffer  == extra padding between left margin and P3=0.0.
      // Bbuffer  == extra padding between bottom margin and P4=0 of P2=1.
      // StaffLen == length of staff lines at page scaling=1.0.
      // sDPI     == internal quantization when printing EPS files.
      //

      constexpr static double lbuffer  = 0.025;   // in inches
      constexpr static double bbuffer  = 0.0625;  // in inches
      constexpr static double staffLen = 7.5;     // in inches
      constexpr static int    sDPI     = 4000;    // dots per inch
};



#endif /* _SCOREPAGEBASE_PRINTINFO_H_INCLUDED */

