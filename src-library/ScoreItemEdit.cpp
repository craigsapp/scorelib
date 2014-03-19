//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:54:35 PST 2014
// Last Modified: Sat Feb 15 18:38:20 PST 2014
// Filename:      ScoreItemEdit.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItemEdit.cpp
// Syntax:        C++11
//
// Description:   The ScoreItemEdit class adds parameter history tracking
//                to the ScoreItemEdit class.
//

#include "ScoreItemEdit.h"

using namespace std;

unsigned int ScoreItemEdit::history_index  = 0;
int          ScoreItemEdit::history_active = 0;
int          ScoreItemEdit::history_thawed = 1;


///////////////////////////////
//
// ScoreItemEdit::ScoreItemEdit -- constructor.
//

ScoreItemEdit::ScoreItemEdit(void) : ScoreItemBase() {
   // do nothing
}


ScoreItemEdit::ScoreItemEdit(const ScoreItemBase& anItem) : 
      ScoreItemBase(anItem) {
   // do nothing
}

ScoreItemEdit::ScoreItemEdit(const ScoreItemEdit& anItem) : 
      ScoreItemBase((ScoreItemBase)anItem) {
   // do nothing
}


ScoreItemEdit::ScoreItemEdit(const vectorSF& parameters) : 
      ScoreItemBase(parameters) {
   // do nothing
}



///////////////////////////////
//
// ScoreItemEdit::~ScoreItemEdit -- deconstructor.
//

ScoreItemEdit::~ScoreItemEdit() {
   // do nothing
}



///////////////////////////////
//
// ScoreItemEdit::operator= -- 
//

ScoreItemEdit& ScoreItemEdit::operator=(const ScoreItemEdit& anItem) {
   if (this == &anItem) {
      return *this;
   }

   *(ScoreItemBase*)this = (ScoreItemBase)anItem;

   // history not copied, or probably better, 
   // store all old parameters in history?
   return *this;
}



///////////////////////////////
//
// ScoreItemEdit::clearHistory -- Forget all stored edit history.
//

void ScoreItemEdit::clearHistory(void) {
   history_list.clear();
}


//////////////////////////////
//
// ScoreItemEdit::deleteNamespace --
//

void ScoreItemEdit::deleteNamespace(const string& nspace) {
   mapSS& params = named_parameters[nspace];
   for (auto& it : params) {
      history_list.emplace_back(nspace, it.first, it.second, 
            SCOREITEM_DELETED, history_index);
   }
   named_parameters.erase(nspace);
}

// Aliases for above function:

void ScoreItemEdit::eraseNamespace(const string& nspace) {
   deleteNamespace(nspace);
}

void ScoreItemEdit::clearNamespace(const string& nspace) {
   deleteNamespace(nspace);
}


//////////////////////////////
//
// ScoreItemEdit::deleteParameter --
//

void ScoreItemEdit::deleteParameter(const string& nspace, const string& key) { 
   int exists = hasParameter(nspace, key);
   if (exists) {
      string value = getParameter(nspace, key);
      history_list.emplace_back(nspace, key, value, SCOREITEM_DELETED, 
            history_index);
      ScoreItemBase::deleteParameter(nspace, key);
   }
}


// Aliases for above function:

void ScoreItemEdit::eraseParameter(const string& nspace, const string& key) { 
   deleteParameter(nspace, key);
}


void ScoreItemEdit::clearParameter(const string& nspace, const string& key) { 
   deleteParameter(nspace, key);
}


void ScoreItemEdit::deleteParameter(const string& key) { 
   deleteParameter("", key);
}


void ScoreItemEdit::eraseParameter(const string& key) { 
   deleteParameter("", key);
}


void ScoreItemEdit::clearParameter(const string& key) { 
   deleteParameter("", key);
}



/////////////////////////////
//
// setParameter --
//

void ScoreItemEdit::setParameter(const string& nspace, const string& key, 
      const string& value) { 
   if (historyIsActive()) {
      if (historyIsThawed()) {
         incrementHistoryIndex();
      }
      if (hasParameter(nspace, key)) {
         string currentValue = getParameter(nspace, key);
         if (currentValue == value) {
            return;
         } 
         appendToHistory(nspace, key, currentValue, value);
      } else {
         appendToHistory(nspace, key, 1, value);
      }
   }
   ScoreItemBase::setParameter(nspace, key, value);
}


void ScoreItemEdit::setParameter(const string& nspace, const string& key, 
      int value) { 
   setParameter(nspace, key, to_string(value));
}


void ScoreItemEdit::setParameter(const string& nspace, const string& key, 
      SCORE_FLOAT value) { 
   setParameter(nspace, key, to_string(value));
}


void ScoreItemEdit::setParameter(const string& key, 
      const string& value) { 
   setParameter("", key, value);
}


void ScoreItemEdit::setParameter(const string& key, int value) { 
   setParameter("", key, value);
}


void ScoreItemEdit::setParameter(const string& key, SCORE_FLOAT value) { 
   setParameter("", key, value);
}


void ScoreItemEdit::setParameter(int index, SCORE_FLOAT value) { 
   if (historyIsActive()) {
      if (historyIsThawed()) {
         incrementHistoryIndex();
      }
      SCORE_FLOAT currentValue = getParameter(index);
      if (currentValue == value) {
         return;
      } 
      appendToHistory(index, currentValue, value);
   }
   ScoreItemBase::setParameter(index, value);
}



//////////////////////////////
//
// ScoreItemEdit::setFixedText --
//

void ScoreItemEdit::setFixedText(const string& text) { 
   if (text == getFixedText()) {
      return;
   } 
   if (historyIsActive()) {
      if (historyIsThawed()) {
         incrementHistoryIndex();
      }
      appendToHistory("_FIXED_TEXT", getFixedText(), text);
   }
   ScoreItemBase::setFixedText(text);
}



// Aliases for the above functions:

void ScoreItemEdit::setP(const string& nspace, const string& key, 
      const string& value) {
   setParameter(nspace, key, value);
}


void ScoreItemEdit::setP(const string& nspace, const string& key, 
      int value) {
   setParameter(nspace, key, value);
}


void ScoreItemEdit::setP(const string& nspace, const string& key, 
      SCORE_FLOAT value) {
   setParameter(nspace, key, value);
}


void ScoreItemEdit::setP(const string& key, const string& value) {
   setParameter("", key, value);
}


void ScoreItemEdit::setP(const string& key, int value) {
   setParameter("", key, value);
}


void ScoreItemEdit::setP(const string& key, SCORE_FLOAT value) {
   setParameter("", key, value);
}


void ScoreItemEdit::setP(int index, SCORE_FLOAT value) {
   setParameter(index, value);
}



///////////////////////////////////////////////////////////////////////////
//
// printing functions
//


//////////////////////////////
//
// ScoreItemEdit::printPmx --
//

ostream& ScoreItemEdit::printPmx(ostream& out) {
   printPmxFixedParameters(out);
   printPmxNamedParameters(out);
   printPmxEditHistory(out);
   return out;
}


//////////////////////////////
//
// ScoreItemEdit::printPmxEditHistory --
//

ostream& ScoreItemEdit::printPmxEditHistory(ostream& out) {
   HistoryList::reverse_iterator it;
   for (it = history_list.rbegin(); it != history_list.rend(); it++) {
      printHistoryPmx(out, *it);
   }
   return out;
}



//////////////////////////////
//
// ScoreItemEdit::printXmlWithEdits --
//

ostream& ScoreItemEdit::printXmlWithEdits(ostream& out, int indentcount, 
      const string& indentstring) {

   printIndent(out, indentcount, indentstring);
   printFixedListPieceXml(out);

   int nspacecount = getNamespaceCount();
   int hcount = history_list.size();

   if (hcount + nspacecount == 0) {
      out << " />" << endl;
   } else {
      out << ">" << endl;
   }

   if (nspacecount > 0) {
      printIndent(out, indentcount+1, indentstring);
      out << "<named-parameters>\n";

      mapNamespace& np = named_parameters;
      int npcount;
      for (auto &it : np) {
         npcount = getNamedParameterCount(it.first);
         if (npcount > 0) {
            printIndent(out, indentcount+2, indentstring);
            out << "<namespace scope=\"";
            out << it.first;
            out << "\">\n";
               printNamedParametersXml(out, it.first, indentcount+3, 
                  indentstring);
            printIndent(out, indentcount+2, indentstring);
            out << "</namespace>\n";
         }

      }

      printIndent(out, indentcount+1, indentstring);
      out << "</named-parameters>\n";
   }

   if (hcount > 0) {
      printIndent(out, indentcount+1, indentstring);
      out << "<parameter-history current=\"";
      out << getHistoryIndex() + 1;
      out << "\">\n";
    
      printHistoryXml(out, indentcount+2, indentstring);

      printIndent(out, indentcount+1, indentstring);
      out << "</parameter-history>\n";
   }

   if (hcount + nspacecount > 0) {
      printIndent(out, indentcount, indentstring);
      out << "</item>" << endl;
   }

   return out;
}



///////////////////////////////////////////////////////////////////////////
//
// Protected Functions:
//

//////////////////////////////
//
// ScoreItemEdit::printHistoryPmx --
//

ostream& ScoreItemEdit::printHistoryPmx(ostream& out, 
      const ParameterHistory& hEntry) {
   return hEntry.printAsPmx(out);
}



//////////////////////////////
//
// ScoreItemEdit::printHistoryXml --
//

ostream& ScoreItemEdit::printHistoryXml(ostream& out, 
      int indentcount, const string& indentstring) {

   HistoryList::reverse_iterator it;
   for (it = history_list.rbegin(); it != history_list.rend(); it++) {
      printIndent(out, indentcount, indentstring);
      it->printAsXml(out);
      out << "\n";
   }

   return out;
}



//////////////////////////////
//
// ScoreItemEdit::appendToHistory --
//

void ScoreItemEdit::appendToHistory(const string& nspace, const string& key, 
      const string& oldvalue, const string& newvalue) {
    history_list.emplace_back(nspace, key, oldvalue, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(const string& nspace, const string& key, 
      int created_state, const string& newvalue) {
    history_list.emplace_back(nspace, key, created_state, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(const string& nspace, const string& key, 
      const string& oldvalue, int deleted_state) {
    history_list.emplace_back(nspace, key, oldvalue, deleted_state, history_index);
}


void ScoreItemEdit::appendToHistory(const string& key, const string& oldvalue,
      const string& newvalue) {
    history_list.emplace_back("", key, oldvalue, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(const string& key, int created_state,
      const string& newvalue) {
    history_list.emplace_back("", key, created_state, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(const string& key, const string& oldvalue,
      int deleted_state) {
    history_list.emplace_back("", key, oldvalue, deleted_state, history_index);
}


void ScoreItemEdit::appendToHistory(int index, SCORE_FLOAT oldvalue, 
      SCORE_FLOAT newvalue) {
    history_list.emplace_back(index, oldvalue, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(int index, int created_state,
      SCORE_FLOAT newvalue) {
    history_list.emplace_back(index, created_state, newvalue, history_index);
}


void ScoreItemEdit::appendToHistory(int index, SCORE_FLOAT oldvalue, 
      int deleted_state) {
    history_list.emplace_back(index, oldvalue, deleted_state, history_index);
}



///////////////////////////////////////////////////////////////////////////
//
// Static Functions:
//

///////////////////////////////
//
// ScoreItemEdit::incrementHistoryIndex --
//

void ScoreItemEdit::incrementHistoryIndex(void) {
   history_index++;
}



///////////////////////////////
//
// ScoreItemEdit::thawHistoryIndex -- Prevent the history index from 
//    incrementing automatically.
//

void ScoreItemEdit::freezeHistoryIndex(void) {
   history_thawed = 0;
}



///////////////////////////////
//
// ScoreItemEdit::thawHistoryIndex -- All the history index to increment 
//    automatically.
//

void ScoreItemEdit::thawHistoryIndex(void) {
   history_thawed = 1;
}



///////////////////////////////
//
// ScoreItemEdit::startHistory -- Start recording parameter changes.
//

void ScoreItemEdit::startHistory(void) {
   history_active = 1;
}



///////////////////////////////
//
// ScoreItemEdit::stopHistory -- Stop keeping track of the history.
//

void ScoreItemEdit::stopHistory(void) {
   history_active = 0;
}



//////////////////////////////
//
// ScoreItemEdit::historyIsActive -- returns true if parameter changes should
//     be recorded.
//

int ScoreItemEdit::historyIsActive(void) {
   return history_active;
}



//////////////////////////////
//
// ScoreItemEdit::historyIsThawed -- returns true if history index will
//     increment automatically.
//

int ScoreItemEdit::historyIsThawed(void) {
   return history_thawed;
}



//////////////////////////////
//
// ScoreItemEdit::historyIsFrozen -- returns true if history index will
//     not be incremented automatically.
//

int ScoreItemEdit::historyIsFrozen(void) {
   return !history_thawed;
}



//////////////////////////////
//
// ScoreItemEdit::getHistoryIndex -- returns the current edit history index.
//

int ScoreItemEdit::getHistoryIndex(void) {
   return history_index;
}


