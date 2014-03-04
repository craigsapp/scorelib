//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sat Mar  1 02:51:26 PST 2014
// Filename:      ScorePageBase.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase.cpp
// Syntax:        C++11
//
// Description:   Base class for ScorePage.  This class handles reading/writing
//                of a ScorePage, and handles all of the data variables for
//                a ScorePage.
//

#include "ScorePageBase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>

using namespace std;


//////////////////////////////
//
// ScorePageBase::ScorePageBase -- Constructor functions.  ScorePageBase
//    can be constructed as an empty container, or data can be loaded
//    from a file, input stream, or copied from the contents of another page.
//

ScorePageBase::ScorePageBase(void) {
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
}


ScorePageBase::ScorePageBase(const char* filename) {
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
   readFile(filename);
}


ScorePageBase::ScorePageBase(istream& instream) { 
   trailer.reserve(8);
   setDefaultPrintParameters();
   clearAnalysisStates();
   readFile(instream);
}


ScorePageBase::ScorePageBase(const ScorePageBase& apage) { 
   // make a copy of an existing page
   // have to deal with unique ids...
   trailer.reserve(8);
   clearAnalysisStates();

   print_info = apage.print_info;
   
   listSIp::const_iterator it;
   const listSIp& itemlist = apage.item_storage;
   ScoreItem* sip;
   for (it = itemlist.begin(); it != itemlist.end(); it++) {
      if ((*it) != NULL) {
         sip = new ScoreItem(**it);
         item_storage.push_back(sip);
      }
   }
}



//////////////////////////////
//
// ScorePageBase::~ScorePageBase -- Delete the ScoreItem storage
//

ScorePageBase::~ScorePageBase() {
   clear();
}



/////////////////////////////
//
// ScorePageBase::clear -- Remove all SCORE items from page.
//

void ScorePageBase::clear(void) {
   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      if ((*it) != NULL) {
         delete (*it);
         *it = NULL;
      }
   }
}



//////////////////////////////
//
// ScorePageBase::prependItem -- make a copy of the item and store 
//   it in the item_storage list. Returns a pointer to the 
//   stored item.
//

ScoreItem* ScorePageBase::prependItem(ScoreItem& anItem) {
   ScoreItem* ptr = new ScoreItem(anItem);
   item_storage.push_front(ptr);
   return ptr;
}



//////////////////////////////
//
// ScorePageBase::appendItem -- make a copy of the item and store 
//   it in the item_storage list. Returns a pointer to the 
//   stored item.
//

ScoreItem* ScorePageBase::appendItem(ScoreItem& anItem) {
   ScoreItem* ptr = new ScoreItem(anItem);
   item_storage.push_back(ptr);
   return ptr;
}


ScoreItem* ScorePageBase::appendItem(const string& itemstring) {
   ScoreItem* ptr = new ScoreItem(itemstring);
   item_storage.push_back(ptr);
   return ptr;
}



//////////////////////////////
//
// ScorePageBase::setDefaultPrintParameters -- Set print parameters to 
//     their default values.
//

void ScorePageBase::setDefaultPrintParameters(void) {
   print_info.setDefaultValues();
}



//////////////////////////////
//
// ScorePageBase::getItemCount -- Return the number of ScoreItems owned 
//     by the page.
//

int ScorePageBase::getItemCount(void) {
   return item_storage.size();
}



//////////////////////////////
//
// ScorePageBase::getFileOrderList -- Return a list of the SCORE items
//   in the sequence in which they are found in the file (such as
//   the input data read from a file).
//

void ScorePageBase::getFileOrderList(vectorSIp& data) {
   data.reserve(item_storage.size());
   data.clear();
   listSIp::iterator it;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      data.push_back(*it);
   }
}



//////////////////////////////
//
// ScorePageBase::getStaffItemList --  Return a 2D list of items on the
//    page.  The first dimension is the staff number (P2), the second
//    dimension is a list of SCORE items on the staff.  The order of the
//    SCORE items is the file order, not necessarily in time/spatial order.
//

void ScorePageBase::getStaffItemList (vectorVSIp& data) {
   data.reserve(MAX_STAFF_COUNT);
   listSIp::iterator it;
   unsigned int staffnum;
   for (it = item_storage.begin(); it != item_storage.end(); it++) {
      staffnum = (*it)->getStaffNumber();
      if (data.size() < staffnum + 1) {
         data.resize(staffnum+1);
      }
      data[staffnum].push_back(*it);
   }
}



//////////////////////////////
//
// ScorePageBase::getStaffItemListOrdered --
//


void ScorePageBase::getStaffItemListOrdered(vectorVSIp& data) {
   getStaffItemList(data);
   vectorVSIp::iterator it;
   for (it = data.begin(); it != data.end(); it++) {
      sort(it->begin(), it->end(), sortP3P4P1);
   }
}




///////////////////////////////////////////////////////////////////////////
//
// ScorePageBase trailer accessing functions.  The SCORE binary file trailer
// typically has 6 float values stored in reverse order (0) is the last
// value in the file:
//
// 0: -9999.0 which is the end of file marker for a SCORE binary file.
// 1: The size of the trailer after index 0 (including this value).  Typically
//    this is 5.0 but may be larger in later versions of score.
// 2: measurement units: 0.0 = inches, 1.0 = centimeters.
// 3: Program version number
// 4: Program serial number
// 5: 0.0 which means end of trailer.  This is the first 4-byte parameter
//    in the trailer stored in a binary file.  Typically this value would
//    be a non-zero parameter count for a SCORE item.  However, when a zero
//    is read for the parameter size, this means that the trailer begins
//    here (and must be read backwards to determine the size and meaning
//    of the fields in the trailer.
//

//////////////////////////////
//
// ScorePageBase::setVersionWinScore -- set version to WinScore.
//

void ScorePageBase::setVersionWinScore(void) {
   if (trailer.size() < 4) {
      trailer.resize(4, 0.0);
   }
   trailer[3] = 6.0;
}



//////////////////////////////
//
// ScorePageBase::getVersion -- Get the SCORE version that was red from 
//      a binary SCORE file. Return 4.0 if undefined.
//

SCORE_FLOAT ScorePageBase::getVersion(void) {
   if (trailer.size() < 4) {
      return 4.0;
   } else {
      return trailer[3];
   }
}



//////////////////////////////
//
// ScorePageBase::setSerialNumber -- Set the SCORE serial number for writing in
//      binary SCORE files.
//

void ScorePageBase::setSerialNumber(uint32_t value) {
   union { uint32_t i; float f; } u;
   u.i = value;
   if (trailer.size() < 5) {
      trailer.resize(5, 0.0);
   }
   trailer[4] = u.f;
}


inline void ScorePageBase::setSerial(uint32_t value) {
   setSerialNumber(value);
}



//////////////////////////////
//
// ScorePageBase::getSerial -- Get the SCORE serial number that was read
//      from a binary SCORE file.  This value may be 0.0 if SCORE version 3
//      or less.
//

uint32_t ScorePageBase::getSerialNumber(void) {
   union { uint32_t i; float f; } u;
   if (trailer.size() > 4) {
      u.f = trailer[4];
      return u.i;
   } else {
      return 0;
   }
}


// Alias:

inline uint32_t ScorePageBase::getSerial(void) {
   return getSerialNumber();
}


//////////////////////////////
//
// ScorePageBase::getUnits -- return 0.0 for inches (or undefined), 
//     or 1.0 for centimeters.  These units are neede for certain types
//     of staff vertical placement types.
//

SCORE_FLOAT ScorePageBase::getUnits(void) {
   if (trailer.size() > 2) {
      return trailer[2];
   } else {
      return 0.0;
   }
}



//////////////////////////////
//
// ScorePageBase::setUnitsInches -- Sets the trailer parameter for
//     units to 0.0 for inches.
//

void ScorePageBase::setUnitsInches(void) {
   if (trailer.size() > 2) {
      trailer[2] = 0.0;
   } else {
      trailer.resize(3, 0.0);
   }
}



//////////////////////////////
//
// ScorePageBase::setUnitsCentimeters -- Sets the trailer parameter for
//     units to 1.0 for centimeters.
//

void ScorePageBase::setUnitsCentimeters(void) {
   if (trailer.size() < 2) {
      trailer.resize(3, 0.0);
   }
   trailer[2] = 1.0;
}


//
// Trailer accessor functions.
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
// ScorePageBase reading functions.
//

//////////////////////////////
//
// ScorePageBase::read -- read binary or ASCII data files or input streams.
//

void ScorePageBase::read(const char* filename, int verboseQ) {
   readFile(filename, verboseQ);
}


void ScorePageBase::read(const string& filename, int verboseQ) {
   readFile(filename.c_str(), verboseQ);
}


void ScorePageBase::read(istream& infile, int verboseQ) {
   readFile(infile, verboseQ);
}


//////////////////////////////
//
// ScorePageBase::readFile -- check whether the file is a binary or
//    ascii file, and read accordingly.
//     default value: verboseQ = 0
//

void ScorePageBase::readFile(const char* filename, int verboseQ) {
   int binaryQ = 0;  // to test if reading a binary or PMX data file.

   #ifdef VISUAL
      ifstream testfile(filename, ios::binary);
   #else
      ifstream testfile(filename);
   #endif

   if (!testfile.is_open()) {
      cerr << "Error: cannot read the file: " << filename << endl;
      exit(1);
   }

   // The last 4 bytes of a binary SCORE file are 00 3c 1c c6 which equals
   // the float value -9999.0.
   testfile.seekg(-4, ios::end);
   unsigned char databytes[4] = {0xff};
   testfile.read((char*)databytes, 4);
   if (databytes[0] == 0x00 && databytes[1] == 0x3c && databytes[2] == 0x1c &&
         databytes[3] == 0xc6) {
      binaryQ = 1;
   } else {
      binaryQ = 0;
   }
   testfile.seekg(0, ios::beg);

   if (binaryQ) {
      readBinary(testfile, verboseQ);
   } else {
      readPmx(testfile, verboseQ);
   }
}



void ScorePageBase::readFile(istream& testfile, int verboseQ) {
   int binaryQ = 0;  // to test if reading a binary or PMX data file.

   // The last 4 bytes of a binary SCORE file are 00 3c 1c c6 which equals
   // the float value -9999.0.
   testfile.seekg(-4, ios::end);
   unsigned char databytes[4] = {0xff};
   testfile.read((char*)databytes, 4);
   if (databytes[0] == 0x00 && databytes[1] == 0x3c && databytes[2] == 0x1c &&
         databytes[3] == 0xc6) {
      binaryQ = 1;
   } else {
      binaryQ = 0;
   }
   testfile.seekg(0, ios::beg);

   if (binaryQ) {
      readBinary(testfile, verboseQ);
   } else {
      readPmx(testfile, verboseQ);
   }
}



//////////////////////////////
//
// ScorePageBase::readPmx -- Read data in ASCII PMX format from given input 
//     textfile.
//     default value: verboseQ = 0
//

void ScorePageBase::readPmx(const char* filename, int verboseQ) {
   ifstream infile(filename, ios::in);

   if (!infile.is_open()) {
      cerr << "Error: cannot read file: " << filename << endl;
      exit(1);
   }

   readPmx(infile, verboseQ);
   infile.close();
}



//////////////////////////////
//
// ScorePageBase::readPmx --  Read a single SCORE page in the ASCII
//      PMX data format from an input stream.
//

void ScorePageBase::readPmx(istream& infile, int verboseQ) {
   clear();

   ScoreItem* sip = NULL; 
   while (!infile.eof()) {
      sip = readPmxScoreLine(infile, verboseQ);
      if (sip != NULL) {
         item_storage.push_back(sip);
      }
   }
}



//////////////////////////////
//
// ScorePageBase::readPmxScoreLine -- read a PMX file for score data.
//   ignore any lines of text which do not start with a number.
//   Also have to deal with P1=15 EPS files.
//

ScoreItem* ScorePageBase::readPmxScoreLine(istream& infile, int verboseQ) {
   char buffer[1024] = {0};
   infile.getline(buffer, 1000, '\n');
   if (verboseQ) {
      cout << "#Read line: " << buffer << endl;
   }

   vector<SCORE_FLOAT> parameters;
   parameters.reserve(ScoreItemBase::SCORE_MAX_FIXED_PARAMETERS);
   string text;
   
   char* ptr = strtok(buffer, "\n\t ");
   float number = 0.0;
   if (ptr != NULL) {
      if (strcmp(ptr, "t") == 0) {
         // process text parameter
         number = 16.0;
         parameters.push_back(number);
         ptr = strtok(NULL, "\n\t ");
         while (ptr != NULL) {
            number = (SCORE_FLOAT)strtod(ptr, NULL);
            parameters.push_back(number);
            ptr = strtok(NULL, "\n\t ");
         }
         // now read the text line for a text item
         infile.getline(buffer, 1000, '\n');
         if (verboseQ) {
            cout << "#Read text line: " << buffer << endl;
         }
         text = buffer;
         int length = text.size();
         if (parameters.size() < 13) {
            parameters.resize(13, 0.0);
         }
         parameters[12] = (SCORE_FLOAT)length;
      } else {
         // process non-text parameter
         number = (SCORE_FLOAT)strtod(ptr, NULL);
         if (number == 0.0) {
            // P1=0 is not a valid PMX item type
            return NULL;
         } else {
            parameters.push_back(number);
            ptr = strtok(NULL, "\n\t ");
            while (ptr != NULL) {
               number = (SCORE_FLOAT)strtod(ptr, NULL);
               parameters.push_back(number);
               ptr = strtok(NULL, "\n\t ");
            }
         }
      }
   }

   if (parameters.size() == 0) {
      return NULL;
   }

   ScoreItem* sip = new ScoreItem(parameters);
   sip->setPageOwner(this);
   if (text.size() > 0) {
      sip->setFixedText(text);
   }
   return sip;
}



//////////////////////////////
//
// ScorePageBase::readBinary -- Read a single SCORE page in the binary
//     data format.
//     default value: verboseQ = 0;
//

void ScorePageBase::readBinary(const char* filename, int verboseQ) {
   #ifdef VISUAL
      fstream infile(filename, ios::in | ios::binary);
   #else
      fstream infile(filename, ios::in);
   #endif

   if (!infile.is_open()) {
      cerr << "Error: cannot open file: " << filename << endl;
      exit(1);
   }

   readBinary(infile, verboseQ);
}



//////////////////////////////
//
// ScorePageBase::readBinary --  Read a single SCORE page in the binary
//      data format from an input stream.
//

void ScorePageBase::readBinary(istream& infile, int verboseQ) {
   clear();

   // first read the count of 4-byte numbers/text chunks in the data file.
   int numbercount = readLittleShort(infile);
   int readcount = 0;   // number of 4-byte values which have been read
   if (verboseQ) {
      cout << "#NUMBER COUNT OF FILE IS " << numbercount << endl;
   }

   // now read the count of numbers in the trailer
   infile.seekg(-8, ios::end);
   int trailerSize = (int)readLittleFloat(infile);

   // go back to the start of the file, after the first number
   infile.seekg(2, ios::beg);

   ScoreItem* sip = NULL;
   double number = 0.0;
   // now read each data number and store
   while (!infile.eof()) {
      if (numbercount - readcount - trailerSize - 1 == 0) {
         break;
      } else if (numbercount - readcount - trailerSize - 1< 0) {
         cout << "Error reading file: data mixes with trailer: "
              << numbercount - readcount - trailerSize - 1
              << endl;
         exit(1);
      } else {
         number = readLittleFloat(infile);
         readcount++;
         number = ScoreItemBase::roundFractionDigits(number, 3);
         if (verboseQ) {
            if (number - (int)number > 0.0) {
               cout << "# Error in number parameter count: " << number << endl;
               exit(1);
            }
         }
         sip = new ScoreItem;
         sip->readBinary(infile, number);
         sip->setPageOwner(this);
         readcount += (int)number;
         item_storage.push_back(sip);
      }
   }

   if (verboseQ) {
      cout << "#Elements: " << item_storage.size() << endl;
      cout << "#READING Trailer: " << endl;
   }

   trailer.clear();
   trailer.reserve(10);
   while (number != -9999.0 && !infile.eof()) {
      number = readLittleFloat(infile);
      trailer.push_back(number);
      if (verboseQ) {
         cout << "#TRAILER NUMBER: " << number << endl;
      }
      readcount++;
   }

   if (readcount != numbercount) {
      cerr << "#Warning: expecting " << numbercount << " numbers in file "
           << " but read " << readcount << endl;
   }
}




//////////////////////////////
//
// ScorePageBase::readLittleShort -- read a short int in little endian form.
//     Number is read as an unsigned short int.
//

int ScorePageBase::readLittleShort(istream& input) {
   unsigned char byteinfo[2];
   input.read((char*)byteinfo, 2);
   int output = 0;
   output = byteinfo[1];
   output = (output << 8) | byteinfo[0];
   return output;
}



//////////////////////////////
//
// ScorePageBase::readLittleFloat -- Read a 4-byte float in little-endian
//      format.
//

SCORE_FLOAT ScorePageBase::readLittleFloat(istream& instream) {
   unsigned char byteinfo[4];
   instream.read((char*)byteinfo, 4);
   union { float f; uint32_t i; } num;
   num.i = 0;
   num.i = byteinfo[3];
   num.i = (num.i << 8) | byteinfo[2];
   num.i = (num.i << 8) | byteinfo[1];
   num.i = (num.i << 8) | byteinfo[0];
   return (SCORE_FLOAT)num.f;
}


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


//
// Score writing functions
//
///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// ScorePageBase::itemChangeNotification -- When a ScoreItem in the
//     item_storage list changes one of its parameters, it will notify
//     the page that it has been changed.  The Page can then decide what,
//     if anything, should be done.
//
// Three messages known by this function:
//    "fixed" : a fixed parameter has been changed.
//    "named" : a named parameter has been changed.
//    "text"  : the fixed parameter text field has been changed.
//
//

void ScorePageBase::itemChangeNotification(ScoreItemBase* sitem, 
      const string& message ) {
   analysis_info.notmodified = 0;
}

void ScorePageBase::itemChangeNotification(ScoreItemBase* sitem, 
      const string& message, int index, SCORE_FLOAT oldp, SCORE_FLOAT newp ) {
   analysis_info.notmodified = 0;
   if (monitor_P3 && (index == P3) && (oldp != newp)) {
      // move item in sorted P3 list.
   }
}



//////////////////////////////
//
// ScorePageBase::clearAnalysisStates -- Set all anlaysis variables to
//     false.
//

void ScorePageBase::clearAnalysisStates(void) {
   analysis_info.clear();
}



