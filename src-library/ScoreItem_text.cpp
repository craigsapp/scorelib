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
// ScoreItem::isItalic -- Returns true if the text is italicized.
//

bool ScoreItem::isItalic(void) {
   if (!isTextItem()) {
      return false;
   }
   string code = getInitialFontCode();
   if (code.size() < 3) {
      return false;
   }
   int number = (code[1] - '0') * 10 + (code[2] - '0');
   switch (number) {
      case  2:    // Times Italic
      case  3:    // Times Bold Italic
      case  6:    // Helvetica Oblique
      case  7:    // Helvetica Bold Oblique
      case 10:    // Helvetica Narrow Oblique
      case 11:    // Helvetica Narrow Bold Oblique
      case 14:    // Palatino Italic
      case 15:    // Palatino Bold Italic
      case 17:    // Avant Garde Book Oblique
      case 19:    // Avant Garde Book Demi Oblique
      case 21:    // Bookman Light Italic
      case 23:    // Bookman Light Demi Italic
      case 26:    // New Century Schoolbook Italic
      case 27:    // New Century Schoolbook Bold Italic
      case 28:    // Zapf Chancery Medium Italic
      case 31:    // Courier Oblique
      case 32:    // Courier Bold Oblique
         return true;
         break;

      // case  0:    // Times Roman
      // case  1:    // Times Bold
      // case  4:    // Helvetica
      // case  5:    // Helvetica Bold
      // case  8:    // Helvetica Narrow
      // case  9:    // Helvetica Narrow Bold
      // case 12:    // Palatino Roman
      // case 13:    // Palatino Bold
      // case 16:    // Avant Garde Book
      // case 18:    // Avant Garde Book Demi
      // case 20:    // Bookman Light
      // case 22:    // Bookman Light Demi
      // case 24:    // New Century Schoolbook Roman
      // case 25:    // New Century Schoolbook Bold
      // case 29:    // Courier
      // case 30:    // Courier Bold
      // case 33:    // Symbol
      // case 34:    // Zapf Dingbats
      //    return false;
   }

    return false;
}



//////////////////////////////
//
// ScoreItem::isBold -- Returns true if the text is bold.
//

bool ScoreItem::isBold(void) {
   if (!isTextItem()) {
      return false;
   }
   string code = getInitialFontCode();
   if (code.size() < 3) {
      return false;
   }
   int number = (code[1] - '0') * 10 + (code[2] - '0');
   switch (number) {
      case  1:    // Times Bold
      case  3:    // Times Bold Italic
      case  5:    // Helvetica Bold
      case  7:    // Helvetica Bold Oblique
      case  9:    // Helvetica Narrow Bold
      case 11:    // Helvetica Narrow Bold Oblique
      case 13:    // Palatino Bold
      case 15:    // Palatino Bold Italic
      case 18:    // Avant Garde Book Demi
      case 19:    // Avant Garde Book Demi Oblique
      case 22:    // Bookman Light Demi
      case 23:    // Bookman Light Demi Italic
      case 25:    // New Century Schoolbook Bold
      case 27:    // New Century Schoolbook Bold Italic
      case 30:    // Courier Bold
      case 32:    // Courier Bold Oblique
         return true;
         break;

      // case  0:    // Times Roman
      // case  2:    // Times Italic
      // case  4:    // Helvetica
      // case  6:    // Helvetica Oblique
      // case  8:    // Helvetica Narrow
      // case 10:    // Helvetica Narrow Oblique
      // case 12:    // Palatino Roman
      // case 14:    // Palatino Italic
      // case 16:    // Avant Garde Book
      // case 17:    // Avant Garde Book Oblique
      // case 20:    // Bookman Light
      // case 21:    // Bookman Light Italic
      // case 24:    // New Century Schoolbook Roman
      // case 26:    // New Century Schoolbook Italic
      // case 28:    // Zapf Chancery Medium Italic
      // case 29:    // Courier
      // case 31:    // Courier Oblique
      // case 33:    // Symbol
      // case 34:    // Zapf Dingbats
      //    return false;

   }

    return false;
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
   int p8 = getP8Int();
   if (p8 >= 1000) {
      // font override parameter
      p8 = p8 % 100;
      string out = "_";
      if (p8 < 10) {
         out += "0";
      }
      out += to_string(p8);
      return out;
   }

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
   if (p6 >= 100.0) {
      // font too large, something must be encoded in 100's digit
      p6 = p6 - int(p6/100) * 100.0;
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
   if (p6 >= 100.0) {
      // font too large, something must be encoded in 100's digit
      p6 = p6 - int(p6/100) * 100.0;
   }
   return 13.56 * staff_size * p6;
}



//////////////////////////////
//
// ScoreItem::hasWordExtension --  Returns true if a lyric syllable has a
//    word extension line (underscore) after it.
//

bool ScoreItem::hasWordExtension(void) {
   // Check if analyzeLyrics() has been run.
   return getParameterBool(ns_auto, np_wordExtension);
}



//////////////////////////////
//
// ScoreItem::hasHyphenAfter -- Returns true if a lyric syllable has
//     a hyphen character after it, linking it to another syllable in the
//     current word.
//

bool ScoreItem::hasHyphenAfter(void) {
   // Check if analyzeLyrics() has been run.
   return getParameterBool(ns_auto, np_hyphenAfter);
}



//////////////////////////////
//
// ScoreItem::hasHyphenBefore -- Returns true if a lyric syllable has
//     a hyphen character before it, linking it to another syllable in the
//     current word.
//

bool ScoreItem::hasHyphenBefore(void) {
   // Check if analyzeLyrics() has been run.
   return getParameterBool(ns_auto, np_hyphenBefore);
}




