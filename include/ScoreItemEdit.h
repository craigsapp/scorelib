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

class ScoreItemEdit : public ScoreItemBase {
   public:
                    ScoreItemEdit        (void);
                    ScoreItemEdit        (const ScoreItemEdit& anItem);
                    ScoreItemEdit        (const ScoreItemBase& anItem);
                    ScoreItemEdit        (const vectorSF& parameters);
                   ~ScoreItemEdit        ();

      ScoreItemEdit& operator=           (const ScoreItemEdit& anItem);

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
      ostream&      printPmx           (ostream& out);
      ostream&      printPmxEditHistory(ostream& out);
      ostream&      printXmlWithEdits  (ostream& out, int indentcount = 0,
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
      static int          history_active;
      static int          history_thawed;
};



#endif  /* _SCOREITEMEDIT_H_INCLUDED */

