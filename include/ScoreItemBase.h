//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed Feb  5 17:11:15 PST 2014
// Last Modified: Fri Feb 14 22:05:54 PST 2014
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

#include <map>
#include <list>
#include <vector>
#include <iostream>

using namespace std;


typedef double                     SCORE_FLOAT;
typedef map<string, string>        mapSS;
typedef map<string, mapSS>         mapNamespace;
typedef vector<SCORE_FLOAT>        vectorSF;
typedef vector<float>              vectorF;
typedef vector<int>                vectorI;

typedef enum {
        P1 = 1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14,
        P15, P16, P17, P18, P19, P20, P21, P22, P23, P24, P25, P26, P27,
        P28, P29, P30, P31, P32, P33, P34, P35, P36, P37, P38, P39, P40,
        P41, P42, P43, P44, P45, P46, P47, P48, P49, P50, P51, P52, P53,
        P54, P55, P56, P57, P58, P59, P60, P61, P62, P63, P64, P65, P66,
        P67, P68, P69, P70, P71, P72, P73, P74, P75, P76, P77, P78, P79,
        P80, P81, P82, P83, P84, P85, P86, P87, P88, P89, P90, P91, P92,
        P93, P94, P95, P96, P97, P98, P99
} PARAM;


typedef enum {
        P1_Note                 = 1,
        P1_Rest                 = 2,
        P1_Clef                 = 3,
        P1_Line                 = 4,
        P1_Slur                 = 5,
        P1_Beam                 = 6,
        P1_Trill                = 7,
        P1_Staff                = 8,
        P1_Symbol               = 9,
        P1_Number               = 10,
        P1_User                 = 11,
        P1_Special              = 12,
        P1_BadLuck              = 13,
        P1_Barline              = 14,
        P1_ImportedEPSGraphic   = 15,
        P1_Text                 = 16,
        P1_KeySignature         = 17,
        P1_MeterSignature       = 18
} PARM1VALUE;


class ScoreItemBase {

   friend class ScorePageBase;

   public:
                    ScoreItemBase(void);
                    ScoreItemBase(const string& stringitem);
                    ScoreItemBase(const ScoreItemBase& anItem);
                    ScoreItemBase(const vectorSF& parameters);
                   ~ScoreItemBase();

      ScoreItemBase& operator=      (const ScoreItemBase& anItem);

      // Accessor functions
      SCORE_FLOAT   getParameter     (int pindex);
      int           getParameterInt  (int pindex);
      const string& getParameter     (const string& nspace, 
                                      const string& key);
      const string& getParameter     (const string& key);
      int           getParameterInt  (const string& key);
      double        getParameterDouble(const string& key);
      double        getParameterDouble(const string& nspace, const string& key);
      const string& getFixedText     (void) const;

      // namespaced parameters
      void          setParameter     (const string& nspace, 
                                      const string& key, const string& value);
      void          setParameter     (const string& nspace, 
                                      const string& key, int value);
      void          setParameter     (const string& nspace, 
                                      const string& key, SCORE_FLOAT value);
      // global namespace parameters
      void          setParameter     (const string& key, const string& value);
      void          setParameter     (const string& key, int value);
      void          setParameter     (const string& key, SCORE_FLOAT value);

      // numeric parameters 
      void          setParameter     (int index, SCORE_FLOAT value);

      // P1=15/16 text
      void          setFixedText     (const string& text);

      int           getNamespaceCount(void) const;
      int           countAllNamedParameters(void);
      int           getNamedParameterCount (const string& nspace);
      int           getNamedParameterCount (void);
      int           getNamedParameterSize  (const string& nspace);
      int           getNamedParameterSize  (void);
      int           getCompactFixedParameterCount(void);
      int           hasParameter     (const string& testkey);
      int           hasParameter     (const string& nspace, 
                                      const string& testkey);
      void          deleteNamespace  (const string& nspace);
      void          eraseNamespace   (const string& nspace);
      void          clearNamespace   (const string& nspace);

      void          deleteParameter  (const string& nspace, const string& key);
      void          eraseParameter   (const string& nspace, const string& key);
      void          clearParameter   (const string& nspace, const string& key);

      void          deleteParameter  (const string& key);
      void          eraseParameter   (const string& key);
      void          clearParameter   (const string& key);

      // Accessor function aliases:
      void          setP             (const string& nspace, 
                                      const string& key, const string& value);
      void          setP             (const string& nspace, const string& key, 
                                      int value);
      void          setP             (const string& nspace, const string& key, 
                                      SCORE_FLOAT value);

      void          setP             (const string& key, const string& value);
      void          setP             (const string& key, int value);
      void          setP             (const string& key, SCORE_FLOAT value);

      void          setP             (int index, SCORE_FLOAT value);

      SCORE_FLOAT   getP             (int pindex);
      int           getPInt          (int pindex);
      const string& getP             (const string& nspace, const string& key);
      const string& getP             (const string& key);
      int           getPInt          (const string& key);
      double        getPDouble       (const string& key);


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


