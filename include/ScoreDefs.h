//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Mar  4 01:08:14 PST 2014
// Last Modified: Tue Mar  4 01:08:18 PST 2014
// Filename:      ScoreDefs.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/ScoreDefs.h
// Syntax:        C++11
//
// Description:   Definitions separated from classes to avoid loops.
//

#ifndef _SCOREDEFS_H_INCLUDED
#define _SCOREDEFS_H_INCLUDED

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


#endif  /* _SCOREDEFS_H_INCLUDED */



