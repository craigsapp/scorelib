//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 19:54:35 PST 2014
// Last Modified: Sun Feb 16 00:25:35 PST 2014
// Filename:      ScoreItemEdit.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreItemEdit.h
// Syntax:        C++11
//
// Description:   The ScoreItemEdit class adds parameter history tracking
//                to the ScoreItemEdit class.
//

#ifndef _SCOREITEMEDIT_H_INCLUDED
#define _SCOREITEMEDIT_H_INCLUDED

#include "ScoreItemBase.h"

using namespace std;

#define SCOREITEM_UNDEFINED 0
#define SCOREITEM_CREATED   1
#define SCOREITEM_DELETED   2

class ParameterHistory {
   protected:
      int         history;
      string      named_nspace;
      string      named_key;
      string      named_oldvalue;
      string      named_newvalue;
      int         fixed_index;
      SCORE_FLOAT fixed_oldvalue;
      SCORE_FLOAT fixed_newvalue;
      int         state;

    public:

      ParameterHistory(void) {
         history     = 0;
         fixed_index = -1;
         state       = SCOREITEM_UNDEFINED;
      }

      ParameterHistory(const string& nspace, const string& key, 
            const string& oldvalue, const string& newvalue, int hindex) {
         history         = hindex;
         named_nspace    = nspace;
         named_key       = key;
         named_oldvalue  = oldvalue;
         named_newvalue  = newvalue;
         fixed_index     = -1;
         state           = SCOREITEM_UNDEFINED;
      }

      ParameterHistory(const string& nspace, const string& key, 
            int created_state, const string& newvalue, int hindex) {
         history         = hindex;
         named_nspace    = nspace;
         named_key       = key;
         named_oldvalue  = "";
         named_newvalue  = newvalue;
         fixed_index     = -1;
         state           = SCOREITEM_CREATED;
      }

      ParameterHistory(const string& nspace, const string& key, 
            const string& oldvalue, int deleted_state, int hindex) {
         history         = hindex;
         named_nspace    = nspace;
         named_key       = key;
         named_oldvalue  = oldvalue;
         named_newvalue  = "";
         fixed_index     = -1;
         state           = SCOREITEM_DELETED;
      }

      ParameterHistory(const string& key, const string& oldvalue, 
            const string& newvalue, int hindex) {
         history         = hindex;
         named_nspace    = "";
         named_key       = key;
         named_oldvalue  = oldvalue;
         named_newvalue  = newvalue;
         fixed_index     = -1;
         state           = SCOREITEM_UNDEFINED;
      }

      ParameterHistory(const string& key, int created_state,
            const string& newvalue, int hindex) {
         history         = hindex;
         named_nspace    = "";
         named_key       = key;
         named_oldvalue  = "";
         named_newvalue  = newvalue;
         fixed_index     = -1;
         state           = SCOREITEM_CREATED;
      }

      ParameterHistory(const string& key, const string& oldvalue, 
            int deleted_state, int hindex) {
         history         = hindex;
         named_nspace    = "";
         named_key       = key;
         named_oldvalue  = oldvalue;
         named_newvalue  = "";
         fixed_index     = -1;
         state           = SCOREITEM_DELETED;
      }

      ParameterHistory(int key, SCORE_FLOAT oldvalue, SCORE_FLOAT newvalue, 
            int hindex) {
         history        = hindex;
         fixed_index    = key;
         fixed_oldvalue = oldvalue;
         fixed_newvalue = newvalue;
         state          = SCOREITEM_UNDEFINED;
      }

      ParameterHistory(int key, int created_state, SCORE_FLOAT newvalue, 
            int hindex) {
         history        = hindex;
         fixed_index    = key;
         fixed_oldvalue = 0;
         fixed_newvalue = newvalue;
         state          = SCOREITEM_CREATED;
      }

      ParameterHistory(int key, SCORE_FLOAT oldvalue, int deleted_state,
            int hindex) {
         history        = hindex;
         fixed_index    = key;
         fixed_oldvalue = oldvalue;
         fixed_newvalue = 0;
         state          = SCOREITEM_DELETED;
      }

      void clear(void) {
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

      ostream& printAsPmx(ostream& out) const {
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

      ostream& printAsXml(ostream& out) {
         out << "<parameter";
         out << " history=\"" << history << "\"";
         if (fixed_index < 0) {
            if (named_nspace != "") {
               out << " namespace=\"";
               ScoreItemBase::printXmlTextEscaped(out, named_nspace);
               out << "\"";
            }
            out << " name=\"";
            ScoreItemBase::printXmlTextEscaped(out, named_key);
            out << "\"";

            if (state == SCOREITEM_CREATED) {
               out << " created=\"true\"";
            } else {
               out << " oldvalue=\"";
               ScoreItemBase::printXmlTextEscaped(out, named_oldvalue);
               out << "\"";
            }

            if (state == SCOREITEM_DELETED) {
               out << " deleted=\"true\"";
            } else {
               out << " newvalue=\"";
               ScoreItemBase::printXmlTextEscaped(out, named_newvalue);
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

};


typedef list<ParameterHistory> HistoryList;


class ScoreItemEdit : public ScoreItemBase {
   public:
                    ScoreItemEdit(void);
                    ScoreItemEdit(const ScoreItemEdit& anItem);
                    ScoreItemEdit(const vectorSF& parameters);
                   ~ScoreItemEdit();

      ScoreItemEdit& operator=      (const ScoreItemEdit& anItem);

      // Edit history management functions
      void          clearHistory         (void);
      static void   incrementHistoryIndex(void);
      static void   freezeHistoryIndex   (void);
      static void   thawHistoryIndex     (void);
      static void   startHistory         (void);
      static void   stopHistory          (void);
      static int    historyIsActive      (void);
      static int    historyIsThawed      (void);
      static int    historyIsFrozen      (void);
      int           getHistoryIndex      (void);

      // Namespace accessor functions
      void          setParameter     (const string& nspace, const string& key, 
                                      const string& value);
      void          setParameter     (const string& nspace, const string& key, 
                                      int value);
      void          setParameter     (const string& nspace, const string& key, 
                                      SCORE_FLOAT value);
      // Global accessor functions
      void          setParameter     (const string& key, const string& value);
      void          setParameter     (const string& key, int value);
      void          setParameter     (const string& key, SCORE_FLOAT value);

      void          setParameter     (int index, SCORE_FLOAT value);
      void          setFixedText     (const string& text);

      // Accessor function aliases:
      void          setP             (const string& nspace, const string& key, 
                                      const string& value);
      void          setP             (const string& nspace, const string& key, 
                                      int value);
      void          setP             (const string& nspace, const string& key, 
                                      SCORE_FLOAT value);
      void          setP             (const string& key, const string& value);
      void          setP             (const string& key, int value);
      void          setP             (const string& key, SCORE_FLOAT value);

      void          setP             (int index, SCORE_FLOAT value);

      void          deleteNamespace  (const string& nspace);
      void          eraseNamespace   (const string& nspace);
      void          clearNamespace   (const string& nspace);

      void          deleteParameter  (const string& nspace, const string& key);
      void          eraseParameter   (const string& nspace, const string& key);
      void          clearParameter   (const string& nspace, const string& key);

      void          deleteParameter  (const string& key);
      void          eraseParameter   (const string& key);
      void          clearParameter   (const string& key);

      // Printing functions
      ostream&      printPmx          (ostream& out);
      ostream&      printPmxEditHistory(ostream& out);
      ostream&      printXmlWithEdits (ostream& out, int indentcount = 0, 
                                       const string& indentstring = "\t");

   protected:
      void          appendToHistory  (const string& nspace, const string& key, 
                                      const string& oldvalue, 
                                      const string& newvalue);
      void          appendToHistory  (const string& nspace, const string& key, 
                                      int created_state,
                                      const string& newvalue);
      void          appendToHistory  (const string& nspace, const string& key, 
                                      const string& oldvalue, 
                                      int deleted_state);

      void          appendToHistory  (const string& key, 
                                      const string& oldvalue,
                                      const string& newvalue);
      void          appendToHistory  (const string& key, 
                                      int created_state,
                                      const string& newvalue);
      void          appendToHistory  (const string& key, 
                                      const string& oldvalue,
                                      int deleted_state);

      void          appendToHistory  (int index, SCORE_FLOAT oldvalue,
                                      SCORE_FLOAT newvalue);
      void          appendToHistory  (int index, int created_state,
                                      SCORE_FLOAT newvalue);
      void          appendToHistory  (int index, SCORE_FLOAT oldvalue,
                                      int deleted_state);

      // Printing functions
      ostream&       printHistoryPmx (ostream& out, 
                                      const ParameterHistory& hEntry);
      ostream&       printHistoryXml (ostream& out, int indentcount, 
                                      const string& indentstring);

   private:
      static unsigned int history_index;
      static int history_active;
      static int history_thawed;
      HistoryList history_list;
};


#endif  /* _SCOREITEMEDIT_H_INCLUDED */


