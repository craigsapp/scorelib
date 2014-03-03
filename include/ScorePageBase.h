//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Feb 16 18:44:37 PST 2014
// Last Modified: Sun Feb 16 20:32:02 PST 2014
// Filename:      ScorePageBase.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScorePageBase.h
// Syntax:        C++11
//
// Description:   Base class for ScorePage.  This class handles reading/writing
//                of a ScorePage, and handles all of the data variables for
//                a ScorePage.
//

#ifndef _SCOREPAGEBASE_H_INCLUDED
#define _SCOREPAGEBASE_H_INCLUDED

#include "ScoreItem.h"
#include "ScorePageBase_AnalysisInfo.h"
#include "ScorePageBase_PrintInfo.h"
#include "ScorePageBase_StaffInfo.h"

//    0 : modified; or undefined
//  >=1 : no change in ScoreItems owned by this page
//        since the last analyses were done.
//    1 : staff analysis has been done.
//    2 : staff duration analysis has been done.
//    3 : system analysis has been done.
//    4 : part analysis has been done.

// Analysis-level enumerations
typedef enum {
        MODIFIED = 0,  // ScoreItem database has/has not been modified.
        STAFF,         // Staff population on page has been analyzed.
        STAFFDUR,      // Staff durations have been analyzed.
        SYSTEM,        // System grouping of staves has been analyzed.
        PART           // Staff -> Part analysis has been done.
} ANALYSIS;

using namespace std;


class ScorePageBase {
   friend class ScoreItemBase;

   public:
               ScorePageBase     (void);
               ScorePageBase     (const char* filename);
               ScorePageBase     (istream& instream);
               ScorePageBase     (const ScorePageBase& apage);
              ~ScorePageBase     ();

      void     clear             (void);
      int      getItemCount      (void);

      // lists of items on the page
      void     getFileOrderList  (vectorSIp& data);
      void     getStaffItemList (vectorVSIp& data);
      void     getStaffItemListOrdered(vectorVSIp& data);

      void     setDefaultPrintParameters(void);
      ScoreItem* appendItem(ScoreItem& anItem);
      ScoreItem* appendItem(const string& itemstring);
      ScoreItem* prependItem(ScoreItem& anItem);

      // binary file trailer related functions:
      SCORE_FLOAT    getVersion         (void);
      void           setVersionWinScore (void);
      uint32_t       getSerial          (void);
      uint32_t       getSerialNumber    (void);
      void           setSerialNumber    (uint32_t value);
      void           setSerial          (uint32_t value);
      SCORE_FLOAT    getUnits           (void);
      void           setUnitsInches     (void);
      void           setUnitsCentimeters(void);

      // file reading functions:
      void           read              (const char* filename, int verboseQ = 0);
      void           read              (const string& filename, int verboseQ = 0);
      void           read              (istream& infile, int verboseQ = 0);
      void           readFile          (const char* filename, int verboseQ = 0);
      void           readFile          (istream& infile, int verboseQ = 0);
      void           readPmx           (const char* filename, int verboseQ = 0);
      void           readPmx           (istream& infile, int verboseQ = 0);
      ScoreItem*     readPmxScoreLine  (istream& infile, int verboseQ = 0);
      void           readBinary        (const char* filename, int verboseQ = 0);
      void           readBinary        (istream& infile, int verboseQ = 0);

      // file writing functions:
      void           writeBinary       (const char* filename);
      ostream&       writeBinary       (ostream& outfile);
      ostream&       writeLittleFloat  (ostream& out, SCORE_FLOAT number);
      ostream&       printPmx          (ostream& out, int roundQ = 1,
                                        int verboseQ = 0);
      ostream&       printPmxFixedParameters(ostream& out, int roundQ,
                                        int verboseQ = 0);
      ostream&       printPmxWithNamedParameters(ostream& out, int roundQ,
                                       int verboseQ);
      ostream&       printAsciiTrailer (ostream& out);
   protected:
      SCORE_FLOAT    readLittleFloat   (istream& instream);
      int            readLittleShort   (istream& input);
      
   protected:
      // Variable "item_storage" contains pointers to all SCORE items on the
      // page.  It is the responsibility of an instance of this class to 
      // delete these items (unless they are transfered to another page
      // before this one is destroyed).
      listSIp item_storage;

      // Variable "trailer" is for storing the trailer of a SCORE binary file.
      // The trailer consists of at least 5 floats.  The numbers in
      // reverse order are:
      // 0: The last number is -9999.0 to indicate the end of the trailer
      // 1: The second to last number is a count of the number of 4-byte
      //    numbers in the trailer.  Typically this is 5.0, but may be 
      //    larger in new versions of SCORE.
      // 2: The measurement unit code: 0.0 = inches, 1.0 = centimeters.
      // 3: The program version number which created the file.
      // 4: The program serial number (or 0.0 for version 3 or earlier)
      // 5: The last number in the trailer (i.e., the first number of the
      //    trailer in the file is 0.0.  Normally this is the position in 
      //    the file which the parameter count for an item is given.
      //    Objects cannot have zero parameters, so when 0.0 is found,
      //    that indicates the start of the trailer.
      // The trailer is stored in the order that it is found in the binary
      // file (or the trailer will be empty if the data was read from an
      // ASCII PMX file).  In other words, -9999.0 should be the last
      // number in the trailer array if the data was read from a binary
      // SCORE file.
      vectorF trailer;

      constexpr static unsigned int    MAX_STAFF_COUNT = 100;

      // staff_info contains information about the staves on the page
      // and how they are grouped into systems.
      StaffInfo staff_info;


   protected:

      // analysis_info contains information about analysis states for
      // data on the page.
      AnalysisInfo analysis_info;

      // print_info contains variable needed to print or process page
      // data as a bitmap.
      PrintInfo print_info;

      static constexpr bool monitor_P3 = 0;


   protected:
      void    itemChangeNotification  (ScoreItemBase* sitem, 
                                       const string& message);
      void    itemChangeNotification  (ScoreItemBase* sitem, 
                                       const string& message, int index,
                                       SCORE_FLOAT oldp, SCORE_FLOAT newp);
      void    clearAnalysisStates     (void);

};


ostream& operator<<(ostream& out, ScorePageBase& aPage);


#endif /* _SCOREPAGEBASE_H_INCLUDED */


