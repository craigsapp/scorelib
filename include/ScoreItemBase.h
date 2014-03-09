//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:11:15 PST 2014
// Last Modified: Tue Mar  4 01:12:30 PST 2014
// Filename:      ScoreItemBase.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreItemBase.h
// Syntax:        C++11
//
// Description:   The ScoreItemBase class forms the basic data storage 
//                class for storing SCORE item parameters.  This
//                class can be inherited directly by either ScoreItem
//                or ScoreItemEdit.  Memory management of ScoreItems is
//                typically handled by ScorePageBase (therefore it is a
//                friend class).
//

#ifndef _SCOREITEMBASE_H_INCLUDED
#define _SCOREITEMBASE_H_INCLUDED

#include "ScoreDefs.h"
#include "ScoreItemEdit_ParameterHistory.h"

#include <list>
#include <iostream>

using namespace std;


class ScoreItemBase {

   friend class ScorePageBase;

   public:
                    ScoreItemBase     (void);
                    ScoreItemBase     (const string& stringitem);
                    ScoreItemBase     (const ScoreItemBase& anItem);
                    ScoreItemBase     (const vectorSF& parameters);
                   ~ScoreItemBase     ();

      ScoreItemBase& operator=        (const ScoreItemBase& anItem);

      // Accessor functions
      SCORE_FLOAT   getParameter      (int pindex);
      int           getParameterInt   (int pindex);
      const string& getParameter      (const string& nspace, 
                                       const string& key);
      const string& getParameter      (const string& key);
      int           getParameterInt   (const string& key);
      double        getParameterDouble(const string& key);
      double        getParameterDouble(const string& nspace, const string& key);
      const string& getFixedText      (void) const;
      SCORE_FLOAT   getParameterFraction(int pindex);

      // namespaced parameters
      void          setParameter      (const string& nspace, 
                                       const string& key, const string& value);
      void          setParameter      (const string& nspace, 
                                       const string& key, int value);
      void          setParameter      (const string& nspace, 
                                       const string& key, SCORE_FLOAT value);
      // global namespace parameters
      void          setParameter      (const string& key, const string& value);
      void          setParameter      (const string& key, int value);
      void          setParameter      (const string& key, SCORE_FLOAT value);

      // numeric parameters 
      void          setParameter      (int index, SCORE_FLOAT value);

      // P1=15/16 text
      void          setFixedText      (const string& text);

      int           getNamespaceCount (void) const;
      int           countAllNamedParameters(void);
      int           getNamedParameterCount (const string& nspace);
      int           getNamedParameterCount (void);
      int           getNamedParameterSize  (const string& nspace);
      int           getNamedParameterSize  (void);
      int           getCompactFixedParameterCount(void);
      int           hasParameter      (const string& testkey);
      int           hasParameter      (const string& nspace, 
                                      const string& testkey);
      void          deleteNamespace   (const string& nspace);
      void          eraseNamespace    (const string& nspace);
      void          clearNamespace    (const string& nspace);
 
      void          deleteParameter   (const string& nspace, const string& key);
      void          eraseParameter    (const string& nspace, const string& key);
      void          clearParameter    (const string& nspace, const string& key);

      void          deleteParameter   (const string& key);
      void          eraseParameter    (const string& key);
      void          clearParameter    (const string& key);

      // Accessor function aliases:
      void          setP              (const string& nspace, 
                                       const string& key, const string& value);
      void          setP              (const string& nspace, const string& key, 
                                       int value);
      void          setP              (const string& nspace, const string& key, 
                                       SCORE_FLOAT value);

      void          setP              (const string& key, const string& value);
      void          setP              (const string& key, int value);
      void          setP              (const string& key, SCORE_FLOAT value);
 
      void          setP              (int index, SCORE_FLOAT value);

      SCORE_FLOAT   getP              (int pindex);
      int           getPInt           (int pindex);
      const string& getP              (const string& nspace, const string& key);
      const string& getP              (const string& key);
      int           getPInt           (const string& key);
      double        getPDouble        (const string& key);


      // Printing functions
      ostream&      printPmx         (ostream& out);
      ostream&      printPmxFixedParameters(ostream& out);
      ostream&      printPmxNamedParameters(ostream& out);
      ostream&      printXml         (ostream& out, int indentcount = 0, 
                                      const string& indentstring = "\t");

     
      static ostream& printXmlTextEscaped(ostream& out, const string& text);
      static ostream& printIndent    (ostream& out, int indentcount, 
                                      const string& indentstring);

      void        setFixedParameterAllocation(int asize);


      // Dummy edit history management functions defined in ScoreItemEdit
      void        clearHistory         (void) { }
      static void incrementHistoryIndex(void) { }
      static void freezeHistoryIndex   (void) { }
      static void thawHistoryIndex     (void) { }
      static void startHistory         (void) { }
      static void stopHistory          (void) { }
      static int  historyIsActive      (void) { return 0; }
      static int  historyIsThawed      (void) { return 0; }
      static int  historyIsFrozen      (void) { return 1; }
      static int  getHistoryIndex      (void) { return 0; }
      ostream&    printPmxWithEdits (ostream& out) { return printPmx(out); }
      ostream&    printXmlWithEdits (ostream& out, int indentcount = 0, 
                                       const string& indentstring = "\t") 
                        { return printXml(out, indentcount, indentstring); }

   protected:
     ostream&      printFixedListPieceXml  (ostream& out);
     ostream&      printNamedParametersXml (ostream& out, 
                                            const string& nspace, 
                                            int indentcount, 
                                            const string& indentstring);
   protected:
      // Page-related interface functions
      void         setPageOwner            (void* page);
      void         notifyPageOfChange      (const string& message);
      void         notifyPageOfChange      (const string& message, int index,
                                            SCORE_FLOAT oldp, SCORE_FLOAT newp);

   public:
      // Binary read/write functions
      int           writeBinary           (ostream& out);
      double        readLittleEndianFloat (istream& instream);
      void          writeLittleEndianFloat(ostream& out, double number);
      void          readBinary            (istream& instream, int pcount);
      void          readPmx               (istream& instream, int verboseQ = 0);
      static double roundFractionDigits   (double number, int digits);

   protected:
      vectorSF     fixed_parameters;
      mapNamespace named_parameters;
      string       fixed_text;  // used for P1=15 and P1=16 SCORE items.
      // fixed_text for P1=15 is an EPS graphic file to include.
      // fixed_text for P1=16 is a text string.
      void* page_owner;

      // sort_sequence is a variable which is used by ScorePageBase or
      // similar classes/functinos to suggest a sequence ordering of 
      // SCROE items within the ScorePageBase primary storage.
      int sort_sequence;

      // staff_duration_offset is the durational offset from the
      // start of the staff on which it is attached until the P3 value
      // of the item.
      SCORE_FLOAT staff_duration_offset;

      // Used by ScoreItemEdit class to store parameter history:
      HistoryList history_list;

   public:
      // Limit the size of fixed paramters so that out-of-memory problems
      // cannot occur if trying to access parameter one billion for example.
      // Larger fixed parameters could be implemented as a form of named 
      // parameter.  The MS-DOS versions of SCORE do not have fixed parameter 
      // sizes larger than 20; the Windows versions of SCORE do not have 
      // fixed parameter sizes larger than 40.
      static constexpr int SCORE_MAX_FIXED_PARAMETERS = 100;

      // The largest P7 duration value for notes and rests.  Anything larger
      // is considered a grace note with 0 duration.
      static constexpr SCORE_FLOAT SCORE_MAX_DURATION = 60.0;

};


#endif  /* _SCOREITEMBASE_H_INCLUDED */



