//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Thu Apr 10 16:45:59 PDT 2014
// Last Modified: Thu Apr 10 16:46:02 PDT 2014
// Filename:      ScorePageOverlay_write.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePageOverlay_write.cpp
// Syntax:        C++11
//
// Description:   This class handles multiple pages of the ScorePage class.
//

#include "ScorePageOverlay.h"

using namespace std;

#include <typeinfo>


//////////////////////////////
//
// operator<< -- Print a ScorePageOverlay object as multi-page PMX data.
//

ostream& operator<<(ostream& out, ScorePageOverlay& overlay) {
   static int cereal = 1;
   if (overlay[0].isMultipageAsComment()) {
      out << "###StartPage:\t" << overlay[0].getFilenameBase() << endl;
   } else {
      out << "RS" << endl;
      string filename = overlay[0].getFilenameBase();
      if (filename.compare("") == 0) {
         filename = "page" + to_string(cereal);
         cereal++;
      }
      out << "SA " << filename << endl;
   }
   out << endl;

   out << overlay[0];
   for (int i=1; i<overlay.getOverlayCount(); i++) {
      out << "\n";
      out << "###StartOverlay:\t";
      if (overlay[i].getFilenameBase() != "") {
         out << overlay[i].getFilenameBase();
      } else {
         out << overlay[0].getFilenameBase() << "-" << i;
      }
      out  << endl;
      out << "\n";
      out << overlay[i];
   }

   if (!overlay[0].isMultipageAsComment()) {
      cout << "\nSM" << endl;
   }

   return out;
}



