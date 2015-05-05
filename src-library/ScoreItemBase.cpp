//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:20:50 PST 2014
// Last Modified: Fri Feb 14 23:33:43 PST 2014
// Filename:      ScoreItemBase.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItemBase.cpp
// Syntax:        C++11
//
// Description:   The ScoreItemBase class forms the basic data storage
//                component for storing SCORE item parameters.
//

#include "ScorePageBase.h"
#include "ScoreUtility.h"
#include <stdlib.h>
#include <string>
#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <string.h>
#include <sstream>

using namespace std;

using _SF = SCORE_FLOAT;

string ScoreItemBase::emptyString("");

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
// ScoreItemBase::clear -- Erase all of the contents of the item.
//     Currently this function is invisible to edit history management...
//

void ScoreItemBase::clear(void) {
   history_list.clear();
   fixed_parameters.clear();
   named_parameters.clear();
   fixed_text.clear();
   // page_owner: not altered for now
   // not altered for now: sort_sequence
   // not altered for now: staff_duration_offset
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
   auto it = named_parameters.find(nspace);
   if (it == named_parameters.end()) {
      return emptyString;
   }
   auto it2 = it->second.find(key);
   if (it2 == it->second.end()) {
      return emptyString;
   }
   return it2->second;

   // This one-line version will create an empty value in the map:
   // return named_parameters[nspace][key];
}


const string& ScoreItemBase::getParameter(const string& key) {
   return getParameter("", key);
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


int ScoreItemBase::getParameterInt(const string& nspace, const string& key) {
   int value;
   try {
      value = stoi(getParameter(nspace, key));
   } catch (invalid_argument& e) {
      value = 0;
   }
   return value;
}


bool ScoreItemBase::getParameterBool(const string& nspace, const string& key) {
   if (!isDefined(nspace, key)) {
      return false;
   }
   if (getParameter(nspace, key) == "false") {
      return false;
   } else if (getParameter(nspace, key) == "0") {
      return false;
   }

   return true;
}


bool ScoreItemBase::getParameterBoolean(const string& nspace, const string& key) {
   return getParameterBool(nspace, key);
}


bool ScoreItemBase::getParameterBool(const string& key) {
   return getParameterBool("", key);
}


bool ScoreItemBase::getParameterBoolean(const string& key) {
   return getParameterBool(key);
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
   if ((pindex < 0) || (pindex >= (int)fixed_parameters.size())) {
      return 0.0;
   }
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


_SF ScoreItemBase::getParameter1(void)  { return getParameter(P1);  }
_SF ScoreItemBase::getParameter2(void)  { return getParameter(P2);  }
_SF ScoreItemBase::getParameter3(void)  { return getParameter(P3);  }
_SF ScoreItemBase::getParameter4(void)  { return getParameter(P4);  }
_SF ScoreItemBase::getParameter5(void)  { return getParameter(P5);  }
_SF ScoreItemBase::getParameter6(void)  { return getParameter(P6);  }
_SF ScoreItemBase::getParameter7(void)  { return getParameter(P7);  }
_SF ScoreItemBase::getParameter8(void)  { return getParameter(P8);  }
_SF ScoreItemBase::getParameter9(void)  { return getParameter(P9);  }
_SF ScoreItemBase::getParameter10(void) { return getParameter(P10); }
_SF ScoreItemBase::getParameter11(void) { return getParameter(P11); }
_SF ScoreItemBase::getParameter12(void) { return getParameter(P12); }
_SF ScoreItemBase::getParameter13(void) { return getParameter(P13); }
_SF ScoreItemBase::getParameter14(void) { return getParameter(P14); }
_SF ScoreItemBase::getParameter15(void) { return getParameter(P15); }
_SF ScoreItemBase::getParameter16(void) { return getParameter(P16); }
_SF ScoreItemBase::getParameter17(void) { return getParameter(P17); }
_SF ScoreItemBase::getParameter18(void) { return getParameter(P18); }
_SF ScoreItemBase::getParameter19(void) { return getParameter(P19); }
_SF ScoreItemBase::getParameter20(void) { return getParameter(P20); }
_SF ScoreItemBase::getParameter21(void) { return getParameter(P21); }
_SF ScoreItemBase::getParameter22(void) { return getParameter(P22); }
_SF ScoreItemBase::getParameter23(void) { return getParameter(P23); }
_SF ScoreItemBase::getParameter24(void) { return getParameter(P24); }
_SF ScoreItemBase::getParameter25(void) { return getParameter(P25); }
_SF ScoreItemBase::getParameter26(void) { return getParameter(P26); }
_SF ScoreItemBase::getParameter27(void) { return getParameter(P27); }
_SF ScoreItemBase::getParameter28(void) { return getParameter(P28); }
_SF ScoreItemBase::getParameter29(void) { return getParameter(P29); }
_SF ScoreItemBase::getParameter30(void) { return getParameter(P30); }

_SF ScoreItemBase::getP1(void)  { return getParameter(P1);  }
_SF ScoreItemBase::getP2(void)  { return getParameter(P2);  }
_SF ScoreItemBase::getP3(void)  { return getParameter(P3);  }
_SF ScoreItemBase::getP4(void)  { return getParameter(P4);  }
_SF ScoreItemBase::getP5(void)  { return getParameter(P5);  }
_SF ScoreItemBase::getP6(void)  { return getParameter(P6);  }
_SF ScoreItemBase::getP7(void)  { return getParameter(P7);  }
_SF ScoreItemBase::getP8(void)  { return getParameter(P8);  }
_SF ScoreItemBase::getP9(void)  { return getParameter(P9);  }
_SF ScoreItemBase::getP10(void) { return getParameter(P10); }
_SF ScoreItemBase::getP11(void) { return getParameter(P11); }
_SF ScoreItemBase::getP12(void) { return getParameter(P12); }
_SF ScoreItemBase::getP13(void) { return getParameter(P13); }
_SF ScoreItemBase::getP14(void) { return getParameter(P14); }
_SF ScoreItemBase::getP15(void) { return getParameter(P15); }
_SF ScoreItemBase::getP16(void) { return getParameter(P16); }
_SF ScoreItemBase::getP17(void) { return getParameter(P17); }
_SF ScoreItemBase::getP18(void) { return getParameter(P18); }
_SF ScoreItemBase::getP19(void) { return getParameter(P19); }
_SF ScoreItemBase::getP20(void) { return getParameter(P20); }
_SF ScoreItemBase::getP21(void) { return getParameter(P21); }
_SF ScoreItemBase::getP22(void) { return getParameter(P22); }
_SF ScoreItemBase::getP23(void) { return getParameter(P23); }
_SF ScoreItemBase::getP24(void) { return getParameter(P24); }
_SF ScoreItemBase::getP25(void) { return getParameter(P25); }
_SF ScoreItemBase::getP26(void) { return getParameter(P26); }
_SF ScoreItemBase::getP27(void) { return getParameter(P27); }
_SF ScoreItemBase::getP28(void) { return getParameter(P28); }
_SF ScoreItemBase::getP29(void) { return getParameter(P29); }
_SF ScoreItemBase::getP30(void) { return getParameter(P30); }

int ScoreItemBase::getP1Int(void)  { return (int)getParameter(P1);  }
int ScoreItemBase::getP2Int(void)  { return (int)getParameter(P2);  }
int ScoreItemBase::getP3Int(void)  { return (int)getParameter(P3);  }
int ScoreItemBase::getP4Int(void)  { return (int)getParameter(P4);  }
int ScoreItemBase::getP5Int(void)  { return (int)getParameter(P5);  }
int ScoreItemBase::getP6Int(void)  { return (int)getParameter(P6);  }
int ScoreItemBase::getP7Int(void)  { return (int)getParameter(P7);  }
int ScoreItemBase::getP8Int(void)  { return (int)getParameter(P8);  }
int ScoreItemBase::getP9Int(void)  { return (int)getParameter(P9);  }
int ScoreItemBase::getP10Int(void) { return (int)getParameter(P10); }
int ScoreItemBase::getP11Int(void) { return (int)getParameter(P11); }
int ScoreItemBase::getP12Int(void) { return (int)getParameter(P12); }
int ScoreItemBase::getP13Int(void) { return (int)getParameter(P13); }
int ScoreItemBase::getP14Int(void) { return (int)getParameter(P14); }
int ScoreItemBase::getP15Int(void) { return (int)getParameter(P15); }
int ScoreItemBase::getP16Int(void) { return (int)getParameter(P16); }
int ScoreItemBase::getP17Int(void) { return (int)getParameter(P17); }
int ScoreItemBase::getP18Int(void) { return (int)getParameter(P18); }
int ScoreItemBase::getP19Int(void) { return (int)getParameter(P19); }
int ScoreItemBase::getP20Int(void) { return (int)getParameter(P20); }
int ScoreItemBase::getP21Int(void) { return (int)getParameter(P21); }
int ScoreItemBase::getP22Int(void) { return (int)getParameter(P22); }
int ScoreItemBase::getP23Int(void) { return (int)getParameter(P23); }
int ScoreItemBase::getP24Int(void) { return (int)getParameter(P24); }
int ScoreItemBase::getP25Int(void) { return (int)getParameter(P25); }
int ScoreItemBase::getP26Int(void) { return (int)getParameter(P26); }
int ScoreItemBase::getP27Int(void) { return (int)getParameter(P27); }
int ScoreItemBase::getP28Int(void) { return (int)getParameter(P28); }
int ScoreItemBase::getP29Int(void) { return (int)getParameter(P29); }
int ScoreItemBase::getP30Int(void) { return (int)getParameter(P30); }


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


bool ScoreItemBase::getParameterBool(int pindex) {
   return getParameterInt(pindex) ? true : false;
}

bool ScoreItemBase::getParameterBoolean(int pindex) {
   return getParameterBool(pindex);
}



//////////////////////////////
//
// ScoreItemBase::getParameterFraction --
//

SCORE_FLOAT ScoreItemBase::getParameterFraction(int pindex) {
   SCORE_FLOAT value = getParameter(pindex);
   if (value < 0.0) {
      return fabs(value) - (int)fabs(value);
   } else {
      return value - (int)value;
   }
}



//////////////////////////////
//
// ScoreItemBase::setParameter -- Set either fixed parameter if the
//     first argument is an integer, or set a named parameter if the
//     first argument is a string.
//

void ScoreItemBase::setParameterQuiet(const string& nspace, const string& key,
      const string& value) {
   named_parameters[nspace][key] = value;
}


void ScoreItemBase::setParameterQuiet(const string& nspace, const string& key,
      int value) {
   named_parameters[nspace][key] = to_string(value);
}


void ScoreItemBase::setParameterQuiet(const string& nspace, const string& key,
      SCORE_FLOAT value) {
   named_parameters[nspace][key] = to_string(value);
}


void ScoreItemBase::setParameterQuiet(const string& nspace, const string& key,
      void* pointer) {
   named_parameters[nspace][key] = to_string((uint64_t)pointer);
}


void ScoreItemBase::storeAutoAddress(void) {
   setParameterQuiet(ns_auto, np_address, this);
}


void ScoreItemBase::setParameter(const string& nspace, const string& key,
      const string& value) {
   setParameterQuiet(nspace, key, value);
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
   if ((value < 0.0001) && (value > -0.0001)) {
      value = 0;
   }
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


void ScoreItemBase::setParameter1(_SF  value) { setParameter(P1,  value); }
void ScoreItemBase::setParameter2(_SF  value) { setParameter(P2,  value); }
void ScoreItemBase::setParameter3(_SF  value) { setParameter(P3,  value); }
void ScoreItemBase::setParameter4(_SF  value) { setParameter(P4,  value); }
void ScoreItemBase::setParameter5(_SF  value) { setParameter(P5,  value); }
void ScoreItemBase::setParameter6(_SF  value) { setParameter(P6,  value); }
void ScoreItemBase::setParameter7(_SF  value) { setParameter(P7,  value); }
void ScoreItemBase::setParameter8(_SF  value) { setParameter(P8,  value); }
void ScoreItemBase::setParameter9(_SF  value) { setParameter(P9,  value); }
void ScoreItemBase::setParameter10(_SF value) { setParameter(P10, value); }
void ScoreItemBase::setParameter11(_SF value) { setParameter(P11, value); }
void ScoreItemBase::setParameter12(_SF value) { setParameter(P12, value); }
void ScoreItemBase::setParameter13(_SF value) { setParameter(P13, value); }
void ScoreItemBase::setParameter14(_SF value) { setParameter(P14, value); }
void ScoreItemBase::setParameter15(_SF value) { setParameter(P15, value); }
void ScoreItemBase::setParameter16(_SF value) { setParameter(P16, value); }
void ScoreItemBase::setParameter17(_SF value) { setParameter(P17, value); }
void ScoreItemBase::setParameter18(_SF value) { setParameter(P18, value); }
void ScoreItemBase::setParameter19(_SF value) { setParameter(P19, value); }
void ScoreItemBase::setParameter20(_SF value) { setParameter(P20, value); }
void ScoreItemBase::setParameter21(_SF value) { setParameter(P21, value); }
void ScoreItemBase::setParameter22(_SF value) { setParameter(P22, value); }
void ScoreItemBase::setParameter23(_SF value) { setParameter(P23, value); }
void ScoreItemBase::setParameter24(_SF value) { setParameter(P24, value); }
void ScoreItemBase::setParameter25(_SF value) { setParameter(P25, value); }
void ScoreItemBase::setParameter26(_SF value) { setParameter(P26, value); }
void ScoreItemBase::setParameter27(_SF value) { setParameter(P27, value); }
void ScoreItemBase::setParameter28(_SF value) { setParameter(P28, value); }
void ScoreItemBase::setParameter29(_SF value) { setParameter(P29, value); }
void ScoreItemBase::setParameter30(_SF value) { setParameter(P30, value); }

void ScoreItemBase::setP1(SCORE_FLOAT  value) { setParameter(P1,  value); }
void ScoreItemBase::setP2(SCORE_FLOAT  value) { setParameter(P2,  value); }
void ScoreItemBase::setP3(SCORE_FLOAT  value) { setParameter(P3,  value); }
void ScoreItemBase::setP4(SCORE_FLOAT  value) { setParameter(P4,  value); }
void ScoreItemBase::setP5(SCORE_FLOAT  value) { setParameter(P5,  value); }
void ScoreItemBase::setP6(SCORE_FLOAT  value) { setParameter(P6,  value); }
void ScoreItemBase::setP7(SCORE_FLOAT  value) { setParameter(P7,  value); }
void ScoreItemBase::setP8(SCORE_FLOAT  value) { setParameter(P8,  value); }
void ScoreItemBase::setP9(SCORE_FLOAT  value) { setParameter(P9,  value); }
void ScoreItemBase::setP10(SCORE_FLOAT value) { setParameter(P10, value); }
void ScoreItemBase::setP11(SCORE_FLOAT value) { setParameter(P11, value); }
void ScoreItemBase::setP12(SCORE_FLOAT value) { setParameter(P12, value); }
void ScoreItemBase::setP13(SCORE_FLOAT value) { setParameter(P13, value); }
void ScoreItemBase::setP14(SCORE_FLOAT value) { setParameter(P14, value); }
void ScoreItemBase::setP15(SCORE_FLOAT value) { setParameter(P15, value); }
void ScoreItemBase::setP16(SCORE_FLOAT value) { setParameter(P16, value); }
void ScoreItemBase::setP17(SCORE_FLOAT value) { setParameter(P17, value); }
void ScoreItemBase::setP18(SCORE_FLOAT value) { setParameter(P18, value); }
void ScoreItemBase::setP19(SCORE_FLOAT value) { setParameter(P19, value); }
void ScoreItemBase::setP20(SCORE_FLOAT value) { setParameter(P20, value); }
void ScoreItemBase::setP21(SCORE_FLOAT value) { setParameter(P21, value); }
void ScoreItemBase::setP22(SCORE_FLOAT value) { setParameter(P22, value); }
void ScoreItemBase::setP23(SCORE_FLOAT value) { setParameter(P23, value); }
void ScoreItemBase::setP24(SCORE_FLOAT value) { setParameter(P24, value); }
void ScoreItemBase::setP25(SCORE_FLOAT value) { setParameter(P25, value); }
void ScoreItemBase::setP26(SCORE_FLOAT value) { setParameter(P26, value); }
void ScoreItemBase::setP27(SCORE_FLOAT value) { setParameter(P27, value); }
void ScoreItemBase::setP28(SCORE_FLOAT value) { setParameter(P28, value); }
void ScoreItemBase::setP29(SCORE_FLOAT value) { setParameter(P29, value); }
void ScoreItemBase::setP30(SCORE_FLOAT value) { setParameter(P30, value); }


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
// ScoreItemBase::addNamedParameters --
//

void ScoreItemBase::addNamedParameters(mapNamespace& input) {
   mapNamespace::iterator it;
   mapSS::iterator its;
   for (it = input.begin(); it != input.end(); it++) {
      for (its = it->second.begin(); its != it->second.end(); its++) {
         named_parameters[it->first][its->first] = its->second;
      }
   }
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


//
// Aliases:
//

int ScoreItemBase::isDefined(const string& nspace, const string& testkey) {
   return hasParameter(nspace, testkey);
}


int ScoreItemBase::isDefined(const string& testkey) {
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
// ScoreItemBase::changeNamespace --  Move a named parameter from
//    one namespace to another.
//

int ScoreItemBase::changeNamespace(const string& newspace,
      const string& oldspace, const string& parameter) {
   if (!isDefined(oldspace, parameter)) {
      return 0;
   }
   setParameter(newspace, parameter, getParameter(oldspace, parameter));
   deleteParameter(oldspace, parameter);
   // deleteParameter does notifyPageOfChange("named");
   return 1;
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
   SCORE_FLOAT p1 = getP1();
   if (((int)p1) == 16) {
      out << 't';
   } else {
      out << p1;
   }
   int i;
   SCORE_FLOAT value;
   for (i=P2; i<=count; i++) {
      value = getParameter(i);
      if (fabs(value) < 0.000001) {
         value = 0.0;
      }
      out << ' ' << value;
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
            out << '@' << it->first << "@" << its->first
                << ":\t" << its->second << endl;
         }
      }
   }
   return out;
}


ostream& ScoreItemBase::printPmxNamedParametersNoAuto(ostream& out) {
   mapNamespace& np = named_parameters;
   mapNamespace::iterator it;
   mapSS::iterator its;
   for (it = np.begin(); it != np.end(); it++) {
      if (it->first == ns_auto) {
         continue;
      }
      for (its = it->second.begin(); its != it->second.end(); its++) {
         if (it->first == "") {
            out << '@' << its->first << ":\t" << its->second << endl;
         } else {
            out << '@' << it->first << "@" << its->first
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
   int p1 = getP1();
   if (p1 == 15) {
      out << " filename=\"";
      SU::printXmlTextEscapedUTF8(out, getFixedText());
      out << "\"";
   }
   if (p1 == 16) {
      out << " text=\"";
      SU::printXmlTextEscapedUTF8(out, getFixedText());
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
      SU::printXmlTextEscapedUTF8(out, it->first);
      out << "\" value=\"";
      SU::printXmlTextEscapedUTF8(out, it->second);
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
// ScoreItemBase::readPmx --  This version does not read named
//    parameters.  See ScorePageBase::readPmx and 
//    ScorePageBase::readNamedParameter for implementing here as well.
//

void ScoreItemBase::readPmx(istream& instream, int verboseQ) {
   char buffer[1024] = {0};
   instream.getline(buffer, 1000, '\n');
   if (verboseQ) {
      cout << "#Read line: " << buffer << endl;
   }

   vectorSF parameters;
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
            parameters[12] = (SCORE_FLOAT)length;
         }
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
   if ((getPInt(P1) == P1_ImportedEPSGraphic) || (getPInt(P1) == P1_Text)) {
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

      float wordsize = fixedcount + (getFixedText().size() + pad) / 4;

      // write the total number of 4-byte words to follow in item:
      writeLittleEndianFloat(out, wordsize);

      for (i=1; i<=fixedcount; i++) {
         writeLittleEndianFloat(out, getP(i));
      }

      // write filename/text string:
      out << getFixedText();

      // write padding after filename/text string:
      for (i=0; i<pad; i++) {
         out << ' ';
      }
      return wordsize + 1;

   } else {
      // SCORE items which do not include a text field:
      writeLittleEndianFloat(out, fixedcount);

      for (i=1; i<=fixedcount; i++) {
         writeLittleEndianFloat(out, getP(i));
      }
      return fixedcount + 1;
   }
}



//////////////////////////////
//
// ScoreItemBase::readNamedParameter -- Read input lines such as:
//    @namespace@key: value
// For PMX data named parameters.
//

void ScoreItemBase::readNamedParameter(mapNamespace& np, char* input) {
   int len = strlen(input);
   char* buf1 = new char[len+4];
   char* buf2 = new char[len+4];
   char* buf3 = new char[len+4];
   const char* nspace = NULL;
   const char* key = NULL;
   const char* value = NULL;
   int i1 = 0;  // write index for namespace string
   int i2 = 0;  // write index for key string
   int i3 = 0;  // write index for value string

   buf1[i1] = '\0';
   buf2[i2] = '\0';
   buf3[i3] = '\0';

   int atcount = 0;
   // int atloc[2];
   // atloc[0] = -1;
   // atloc[1] = -1;

   // find first two at-signs:
   int i;
   for (i=0; i<len; i++) {
      if (input[i] == ':') {
         // Either the end of the @namespace@key: sequence, or not 
         // allowed to have colon in namespace string (or key):
         break;
      }
      if (input[i] == '@') {
         // atloc[atcount++] = i;
         if (atcount > 2) {
            // can't have @ sign in namespace or key, ignore parameter:
            delete [] buf1;
            delete [] buf2;
            delete [] buf3;
            return;
         }
         continue;
      }
      if (std::isspace(input[i])) {
         // ignore spaces in namespace or key:
         continue;
      }

      if (atcount == 1) {
         buf1[i1++] = input[i];
         buf1[i1] = '\0';
      } else if (atcount == 2) {
         buf2[i2++] = input[i];
         buf2[i2] = '\0';
      }
   }

   if (atcount == 0) {
      // Not a named parameter; do nothing.
      delete [] buf1;
      delete [] buf2;
      delete [] buf3;
      return;
   }

   // should pointing to colon, exit function if not:
   if (input[i] != ':') {
      delete [] buf1;
      delete [] buf2;
      delete [] buf3;
      return;
   }

   // skip any whitespace at start of value:
   while (++i<len) {
      if (!std::isspace(input[i])) {
         break;
      }
   }

   // copy value into buf3:
   while (i<len) {
      buf3[i3++] = input[i];
      buf3[i3] = '\0';
      i++;
   }

   // trim space off of end of value
   while (--i3 > 0) {
      if (std::isspace(buf3[i3])) {
         buf3[i3] = '\0';
      } else {
         break;
      }
   }

   if (atcount == 1) {
      // global namespace (empty string):
      nspace = buf2;
      key = buf1;
   } else {
      nspace = buf1;
      key = buf2;
   }
   value = buf3;

   if (key[0] == '\0') {
      // Don't allow a null key string
      delete [] buf1;
      delete [] buf2;
      delete [] buf3;
      return;
   }

   // Store the parameter:
   np[nspace][key] = value;

   delete [] buf1;
   delete [] buf2;
   delete [] buf3;
}


