//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue May  6 16:06:35 PDT 2014
// Last Modified: Tue May  6 16:06:37 PDT 2014
// Filename:      ScoreUtility_text.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScoreUtility_text.cpp
// Syntax:        C++11
//
// Description:   Processing of text in SCORE.
//

#include "ScoreUtility.h"
#include <iostream>

using namespace std;


//////////////////////////////
//
// ScoreItemBase::printXmlTextEscapedUTF8 --
//

ostream& ScoreUtility::printXmlTextEscapedUTF8(ostream& out, 
      const string& text) {

   int length = text.size();
   char ch;
   int i;
   for (i=0; i<length; i++) {
      ch = text[i];
      switch (ch) {
         case '&':
            out << "&amp;";
            break;
         case '"':
            out << "&quot;";
            break;
         case '\'':
            out << "&apos;";
            break;
         case '>':
            if (i < length-2) {
               if (text[i+1] == '>') {
                  // grave accent on next note
                  out << text[i+2] << (char)0xcc << (char)0x80;
                  i+= 2;
                  continue;
               }
            }
            out << "&gt;";
            break;
         case '<':
            if (i < length-2) {
               if (text[i+1] == '<') {
                  // acute accent on next note
                  out << text[i+2];
                  out << (unsigned char)0xcc;
                  out << (unsigned char)0x81;
                  i+= 2;
                  continue;
               }
            }
            out << "&lt;";
            break;
         default:
            out << ch;
      }
   }

   return out;
}



