//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 21:34:02 PST 2014
// Last Modified: Thu Feb  6 01:05:52 PST 2014
// Filename:      ParameterHistory.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ParameterHistory.cpp
// Syntax:        C++11
//
// Description:   The ParameterHistory class is used by the ScoreItemEdit
//                class to store changes in parameters in the ScoreItemBase
//                class.
//

#include "ScoreItemEdit_ParameterHistory.h"
#include "ScoreItemBase.h"
#include "ScoreUtility.h"

using namespace std;


ParameterHistory::ParameterHistory(void) {
   history     = 0;
   fixed_index = -1;
   state       = SCOREITEM_UNDEFINED;
}

ParameterHistory::ParameterHistory(const string& nspace, const string& key, 
      const string& oldvalue, const string& newvalue, int hindex) {
   history         = hindex;
   named_nspace    = nspace;
   named_key       = key;
   named_oldvalue  = oldvalue;
   named_newvalue  = newvalue;
   fixed_index     = -1;
   state           = SCOREITEM_UNDEFINED;
}

ParameterHistory::ParameterHistory(const string& nspace, const string& key, 
      int created_state, const string& newvalue, int hindex) {
   history         = hindex;
   named_nspace    = nspace;
   named_key       = key;
   named_oldvalue  = "";
   named_newvalue  = newvalue;
   fixed_index     = -1;
   state           = SCOREITEM_CREATED;
}

ParameterHistory::ParameterHistory(const string& nspace, const string& key, 
      const string& oldvalue, int deleted_state, int hindex) {
   history         = hindex;
   named_nspace    = nspace;
   named_key       = key;
   named_oldvalue  = oldvalue;
   named_newvalue  = "";
   fixed_index     = -1;
   state           = SCOREITEM_DELETED;
}

ParameterHistory::ParameterHistory(const string& key, const string& oldvalue, 
      const string& newvalue, int hindex) {
   history         = hindex;
   named_nspace    = "";
   named_key       = key;
   named_oldvalue  = oldvalue;
   named_newvalue  = newvalue;
   fixed_index     = -1;
   state           = SCOREITEM_UNDEFINED;
}

ParameterHistory::ParameterHistory(const string& key, int created_state,
      const string& newvalue, int hindex) {
   history         = hindex;
   named_nspace    = "";
   named_key       = key;
   named_oldvalue  = "";
   named_newvalue  = newvalue;
   fixed_index     = -1;
   state           = SCOREITEM_CREATED;
}

ParameterHistory::ParameterHistory(const string& key, const string& oldvalue, 
      int deleted_state, int hindex) {
   history         = hindex;
   named_nspace    = "";
   named_key       = key;
   named_oldvalue  = oldvalue;
   named_newvalue  = "";
   fixed_index     = -1;
   state           = SCOREITEM_DELETED;
}

ParameterHistory::ParameterHistory(int key, SCORE_FLOAT oldvalue, SCORE_FLOAT newvalue, 
      int hindex) {
   history        = hindex;
   fixed_index    = key;
   fixed_oldvalue = oldvalue;
   fixed_newvalue = newvalue;
   state          = SCOREITEM_UNDEFINED;
}

ParameterHistory::ParameterHistory(int key, int created_state, SCORE_FLOAT newvalue, 
      int hindex) {
   history        = hindex;
   fixed_index    = key;
   fixed_oldvalue = 0;
   fixed_newvalue = newvalue;
   state          = SCOREITEM_CREATED;
}

ParameterHistory::ParameterHistory(int key, SCORE_FLOAT oldvalue, int deleted_state,
      int hindex) {
   history        = hindex;
   fixed_index    = key;
   fixed_oldvalue = oldvalue;
   fixed_newvalue = 0;
   state          = SCOREITEM_DELETED;
}

void ParameterHistory::clear(void) {
   history         = 0;
   named_nspace    = "";
   named_key       = "";
   named_oldvalue  = "";
   named_newvalue  = "";
   fixed_index     = -1;
   fixed_oldvalue  = 0;
   fixed_newvalue  = 0;
   state           = SCOREITEM_UNDEFINED;
}

ostream& ParameterHistory::printAsPmx(ostream& out) const {
   out << "@_HISTORY-" << history << ":";
   if (fixed_index >= 0) {
      out << fixed_index << ":\t" << fixed_oldvalue 
          << "\t=>\t" << fixed_newvalue << endl;
   } else {
      if (named_nspace == "") {
         out << "@" << named_key << ":\t";
         if (state == SCOREITEM_CREATED) {
            out << "_CREATED"; 
         } else {
            out << named_oldvalue;
         }
         out  << "\t=>\t";
         if (state == SCOREITEM_DELETED) {
            out << "_DELETED";
         } else {
            out << named_newvalue;
         }
         out << endl;
      } else {
         out << "@" << named_nspace << "::" 
             << named_key << ":\t";
         if (state == SCOREITEM_CREATED) {
            out << "_CREATED";
         } else {
            out << named_oldvalue;
         }
         out << "\t=>\t";
         if (state == SCOREITEM_DELETED) {
            out << "_DELETED";
         } else {
            out << named_newvalue;
         }
         out << endl;
      }
   }
   return out;
}

ostream& ParameterHistory::printAsXml(ostream& out) {
   out << "<parameter";
   out << " history=\"" << history << "\"";
   if (fixed_index < 0) {
      if (named_nspace != "") {
         out << " namespace=\"";
         SU::printXmlTextEscapedUTF8(out, named_nspace);
         out << "\"";
      }
      out << " name=\"";
      SU::printXmlTextEscapedUTF8(out, named_key);
      out << "\"";

      if (state == SCOREITEM_CREATED) {
         out << " created=\"true\"";
      } else {
         out << " oldvalue=\"";
         SU::printXmlTextEscapedUTF8(out, named_oldvalue);
         out << "\"";
      }

      if (state == SCOREITEM_DELETED) {
         out << " deleted=\"true\"";
      } else {
         out << " newvalue=\"";
         SU::printXmlTextEscapedUTF8(out, named_newvalue);
         out << "\"";
      }

   } else {
      out << " index=\"" << fixed_index << "\"";

      if (state == SCOREITEM_CREATED) {
         out << " created=\"true\"";
      } else {
         out << " oldvalue=\"" << fixed_oldvalue << "\"";
      }

      if (state == SCOREITEM_DELETED) {
         out << " deleted=\"true\"";
      } else {
         out << " newvalue=\"" << fixed_newvalue << "\"";
      }

   }
   out << " />";

   return out;
}



