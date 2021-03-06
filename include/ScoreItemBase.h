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

class ScorePage;

class ScoreItemBase {

   friend class ScorePageBase;
   friend class ScoreItem;

   public:
                    ScoreItemBase     (void);
                    ScoreItemBase     (const string& stringitem);
                    ScoreItemBase     (const ScoreItemBase& anItem);
                    ScoreItemBase     (const vectorSF& parameters);
                   ~ScoreItemBase     ();

      ScoreItemBase& operator=        (const ScoreItemBase& anItem);
      void          clear             (void);

      // Accessor functions
      SCORE_FLOAT   getParameter      (int pindex);
      int           getParameterInt   (int pindex);
      bool          getParameterBool  (int pindex);
      bool          getParameterBoolean(int pindex);
      const string& getParameter      (const string& nspace,
                                       const string& key);
      const string& getParameter      (const string& key);

      SCORE_FLOAT   getParameter1     (void);
      SCORE_FLOAT   getParameter2     (void);
      SCORE_FLOAT   getParameter3     (void);
      SCORE_FLOAT   getParameter4     (void);
      SCORE_FLOAT   getParameter5     (void);
      SCORE_FLOAT   getParameter6     (void);
      SCORE_FLOAT   getParameter7     (void);
      SCORE_FLOAT   getParameter8     (void);
      SCORE_FLOAT   getParameter9     (void);
      SCORE_FLOAT   getParameter10    (void);
      SCORE_FLOAT   getParameter11    (void);
      SCORE_FLOAT   getParameter12    (void);
      SCORE_FLOAT   getParameter13    (void);
      SCORE_FLOAT   getParameter14    (void);
      SCORE_FLOAT   getParameter15    (void);
      SCORE_FLOAT   getParameter16    (void);
      SCORE_FLOAT   getParameter17    (void);
      SCORE_FLOAT   getParameter18    (void);
      SCORE_FLOAT   getParameter19    (void);
      SCORE_FLOAT   getParameter20    (void);
      SCORE_FLOAT   getParameter21    (void);
      SCORE_FLOAT   getParameter22    (void);
      SCORE_FLOAT   getParameter23    (void);
      SCORE_FLOAT   getParameter24    (void);
      SCORE_FLOAT   getParameter25    (void);
      SCORE_FLOAT   getParameter26    (void);
      SCORE_FLOAT   getParameter27    (void);
      SCORE_FLOAT   getParameter28    (void);
      SCORE_FLOAT   getParameter29    (void);
      SCORE_FLOAT   getParameter30    (void);

      SCORE_FLOAT   getP1             (void);
      SCORE_FLOAT   getP2             (void);
      SCORE_FLOAT   getP3             (void);
      SCORE_FLOAT   getP4             (void);
      SCORE_FLOAT   getP5             (void);
      SCORE_FLOAT   getP6             (void);
      SCORE_FLOAT   getP7             (void);
      SCORE_FLOAT   getP8             (void);
      SCORE_FLOAT   getP9             (void);
      SCORE_FLOAT   getP10            (void);
      SCORE_FLOAT   getP11            (void);
      SCORE_FLOAT   getP12            (void);
      SCORE_FLOAT   getP13            (void);
      SCORE_FLOAT   getP14            (void);
      SCORE_FLOAT   getP15            (void);
      SCORE_FLOAT   getP16            (void);
      SCORE_FLOAT   getP17            (void);
      SCORE_FLOAT   getP18            (void);
      SCORE_FLOAT   getP19            (void);
      SCORE_FLOAT   getP20            (void);
      SCORE_FLOAT   getP21            (void);
      SCORE_FLOAT   getP22            (void);
      SCORE_FLOAT   getP23            (void);
      SCORE_FLOAT   getP24            (void);
      SCORE_FLOAT   getP25            (void);
      SCORE_FLOAT   getP26            (void);
      SCORE_FLOAT   getP27            (void);
      SCORE_FLOAT   getP28            (void);
      SCORE_FLOAT   getP29            (void);
      SCORE_FLOAT   getP30            (void);

      int           getP1Int          (void);
      int           getP2Int          (void);
      int           getP3Int          (void);
      int           getP4Int          (void);
      int           getP5Int          (void);
      int           getP6Int          (void);
      int           getP7Int          (void);
      int           getP8Int          (void);
      int           getP9Int          (void);
      int           getP10Int         (void);
      int           getP11Int         (void);
      int           getP12Int         (void);
      int           getP13Int         (void);
      int           getP14Int         (void);
      int           getP15Int         (void);
      int           getP16Int         (void);
      int           getP17Int         (void);
      int           getP18Int         (void);
      int           getP19Int         (void);
      int           getP20Int         (void);
      int           getP21Int         (void);
      int           getP22Int         (void);
      int           getP23Int         (void);
      int           getP24Int         (void);
      int           getP25Int         (void);
      int           getP26Int         (void);
      int           getP27Int         (void);
      int           getP28Int         (void);
      int           getP29Int         (void);
      int           getP30Int         (void);

      int           getParameterInt   (const string& nspace, const string& key);
      int           getParameterInt   (const string& key);
      bool          getParameterBool  (const string& nspace, const string& key);
      bool          getParameterBool  (const string& key);
      bool          getParameterBoolean(const string& nspace, const string& key);
      bool          getParameterBoolean(const string& key);
      double        getParameterDouble(const string& key);
      double        getParameterDouble(const string& nspace, const string& key);
      const string& getFixedText      (void) const;
      SCORE_FLOAT   getParameterFraction(int pindex);

      // namespaced parameters

/*
      void          setParameter      (const string& nspace,
                                       const string& key, const string& value);
      void          setParameter      (const string& nspace,
                                       const string& key, int value);
      void          setParameter      (const string& nspace,
                                       const string& key, SCORE_FLOAT value);
*/

      int           changeNamespace   (const string& newspace,
                                       const string& oldspace,
                                       const string& parameter);

      void          setParameterNoisy (const string& nspace,
                                       const string& key, const string& value);
      void          setParameterNoisy (const string& nspace,
                                       const string& key, int value);
      void          setParameterNoisy (const string& nspace,
                                       const string& key, SCORE_FLOAT value);
      void          setParameterNoisy (const string& nspace,
                                       const string& key, void* pointer);

      void          setParameterQuiet (const string& nspace,
                                       const string& key, const string& value);
      void          setParameterQuiet (const string& nspace,
                                       const string& key, int value);
      void          setParameterQuiet (const string& nspace,
                                       const string& key, SCORE_FLOAT value);
      void          setParameterQuiet (const string& nspace,
                                       const string& key, void* pointer);

      void          storeAutoAddress  (void);


      // global namespace parameters
      void          setParameterNoisy (const string& key, const string& value);
      void          setParameterNoisy (const string& key, int value);
      void          setParameterNoisy (const string& key, SCORE_FLOAT value);

      // numeric parameters
      void          setParameterNoisy   (int index, SCORE_FLOAT value);

      void          setParameter1Noisy  (SCORE_FLOAT value);
      void          setParameter2Noisy  (SCORE_FLOAT value);
      void          setParameter3Noisy  (SCORE_FLOAT value);
      void          setParameter4Noisy  (SCORE_FLOAT value);
      void          setParameter5Noisy  (SCORE_FLOAT value);
      void          setParameter6Noisy  (SCORE_FLOAT value);
      void          setParameter7Noisy  (SCORE_FLOAT value);
      void          setParameter8Noisy  (SCORE_FLOAT value);
      void          setParameter9Noisy  (SCORE_FLOAT value);
      void          setParameter10Noisy (SCORE_FLOAT value);
      void          setParameter11Noisy (SCORE_FLOAT value);
      void          setParameter12Noisy (SCORE_FLOAT value);
      void          setParameter13Noisy (SCORE_FLOAT value);
      void          setParameter14Noisy (SCORE_FLOAT value);
      void          setParameter15Noisy (SCORE_FLOAT value);
      void          setParameter16Noisy (SCORE_FLOAT value);
      void          setParameter17Noisy (SCORE_FLOAT value);
      void          setParameter18Noisy (SCORE_FLOAT value);
      void          setParameter19Noisy (SCORE_FLOAT value);
      void          setParameter20Noisy (SCORE_FLOAT value);
      void          setParameter21Noisy (SCORE_FLOAT value);
      void          setParameter22Noisy (SCORE_FLOAT value);
      void          setParameter23Noisy (SCORE_FLOAT value);
      void          setParameter24Noisy (SCORE_FLOAT value);
      void          setParameter25Noisy (SCORE_FLOAT value);
      void          setParameter26Noisy (SCORE_FLOAT value);
      void          setParameter27Noisy (SCORE_FLOAT value);
      void          setParameter28Noisy (SCORE_FLOAT value);
      void          setParameter29Noisy (SCORE_FLOAT value);
      void          setParameter30Noisy (SCORE_FLOAT value);

      void          setP1N            (SCORE_FLOAT value);
      void          setP2N            (SCORE_FLOAT value);
      void          setP3N            (SCORE_FLOAT value);
      void          setP4N            (SCORE_FLOAT value);
      void          setP5N            (SCORE_FLOAT value);
      void          setP6N            (SCORE_FLOAT value);
      void          setP7N            (SCORE_FLOAT value);
      void          setP8N            (SCORE_FLOAT value);
      void          setP9N            (SCORE_FLOAT value);
      void          setP10N           (SCORE_FLOAT value);
      void          setP11N           (SCORE_FLOAT value);
      void          setP12N           (SCORE_FLOAT value);
      void          setP13N           (SCORE_FLOAT value);
      void          setP14N           (SCORE_FLOAT value);
      void          setP15N           (SCORE_FLOAT value);
      void          setP16N           (SCORE_FLOAT value);
      void          setP17N           (SCORE_FLOAT value);
      void          setP18N           (SCORE_FLOAT value);
      void          setP19N           (SCORE_FLOAT value);
      void          setP20N           (SCORE_FLOAT value);
      void          setP21N           (SCORE_FLOAT value);
      void          setP22N           (SCORE_FLOAT value);
      void          setP23N           (SCORE_FLOAT value);
      void          setP24N           (SCORE_FLOAT value);
      void          setP25N           (SCORE_FLOAT value);
      void          setP26N           (SCORE_FLOAT value);
      void          setP27N           (SCORE_FLOAT value);
      void          setP28N           (SCORE_FLOAT value);
      void          setP29N           (SCORE_FLOAT value);
      void          setP30N           (SCORE_FLOAT value);

      // P1=15/16 text
      void          setFixedText      (const string& text);

      int           getNamespaceCount (void) const;
      int           countAllNamedParameters(void);
      int           getNamedParameterCount (const string& nspace);
      int           getNamedParameterCount (void);
      int           getNamedParameterSize  (const string& nspace);
      int           getNamedParameterSize  (void);
      int           getCompactFixedParameterCount(void);
      void          addNamedParameters(mapNamespace& input);
      int           hasParameter      (const string& testkey);
      int           hasParameter      (const string& nspace,
                                      const string& testkey);
      int           isDefined         (const string& testkey);
      int           isDefined         (const string& nspace,
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
      void          setPN             (const string& nspace,
                                       const string& key, const string& value);
      void          setPN             (const string& nspace, const string& key,
                                       int value);
      void          setPN             (const string& nspace, const string& key,
                                       SCORE_FLOAT value);

      void          setPN             (const string& key, const string& value);
      void          setPN             (const string& key, int value);
      void          setPN             (const string& key, SCORE_FLOAT value);

      void          setPN             (int index, SCORE_FLOAT value);

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
      ostream&      printPmxNamedParametersNoAuto(ostream& out);
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

      static string emptyString;

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

      
      // extract a named parameter from an input PMX line:
      static void readNamedParameter (mapNamespace& np, char* input);
};


#endif  /* _SCOREITEMBASE_H_INCLUDED */



