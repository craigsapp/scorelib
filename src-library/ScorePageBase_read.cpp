//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 16:25:02 PST 2014
// Last Modified: Mon Mar  3 16:25:04 PST 2014
// Filename:      ScorePageBase_read.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase_read.cpp
// Syntax:        C++11
//
// Description:   Data reading functions for ScorePageBase.
//

#include "ScorePageBase.h"
#include <fstream>
#include <string.h>
#include <sstream>

using namespace std;


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


void ScorePageBase::read(istream& instream, int verboseQ) {
   readStream(instream, verboseQ);
}


//////////////////////////////
//
// ScorePageBase::readFile -- check whether the file is a binary or
//    ascii file, and read accordingly.
//     default value: verboseQ = 0
//

void ScorePageBase::readFile(const string& filename, int verboseQ) {
   readFile(filename.data(), verboseQ);
}


void ScorePageBase::readFile(const char* filename, int verboseQ) {
   int binaryQ = 0;  // to test if reading a binary or PMX data file.

   ifstream testfile(filename);

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
   setFilename(filename);
}



void ScorePageBase::readStream(istream& testfile, int verboseQ) {
   int binaryQ = 0;  // to test if reading a binary or PMX data file.

   if (&testfile == &cin) {
      // presume that standard input is PMX data, and not binary data.
      binaryQ = 0;
   } else {
      // The last 4 bytes of a binary SCORE file are 00 3c 1c c6 which equals
      // the float value -9999.0.
      testfile.seekg(-4, ios::end);
      unsigned char databytes[4] = {0xff};
      testfile.read((char*)databytes, 4);
      if (databytes[0] == 0x00 && databytes[1] == 0x3c && 
            databytes[2] == 0x1c && databytes[3] == 0xc6) {
         binaryQ = 1;
      } else {
         binaryQ = 0;
      }
      testfile.seekg(0, ios::beg);
   }

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
   setFilename(filename);
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
         // setPageOwner will store the pointer for the ScoreItem
         // on the given page.  The page will delete it when it
         // is deconstructed.
         sip->setPageOwner(this);
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

   vectorSF parameters;
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
            parameters[12] = (SCORE_FLOAT)length;
         }
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
   item_storage.push_back(sip);
   return sip;
}



//////////////////////////////
//
// ScorePageBase::addPmxData -- Add one or more PMX line of data to the 
//     page.
//

void ScorePageBase::addPmxData(istream& data) {
   while (!data.eof()) {
      readPmxScoreLine(data);
   }
}

void ScorePageBase::addPmxData(const string& data) {
   istringstream instream(data);
   while (!instream.eof()) {
      readPmxScoreLine(instream);
   }
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
   setFilename(filename);
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
         readcount += (int)number;
         sip->setPageOwner(this);
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



