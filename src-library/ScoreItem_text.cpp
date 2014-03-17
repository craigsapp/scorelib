//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar 17 14:30:12 PDT 2014
// Last Modified: Mon Mar 17 14:30:14 PDT 2014
// Filename:      ScoreItem_text.cpp
// URL:           https://github.com/craigsapp/scorelib/blob/master/src-library/ScoreItem_text.cpp
// Syntax:        C++11
//
// Description:   Functions which (primarily) process P1=16 (text) items.
//

#include "ScoreItem.h"

using namespace std;


//////////////////////////////
//
// ScoreItem::make99Text -- Create a font-99 text item.  Font-99 text items
//    get inserted directly into PostScript output.
//

void ScoreItem::make99Text(const string& text) {
   clear();
   setItemType(P1_Text);
   setStaffNumber(1);
   setHorizontalPosition(1);
   if (text.compare(0, 3, "_99") == 0) {
      setFixedText(text);
   } else {
      setFixedText("_99" + text);
   }
}



//////////////////////////////
//
// ScoreItem::makeSvgCode -- Create a font-99 text item with a prefix of
//    %svg% before the input text.  This function will replace '/' characters
//    with '\' characters, since SCORE will convert '\' charcters into
//    '/' characters when printing to PostScript.
//

void ScoreItem::makeSvgCode(const string& text) {
   string copy;
   copy.reserve(text.size() * 2);
   for (auto& it : text) {
      switch (it) {
         case '/':
            copy += '\\';
            break;
         default:
            copy += it;
      }
   }
   make99Text("_99%svg%" + copy);
}



