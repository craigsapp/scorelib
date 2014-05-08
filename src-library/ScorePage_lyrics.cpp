//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 02:06:16 PDT 2014
// Last Modified: Tue May  6 02:06:19 PDT 2014
// Filename:      ScorePage_lyrics.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScorePage_lyrics.cpp
// Syntax:        C++11
//
// Description:   This file contains ScorePage class functions related to
//                lyrics analysis (which happens on the ScorePageSet level)..
//

#include "ScorePage.h"

using namespace std;


//////////////////////////////
//
// ScorePage::getLyricsGroup -- Given a ScoreItem pointer to a note,
//    this function returns a list of chord notes if the input
//    note belongs to a chord.  Will return an empty list if the
//    note is not part of a chord.
//

vectorSIp*  ScorePage::getLyricsGroup(ScoreItem* item) {
   // need to deal with validating lyrics analysis here...
   return lyrics_database.lyricslist(item);
}



