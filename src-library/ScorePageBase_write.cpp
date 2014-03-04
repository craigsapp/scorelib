//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 16:26:52 PST 2014
// Last Modified: Mon Mar  3 16:26:55 PST 2014
// Filename:      ScorePageBase_write.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase_write.cpp
// Syntax:        C++11
//
// Description:   Data reading functions for ScorePageBase.
//

#include "ScorePageBase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// ScorePageBase writing functions.
//

//////////////////////////////
//
// ScorePageBase::printPmx -- Print data as ASCII PMX data.
//     default value: roundQ   = 1
//     default value: verboseQ = 0
//

ostream& ScorePageBase::printPmx(ostream& out, int roundQ, int verboseQ) {
   if (verboseQ) {
      cout << "# OBJECTS TO WRITE: " << getItemCount() << endl;
   }

   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      (*it)->printPmx(out);
   }

   out << flush;
   return out;
}



//////////////////////////////
//
// ScorePageBase::printPmx -- Print PMX data with any named parameters.
//

ostream& ScorePageBase::printPmxWithNamedParameters(ostream& out, int roundQ,
      int verboseQ) {
   if (verboseQ) {
      cout << "# OBJECTS TO WRITE: " << getItemCount() << endl;
   }

   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      (*it)->printPmx(out);
   }

   out << flush;
   return out;
}



//////////////////////////////
//
// ScorePageBase::printPmxFixedParameters -- Print PMX data without 
//      any named parameters.
//

ostream& ScorePageBase::printPmxFixedParameters(ostream& out, int roundQ,
      int verboseQ) {
   if (verboseQ) {
      cout << "# OBJECTS TO WRITE: " << getItemCount() << endl;
   }

   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      (*it)->printPmxFixedParameters(out);
   }

   out << flush;
   return out;
}



//////////////////////////////
//
// ScorePageBase::writeBinary --  Write SCORE data to a binary files.
//

void ScorePageBase::writeBinary(const char* filename) {
   #ifdef VISUAL
      fstream outfile(filename, ios::out | ios::binary);
   #else
      fstream outfile(filename, ios::out);
   #endif

   if (!outfile.is_open()) {
      cerr << "Error: cannot write file: " << filename << endl;
      exit(1);
   }

   writeBinary(outfile);
   outfile.close();
}



//////////////////////////////
//
// ScorePageBase::writeBinary --  Write SCORE data to a binary output stream.
//     Data is stored in a temporary buffer until all of the items
//     have been written.  Then the total parameter count (4-byte groupings)
//     in the file have been written.
//

ostream& ScorePageBase::writeBinary(ostream& outfile) {

   stringstream temps;

   int writecount = 0;   // number of 4-byte values which have been written

   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      writecount += (*it)->writeBinary(temps);
   }

   // write the trailer, must be at least (6) numbers long:
   // (0) -9999.0
   // (1) size of trailer after and including this number, typically 5.0
   // (2) units
   // (3) version
   // (4) serial
   // (5) 0.0

   if (trailer.size() < 6) {
      trailer.resize(6, 0.0);
   }
   // make sure that the first value is -9999.0 (the last byte in the file);
   trailer[0] = -9999.0f;
   // make sure that the second value is the size of the trailer minus 1:
   trailer[1] = (float)(trailer.size() - 1);
   // make sure that the last value in trailer is 0.0:
   trailer.back() = 0.0f;

   // write the trailer
   vectorF::reverse_iterator itf;
   for (itf = trailer.rbegin(); itf != trailer.rend(); itf++) {
      writeLittleFloat(temps, *itf);
      writecount++;
   }

   int version = trailer[3];
   if ((version < 6.0) && (writecount > 0xffff)) {
      cerr << "Warning: data count (" << writecount << ") exceeds safe level"
           << endl;
   }

   // print the number of 4-byte groups in the data:
   if (writecount < 0xffff) {
      // write two byte counter
      unsigned char blo = writecount & 0xff;
      unsigned char bhi = (writecount >> 8) & 0xff;
      outfile.write((char*)&blo, 1);
      outfile.write((char*)&bhi, 1);
   } else {
      // write four-byte counter 
      unsigned char bloest = writecount & 0xff;
      unsigned char bloer  = (writecount >> 8)  & 0xff;
      unsigned char bhier  = (writecount >> 16) & 0xff;
      unsigned char bhiest = (writecount >> 24) & 0xff;
      outfile.write((char*)&bloest, 1);
      outfile.write((char*)&bloer,  1);
      outfile.write((char*)&bhier,  1);
      outfile.write((char*)&bhiest, 1);
   }

   outfile << temps.rdbuf();

   return outfile;
}



//////////////////////////////
//
// ScorePageBase::writeLittleFloat -- Write a 4-byte float in little-endian
//      byte order.
//

ostream& ScorePageBase::writeLittleFloat(ostream& out, SCORE_FLOAT number) {
   union { float f; unsigned int i; } num;
   num.f = (float)number;
   char byteinfo[4];
   byteinfo[0] = (char)( num.i        & 0xff);
   byteinfo[1] = (char)((num.i >> 8)  & 0xff);
   byteinfo[2] = (char)((num.i >> 16) & 0xff);
   byteinfo[3] = (char)((num.i >> 24) & 0xff);
   out.write(byteinfo, 4);
   return out;
}



//////////////////////////////
//
// operator<< -- print ASCII PMX version of page.
//

ostream& operator<<(ostream& out, ScorePageBase& aPage) {
   return aPage.printPmx(out);
}


/////////////////////////////
//
// ScorePageBase::printAsciiTrailer -- Print data parameters from the
//    SCORE binary file trailer:
//    #UNITS: inches|centimeters
//    #VERSION: 3.0|4.0|5.0|6.0
//    #SERIAL: serial number (or 0 if none, such as in versions < 4.0)
//

ostream& ScorePageBase::printAsciiTrailer(ostream& out) {
   SCORE_FLOAT units = getUnits();
   if (units == 0.0) {
      out << "#UNITS:\tinches\n";
   } else if (units == 1.0) {
      out << "#UNITS:\tcentimeters\n";
   } else {
      out << "#UNITS:\t" << units << "\n";
   }

   SCORE_FLOAT version = getVersion();
   out << "#VERSION:\t" << version << "\n";
  
   int serial = getSerialNumber();
   if (serial != 0) {
      out << "#SERIAL:\t" << serial << endl;
   }

   return out;
}

