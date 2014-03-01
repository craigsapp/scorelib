//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:20:50 PST 2014
// Last Modified: Fri Feb 14 23:33:43 PST 2014
// Filename:      ScoreItemBase.cpp
// Web Address:   https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItemBase.cpp
// Syntax:        C++11
//
// Description:   The ScoreItemBase class forms the basic data storage 
//                component for storing SCORE item parameters.
//

#include "ScorePageBase.h"
#include <stdlib.h>
#include <string>
#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <string.h>
#include <sstream>

using namespace std;

//////////////////////////////
//
// ScoreItemBase::ScoreItemBase -- constructor.
//

ScoreItemBase::ScoreItemBase(void) {
   page_owner       = NULL;
   staff_duration_offset = -1;
}


ScoreItemBase::ScoreItemBase(const ScoreItemBase& anItem) {
   fixed_parameters = anItem.fixed_parameters;
   named_parameters = anItem.named_parameters;
   fixed_text       = anItem.fixed_text;
   page_owner       = NULL;
   staff_duration_offset = -1;
}


ScoreItemBase::ScoreItemBase(const vectorSF& parameters) {
   fixed_parameters.reserve(parameters.size()+1);
   fixed_parameters.push_back(0.0); // 0th index not used
   fixed_parameters.insert(fixed_parameters.end(), 
         parameters.begin(), parameters.end());
   page_owner       = NULL;
   staff_duration_offset = -1;
}


ScoreItemBase::ScoreItemBase(const string& stringitem) {
   page_owner = NULL;
   staff_duration_offset = -1;
   stringstream ss;
   ss << stringitem;
   readPmx(ss);
}



//////////////////////////////
//
// ScoreItemBase::~ScoreItemBase -- deconstructor.
//

ScoreItemBase::~ScoreItemBase() {
   // do nothing
}



//////////////////////////////
//
// ScoreItemBase::operator= -- Copy from another item object.
//

ScoreItemBase& ScoreItemBase::operator=(const ScoreItemBase& anItem) {
   if (this == &anItem) {
      return *this;
   }

   fixed_parameters = anItem.fixed_parameters;
   named_parameters = anItem.named_parameters;
   fixed_text       = anItem.fixed_text;
   page_owner       = NULL;
   staff_duration_offset = -1;

   return *this;
}



//////////////////////////////
//
// ScoreItemBase::getParameter -- return a fixed or named parameter value,
//      depending on the data type of the input argument.  Integer values
//      will access the fixed parameter list, string values will access
//      the named parameter list.
//

const string& ScoreItemBase::getParameter(const string& nspace, 
      const string& key) {
   return named_parameters[nspace][key];
}


const string& ScoreItemBase::getParameter(const string& key) {
   return named_parameters[""][key];
}


int ScoreItemBase::getParameterInt(const string& key) {
   int value;
   try {
      value = stoi(getParameter(key));
   } catch (invalid_argument& e) {
      value = 0;
   }
   return value;
}


double ScoreItemBase::getParameterDouble(const string& key) {
   double value;
   try {
      value = stod(getParameter(key));
   } catch (invalid_argument& e) {
      value = 0.0;
   }
   return value;
}


double ScoreItemBase::getParameterDouble(const string& nspace, 
      const string& key) {
   double value;
   try {
      value = stod(getParameter(nspace, key));
   } catch (invalid_argument& e) {
      value = 0.0;
   }
   return value;
}


SCORE_FLOAT ScoreItemBase::getParameter(int pindex) {
   return fixed_parameters[pindex];
}


// Aliases for the above functions:

const string& ScoreItemBase::getP(const string& nspace, 
      const string& key) {
   return getParameter(nspace, key);
}


const string& ScoreItemBase::getP(const string& key) {
   return getParameter(key);
}


int ScoreItemBase::getPInt(const string& key) {
   return getParameterInt(key);
}

double ScoreItemBase::getPDouble(const string& key) {
   return getParameterDouble(key);
}


SCORE_FLOAT ScoreItemBase::getP(int pindex) {
   return getParameter(pindex);
}



//////////////////////////////
//
// ScoreItemBase::getParameterInt -- return the int version of the value.
//

int ScoreItemBase::getParameterInt(int pindex) {
   return (int)getParameter(pindex);
}


int ScoreItemBase::getPInt(int pindex) {
   return getParameterInt(pindex);
}



//////////////////////////////
//
// ScoreItemBase::setParameter -- Set either fixed parameter if the
//     first argument is an integer, or set a named parameter if the
//     first argument is a string.
//

void ScoreItemBase::setParameter(const string& nspace, const string& key, 
      const string& value) {
   named_parameters[nspace][key] = value;
   notifyPageOfChange("named");
}

// Aliases and variants of named parameter setting:

void ScoreItemBase::setParameter(const string& nspace, const string& key, 
      int value) {
   setParameter(nspace, key, to_string(value));
}


void ScoreItemBase::setParameter(const string& nspace, const string& key, 
      SCORE_FLOAT value) {
   char buffer[32];
   sprintf(buffer, "%g", value);
   setParameter(nspace, key, buffer);
}


void ScoreItemBase::setParameter(const string& key, const string& value) {
   setParameter("", key, value);
}


void ScoreItemBase::setParameter(const string& key, int value) {
   setParameter("", key, to_string(value));
}


void ScoreItemBase::setParameter(const string& key, SCORE_FLOAT value) {
   char buffer[32];
   sprintf(buffer, "%g", value);
   setParameter("", key, buffer);
}


void ScoreItemBase::setParameter(int index, SCORE_FLOAT value) {
   vectorSF& fp = fixed_parameters;
   // die if too large of parameter index:
   if (index > SCORE_MAX_FIXED_PARAMETERS) {
      cerr << "ERROR: too large an index: " << index << endl;
      exit(1);
   }

   // automatically grow if no storage for given index location:
   if ((int)fp.size() < index+1) {
      fp.resize(index+1, 0.0);
   }

   SCORE_FLOAT oldp = fp[index];
   fp[index] = value;
   notifyPageOfChange((string)"fixed", index, oldp, value);
}


// Aliases for the above functions:

void ScoreItemBase::setP(const string& nspace, const string& key, 
      const string& value) {
   setParameter(nspace, key, value);
}


void ScoreItemBase::setP(const string& nspace, const string& key, 
      int value) {
   setParameter(nspace, key, value);
}


void ScoreItemBase::setP(const string& nspace, const string& key, 
      SCORE_FLOAT value) {
   setParameter(nspace, key, value);
}


void ScoreItemBase::setP(const string& key, const string& value) {
   setParameter("", key, value);
}


void ScoreItemBase::setP(const string& key, int value) {
   setParameter("", key, value);
}


void ScoreItemBase::setP(const string& key, SCORE_FLOAT value) {
   setParameter("", key, value);
}


void ScoreItemBase::setP(int index, SCORE_FLOAT value) {
   setParameter(index, value);
}



//////////////////////////////
//
// ScoreItemBase::setFixedText -- Store the text string for P1=16 or P1=15.
//

void ScoreItemBase::setFixedText(const string& text) {
   fixed_text = text;
   notifyPageOfChange("text");
}



//////////////////////////////
//
// ScoreItemBase::setFixedParameterAllocation --
//

void ScoreItemBase::setFixedParameterAllocation(int asize) {
    fixed_parameters.reserve(asize);
}



//////////////////////////////
//
// ScoreItemBase::hasParameter -- returns true (1) if there is a named 
//    parameter matching the input argument.  Returns false (0) if there 
//    is not a named parameter matching the input argument.
//

int ScoreItemBase::hasParameter(const string& nspace, 
      const string& testkey) {
   return named_parameters[nspace].count(testkey);
}


int ScoreItemBase::hasParameter(const string& testkey) {
   return hasParameter("", testkey);
}



//////////////////////////////
//
// ScoreItemBase::deleteNamespace --
//

void ScoreItemBase::deleteNamespace(const string& nspace) {
   named_parameters.erase(nspace);
   notifyPageOfChange("named");
}

// Aliases for above function:

void ScoreItemBase::eraseNamespace(const string& nspace) {
   deleteNamespace(nspace);
}

void ScoreItemBase::clearNamespace(const string& nspace) {
   clearNamespace(nspace);
}



//////////////////////////////
//
// ScoreItemBase::deleteParameter --
//

void ScoreItemBase::deleteParameter(const string& nspace, const string& key) {
   named_parameters[nspace].erase(key);
   notifyPageOfChange("named");
}

// Aliases for above function:

void ScoreItemBase::eraseParameter(const string& nspace, const string& key) { 
   deleteParameter(nspace, key);
}


void ScoreItemBase::clearParameter(const string& nspace, const string& key) { 
   deleteParameter(nspace, key);
}


void ScoreItemBase::deleteParameter(const string& key) { 
   deleteParameter("", key);
}


void ScoreItemBase::eraseParameter(const string& key) { 
   deleteParameter("", key);
}


void ScoreItemBase::clearParameter(const string& key) { 
   deleteParameter("", key);
}



//////////////////////////////
//
// ScoreItemBase::getCompactFixedParameterCount -- Return the number of fixed
//     parameters, excluding any trailing zero values.  But keep at least
//     one value if all happen to be 0.0.  The first index value (parameter 0)
//     is not included in this count.
//

int ScoreItemBase::getCompactFixedParameterCount(void) {
   int i;
   vectorSF& fp = fixed_parameters;
   int length = fp.size();
   for (i=length-1; i>1; i--) {
      if (fp[i] != 0.0) {
         return i;
      }
   }
   return 1;
}



//////////////////////////////
//
// ScoreItemBase::getNamespaceCount -- 
//

int ScoreItemBase::getNamespaceCount(void) const {
   return named_parameters.size();
}



//////////////////////////////
//
// ScoreItemBase::getNamedParameterCount --
//

int ScoreItemBase::getNamedParameterCount(const string& nspace) {
   return named_parameters[nspace].size();
}


int ScoreItemBase::getNamedParameterCount(void) {
   return named_parameters[""].size();
}


// Aliases:

int ScoreItemBase::getNamedParameterSize(const string& nspace) {
   return getNamedParameterCount(nspace);
}


int ScoreItemBase::getNamedParameterSize(void) {
   return getNamedParameterCount();
}



//////////////////////////////
//
// ScoreItemBase::countAllNamedParameters --
//

int ScoreItemBase::countAllNamedParameters(void) {
   int output = 0;

   mapNamespace& np = named_parameters;
   mapNamespace::iterator it;
   for (it = np.begin(); it != np.end(); it++) {
      output += it->second.size();
   }

   return output;
}



//////////////////////////////
//
// ScoreItemBase::getFixedText --
//

const string& ScoreItemBase::getFixedText(void) const {
   return fixed_text;
}


///////////////////////////////////////////////////////////////////////////
//
// Page interface functions
//

//////////////////////////////
//
// ScoreItemBase::setPageOwner -- set a pointer to the page which
//   owns this SCORE item (the page which manages the memory allocation
//   for the item.)
//

void ScoreItemBase::setPageOwner(void* page) {
   page_owner = page;
}



//////////////////////////////
//
// ScoreItemBase::notifyPageOfChange --
//

void ScoreItemBase::notifyPageOfChange(const string& message) {
   if (page_owner != NULL) {
      ((ScorePageBase*)page_owner)->itemChangeNotification(this, message);
   }
}

void ScoreItemBase::notifyPageOfChange(const string& message, int index,
      SCORE_FLOAT oldp, SCORE_FLOAT newp) {
   if (page_owner != NULL) {
      ((ScorePageBase*)page_owner)->itemChangeNotification(this, message, 
            index, oldp, newp);
   }
}


///////////////////////////////////////////////////////////////////////////
//
// ScoreItemBase writing functions.
//


//////////////////////////////
//
// ScoreItemBase::printPmx -- print the ScoreItemBase as ASCII PMX text.  The
//     fixed parameters are listed on a single line as floats (doubles).  The 
//     named parameters are given one on each following line prefixed with @ 
//     and separated by a colon and space(s) after the key name.
//

ostream& ScoreItemBase::printPmx(ostream& out) {
   printPmxFixedParameters(out);
   printPmxNamedParameters(out);
   return out;
}


ostream& ScoreItemBase::printPmxFixedParameters(ostream& out) {
   int count = getCompactFixedParameterCount();
   // print the first number, using "t" if P1
   SCORE_FLOAT p1 = getParameter(P1);
   if (p1 == 16) {
      out << 't';
   } else {
      out << p1;
   }
   int i;
   for (i=P2; i<=count; i++) {
      out << ' ' << getParameter(i);
   }
   out << '\n';
   // if P1==15|16, then print the text associated with it here.
   if ((p1 == 16) | (p1 == 15)) {
      out << getFixedText() << '\n';
   }
   return out;
}


ostream& ScoreItemBase::printPmxNamedParameters(ostream& out) {
   mapNamespace& np = named_parameters;
   mapNamespace::iterator it;
   mapSS::iterator its;
   for (it = np.begin(); it != np.end(); it++) {
      for (its = it->second.begin(); its != it->second.end(); its++) {
         if (it->first == "") {
            out << '@' << its->first << ":\t" << its->second << endl;
         } else {
            out << '@' << it->first << "::" << its->first 
                << ":\t" << its->second << endl;
         }
      }
   }
   return out;
}



//////////////////////////////
//
// ScoreItemBase::printXml -- print the ScoreItemBase as an ScoreXML element.
//     Fixed parameters which are equal to 0.0 are not listed in the attributes,
//     since 0.0 is their default value.  Fixed parameters become attributes
//     on the element, and named parameters are subelements.
//

ostream& ScoreItemBase::printXml(ostream& out, int indentcount, 
      const string& indentstring) {

   printIndent(out, indentcount, indentstring);
   printFixedListPieceXml(out);

   int nspacecount = getNamespaceCount();
   if (nspacecount == 0) {
      out << " />" << endl;
   } else {
      out << ">" << endl;
   }

   if (nspacecount > 0) {
      printIndent(out, indentcount+1, indentstring);
      out << "<named-parameters>\n";

      mapNamespace& np = named_parameters;
      mapNamespace::iterator it;
      int npcount;
      for (it = np.begin(); it != np.end(); it++) {
         npcount = getNamedParameterCount(it->first);
         if (npcount > 0) {
            printIndent(out, indentcount+2, indentstring);
            out << "<namespace scope=\"";
            out << it->first;
            out << "\">\n";
               printNamedParametersXml(out, it->first, indentcount+3, 
                  indentstring);
            printIndent(out, indentcount+2, indentstring);
            out << "</namespace>\n";
         }

      }

      printIndent(out, indentcount+1, indentstring);
      out << "</named-parameters>\n";

      printIndent(out, indentcount, indentstring);
      out << "</item>" << endl;
   }

   return out;
}



//////////////////////////////
//
// ScoreItemBase::printXmlTextEscaped --
//

ostream& ScoreItemBase::printXmlTextEscaped(ostream& out, 
      const string& text) {

   int length = text.size();
   char ch;
   int i;
   for (i=0; i<length; i++) {
      ch = text[i];
      switch (ch) {
         case '&':
            out << "&amp;";
            break;
         case '"':
            out << "&quot;";
            break;
         case '\'':
            out << "&apos;";
            break;
         case '>':
            out << "&gt;";
            break;
         case '<':
            out << "&lt;";
            break;
         default:
            out << ch;
      }
   }

   return out;
}



//////////////////////////////
//
// ScoreItemBase::printFixedListPieceXml -- Print the fixed parameter
//    portion of an XML element.  Not closing the tag.
//

ostream& ScoreItemBase::printFixedListPieceXml(ostream& out) {
   out << "<item";
   int fpcount = getCompactFixedParameterCount();
   SCORE_FLOAT value;
   int i;
   for (i=1; i<=fpcount; i++) {
      value = getParameter(i);
      if (value != 0.0) {
         out << " p" << i << "=\"" << value << "\"";
      }
   }
   int p1 = getParameter(P1);
   if (p1 == 15) {
      out << " filename=\"";
      printXmlTextEscaped(out, getFixedText());
      out << "\"";
   }
   if (p1 == 16) {
      out << " text=\"";
      printXmlTextEscaped(out, getFixedText());
      out << "\"";
   }

   return out;
}



//////////////////////////////
//
// ScoreItemBase::printNamedParametersXml --
//

ostream& ScoreItemBase::printNamedParametersXml(ostream& out, 
      const string& nspace, int indentcount, const string& indentstring) {

   mapSS& np = named_parameters[nspace];
   mapSS::iterator it;
   for (it = np.begin(); it != np.end(); it++) {
      printIndent(out, indentcount, indentstring);
      out << "<parameter name=\"";
      printXmlTextEscaped(out, it->first);
      out << "\" value=\"";
      printXmlTextEscaped(out, it->second);
      out << "\" />";
      out << endl;
   }

   return out;
}



///////////////////////////////
//
// ScoreItemBase::printIndent --
//

ostream& ScoreItemBase::printIndent(ostream& out, int indentcount, 
      const string& indentstring) {
   for (int i=0; i<indentcount; i++) {
      out << indentstring;
   }
   return out;
}


///////////////////////////////////////////////////////////////////////////
//
// SCORE file reading/writing functions
//


//////////////////////////////
//
// ScoreItemBase::readPmx --
//

void ScoreItemBase::readPmx(istream& instream, int verboseQ) {
   char buffer[1024] = {0};
   instream.getline(buffer, 1000, '\n');
   if (verboseQ) {
      cout << "#Read line: " << buffer << endl;
   }

   vector<SCORE_FLOAT> parameters;
   parameters.reserve(16);
   parameters.push_back(0);  // first index (0) is not used.
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
         instream.getline(buffer, 1000, '\n');
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
            return;
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

   fixed_parameters = parameters;
}



//////////////////////////////
//
// ScoreItemBase::readBinary -- Read one item from a binary SCORE .MUS file.
//    Still need to deal with P1=15 (PostScript filenames).
//

void ScoreItemBase::readBinary(istream& instream, int pcount) {
   int i;
   if (pcount < 1) {
      cout << "Error: invalid parameter count: " << pcount << endl;
      exit(1);
   }
   setFixedParameterAllocation(pcount+1);
   SCORE_FLOAT value = readLittleEndianFloat(instream);;
   setParameter(P1, value);

   if (getPInt(P1) == P1_Text) {
      setFixedParameterAllocation(14+1);
      for (i=P2; i<P14; i++) {
         value = readLittleEndianFloat(instream);
         // setParameter(i, roundFractionDigits(value, 3));
         setParameter(i, value);
      }
      int count = getPInt(P12);
      char buffer[1024] = {0};
      if (count > 1000) {
         count = 1000;
      }
      instream.read(buffer, count);
      fixed_text = buffer;

      int extra = getPInt(P12) % 4;
      if (extra > 0) {
         instream.read(buffer, 4-extra);
         buffer[4-extra] = '\0';
         // extra string should be all spaces, but occasionally
         // this is not true.
         //for (i=0; i<4-extra; i++) {
         // if (buffer[i] != 0x20) {
         //   cout << "Error: non-space character found" << endl;
         //   cout << "Found character: \"" << buffer[i] << "\"" << endl;
         //   exit(1);
         // }
         //}
      }
   } else {
      // non-text data parameters
      for (i=P2; i<=pcount; i++) {
         setParameter(i, readLittleEndianFloat(instream));
      }
   }
}



///////////////////////////////
//
// ScoreItemBase::readLittleEndianFloat --
//

double ScoreItemBase::readLittleEndianFloat(istream& instream) {
   unsigned char byteinfo[4];
   instream.read((char*)byteinfo, 4);
   union { float f; uint32_t i; } num;
   num.i = 0;
   num.i = byteinfo[3];
   num.i = (num.i << 8) | byteinfo[2];
   num.i = (num.i << 8) | byteinfo[1];
   num.i = (num.i << 8) | byteinfo[0];

   return (double)num.f;
}



///////////////////////////////
//
// ScoreItemBase::writeLittleEndianFloat --
//

void ScoreItemBase::writeLittleEndianFloat(ostream& out, double number) {
   float fnumber = (float)number;
   union { float f; uint32_t i; } num;
   num.f = fnumber;
   char byteinfo[4];
   byteinfo[0] = (char)( num.i        & 0xff);
   byteinfo[1] = (char)((num.i >> 8)  & 0xff);
   byteinfo[2] = (char)((num.i >> 16) & 0xff);
   byteinfo[3] = (char)((num.i >> 24) & 0xff);
   out.write(byteinfo, 4);
}



//////////////////////////////
//
// ScoreItemBase::roundFractionDigits -- Parameter data in SCORE binary 
//    files are stored as floats.  These floats are not reliable past the third
//    decimal place in the number, so typically this fuction will be called
//    after reading a float from a binary file, using digits=3.
//

double ScoreItemBase::roundFractionDigits(double number, int digits) {
   double dshift = pow(10.0, digits);
   if (number < 0.0) {
      return (int(number * dshift - 0.5))/dshift;
   } else {
      return (int(number * dshift + 0.5))/dshift;
   }
}



//////////////////////////////
//
// ScoreItemBase::writeBinary -- print the fixed parameter data in binary
//    format as found in a SCORE .mus file where the count of four-byte
//    units to follow in the item's data is written first.  The return value
//    is the total number of bytes written divided by 4.
//

int ScoreItemBase::writeBinary(ostream& out) {
   int fixedcount = getCompactFixedParameterCount();
   if (fixedcount < 1) {
      return 0;
   }

   // WinScore can't understand data fields smaller than 3
   if (fixedcount < 3) {
      fixedcount = 3;
   }

   int i;
   if ((getPInt(P1) == P1_ImportedEPSGraphic)
         || (getPInt(P1) == P1_Text)) {
      // process a EPS file item or Text item.

      // There must be 13 parameters before the name of the file or the text.
      fixedcount = 13;

      // text is stored in fixed_text, but need to add extra spaces after 
      // filename/text to make the length of the filename/text be a multiple 
      // of four bytes
      int pad = getFixedText().size() % 4;
      if (pad > 0) {
         pad = 4 - pad;
      }

      float wordsize = fixedcount + (getFixedText().size() + pad) % 4;
    
      // write the total number of 4-byte words to follow in item:
      writeLittleEndianFloat(out, wordsize);

      for (i=1; i<=fixedcount; i++) {
         writeLittleEndianFloat(out, getP(i));
      }

      // write filename:
      out << getFixedText();
  
      // write padding after filename:
      for (i=0; i<pad; i++) {
         out << ' ';
      }
      return wordsize + 1;

   } else{

      // SCORE items which do not include the fixed text field:
      writeLittleEndianFloat(out, fixedcount);

      for (i=1; i<=fixedcount; i++) {
         writeLittleEndianFloat(out, getP(i));
      }
      return fixedcount + 1;
   }
}




