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
#include "ScoreUtility.h"

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



//////////////////////////////
//
// ScoreItem::getText -- Return the fixed text field of the object.
//

const string& ScoreItem::getText(void) {
   return getFixedText();
}



//////////////////////////////
//
// ScoreItem::getTextWithoutInitialFontCode -- Return the fixed text field 
//    of the object without the intial font code, if present.
//

string ScoreItem::getTextWithoutInitialFontCode(void) {
   if ((getFixedText().size() >= 3) && (getFixedText()[0] == '_') 
         && isdigit(getFixedText()[1]) 
         && isdigit(getFixedText()[2]) ) {
      return getFixedText().substr(3, string::npos);
   } else {
      return getFixedText();
   }
}

//
// Alias:
//

string ScoreItem::getTextNoFont(void) {
   return getTextWithoutInitialFontCode();
}



//////////////////////////////
//
// ScoreItem::getInitialFontCode -- Return the initial font code, or an empty
//     string if none.
//
//    _00 = Times Roman
//    _01 = Times Bold
//    _02 = Times Italic
//    etc.
//

string ScoreItem::getInitialFontCode(void) { 
   if ((getFixedText().size() >= 3) && (getFixedText()[0] == '_') 
      && isdigit(getFixedText()[1]) 
      && isdigit(getFixedText()[2]) ) {
      return getFixedText().substr(0, 3);
   } else {
      return (string)"";
   }
}



//////////////////////////////
//
// ScoreItem::getInitialFontCodeAsInteger -- Return -1 if no intial font code
//     in fixed text string.
//
//   -1 = undefined
//    0 = Times Roman
//    1 = Times Bold
//    2 = Times Italic
//    etc.
//

int ScoreItem::getInitialFontCodeAsInteger(void) { 
   if ((getFixedText().size() >= 3) && (getFixedText()[0] == '_') 
      && isdigit(getFixedText()[1]) 
      && isdigit(getFixedText()[2]) ) {
      return (getFixedText()[1] - '0') * 10 + (getFixedText()[2] - '0');
   } else {
      return -1;
   }
} 



//////////////////////////////
//
// ScoreItem::setText -- Return the fixed text field of the object.
//

void ScoreItem::setText(const string& text) {
   setFixedText(text);
}



//////////////////////////////
//
// ScoreItem::getTextNoFontXmlEscapedUTF8 --
//

string ScoreItem::getTextNoFontXmlEscapedUTF8(void) {
   string temp = ScoreItem::getTextWithoutInitialFontCode();
   return SU::getTextNoFontXmlEscapedUTF8(temp);
}



//////////////////////////////
//
// ScoreItem::getFontSizeInPoints --
//

SCORE_FLOAT ScoreItem::getFontSizeInPoints(void) {
   if (!isTextItem()) {
      return 0.0;
   }
   SCORE_FLOAT staff_size = 1.0;
   SCORE_FLOAT tolerance = 0.001;
   ScoreItem* staffitem = getStaffOwner();
   if (staffitem == NULL) {
      staff_size = 1.0;
   } else {
      staff_size = staffitem->getP5();
      if (fabs(staff_size) < tolerance) {
         staff_size = 1.0;
      }
   }
   SCORE_FLOAT p6 = fabs(getP6());
   if (p6 < tolerance) {
      p6 = 1.0;
   }
   return 13.56 * staff_size * p6;
}



SCORE_FLOAT ScoreItem::getFontSizeInPoints(SCORE_FLOAT staff_size) {
   if (!isTextItem()) {
      return 0.0;
   }
   SCORE_FLOAT tolerance = 0.001;
   // Treat negative scalings as positive when extracting the font size.
   SCORE_FLOAT p6 = fabs(getP6());
   if (p6 < tolerance) {
      p6 = 1.0;
   }
   return 13.56 * staff_size * p6;
}



