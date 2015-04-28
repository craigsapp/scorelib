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
   out << SU::getTextNoFontXmlEscapedUTF8(text);
   return out;
}


//////////////////////////////
//
// ScoreItemBase::getTextNoFontXmlEscapedUTF8 --
//

string ScoreUtility::getTextNoFontXmlEscapedUTF8(const string& text) {
   string output;
   output.reserve(output.size()*2);

   int length = text.size();
   char ch;
   int i;
   for (i=0; i<length; i++) {
      ch = text[i];
      switch (ch) {
         case '&':
            output += "&amp;";
            break;
         case '"':
            output += "&quot;";
            break;
         case '\'':
            output += "&apos;";
            break;
         case '>':
            if (i < length-2) {
               if (text[i+1] == '>') {
                  // grave accent on next note
                  output.push_back(text[i+2]);
                  output.push_back((unsigned char)0xcc);
                  output.push_back((unsigned char)0x80);
                  i+= 2;
                  continue;
               }
            }
            output += "&gt;";
            break;
         case '<':
            if (i < length-2) {
               if (text[i+1] == '<') {
                  // acute accent on next note
                  output.push_back(text[i+2]);
                  output.push_back((unsigned char)0xcc);
                  output.push_back((unsigned char)0x81);
                  i+= 2;
                  continue;
               }
            }
            output += "&lt;";
            break;
         default:
            output.push_back(ch);
      }
   }

   return output;
}



