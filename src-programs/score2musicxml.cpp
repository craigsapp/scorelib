//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Apr 22 17:43:27 PDT 2014
// Last Modified: Tue Apr 22 17:43:31 PDT 2014
// Filename:      score2musicxml.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/score2musicxml.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/score2musicxml
// Syntax:        C++ 11
//
// Description:   Converts a ScorePageSet into MusicXML.  If there is one 
//                segment <score-partwise> will be used.  If there are more
//                than one segment to process <opus> will be used.
//

#include "scorelib.h"
#include <chrono>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

void     processData                (ScorePageSet& infiles, Options& opts);
ostream& convertSingleSegment       (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ostream& convertAllSegmentsToOpus   (ostream& out, ScorePageSet& infiles);
ostream& printIndent                (ostream& out, int indent, string text);
void     printIndent                (stringstream& out, int indent, 
                                     string text);
ostream& printPart                  (ostream& out, ScorePageSet& infiles, 
                                     int segment, int part, int indent);
ostream& printPartList              (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
void     printToFile                (ScorePageSet& infiles, int segment, 
                                     string& filebase);
void     convertAllSegmentsToSeparateFiles(ScorePageSet& infiles, 
                                           string& filebase);
ostream& printIdentification        (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent, 
                                     map<string, ScoreItem*>& credits);
ostream& printDefaults              (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ostream& printCredits               (ostream& out, 
                                     map<string, ScoreItem*>& credits, 
                                     int indent);
ostream& printPartInfo              (ostream& out, ScorePageSet& infiles, 
                                     int segment, int part, int indent, 
                                     int partcount);
ostream& printXml                   (ostream& out, const string& text);
void     printSystem                (vector<stringstream*>& partouts, 
                                     ScorePageSet& infiles, int segment, 
                                     int systemindex, int indent, 
                                     vectorVSIp& curclef, vectorVSIp& curkey,
                                     vectorVSIp& curtime, int& mcounter,
                                     int divisions, vectorSF& staffsizes,
                                     vectorI& partVisible);
void     printPartMeasure           (stringstream& out, ScorePageSet& infiles, 
                                     ScorePage& page, 
                                     AddressSystem& partaddress,
                                     SystemMeasure& measureitems, 
                                     int sysindex, int measureindex, 
                                     int partindex, vectorVSIp& curclef, 
                                     vectorVSIp& curkey, vectorVSIp& curtime, 
                                     int mcounter, int indent, int divisions,
                                     int systemindex, SCORE_FLOAT& staffsize,
                                     int segmentindex, vectorI& partVisible);
void     printNote                  (ostream& out, ScoreItem* si,
                                     int partstaff, int indent, int divisions,
                                     SCORE_FLOAT measureP3);
void     printRest                  (ostream& out, ScoreItem* si, 
                                     int partstaff, int indent, 
                                     int divisions, SCORE_FLOAT measureP3);
void     printMeasureAttributes     (ostream& out, vectorSIp& items, int index, 
                                     int sysindex, int partindex, 
                                     int partstaff, int measureindex, 
                                     int divisions, int indent, 
                                     ScoreItem* currkey, ScoreItem* currtime, 
                                     ScoreItem* currclef, 
                                     SCORE_FLOAT& laststaffsize,
                                     vectorI& partVisible);
int      printClefItem              (stringstream& stream, ScoreItem* item, 
                                     int indent);
int      printTimeSigItem           (stringstream& stream, ScoreItem* item, 
                                     int indent);
int      printKeySigItem            (stringstream& stream, ScoreItem* item, 
                                     int indent);
void     printBarlineStyle          (ostream& out, SystemMeasure& measureitems, 
                                     int partstaff, int indent);
void     printForwardBarlineStyle   (ostream& out, SystemMeasure& measureitems, 
                                     int partstaff, int indent);
void     printNoteNotations         (ostream& out, ScoreItem* si, int indent, 
                                     string& notetype);
string   getNoteType                (ScoreItem* si);
string   getRestType                (ScoreItem* si);
void     printLyrics                (ostream& out, ScoreItem* si, int indent);
void     printDirection             (ostream& out, ScoreItem* si, 
                                     ScoreItem* anchor, int indent);
void     printDirections            (ostream& out, SystemMeasure& measureitems,
                                     int index, int partstaff, int indent, 
                                     int divisions);
void     printDirectionsBackwards   (ostream& out, SystemMeasure& measureitems, 
                                     int index, int partstaff, int indent, 
                                     int divisions);
void     printDirectionsForwards    (ostream& out, SystemMeasure& items, 
                                     int index, int partstaff, int indent, 
                                     int divisions);
void     printMinorColoration       (ostream& out, SystemMeasure& measureitems, 
                                     int index, int partstaff, int indent, 
                                     int divisions);
void     printSystemLayout          (stringstream& out, ScorePage& page, 
                                     const AddressSystem& system, int indent);
void     printStaffLayout           (stringstream& out, ScorePage& page, 
                                     const AddressSystem& system, int indent);
ScoreItem* printComposer            (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ScoreItem* printTitle               (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
SCORE_FLOAT getMusicXmlTenthsBetweenStaves(ScoreItem* lostaff, 
                                     ScoreItem* histaff);
SCORE_FLOAT getMusicXmlTenthsFromStaffTopToPageBottom(ScoreItem* staff);
int      printGroup1                (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
void     printDufayCredits          (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ostream& printTimeModification      (ostream& out, int indent, ScoreItem* si, 
                                     int divisions);
ostream& printMensurationDirection  (ostream& out, ScoreItem* item, 
                                     int partstaff, int indent);
void     extractMensurationDirection(ostream& out, vectorSIp& items, 
                                     int index, int partstaff, int indent);

#define INDENT_STRING "\t"

int      locationQ        = 1;
int      debugQ           = 0;
int      rhythmicScalingQ = 0;
int      invisibleQ       = 0;
int      Scaling          = 0;
int      lyricsQ          = 1;
int      systemBreaksQ    = 1;
int      pageBreaksQ      = 1;
int      StartTempo       = -1;
int      movementQ        = 0;
int      dufayQ           = 0;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("L|no-location=b", 
         "Don't print location of each measure as comment");
   opts.define("s|segment=i:0", 
         "Segment number to process");
   opts.define("f|filebase=s:filebase", 
         "Base of filename for multiple segments");
   opts.define("debug=b", 
         "Print debugging information");
   opts.define("scale|scaling|rhythmic-scaling=i:0", 
         "Global rhythmic scaling of durations");
   opts.define("I|no-invisible=b", 
         "Don't convert invisible rests/notes");
   opts.define("no-lyrics=b", 
         "Don't process Lyrics");
   opts.define("no-system-breaks=b", 
         "Don't print system break information");
   opts.define("no-page-breaks=b", 
         "Don't print page break information");
   opts.define("M|movement=b", 
         "Treat entire input as a single movment");
   opts.define("D|dufay=b", 
         "Use default options for Dufay translations");
   opts.process(argc, argv);

   locationQ        = !opts.getBoolean("no-location");
   debugQ           =  opts.getBoolean("debug");
   rhythmicScalingQ =  opts.getBoolean("rhythmic-scaling");
   invisibleQ       =  opts.getBoolean("no-invisible");
   Scaling          =  opts.getInteger("rhythmic-scaling");
   lyricsQ          = !opts.getBoolean("no-lyrics");
   systemBreaksQ    = !opts.getBoolean("no-system-breaks");
   pageBreaksQ      = !opts.getBoolean("no-page-breaks");
   movementQ        =  opts.getBoolean("movement");
   dufayQ           =  opts.getBoolean("dufay");
   if (dufayQ) {
      rhythmicScalingQ = 1;
      Scaling          = 1;
      invisibleQ       = 1;
      StartTempo       = 525;
   }
 
   ScorePageSet infiles(opts);
   processData(infiles, opts);

   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// processData -- Start doing the work.
//

void processData(ScorePageSet& infiles, Options& opts) {

   if (movementQ) {
      infiles.analyzeSingleSegment();
   } else {
      infiles.analyzeSegmentsByIndent();
   }

   // infiles.analyzePitch();
   infiles.analyzeTies();
   infiles.analyzeTuplets();
   if (lyricsQ) {
      infiles.analyzeLyrics();
   }

   if (opts.getBoolean("segment")) {
      // Segment on command line is indexed to 1, but in
      // ScorePageSet, it is indexed to 0, so subtracting one.
      int segment = opts.getInteger("segment") - 1;
      convertSingleSegment(cout, infiles, segment, 0);
   } else if (infiles.getSegmentCount() == 1) {
      convertSingleSegment(cout, infiles, 0, 0);
   } else if (opts.getBoolean("filebase")) {
      string filebase = opts.getString("filebase");
      convertAllSegmentsToSeparateFiles(infiles, filebase);
   } else {
      convertAllSegmentsToOpus(cout, infiles);
   }
}



//////////////////////////////
//
// convertAllSegmentsToSeparateFiles --
//

void convertAllSegmentsToSeparateFiles(ScorePageSet& infiles, 
      string& filebase) {
   int segmentcount =  infiles.getSegmentCount();
   for (int i=0; i<segmentcount; i++) {
      printToFile(infiles, i, filebase);
   }
}



//////////////////////////////
//
// printToFile -- Print each segment in the input data into a separate
//      file, indexed according to the segment number.  Segment numbers
//      are indexed by 0 in the ScorePageSet object, but indexed by 1
//      in the filenames.
//

void printToFile(ScorePageSet& infiles, int segment, string& filebase) {
   string filename = filebase + "-" + to_string(segment);
   if (segment < 100) { filename += "0"; }
   if (segment < 10)  { filename += "0"; }
   filename += to_string(segment);
   filename += ".pmx";
   ofstream outfile(filename);
   if (!outfile.is_open()) {
      cerr << "Error: cannot write to " << filename << endl;
   }
   convertSingleSegment(outfile, infiles, segment, 0);
}



//////////////////////////////
//
// convertSingleSegment --
//

ostream& convertSingleSegment(ostream& out, ScorePageSet& infiles, 
   int segment, int indent) {
   if (indent == 0) {
      out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
      out << "<!DOCTYPE score-partwise PUBLIC ";
      out << "\"-//Recordare//DTD MusicXML 3.0 Partwise//EN\" ";
      out << "\"http://www.musicxml.org/dtds/partwise.dtd\">\n";
   }

   int divisions = infiles.getLCMRhythm(segment);

   printIndent(out, indent++, "<score-partwise version=\"3.0\">\n");

   map<string, ScoreItem*> credits;

   // <movement-title>
   credits["title"] = printTitle(out, infiles, segment, indent);        

   // <identification>
   printIdentification(out, infiles, segment, indent, credits); 

   // <defaults>
   printDefaults(out, infiles, segment, indent);     

   // <credit>*
   printCredits(out, credits, indent);
   if (dufayQ) {
      printDufayCredits(out, infiles, segment, indent);
   }

   // <part-list>
   printPartList(out, infiles, segment, indent);

   ScoreSegment& seg = infiles.getSegment(segment);
   int partcount = seg.getPartCount();
   vector<stringstream*> partoutput;
   partoutput.resize(partcount);
   fill(partoutput.begin(), partoutput.end(), (stringstream*)NULL);
 
   int systemcount = seg.getSystemCount();

   int i;
   string tempstring;

   for (i=0; i<partcount; i++) {
      partoutput[i] = new stringstream;
      *partoutput[i] << endl;
      printIndent(*partoutput[i], indent, "<part id=\"P");
      tempstring = to_string(i+1);
      *partoutput[i] << tempstring << "\">\n";
   }

   vectorVSIp current_clef(partcount);
   vectorVSIp current_keysig(partcount);
   vectorVSIp current_timesig(partcount);
   for (i=0; i<partcount; i++) {
      current_clef[i].push_back(NULL);
      current_keysig[i].push_back(NULL);
      current_timesig[i].push_back(NULL);
   }
    

   int measure_counter = 1;

   vectorSF staffsizes(partcount);
   fill(staffsizes.begin(), staffsizes.end(), 1.0);

   vectorI partVisible(partcount);
   fill(partVisible.begin(), partVisible.end(), 1);

   for (i=0; i<systemcount; i++) {
      printSystem(partoutput, infiles, segment, i, indent+1, current_clef, 
            current_keysig, current_timesig, measure_counter, divisions, 
            staffsizes, partVisible);
   }

   for (i=0; i<partcount; i++) {
      printIndent(*partoutput[i], indent, "</part>\n");
      out << partoutput[i]->str();
   }

   printIndent(out, --indent, "</score-partwise>\n");

   for (i=0; i<partcount; i++) {
      delete partoutput[i];
      partoutput[i] = NULL;
   }
   partoutput.resize(0);

   return out;
}



//////////////////////////////
//
// printCredits --
//
//  <credit page="1">
//    <credit-words default-x="70" default-y="1453" font-size="12" valign="top">Score</credit-words>
//  </credit>
//  <credit page="2">
//    <credit-type>page number</credit-type>
//    <credit-words default-x="70" default-y="1481" font-size="12" valign="top">2</credit-words>
//  </credit>
//

ostream& printCredits(ostream& out, map<string, ScoreItem*>& credits, 
      int indent) {
   int p2;
   SCORE_FLOAT p3;
   SCORE_FLOAT p4;
   SCORE_FLOAT scale;
   SCORE_FLOAT staffoffset;
   // SCORE_FLOAT topmargin = 11.0;
   SCORE_FLOAT fontsize = 10.0;
   ScoreItem* si;
   SCORE_FLOAT defaulty;
   SCORE_FLOAT defaultx;

   SCORE_FLOAT staffv;

   int pageindex;
   for (auto& it : credits) {
      si = it.second;
      if (si == NULL) {
         continue;
      }
      string text = si->getTextWithoutInitialFontCode();
      if (text.length() == 0) {
         continue;
      }
      p2              = si->getStaffNumber();    
      p3              = si->getHPos();
      scale           = si->getStaffScale();
      staffoffset     = si->getStaffVerticalOffset();
      p4              = si->getVPos();
      pageindex       = si->getPageIndex();
      fontsize        = si->getFontSizeInPoints(scale);
      string function = si->getParameter(ns_auto, np_function);

      staffv = 0.8125 + 0.7875 * (p2 - 1) + staffoffset * scale * 0.0875 / 2.0;
      defaulty = staffv + (p4 - 2.0) * scale * 0.0875 / 2.0;
      defaulty = defaulty * 25.4 * 40 / 8.89;

      // defaultx = p3 / 200.0 * 7.5 * 25.4 * 40 / 8.89;
      defaultx = p3 * 5.0 * 6.0 / 7.0;
      defaultx += 0.525 * 25.4 * 40 / 8.89;

      printIndent(out, indent++, "<credit");
      out << " page=\"" << pageindex+1 << "\">\n";
      if (function == "title") {
         printIndent(out, indent, "<credit-type>title</credit-type>\n");
      } else if (function == "composer") {
         printIndent(out, indent, "<credit-type>composer</credit-type>\n");
      }
      printIndent(out, indent, "<credit-words");
      out << " default-x=\"" << defaultx << "\"";
      out << " default-y=\"" << defaulty << "\"";
      out << " font-size=\"" << fontsize << "\"";
      out << ">";
   
      out << SU::getTextNoFontXmlEscapedUTF8(text);
      out << "</credit-words>\n";

      printIndent(out, --indent, "</credit>\n");

   }
   return out;
}



//////////////////////////////
//
// printDufayCredits --
//

void printDufayCredits(ostream& out, ScorePageSet& infiles, int segment, 
      int indent) {
   map<string, ScoreItem*> credits;

   ScoreSegment& seg = infiles.getSegment(segment);
   const vectorVASp& addresses = seg.getSystemAddresses(0);
   ScorePage* page;
   int sysindex;
   int p2, targetp2;
   SCORE_FLOAT p4;
   SCORE_FLOAT p3;
   int i, j;

   int counter = 0;
   string teststr;

   // process page headers
   for (i=0; i<addresses.size(); i++) {
      page = infiles.getPage(*addresses[i][0]);
      sysindex = addresses[i][0]->getSystemIndex();
      if (sysindex != 0) {
         continue;
      }
      vectorVVSIp& staves = page->getStaffItemsBySystem();
      targetp2 = staves[0][staves[0].size()-1][0]->getStaffNumber();
      vectorSIp& items = page->getSystemItems(sysindex);
      for (j=0; j<items.size(); j++) {
         if (!items[j]->isTextItem()) {
            continue;
         }
         p2 = items[j]->getStaffNumber();
         if (p2 != targetp2) {
            continue;
         }
         p4 = items[j]->getVPos();
         if (p4 < 13) {
            continue;
         }
         if (items[j]->isDefined(ns_auto, np_function)) {
            continue;
         }
         teststr = items[j]->getFixedText().substr(3,17);
         if (teststr.compare("Guillaume Du Fay,") == 0) {
            credits["credit" + to_string(counter++)] = items[j];
            items[j]->setParameterQuiet(ns_auto, np_function, "credit");
         } 
      }
   }
   
   // process page footers
   for (i=0; i<addresses.size(); i++) {
      page = infiles.getPage(*addresses[i][0]);
      sysindex = addresses[i][0]->getSystemIndex();
      vectorVVSIp& staves = page->getStaffItemsBySystem();
      if (sysindex != staves.size() - 1) {
         continue;
      }
      targetp2 = staves[sysindex][0][0]->getStaffNumber();
      vectorSIp& items = page->getSystemItems(sysindex);
      for (j=0; j<items.size(); j++) {
         if (!items[j]->isTextItem()) {
            continue;
         }
         p2 = items[j]->getStaffNumber();
         if (p2 != targetp2) {
            continue;
         }
         p4 = items[j]->getVPos();
         if (p4 > -5) {
            continue;
         }
         if (items[j]->isDefined(ns_auto, np_function)) {
            continue;
         }
         p3 = items[j]->getHPos();
         if (p3 > 50) {
            // only found on left side of page.
            break;
         }
         teststr = items[j]->getFixedText().substr(3,2);
         if (teststr.compare("D-") == 0) {
            credits["credit" + to_string(counter++)] = items[j];
            items[j]->setParameterQuiet(ns_auto, np_function, "credit");
         }
      }
   }
   
   printCredits(out, credits, indent);
}



//////////////////////////////
//
// printDefaults --
//
//     
//  <defaults>
//    <scaling>
//      <millimeters>7.2319</millimeters>
//      <tenths>40</tenths>
//    </scaling>
//    <page-layout>
//      <page-height>1545</page-height>
//      <page-width>1194</page-width>
//      <page-margins type="both">
//        <left-margin>70</left-margin>
//        <right-margin>70</right-margin>
//        <top-margin>88</top-margin>
//        <bottom-margin>88</bottom-margin>
//      </page-margins>
//    </page-layout>
//    <system-layout>
//      <system-margins>
//        <left-margin>0</left-margin>
//        <right-margin>0</right-margin>
//      </system-margins>
//      <system-distance>121</system-distance>
//      <top-system-distance>70</top-system-distance>
//    </system-layout>
//    <staff-layout>
//      <staff-distance>113</staff-distance>
//    </staff-layout>
//    <appearance>
//      <line-width type="stem">0.7487</line-width>
//      <line-width type="beam">5</line-width>
//      <line-width type="staff">0.7487</line-width>
//      <line-width type="light barline">0.7487</line-width>
//      <line-width type="heavy barline">5</line-width>
//      <line-width type="leger">0.7487</line-width>
//      <line-width type="ending">0.7487</line-width>
//      <line-width type="wedge">0.7487</line-width>
//      <line-width type="enclosure">0.7487</line-width>
//      <line-width type="tuplet bracket">0.7487</line-width>
//      <note-size type="grace">60</note-size>
//      <note-size type="cue">60</note-size>
//      <distance type="hyphen">120</distance>
//      <distance type="beam">8</distance>
//    </appearance>
//    <music-font font-family="Maestro,engraved" font-size="20.5"/>
//    <word-font font-family="Times New Roman" font-size="10.25"/>
//  </defaults>
//

ostream& printDefaults(ostream& out, ScorePageSet& infiles, 
      int segment, int indent) {
   printIndent(out, indent++, "<defaults>\n");

   // Height of default sized staff in millimeters:
   SCORE_FLOAT mm = 8.89;

   // Height of default sized staff in MusicXML tenths (always 40):
   SCORE_FLOAT tenths = 40.0;

   SCORE_FLOAT tfactor = 40.0 / 8.89;

   // <scaling>
   printIndent(out, indent++, "<scaling>\n");
   printIndent(out, indent,   "<millimeters>");
   out << mm << "</millimeters>\n";
   printIndent(out, indent,   "<tenths>");
   out << tenths << "</tenths>\n";
   printIndent(out, --indent, "</scaling>\n");

   // <page-layout>
   printIndent(out, indent++, "<page-layout>\n");

   // Hardwired to 8.5" x 11" paper for now:
   printIndent(out, indent,   "<page-height>");
   out << 11.0 * 25.4 * tfactor << "</page-height>\n";
   printIndent(out, indent,   "<page-width>");
   out << 8.5 * 25.4 * tfactor << "</page-width>\n";

   printIndent(out, indent++, "<page-margins type=\"both\">\n");
   printIndent(out, indent,   "<left-margin>");
   out << 0.525 * 25.4 * tfactor << "</left-margin>\n";
   printIndent(out, indent,   "<right-margin>");
   out << 0.475 * 25.4 * tfactor  << "</right-margin>\n";
   // top and bottom defaults margins are more fuzzy, but required
   // if the left and right margins are given.  For now, just set the
   // top and bottom margins to 0.
   printIndent(out, indent,   "<top-margin>0</top-margin>\n");
   // <bottom-margin> (0.8125" to bottom line of bottom staff)
   printIndent(out, indent,   "<bottom-margin>0</bottom-margin>\n");
   printIndent(out, --indent, "</page-margins>\n");
   printIndent(out, --indent, "</page-layout>\n");

   // <system-layout>
   printIndent(out, indent++, "<system-layout>\n");
   printIndent(out, indent++, "<system-margins>\n");
   printIndent(out, indent,   "<left-margin>0</left-margin>\n");
   printIndent(out, indent,   "<right-margin>0</right-margin>\n");
   printIndent(out, --indent, "</system-margins>\n");
   printIndent(out, --indent, "</system-layout>\n");

   printIndent(out, --indent, "</defaults>\n");

   return out;
}



//////////////////////////////
//
// printIdentification --
//
// <identification>
//    <encoding>
//       <software>xxx</software>
//       <encoding-date>yyyy-mm-dd</encoding-date>
//       <supports attribute="new-system" element="print" type="yes" value="yes"/>
//       <supports attribute="new-page" element="print" type="yes" value="yes"/>
//    </encoding>
//  </identification>
//     
//

ostream& printIdentification(ostream& out, ScorePageSet& infiles, 
      int segment, int indent, map<string, ScoreItem*>& credits) {
   printIndent(out, indent++, "<identification>\n");

   // <creator> (composer)
   credits["composer"] = printComposer(out, infiles, segment, indent);

   if (dufayQ) {
      printIndent(out, indent, "<rights>Alejandro Planchart</rights>\n");
   }

   // <encoding>
   printIndent(out, indent++, "<encoding>\n");
   printIndent(out, indent, "<software>");
   out << "score2musicxml alpha version</software>\n";
   printIndent(out, indent, "<encoding-date>");
   auto now = chrono::system_clock::now();
   auto in_time = chrono::system_clock::to_time_t(now);
   out << put_time(localtime(&in_time), "%Y-%m-%d");
   out << "</encoding-date>\n";
   printIndent(out, indent, "<supports attribute=\"new-system\"");
   out << " element=\"print\" type=\"yes\" value=\"yes\"/>\n";
   printIndent(out, indent, "<supports attribute=\"new-page\"");
   out << " element=\"print\" type=\"yes\" value=\"yes\"/>\n";
   printIndent(out, --indent, "</encoding>\n");

   printIndent(out, --indent, "</identification>\n");
   return out;
}



//////////////////////////////
//
// printTitle --
//

ScoreItem* printTitle(ostream& out, ScorePageSet& infiles, int segment, 
      int indent) {
   ScoreSegment& seg = infiles.getSegment(segment);
   const AddressSystem& sys = seg.getSystemAddress(0);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   vectorSIp& items = page->getSystemItems(sysindex);
   vectorVVSIp& staves = page->getStaffItemsBySystem();
   int staffcount = staves[sysindex].size();
   int targetp2 = staves[sysindex][staffcount-1][0]->getStaffNumber();

   SCORE_FLOAT p3cutoff = 25.0;
   SCORE_FLOAT p3start = 100.0;
   int i;
   int p2;
   SCORE_FLOAT p3;
   SCORE_FLOAT p4;
   ScoreItem* candidate = NULL;
   SCORE_FLOAT candidatep4 = 11; // top of staff lines.
   for (i=items.size()-1; i>=0; i--) {
      if (!items[i]->isTextItem()) {
         continue;
      }
      p2 = items[i]->getStaffNumber();
      if (p2 != targetp2) {
         continue;
      }
      p3 = items[i]->getHPos();
      if (p3 < p3cutoff) {
        break;
      }
      if (p3 > p3start) {
         continue;
      }
      if (items[i]->getFixedText().length() < 6) {
         // Too short (probably a page number)
         continue;
      }
      // Should also filter numbers...
      p4 = items[i]->getVPos();
      if (p4 > candidatep4) {
         candidate = items[i];
         candidatep4 = p4;
      }
   }

   if (candidate == NULL) {
      return NULL;
   }

   candidate->setParameterQuiet(ns_auto, np_function, "title");
   printIndent(out, indent, "<movement-title>");
   string name = candidate->getTextWithoutInitialFontCode();
   out << SU::getTextNoFontXmlEscapedUTF8(name);
   out << "</movement-title>\n";

   return candidate;
}



//////////////////////////////
//
// printComposer --
//

ScoreItem* printComposer(ostream& out, ScorePageSet& infiles, int segment, 
      int indent) {
   ScoreSegment& seg = infiles.getSegment(segment);
   const AddressSystem& sys = seg.getSystemAddress(0);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   vectorSIp& items = page->getSystemItems(sysindex);
   vectorVVSIp& staves = page->getStaffItemsBySystem();
   int staffcount = staves[sysindex].size();
   int targetp2 = staves[sysindex][staffcount-1][0]->getStaffNumber();

   SCORE_FLOAT p3cutoff = 150.0;
   int i;
   int p2;
   SCORE_FLOAT p3;
   SCORE_FLOAT p4;
   ScoreItem* candidate = NULL;
   SCORE_FLOAT candidatep4 = 11; // top of staff lines.
   for (i=items.size()-1; i>=0; i--) {
      if (!items[i]->isTextItem()) {
         continue;
      }
      p2 = items[i]->getStaffNumber();
      if (p2 != targetp2) {
         continue;
      }
      p3 = items[i]->getHPos();
      if (p3 < p3cutoff) {
        break;
      }
      if (items[i]->getFixedText().length() < 6) {
         // Too short (probably a page number)
         continue;
      }
      // Should also filter numbers...
      p4 = items[i]->getVPos();
      if (p4 > candidatep4) {
         candidate = items[i];
         candidatep4 = p4;
      }
   }

   if (candidate == NULL) {
      return NULL;
   }

   candidate->setParameterQuiet(ns_auto, np_function, "composer");
   printIndent(out, indent, "<creator type=\"composer\">");
   string name = candidate->getTextWithoutInitialFontCode();
   out << SU::getTextNoFontXmlEscapedUTF8(name);
   out << "</creator>\n";

   return candidate;
}



//////////////////////////////
//
// printPartList -- Print the <part-list> element.
//

ostream& printPartList(ostream& out, ScorePageSet& infiles, 
    int segment, int indent) {
   printIndent(out, indent++, "<part-list>\n");

   int group1 = printGroup1(out, infiles, segment, indent);

   int partcount = infiles.getSegment(segment).getPartCount();
   for (int i=0; i<partcount; i++) {
      printPartInfo(out, infiles, segment, i, indent, partcount);
   }

   if (group1) {
      printIndent(out, indent, "<part-group number=\"1\" type=\"stop\"/>\n");
   }

   printIndent(out, --indent, "</part-list>\n");
   return out;
}



////////////////////////////////
//
// printGroup1 --
//   Example:
//     <part-group number="1" type="start">
//        <group-symbol default-x="-5">bracket</group-symbol>
//        <group-barline>no</group-barline>
//     </part-group>
//

int printGroup1(ostream& out, ScorePageSet& infiles, int segment, int indent) {
   ScoreSegment& seg = infiles.getSegment(segment);
   int partcount = seg.getPartCount();
   const AddressSystem& sys = seg.getSystemAddress(0);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   int i;
   vectorSIp& items     = page->getSystemItems(sysindex);
   if (items.size() == 0) {
      return 0;
   }

   SCORE_FLOAT barp3 = -1;
   SCORE_FLOAT p3;
   int staffheight = 0;
   int count;

   for (i=items.size()-1; i>=0; i--) {
      if (!items[i]->isBarlineItem()) {
         continue;
      }
      p3 = items[i]->getHPos();
      if (barp3 < 0) {
         barp3 = p3;
      } else if (barp3 > p3) {
         break;
      }
      count = items[i]->getP4Int();
      if (count == 0) {
         count = 1;
      }
      if (staffheight < count) {
         staffheight = count;
      }
   }

   int barlinestyle = 0;

   barp3 = -1;
   for (i=0; i<items.size(); i++) {
      if (!items[i]->isBarlineItem()) {
         continue;
      }
      p3 = items[i]->getHPos();
      if (barp3 < 0) {
         barp3 = p3;
      } else if (barp3 + 2 < p3) {
         break;
      }
      count = items[i]->getP4Int();
      if (count == 0) {
         count = 1;
      }
      if (partcount != count) {
         continue;
      }

      barlinestyle = items[i]->getP5Int();
      if (barlinestyle != 0) {
         break;
      }
   }

   if (barlinestyle == 9) {
      // square bracket
      printIndent(out, indent++, "<part-group number=\"1\" type=\"start\">\n");
      printIndent(out, indent, "<group-symbol");
      out << " default-x=\"-5\">bracket</group-symbol>\n";

      if (staffheight == 1) {
         // staves are not barred together
         printIndent(out, indent, "<group-barline>no</group-barline>\n");
      } else {
         // out << "XXX " << staffheight << "XXX";
      }
      printIndent(out, --indent, "</part-group>\n");
      return 1;
   }

   return 0;
}



//////////////////////////////
//
// printPartInfo -- print the <score-part> element for an individual part.
//
//  <score-part id="P1">
//    <part-name print-object="no">Part name</part-name>
//    <score-instrument id="P1-I1">
//      <instrument-name>Name of instrument</instrument-name>
//    </score-instrument>
//    <midi-instrument id="P1-I1">
//      <midi-channel>1</midi-channel>
//      <midi-bank>15489</midi-bank>
//      <midi-program>1</midi-program>
//      <volume>80</volume>
//      <pan>0</pan>
//    </midi-instrument>
//  </score-part>
//

ostream& printPartInfo(ostream& out, ScorePageSet& infiles, int segment, 
   int part, int indent, int partcount) {
   ScoreSegment& seg = infiles.getSegment(segment);
   printIndent(out, indent++, "<score-part id=\"P");
   out << part+1 << "\">\n";
   string partname = seg.getPartName(partcount-part-1);
   printIndent(out, indent, "<part-name");
   if (partname != "") {
      out << ">";
      printXml(out, partname);
      out << "</part-name>\n";
   } else {
      out << " print-object=\"no\">Part";
      out << part+1 << "\"</part-name>\n";
   }
   printIndent(out, --indent, "</score-part>\n");
   return out;
}



//////////////////////////////
//
// printXml -- Escape various characters and make sure UTF-8.
//

ostream&  printXml(ostream& out, const string& text) {
   for (int i=0; i<text.size(); i++) {
      switch (text[i]) {
         case '\"': out << "&quot;"; break;
         case '<':  out << "&lt;";  break;
         case '>':  out << "&gt;";  break;
         default:   out << text[i];
      }
   }
   return out;
}



//////////////////////////////
//
// convertAllSegmentsToOpus --
//

ostream& convertAllSegmentsToOpus(ostream& out, ScorePageSet& infiles) {
   int scount = infiles.getSegmentCount();
   int indent = 0;
   out << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
   out << "<!DOCTYPE opus PUBLIC \"-//Recordare//DTD MusicXML 3.0 Opus//EN\"";
   out << " \"http://www.musicxml.org/dtds/pus.dtd\">\n";
   printIndent(out, indent++, "<opus>\n");
   for (int i=0; i<scount; i++) {
      convertSingleSegment(out, infiles, i, indent);
   }
   printIndent(out, --indent, "<opus>\n");
   return out;
}



//////////////////////////////
//
// printIndent --
//

ostream& printIndent(ostream& out, int indent, string text) {
   for (int i=0; i<indent; i++) {
      out << INDENT_STRING;
   }
   out << text;
   return out;
}


void printIndent(stringstream& out, int indent, string text) {
   for (int i=0; i<indent; i++) {
      out << INDENT_STRING;
   }
   out << text;
}



//////////////////////////////
//
// printSystem -- print all parts on the given system.
//

void printSystem(vector<stringstream*>& partouts, ScorePageSet& infiles, 
      int segment, int systemindex, int indent, vectorVSIp& curclef, 
      vectorVSIp& curkey, vectorVSIp& curtime, int& mcounter, int divisions,
      vectorSF& staffsizes, vectorI& partVisible) {

   ScoreSegment& seg = infiles.getSegment(segment);
   const AddressSystem& sys = seg.getSystemAddress(systemindex);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   int barcount = page->getSystemBarCount(sysindex);
   int partcount = partouts.size();
   AddressSystem partaddress;
   int i, j;
   for (i=0; i<barcount; i++) {
      SystemMeasure& measureitems = page->getSystemMeasure(sysindex, i);
      if (measureitems.getDuration() == 0.0) {
         continue;
      }
      for (j=0; j<partcount; j++) {
         partaddress = seg.getPartAddress(systemindex, j);
         printPartMeasure(*partouts[j], infiles, *page, partaddress, 
               measureitems, sysindex, i, j, curclef, curkey, curtime, 
               mcounter, indent, divisions, systemindex, staffsizes[j],
               segment, partVisible);
      }
      for (j=0; j<partcount; j++) {
         printIndent(*partouts[j], indent, 
            "<!--=======================================================-->\n");
      }
      mcounter++;
   }
}



//////////////////////////////
//
// printPartMeasure -- print a single measure for the given part
//

void printPartMeasure(stringstream& out, ScorePageSet& infiles, 
      ScorePage& page, AddressSystem& partaddress, SystemMeasure& measureitems,
      int sysindex, int measureindex, int partindex, vectorVSIp& curclef, 
      vectorVSIp& curkey, vectorVSIp& curtime, int mcounter, int indent,
      int divisions, int systemindex, SCORE_FLOAT& staffsize, 
      int segmentindex, vectorI& partVisible) {

   bool firstSystem          = (systemindex == 0);
   bool firstMeasureOnSystem = (measureindex == 0);
   bool startOfWork          = firstMeasureOnSystem && firstSystem;
   bool topOfPage            = (partaddress.getSystemIndex() == 0);
   bool firstMeasureOnPage   = topOfPage && (measureindex == 0);

   ScoreItem* currentclef = curclef[partindex][0];
   ScoreItem* currentkey  = curkey[partindex][0];
   ScoreItem* currenttime = curtime[partindex][0];
   ScoreItem* nextclef = NULL;
   ScoreItem* nextkey  = NULL;
   ScoreItem* nexttime = NULL;
 
   double measuredur = measureitems.getDuration();
   if (measuredur - (int)measuredur < 0.0001) {
      measuredur = (int)measuredur;
   } else if (measuredur - (int)measuredur > 0.9999) {
      measuredur = (int)measuredur + 1;
   }
   int partstaff = page.getPageStaffIndex(partaddress);

   SCORE_FLOAT width = measureitems.getP3Width();
   width = width * 5.0 * 6.0 / 7.0;
   
   SCORE_FLOAT measureP3 = measureitems.getP3();

   if (locationQ) {
      printIndent(out, indent, "<!--");
      out << " page=\""         << partaddress.getPageIndex()+1 << "\"";
      out << " system=\""       << partaddress.getSystemIndex()+1 << "\"";
      out << " systemstaff=\""  << partaddress.getSystemStaffIndex()+1;
      out << "\"";
      out << " pagestaff=\""    << partstaff << "\"";
      out << " staffmeasure=\"" << measureindex+1 << "\"";
      out << " measuredur=\""   << measuredur << "\"";
      out << " -->\n";
   }

   printIndent(out, indent++, "<measure");
   out << " number=\"" << mcounter << "\"";
   out << " width=\"" << width << "\"";
   out << ">\n";

   vectorSIp& items = measureitems.getItems();
   int i;
   int staff;
   ScoreItem* si;

   // <barline>
   printForwardBarlineStyle(out, measureitems, partstaff, indent);

   // <print>

   stringstream layout;
   printSystemLayout(layout, page, partaddress, indent+1);
   printStaffLayout(layout, page, partaddress, indent+1);

   if (firstMeasureOnSystem) {
      if (pageBreaksQ && firstMeasureOnPage && !startOfWork) {
         printIndent(out, indent, "<print new-page=\"yes\"");
         int pagenumber = partaddress.getPageIndex() + 1;
         out << " page-number=\"" << pagenumber << "\"";
         if (layout.rdbuf()->in_avail()) {
            out << ">\n";
            out << layout.str();
            printIndent(out, indent, "</print>\n");
         } else {
            out << "/>\n";
         }
      } else if (systemBreaksQ && firstMeasureOnSystem && !startOfWork) {
         printIndent(out, indent, "<print new-system=\"yes\"");
         if (layout.rdbuf()->in_avail()) {
            out << ">\n";
            out << layout.str();
            printIndent(out, indent, "</print>\n");
         } else {
            out << "/>\n";
         }
      } else if (startOfWork) {
         printIndent(out, indent, "<print");
         int pagenumber = partaddress.getPageIndex() + 1;
         out << " page-number=\"" << pagenumber << "\"";
         if (layout.rdbuf()->in_avail()) {
            out << ">\n";
            out << layout.str();
            printIndent(out, indent, "</print>\n");
         } else {
            out << "/>\n";
         }
      }
   }

   // <attributes>
   printMeasureAttributes(out, items, 0, sysindex, partindex, partstaff, 
         measureindex, divisions, indent, currentkey, currenttime, 
         currentclef, staffsize, partVisible);

   // <sound> -- Finale ignores tempo markings.
   if (startOfWork) {
      if (StartTempo > 0) {
         printIndent(out, indent, "<sound tempo=\"");
         out << StartTempo << "\"/>\n";
      }
   }

   // <direction> (handled in note section below)

   // <note>
   for (i=0; i<items.size(); i++) {
      si = items[i];
      staff = si->getStaffNumber();
      if (staff != partstaff) {
         continue;
      }

      if (si->isBarlineItem()) {
         // barlines are not handled within this loop.
         continue;
      }

      if (debugQ) {
         out << si;
      }

      if (si->isClefItem()) {
         nextclef = si;
      }
      if (si->isKeySignatureItem()) {
         nextkey = si;
      }
      if (si->isTimeSignatureItem()) {
         if (!si->isSingleNumber()) {
            nexttime = si;
         }
      }

      if (dufayQ) {
         // handle mensuration marks
         if (si->isTimeSigItem()) {
            extractMensurationDirection(out, items, i, partstaff, indent);
         }
      }

      if (si->isNoteItem()) {
         if (si->isSecondaryChordNote()) {
            // handled with primary notes.
            continue;
         }
         printDirections(out, measureitems, i, partstaff, indent, divisions);
         printNote(out, si, partstaff, indent, divisions, measureP3);
      }

      if (si->isRestItem()) {
         printDirections(out, measureitems, i, partstaff, indent, divisions);
         if (!(invisibleQ && si->isInvisible())) {
            printRest(out, si, partstaff, indent, divisions, measureP3);
         }
      }

   }

   printBarlineStyle(out, measureitems, partstaff, indent);

   printIndent(out, --indent, "</measure>\n");

   if (nextclef != NULL) {
      curclef[partindex][0] = nextclef;
   }
   if (nextkey != NULL) {
      curkey[partindex][0] = nextkey;
   }
   if (nexttime != NULL) {
      curtime[partindex][0] = nexttime;
   }
}



//////////////////////////////
//
// extractMensurationDirection -- Look for mensuration information within
//     the neighborhood of the time signature.
//
//

void  extractMensurationDirection(ostream& out, vectorSIp& items, 
      int index, int partstaff, int indent) {
   ScoreItem* si = items[index];
   if (!si->isTimeSigItem()) {
      return;
   }
   if (si->isSingleNumber()) {
      int count = si->getTimeSignatureBottom();
      SCORE_FLOAT scale = si->getStaffScale();
      printIndent(out, indent++, "<direction placement=\"above\">\n");
      printIndent(out, indent++, "<direction-type>\n");
      printIndent(out, indent,   "<words");
      out << " default-y=\"" << 5 * scale << "\"";
      out << " relative-x=\"-32\"";
      out << " font-weight=\"bold\"";
      out << " color=\"#ff0000\"";
      out << ">barlines:" << count << ",dash</words>\n";
      printIndent(out, --indent, "</direction-type>\n");
      printIndent(out, --indent, "</direction>\n");
      return;
   }

   int hpos = si->getHorizontalPosition();
   int staff = si->getStaffNumber();
   int tstaff;
   int tpos;

   int i;
   for (i=index+1; i<items.size(); i++) {
      tstaff = items[i]->getStaffNumber();
      if (tstaff != staff) {
         continue;
      }
      tpos = items[i]->getHorizontalPosition();
      if (tpos - hpos > 10) {
         break;
      }
      printMensurationDirection(out, items[i], partstaff, indent);
   }

   for (i=index-1; i>=0; i--) {
      tstaff = items[i]->getStaffNumber();
      if (tstaff != staff) {
         continue;
      }
      tpos = items[i]->getHorizontalPosition();
      if (tpos - hpos > 10) {
         break;
      }
      printMensurationDirection(out, items[i], partstaff, indent);
   }


}


//////////////////////////////
//
// printMensurationDirection --
//
// MenCircle = P1=12 unfilled circle
//

ostream& printMensurationDirection(ostream& out, ScoreItem* item, 
      int partstaff, int indent) {
   SCORE_FLOAT vpos = (item->getVPos() - 11.0) * 5.0;
   if (vpos < 0) {
      // don't look at anything below the top staff line.
      return out;
   }

   // men3 is the text "3" above a time signature
   if (item->isTextItem()) {
      string text = item->getTextWithoutInitialFontCode();
      if (text.compare("3") == 0) {
         printIndent(out, indent++, "<direction placement=\"above\">\n");
         printIndent(out, indent++, "<direction-type>\n");
         printIndent(out, indent,   "<words");
         out << " default-y=\"" << vpos << "\"";
         out << " relative-x=\"-32\"";
         out << " font-weight=\"bold\"";
         out << ">men3</words>\n";
         printIndent(out, --indent, "</direction-type>\n");
         printIndent(out, --indent, "</direction>\n");
         return out;
      }
   }
   if (!item->isShapeItem()) {
      return out;
   }
   if (item->isUnfilledCircle()) { 
      int p9 = item->getP9Int();
      int p10 = item->getP10Int();
      if ((p9 == 0) && (p10 == 0)) {
         SCORE_FLOAT vpos = (item->getVPos() - 11.0) * 5.0;
         vpos -= item->getVerticalRadius() * 5.0;
         vpos *= item->getStaffScale();
         printIndent(out, indent++, "<direction placement=\"above\">\n");
         printIndent(out, indent++, "<direction-type>\n");
         printIndent(out, indent,   "<words");
         out << " default-y=\"" << vpos << "\"";
         out << " relative-x=\"-32\"";
         out << " font-weight=\"bold\"";
         out << ">menCircle</words>\n";
         printIndent(out, --indent, "</direction-type>\n");
         printIndent(out, --indent, "</direction>\n");
         return out;
      } else {
         SCORE_FLOAT vpos = (item->getVPos() - 11.0) * 5.0;
         vpos -= item->getVerticalRadius() * 5.0;
         vpos *= item->getStaffScale();
         printIndent(out, indent++, "<direction placement=\"above\">\n");
         printIndent(out, indent++, "<direction-type>\n");
         printIndent(out, indent,   "<words");
         out << " default-y=\"" << vpos << "\"";
         out << " relative-x=\"-32\"";
         out << " font-weight=\"bold\"";
         out << ">menC</words>\n";
         printIndent(out, --indent, "</direction-type>\n");
         printIndent(out, --indent, "</direction>\n");
         return out;
      }
   }

   // deal with menC here

   return out;
}



//////////////////////////////
//
// printSystemLayout --
//

void printSystemLayout(stringstream& out, 
      ScorePage& page, const AddressSystem& system, int indent) {
   int sysindex        = system.getSystemIndex();
   int sysstaffindex   = system.getSystemStaffIndex();
   vectorVVSIp& staves = page.getStaffItemsBySystem();

   stringstream out2;

   int staffcount = staves[sysindex].size();
   if (staffcount <= 0) {
      return;
   }

   bool topStaffOnSystem = (sysstaffindex == staffcount - 1);
   bool topStaffOnPage   = topStaffOnSystem && (sysindex == 0);

   vectorSF leftside(staffcount);
   vectorSF rightside(staffcount);

   int i;
   for (i=0; i<staffcount; i++) {
      if (staves[sysindex][i].size() == 0) {
         continue;
      }
      leftside[i]  = staves[sysindex][i][0]->getHPos();
      rightside[i] = staves[sysindex][i][0]->getHPosRight();
   }
 
   sort(leftside.begin(), leftside.end());
   sort(rightside.begin(), rightside.end());

   SCORE_FLOAT sysleft  = leftside[0];
   SCORE_FLOAT sysright = rightside.back();

   indent++;

   if ((sysleft != 0.0) || (sysright != 200.0)) {
      // The system staves are not in default positions, so 
      // print where they should go.
      SCORE_FLOAT leftmargin = sysleft * 7.0 / 6.0 * 5.0;
      SCORE_FLOAT rightmargin = (200.0 - sysright) * 7.0 / 6.0 * 5.0;

      SCORE_FLOAT threshold = 0.1;
      if ((fabs(leftmargin) > threshold) || (fabs(rightmargin) > threshold)) {

         // <system-margins>
         printIndent(out2, indent++, "<system-margins>\n");
         printIndent(out2, indent,   "<left-margin>");
         out2 << leftmargin << "</left-margin>\n";
         printIndent(out2, indent,   "<right-margin>");
         out2 << rightmargin << "</right-margin>\n";
         printIndent(out2, --indent, "</system-margins>\n");
      }
   }

   // <system-distance>
   // If the staff is at the top of a system, then print
   // the distance to the bottom of the next system.  If 
   // there is no staff above the top staff of the system,
   // then display distance to top margin with <top-system-distance>
 
   ScoreItem* lostaff;
   ScoreItem* histaff;

   if (topStaffOnPage) {
      // Calculate distance to top margin.  Currently, the top margin
      // is hard-wired to 0.675 inches from the top of the page (11 inches
      // high).
      lostaff = staves[sysindex][sysstaffindex][0];
      SCORE_FLOAT lovpos = getMusicXmlTenthsFromStaffTopToPageBottom(lostaff);
      // High position is the top margin (currently the top of the page
      // because the top page margin is 0:
      SCORE_FLOAT hivpos = (11.00 - 0.0) * 25.4 * 40 / 8.89;
      printIndent(out2, indent, "<top-system-distance>");
      out2 << hivpos - lovpos << "</top-system-distance>\n";
   } else if (topStaffOnSystem && (sysindex > 0)) {
      // Calculate distance to bottom staff of next higher system
      lostaff = staves[sysindex][sysstaffindex][0];
      histaff = staves[sysindex-1][0][0];
      SCORE_FLOAT distance = getMusicXmlTenthsBetweenStaves(lostaff, histaff);
      printIndent(out2, indent, "<system-distance>");
      out2 << distance << "</system-distance>\n";
   }

   if (!out2.rdbuf()->in_avail()) {
      return;
   }

   indent--;

   printIndent(out, indent++, "<system-layout>\n");
   out << out2.str();
   printIndent(out, --indent, "</system-layout>\n");
}



//////////////////////////////
//
// getMusicXmlTenthsFromStaffTopToPageBottom --
//

SCORE_FLOAT getMusicXmlTenthsFromStaffTopToPageBottom(ScoreItem* staff) {
   int p2 = staff->getStaffNumber();
   SCORE_FLOAT inches = 0.8125 + (p2 - 1) * 0.7875;
   SCORE_FLOAT displacement = staff->getP4();
   SCORE_FLOAT scale = staff->getScale();
   SCORE_FLOAT offset = displacement * scale * 0.0875 / 2.0;
   SCORE_FLOAT staffheight = 8.0 * scale * 0.0875 / 2.0;
   inches += offset + staffheight;
   SCORE_FLOAT mm = inches * 25.4;
   SCORE_FLOAT tenths = mm * 40 / 8.89;
   return tenths;
}



//////////////////////////////
//
// printStaffLayout --
//

void printStaffLayout(stringstream& out, 
      ScorePage& page, const AddressSystem& system, int indent) {
   int sysindex = system.getSystemIndex();
   vectorVVSIp& staves = page.getStaffItemsBySystem();
   int sysstaffindex = system.getSystemStaffIndex();
   if (sysstaffindex < 0) {
      // tacet part of some sort
      return;
   }
 
   int staffcount = staves[sysindex].size();
   if (staffcount <= 0) {
      return;
   }

   if (sysstaffindex == staffcount - 1) {
     // top staff, so don't use staff layout.
     return;
   }

   ScoreItem* histaff = staves[sysindex][sysstaffindex+1][0];
   ScoreItem* lostaff = staves[sysindex][sysstaffindex][0];
 
   if ((histaff == NULL) || (lostaff == NULL)) {
      // no staff lines for some strange reason.
      return;
   }

   SCORE_FLOAT staffdistance = getMusicXmlTenthsBetweenStaves(lostaff, histaff);

   printIndent(out, indent++, "<staff-layout>\n");
   printIndent(out, indent, "<staff-distance>");
   out << staffdistance << "</staff-distance>\n";
   printIndent(out, --indent, "</staff-layout>\n");
}


//////////////////////////////
//
// getMusicXmlTenthsBetweenStaves --
//

SCORE_FLOAT getMusicXmlTenthsBetweenStaves(ScoreItem* lostaff, 
      ScoreItem* histaff) {

   SCORE_FLOAT p2hi = histaff->getStaffNumber();
   SCORE_FLOAT p2lo = lostaff->getStaffNumber();
   SCORE_FLOAT p2diff = p2hi - p2lo;

   SCORE_FLOAT scalehi = histaff->getScale();
   SCORE_FLOAT scalelo = lostaff->getScale();

   // staff distance is the distance between the default P2 locations
   // minus the height of the bottom staff 
   // minus the vertical offset of the bottom staff
   // plus the vertical offset of the top staff
   // Units are in MusicXML tenths (1/10 of nominal size spacing)
   SCORE_FLOAT difference = p2diff * 90.0;
   SCORE_FLOAT loheight = 8.0 * 5.0 * scalelo;
   SCORE_FLOAT looffset = lostaff->getP4() * 5.0 * scalelo;
   SCORE_FLOAT hioffset = histaff->getP4() * 5.0 * scalehi;
   SCORE_FLOAT staffdistance = difference - loheight - looffset + hioffset;

   // The distances are off by a diatonic step for some reason,
   // so removing some space from the distance to correct it.
   staffdistance -= 2.5;

   return staffdistance;
}



//////////////////////////////
//
// printDirections -- print text which is not treated as lyrics.
//      Will have to be adjusted when layers are implemented, since
//      this function will print the same text item for different
//      layers.
//

void printDirections(ostream& out, SystemMeasure& measureitems, int index, 
      int partstaff, int indent, int divisions) {
   printDirectionsBackwards(out, measureitems, index, partstaff, indent, 
         divisions);
   printDirectionsForwards(out, measureitems, index, partstaff, indent, 
         divisions);
   if (dufayQ) {
      // Search for minor color markings:
      // printMinorColoration(out, measureitems, index, partstaff, indent, 
      //      divisions);
   }
}



//////////////////////////////
//
// printMinorColoration --
//

void printMinorColoration(ostream& out, SystemMeasure& measureitems, 
      int index, int partstaff, int indent, int divisions) {
   vectorSIp& items = measureitems.getItems();
   int i;
   int colorstart = 0;
   int colorend   = 0;
   SCORE_FLOAT startp3 = items[index]->getHPos();
   SCORE_FLOAT threshold = 0.1;
   SCORE_FLOAT testp3;
   int nextNoteP3 = 210.0;
   int lastNoteP3 = -10.0;
   SCORE_FLOAT vpos = 11.0;
   int staff = items[index]->getStaffNumber();

   for (i=index-1; i>=0; i--) {
      if (!items[i]->isNoteItem()) {
         continue;
      }
      if (items[i]->getStaffNumber() != staff) {
         continue;
      }
      testp3 = items[i]->getHPos();
      if (fabs(testp3 - startp3) > threshold) {
         lastNoteP3 = testp3;
         break;
      }
   }

   for (i=index+1; i<items.size(); i++) {
      if (!items[i]->isNoteItem()) {
         continue;
      }
      if (items[i]->getStaffNumber() != staff) {
         continue;
      }
      testp3 = items[i]->getHPos();
      if (fabs(testp3 - startp3) > threshold) {
         nextNoteP3 = testp3;
         break;
      }
   }

   for (i=index-1; i>=0; i--) {
      if (items[i]->getStaffNumber() != staff) {
         continue;
      }
      testp3 = items[i]->getHPos();
      if (testp3 < (lastNoteP3 + startp3)/2.0) {
         break;
      }
      if (!items[i]->isSymbolItem()) {
         continue;
      }
      // minor coloration bracket is SCORE library object 206
      if (items[i]->getP5() != 206.0) {
         continue;
      }
      if (items[i]->getP7() < 0) {
         colorend = 1;
      } else {
         colorstart = 1;
      }
      vpos = items[i]->getVPos();
   }

   for (i=index+1; i<items.size(); i++) {
      if (items[i]->getStaffNumber() != staff) {
         continue;
      }
      testp3 = items[i]->getHPos();
      if (testp3 > (nextNoteP3 + startp3)/2.0) {
         break;
      }
      if (!items[i]->isSymbolItem()) {
         continue;
      }
      // minor coloration bracket is SCORE library object 206
      if (items[i]->getP5() != 206.0) {
         continue;
      }
      if (items[i]->getP7() < 0) {
         // also P9=270, but not checking
         colorend = 1;
      } else {
         colorstart = 1;
      }
      vpos = items[i]->getVPos();
   }

   if (colorend && colorstart) {
      printIndent(out, indent++, "<direction placement=\"above\">\n");
      printIndent(out, indent++, "<direction-type>\n");
      printIndent(out, indent,   "<words");
      out << " font-weight=\"bold\" default-y=\"" 
          << (vpos - 12.0)*5.0 << "\">minorColor</words>\n";
      printIndent(out, --indent, "</direction-type>\n");
      printIndent(out, --indent, "</direction>\n");
   } else if (colorstart) {
      printIndent(out, indent++, "<direction placement=\"above\">\n");
      printIndent(out, indent++, "<direction-type>\n");
      printIndent(out, indent,   "<words");
      out << " font-weight=\"bold\" default-y=\"" 
          << (vpos - 12.0)*5.0 << "\">minorColorEnd</words>\n";
      printIndent(out, --indent, "</direction-type>\n");
      printIndent(out, --indent, "</direction>\n");
   } else if (colorend) {
      printIndent(out, indent++, "<direction placement=\"above\">\n");
      printIndent(out, indent++, "<direction-type>\n");
      printIndent(out, indent,   "<words");
      out << " font-weight=\"bold\" default-y=\"" 
          << (vpos - 12.0)*5.0 << "\">minorColorEnd</words>\n";
      printIndent(out, --indent, "</direction-type>\n");
      printIndent(out, --indent, "</direction>\n");
   }
}



//////////////////////////////
//
// printDirectionsBackwards -- print any free-form text from the current 
//      position (expected to be a note or rest), backwards to the previous
//      note/rest on the given staff within the list of system items for the
//      current line.
//

void printDirectionsBackwards(ostream& out, SystemMeasure& measureitems, 
      int index, int partstaff, int indent, int divisions) {
   vectorSIp& items = measureitems.getItems();
   ScoreItem* si;
   int i;
   int p1;
   int p2;
   int startp2 = items[index]->getStaffNumber();
   SCORE_FLOAT threshold = 0.01;
   SCORE_FLOAT startp3 = items[index]->getHPos();
   SCORE_FLOAT p3;

   ScoreItem* endobject = NULL;
   // find next terminating item backwards
   for (i=index; i>=0; i--) {
      si = items[i];
      p2 = si->getStaffNumber();
      if (p2 != startp2) {
         continue;
      }
      p1 = si->getP1Int();
      if (p1 == P1_Staff) {
         endobject = si;
         break;
      }
      if (si->isNoteItem() && (si->isSecondaryChordNote())) {
         continue;
      }
      p3 = si->getHPos();
      if (fabs(p3-startp3) < threshold) {
         continue;
      }
      if (((p1 == P1_Note) || (p1 == P1_Rest)) &&
            (i != index)) {
         endobject = si;
         break;
      }
   }
   SCORE_FLOAT terminal_p3 = measureitems.getP3Left();
   if (endobject != NULL) {
      terminal_p3 = endobject->getHPos();
      if (endobject->isNoteItem() || endobject->isRestItem()) {
         terminal_p3 = (terminal_p3 + startp3)/2.0;
      }
   }

   for (i=index; i>=0; i--) {
      si = items[i];
      if (si->isSecondaryChordNote()) {
         continue;
      }
      p2 = si->getStaffNumber();
      if (p2 != startp2) {
         continue;
      }
      p1 = si->getP1Int();
      if (p1 == P1_Staff) {
         // don't try to print text before a staff.
         break;
      }
      p3 = si->getHPos();
      if (p3 < terminal_p3) {
         // don't look beyond 50% of the way to the next note.
         break;
      }
      if (p1 != P1_Text) {
         continue;
      }
      if (si->isDefined(ns_auto, np_verseLine) ||
          si->isDefined(ns_lyrics, np_verseLine)) {
         // lyrics are printed inside of <note> elements, not outside.
         continue;
      }
      if (si->isDefined(ns_auto, np_function) ||
          si->isDefined(np_function)) {
         // text is being used for another purpose (composer, title)).
         continue;
      }
 
      // found some text which should be printed as a <direction>
      printDirection(out, si, items[index], indent);
   }
}



//////////////////////////////
//
// printDirectionsForwards -- print any free-form text from the current 
//      position (expected to be a note or rest), forwards to the previous
//      note/rest on the given staff within the list of system items for the
//      current line.  The input vectorSIp items is for a
//

void printDirectionsForwards(ostream& out, SystemMeasure& measureitems, 
      int index, int partstaff, int indent, int divisions) {
   vectorSIp& items = measureitems.getItems();
   ScoreItem* si;

   if (!items[index]->hasDuration()) {
      return;
   }
   int i;
   int p1;
   int p2;
   int startp2 = items[index]->getStaffNumber();
   SCORE_FLOAT threshold = 0.1;
   SCORE_FLOAT startp3 = items[index]->getHPos();
   SCORE_FLOAT p3;

   ScoreItem* endobject = NULL;
   // find next terminal item forwards
   for (i=index+1; i<items.size(); i++) {
      si = items[i];
      p3 = si->getHPos();
      p2 = si->getStaffNumber();
      if (p2 != startp2) {
         continue;
      }

      if (si->isSecondaryChordNote()) {
         continue;
      }
      if (fabs(p3-startp3) < threshold) {
         continue;
      }
      p1 = si->getP1Int();
      if (((p1 == P1_Note) || (p1 == P1_Rest)) && (i != index)) {
         endobject = si;
         break;
      }
   }

   SCORE_FLOAT terminal_p3 = measureitems.getP3Right();
   if (endobject != NULL) {
      terminal_p3 = endobject->getHPos();
      if (endobject->isNoteItem() || endobject->isRestItem()) {
         terminal_p3 = (terminal_p3 + startp3)/2.0;
      }
   }

   for (i=index; i<items.size(); i++) {
      si = items[i];

      p2 = si->getStaffNumber();
      if (p2 != startp2) {
         continue;
      }
      p1 = si->getP1Int();
      if (p1 == P1_Staff) {
         // don't try to print text before a staff.
         break;
      }
      if (p1 != P1_Text) {
         continue;
      }
      p3 = si->getHPos();

      if (p3 > terminal_p3) {
         // don't look beyond 50% of the way to the next note.
         break;
      }
      if (si->isDefined(ns_auto, np_verseLine) ||
          si->isDefined(ns_lyrics, np_verseLine)) {
         // lyrics are printed inside of <note> elements, not outside.
         continue;
      }
      if (si->isDefined(ns_auto, np_function) ||
          si->isDefined(np_function)) {
         // text is being used for another purpose (composer, title)).
         continue;
      }
 
      // found some text which should be printed as a <direction>
      printDirection(out, si, items[index], indent);
   }
}



//////////////////////////////
//
// printDirection --
//

void printDirection(ostream& out, ScoreItem* si, ScoreItem* anchor, 
      int indent) {
   string word = si->getTextNoFontXmlEscapedUTF8();
   if (word.size() == 0) {
      return;
   }
   if (dufayQ) {
      if (word.compare("3") == 0) {
         // "3" means men3 and is handled elsewhere.
         return;
      }
      if ((word.compare("a") == 0) && si->isAboveStaff() 
            && (si->getHPos() < 30)) {
         // "a" is used on some measure numbers
         return;
      }
      if ((word.compare("Duo") == 0) && si->isAboveStaff()) {
         return;
      }
   }

   if (dufayQ) {
      if (word == "= ]") {
         // incomplete rhythmic scaling (which is undone), so not needed.
         return;
      }
      if (word == "= ].") {
         // incomplete rhythmic scaling (which is undone), so not needed.
         return;
      }
   }

   SCORE_FLOAT staffscale = si->getStaffScale();
   SCORE_FLOAT p4         = si->getVPos();
   SCORE_FLOAT p11        = si->getP11();
   SCORE_FLOAT defaulty   = (p4 - 11.0) * 5.0 * staffscale;
   SCORE_FLOAT relativex  = p11 * 5.0; /* times staffscale? */

   // relative-x probably more complicated to calculate:
   //SCORE_FLOAT p3 = si->getHPos();
   //SCORE_FLOAT anchorp3 = si->getHPos();
   //SCORE_FLOAT relativex = (anchorp3 - p3) * 5;

   printIndent(out, indent++, "<direction");
   if (p4 > 7) {
      out << " placement=\"above\"";
   } else {
      out << " placement=\"below\"";
   }
   out << ">\n";

   SCORE_FLOAT fontsize = si->getFontSizeInPoints(staffscale);

   printIndent(out, indent++, "<direction-type>\n");
   printIndent(out, indent, "<words");
   out << " default-y=\"" << defaulty << "\"";
   if (relativex != 0.0) {
      out << " relative-x=\"" << relativex << "\"";
   }
   out << " font-size=\"" << fontsize << "\"";
   //out << " font-weight=\"bold\"";
   out << ">";
   out << word;
   out << "</words>\n";

   printIndent(out, --indent, "</direction-type>\n");
   printIndent(out, --indent, "</direction>\n");
}



//////////////////////////////
//
// printForwardBarlineStyle -- print a forward repeat if required.
//

void printForwardBarlineStyle(ostream& out, SystemMeasure& measureitems, 
      int partstaff, int indent) {
   int i;
   ScoreItem* barline = NULL;
   vectorSIp& startbarlines = measureitems.getStartBarlines();

   int teststaff;
   int barheight;
   for (i=startbarlines.size()-1; i>=0; i--) {
      teststaff = startbarlines[i]->getStaffNumber();
      barheight = startbarlines[i]->getHeight();
      if (teststaff + barheight >= partstaff) {
         barline = startbarlines[i];
         break;
      }
   }
   if (barline == NULL) {
      return;
   }
   int style = barline->getP5Int();

   switch (style) {
      case 4:                      // right-facing repeat 
      case 5:                      // both-way repeat, light-heavy-light lines
         printIndent(out, indent, "<barline location=\"left\">\n");
         printIndent(out, indent+1, "<bar-style>heavy-light</bar-style>\n");
         printIndent(out, indent+1, 
               "<repeat direction=\"forward\" winged=\"none\"/>\n");
         printIndent(out, indent, "</barline>\n");
         return;
   }
}



//////////////////////////////
//
// printBarlineStyle -- Only checking the first barline found which 
//       affects the part.
//

void printBarlineStyle(ostream& out, SystemMeasure& measureitems, 
      int partstaff, int indent) {
   int i;
   ScoreItem* barline = NULL;
   vectorSIp& endbarlines = measureitems.getEndBarlines();

   int teststaff;
   int barheight;
   for (i=0; i<endbarlines.size(); i++) {
      teststaff = endbarlines[i]->getStaffNumber();
      barheight = endbarlines[i]->getHeight();
      if (teststaff + barheight >= partstaff) {
         barline = endbarlines[i];
         break;
      }
   }
   if (barline == NULL) {
      // implicit invisible barline
      printIndent(out, indent, "<barline location=\"right\">\n");
      printIndent(out, indent+1, "<bar-style>none</bar-style>\n");
      printIndent(out, indent, "</barline>\n");
      return;
   }

   if (barline->getP4() < 0) {
      // explicit invisible barline
      printIndent(out, indent, "<barline location=\"right\">\n");
      printIndent(out, indent+1, "<bar-style>none</bar-style>\n");
      printIndent(out, indent, "</barline>\n");
   }

   int style = barline->getP5Int();
   if (style == 0) {   // normal barline, don't do anything
      return;
   }

   switch (style) {
      case 1:                      // double barline
         printIndent(out, indent, "<barline location=\"right\">\n");
         printIndent(out, indent+1, "<bar-style>light-light</bar-style>\n");
         printIndent(out, indent, "</barline>\n");
         return;
      case 2:                      // final barline (light-heavy style)
         printIndent(out, indent, "<barline location=\"right\">\n");
         printIndent(out, indent+1, "<bar-style>light-heavy</bar-style>\n");
         printIndent(out, indent, "</barline>\n");
         return;
      case 3:                      // left-facing repeat 
         printIndent(out, indent, "<barline location=\"right\">\n");
         printIndent(out, indent+1, "<bar-style>light-heavy</bar-style>\n");
         printIndent(out, indent+1, 
               "<repeat direction=\"backward\" winged=\"none\"/>\n");
         printIndent(out, indent, "</barline>\n");
         return;
      case 4:                      // right-facing repeat 
         return;
      case 5:                      // both-way repeat, light-heavy-light lines
         printIndent(out, indent, "<barline location=\"right\">\n");
         printIndent(out, indent+1, "<bar-style>light-heavy</bar-style>\n");
         printIndent(out, indent+1, 
               "<repeat direction=\"backward\" winged=\"none\"/>\n");
         printIndent(out, indent, "</barline>\n");
         // forward part of repeat is handled previously, 
         // this is just the back repeat part.
         return;
      case 6:                      // both-way repeat, two heavy lines
         return;
      case 7:                      // dashed barline
         printIndent(out, indent, "<barline location=\"right\">\n");
         printIndent(out, indent+1, "<bar-style>dashed</bar-style>\n");
         printIndent(out, indent, "</barline>\n");
         return;
   }

   // style ==  8 is a brace
   // style ==  9 is a bracket
   // style == 10 is a subbrackt

}



//////////////////////////////
//
// printMeasureAttributes --
//

void printMeasureAttributes(ostream& out, vectorSIp& items, int index, 
      int sysindex, int partindex, int partstaff, int measureindex, 
      int divisions, int indent, ScoreItem* currkey, ScoreItem* currtime, 
      ScoreItem* currclef, SCORE_FLOAT& laststaffsize, vectorI& partVisible) {

   stringstream divisionstream;
   stringstream keystream;
   stringstream timestream;
   stringstream clefstream;
   stringstream staffsizestream;
   stringstream hiddenstream;

   indent++;
   int printing = 0;
   int i;
   SCORE_FLOAT staffsize = -1.0;

   if ((measureindex == 0) && (partstaff < 0) && (partVisible[partindex] == 1)) {
      // part staff is not displayed on this system, but the previous
      // system showed it, so hide this system.
      partVisible[partindex] = 0;
      printIndent(hiddenstream, indent, "<staff-details print-object=\"no\"/>\n");
      printing++;
   } else {
      if ((partstaff > 0) && (measureindex == 0) && (partVisible[partindex] == 0)) {
         // Unhide the part.  The part staff was hidden on the previous system
         // but this system should show it.
         partVisible[partindex] = 1;
         printIndent(hiddenstream, indent, 
            "<staff-details print-object=\"yes\"/>\n");
         printing++;
      }

      for (i=index; i<items.size(); i++) {
         if (items[i]->getStaffNumber() != partstaff) {
            continue;
         }
         if (staffsize < 0.0) {
            ScoreItem* staff = items[i]->getStaffOwner();
            if (staff != NULL) {
               staffsize = staff->getScale();
            }
         }
         if (items[i]->hasDuration()) {
            break;
         }
         if (items[i]->isKeySigItem()&&(!SU::equalKeySigs(items[i], currkey))){
            printing += printKeySigItem(keystream, items[i], indent);
         }
         if (items[i]->isTimeSigItem()&&(!SU::equalTimeSigs(items[i], currtime))){
            if (!items[i]->isSingleNumber()) {
               printing += printTimeSigItem(timestream, items[i], indent);
            }
         }
         if (items[i]->isClefItem() && (!SU::equalClefs(items[i], currclef))) {
            printing += printClefItem(clefstream, items[i], indent);
         }
      }
   }

   if ((index == 0) && (sysindex == 0) && (measureindex == 0)) {
      printIndent(divisionstream, indent, "<divisions>");
      divisionstream << divisions << "</divisions>\n";
      printing = 1;
   }

   if (staffsize < 0) {
      // If the staff is hidden, use the staff size from the 
      // previous system.
      staffsize = laststaffsize;
   }
   if (staffsize != laststaffsize) {
      // Only the default size matters, so always change it if not equal
      // to the default.  Currently the default size is always set to 1.0.
      // laststaffsize = staffsize;
      printIndent(staffsizestream, indent++, "<staff-details>\n");
      printIndent(staffsizestream, indent,   "<staff-size>");
      staffsizestream << staffsize * 100.0 << "</staff-size>\n";
      printIndent(staffsizestream, --indent, "</staff-details>\n");
      printing = 1;
   }


   if (!printing) {
      return;
   }

   indent--;
   printIndent(out, indent++, "<attributes>\n");

   // http://www.musicxml.com/UserManuals/MusicXML/MusicXML.htm#EL-MusicXML-attributes.htm
   // <level>

   // <divisions>
   if (divisionstream.rdbuf()->in_avail()) {
      out << divisionstream.str();
   }

   // <key>
   if (keystream.rdbuf()->in_avail()) {
      out << keystream.str();
   }

   // <time>
   if (timestream.rdbuf()->in_avail()) {
      out << timestream.str();
   }

   // <staves> -- Used if more than one staff for a part.
   // <part-symbol> -- symbol for multi-staff part.
   // <instruments> -- if more than one instrument.

   // <clef>
   if (clefstream.rdbuf()->in_avail()) {
      out << clefstream.str();
   }

   // <staff-details>
   if (staffsizestream.rdbuf()->in_avail()) {
      out << staffsizestream.str();
   }
   if (hiddenstream.rdbuf()->in_avail()) {
      out << hiddenstream.str();
   }

   // <transpose>
   // <directive>
   // <measure-style>

   printIndent(out, --indent, "</attributes>\n");
}



//////////////////////////////
//
// printKeySigItem --
//

int printKeySigItem(stringstream& stream, ScoreItem* item, int indent) {
   if (item == NULL) {
      return 0;
   }
   if (!item->isKeySigItem()) {
      return 0;
   }
   int p5 = item->getAccidentalCount();
  
   if (abs(p5) < 10) {
      printIndent(stream, indent++, "<key>\n");   
      printIndent(stream, indent, "<fifths>");   
      stream << p5 << "</fifths>\n";
      // don't know the mode, and anyway it is irrelevant to know for key sig.
      printIndent(stream, --indent, "</key>\n");   
      return 1;
   }

   // +/-100 in p5 is a cancellation signature.
   // Figure out how to represent in MusicXML and print here...

   return 0;
}



//////////////////////////////
//
// printTimeSigItem -- Only handles simple time signatures which have an
// integer top and a power of two bottom, plus C and Cut-C.
//

int printTimeSigItem(stringstream& stream, ScoreItem* item, int indent) {
   if (item == NULL) {
      return 0;
   }
   if (!item->isTimeSigItem()) {
      return 0;
   }
   int symbol = 0;

   int beats = item->getP5Int();
   int beattype = item->getP6Int();

   if (beattype == 0) {
      // Don't know what to do, so just ignore for now.
      // This is a single number (P5).
      return 0;
   }

   if ((beats == 99) && (beattype == 1)) {
      // Common time
      symbol = 1;
      beats = 4;
      beattype = 4;
   } else if ((beats == 98) && (beattype == 1)) {
      // Cut time
      symbol = 2;
      beats = 2;
      beattype = 2;
   }

   if (rhythmicScalingQ) {
      beattype = (int)(beattype * pow(2.0, -Scaling));
      if (beattype < 1) {
         beattype = 1;
      }
   }

   printIndent(stream, indent++, "<time");
   switch (symbol) {
      case 1: stream << " symbol=\"common\""; break;
      case 2: stream << " symbol=\"cut\""; break;
   }
   stream << ">\n";

   printIndent(stream, indent, "<beats>");
   stream << beats << "</beats>\n";
   printIndent(stream, indent, "<beat-type>");
   stream << beattype << "</beat-type>\n";

   printIndent(stream, --indent, "</time>\n");

   return 1;
}



//////////////////////////////
//
// printClefItem --
//

int printClefItem(stringstream& stream, ScoreItem* item, int indent) {
   if (item == NULL) {
      return 0;
   }
   if (!item->isClefItem()) {
      return 0;
   }
   int shape = item->getP5Int();
   double fraction = item->getP5() - shape;

   char sign = '?';
   int  line = -1;
   switch (shape) {
      case 0:             // G clef
         sign = 'G';
         line = 2;
         break;
      case 1:             // F clef
         sign = 'F';
         line = 4;
         break;
      case 2:             // C clef (alto placement)
         sign = 'C';
         line = 3;
         break;
      case 3:             // C clef (tenor placement)
         sign = 'C';
         line = 4;
         break;
      case 4:             // percussion clef
         break;
   }

   int p4 = item->getP4();
   if (p4 != 0) {
      line += p4/2;
   }

   printIndent(stream, indent++, "<clef>\n");

   printIndent(stream, indent, "<sign>");
   stream << sign << "</sign>\n";

   printIndent(stream, indent, "<line>");
   stream << line << "</line>\n";

   if (fabs(fraction - 0.8) < 0.001) {
      printIndent(stream, indent, 
            "<clef-octave-change>-1</clef-octave-change>\n");
   }

   printIndent(stream, --indent, "</clef>\n");

   return 1;
}



//////////////////////////////
//
// printRest -- Print a rest.
//

void printRest(ostream& out, ScoreItem* si, int partstaff, int indent,
      int divisions, SCORE_FLOAT measureP3) {

   SCORE_FLOAT p3 = si->getHPos();
   SCORE_FLOAT defaultx = (p3 - measureP3) * 5.0 * 6.0 / 7.0;
   SCORE_FLOAT duration = si->getDuration();

   printIndent(out, indent++, "<note");
   out << " default-x=\"" << defaultx << "\"";
   out << ">\n";
   printIndent(out, indent, "<rest/>\n");

   // <duration>
   int notedivs = (int)(divisions * duration + 0.5);
   if (notedivs > 0) {  // dont't print if grace notes;
      if (rhythmicScalingQ) {
         notedivs = int(notedivs * pow(2.0, Scaling)+0.5);
      }
      printIndent(out, indent, "<duration>");
      out << notedivs << "</duration>\n";
   }

   // <voice>

   // <type>
   string resttype = getRestType(si);
   if (resttype.size() > 0) {
      printIndent(out, indent, "<type>" + resttype + "</type>\n");
   }

   // <dot>
   int dotcount = si->getDotCount();
   for (int dot=0; dot<dotcount; dot++) {
      printIndent(out, indent, "<dot/>\n");
   }

   // <time-modification>
   printTimeModification(out, indent, si, divisions);

   // <lyric>
   printLyrics(out, si, indent);

   printIndent(out, --indent, "</note>\n");
}



//////////////////////////////
//
// printNoteNotations -- fermatas, slurs, arpeggios, etc.  Generally
//     can be placed in any order.
//

void printNoteNotations(ostream& out, ScoreItem* si, int indent, 
      string& notetype) {
   stringstream notations;

   // <accidental-mark> ///////////////////////////////////////////////////
   // <arpeggiate> ////////////////////////////////////////////////////////
   // <articulations> /////////////////////////////////////////////////////
   // <dynamics> //////////////////////////////////////////////////////////

   // <fermata>  //////////////////////////////////////////////////////////
   if (si->hasFermata()) {
      printIndent(notations, indent+1, "<fermata");
      if (si->getArticulation() > 0) {
         notations << " type=\"upright\"";
      } else {
         notations << " type=\"inverted\"";
      }
      // @default-y can be used to control vertical placement of fermata.
      notations << "/>\n";
   }

   // <glissando> /////////////////////////////////////////////////////////
   // <non-arpeggiate> ////////////////////////////////////////////////////
   // <ornaments> /////////////////////////////////////////////////////////
   // <other-notation> ////////////////////////////////////////////////////
   // <slide> /////////////////////////////////////////////////////////////
   // <technical> /////////////////////////////////////////////////////////

   // <tied> //////////////////////////////////////////////////////////////
   // attributes: type, number, line-type, dash-length, space-length, 
   //      deafult-x, default-y, relative-x, relative-y, placement,
   //      orientation, bezier-offset, bezier-offset2, bezier-x, 
   //      bezier-y, bezier-x2, bezier-y2, color.
   if (si->isTieGroupStart()) {
      printIndent(notations, indent+1, "<tied type=\"start\"/>\n");
   } else if (si->isTieGroupEnd()) {
      printIndent(notations, indent+1, "<tied type=\"stop\"/>\n");
   } else if (si->isTieGroupMiddle()) {
      printIndent(notations, indent+1, "<tied type=\"stop\"/>\n");
      printIndent(notations, indent+1, "<tied type=\"start\"/>\n");
   }

   // <tuplet> ////////////////////////////////////////////////////////////
   if (si->inTupletGroup()) {
      if (si->tupletGroupHasBracket()) {
         if (si->isFirstNoteInTupletGroup()) {
            printIndent(notations, indent+1, "<tuplet bracket=\"yes\"");
            notations << " type=\"start\"/>\n";
         } else if (si->isLastNoteInTupletGroup()) { 
            printIndent(notations, indent+1, "<tuplet bracket=\"yes\"");
            notations << " type=\"stop\"/>\n";
         }
      }
   }

   if (notations.rdbuf()->in_avail()) {
      printIndent(out, indent, "<notations>\n");
      out << notations.str();
      printIndent(out, indent, "</notations>\n");
   }
}



//////////////////////////////
//
// printNote -- Print a note and any chord-notes attached to it.
//

void printNote(ostream& out, ScoreItem* si, int partstaff, int indent,
      int divisions, SCORE_FLOAT measureP3) {

   SCORE_FLOAT p3 = si->getHPos();
   SCORE_FLOAT defaultx = (p3 - measureP3) * 5.0 * 6.0 / 7.0;

   printIndent(out, indent++, "<note");
   out << " default-x=\"" << defaultx << "\"";
   out << ">\n";

   double duration = si->getDuration();

   // <grace> ////////////////////////////////////////////////
   if (duration == 0.0) {
      printIndent(out, indent, "<grace/>\n");
      // deal with slash on note, but only for primary note in
      // grace chord.
   }

   // <chord> ////////////////////////////////////////////////
   if (si->isSecondaryChordNote()) {
      printIndent(out, indent, "<chord/>\n");
   }

   int  base40 = si->getParameterInt(ns_auto, np_base40Pitch);
   char step   = SU::base40ToUCDiatonicLetter(base40);
   int  alter  = SU::base40ToChromaticAlteration(base40);
   if (si->hasEditorialAccidental()) {
      alter = si->getEditorialAccidental();
   }
   int  octave = base40 / 40;

   // <pitch> ////////////////////////////////////////////////
   printIndent(out, indent++, "<pitch>\n");

   printIndent(out, indent, "<step>");
   out << step << "</step>\n";

   if (alter != 0) {
      printIndent(out, indent, "<alter>");
      out << alter << "</alter>\n";
   }

   printIndent(out, indent, "<octave>");
   out << octave << "</octave>\n";
   printIndent(out, --indent, "</pitch>\n");

   // <duration> /////////////////////////////////////////////
   int notedivs = (int)(divisions * duration + 0.5);
   if (notedivs > 0) {  // dont't adjust grace notes;
      if (rhythmicScalingQ) {
         notedivs = int(notedivs * pow(2.0, Scaling)+0.5);
      }
      printIndent(out, indent, "<duration>");
      out << notedivs << "</duration>\n";
   }
   // <tie> ////////////////////////////////////////////////
   if (si->inTieGroup()) {
      if (si->isTieGroupStart()) {
         printIndent(out, indent, "<tie type=\"start\"/>\n");
      } else if (si->isTieGroupEnd()) {
         printIndent(out, indent, "<tie type=\"stop\"/>\n");
      } else if (si->isTieGroupMiddle()) {
         printIndent(out, indent, "<tie type=\"stop\"/>\n");
         printIndent(out, indent, "<tie type=\"start\"/>\n");
      }
   }

   // <voice> ////////////////////////////////////////////////////////////

   // <type> /////////////////////////////////////////////////////////////
   string notetype = getNoteType(si);
   if (notetype.size() > 0) {
      printIndent(out, indent, "<type>" + notetype + "</type>\n");
   }

   // <dot> ///////////////////////////////////////////////////////////////
   int dotcount = si->getDotCount();
   for (int dot=0; dot<dotcount; dot++) {
      printIndent(out, indent, "<dot/>\n");
   }

   // <accidental> ////////////////////////////////////////////////////////
   if (si->hasEditorialAccidental()) {
      printIndent(out, indent, "<accidental parentheses=\"yes\">");
      switch (si->getEditorialAccidental()) {
         case -2: out << "double-flat";  break;
         case -1: out << "flat";         break;
         case  0: out << "natural";      break;
         case  1: out << "sharp";        break;
         case  2: out << "double-sharp"; break;
         default: out << "unknown-accidental" << si->getEditorialAccidental(); 
            break;
      }
      out << "</accidental>\n";
   } else if (si->hasPrintedAccidental()) {
      printIndent(out, indent, "<accidental>");
      switch (alter) {
         case -2: out << "double-flat";  break;
         case -1: out << "flat";         break;
         case  0: out << "natural";      break;
         case  1: out << "sharp";        break;
         case  2: out << "double-sharp"; break;
         default: out << "unknown-accidental" << si->getEditorialAccidental(); 
           break;
      }
      out << "</accidental>\n";
   }

   // <time-modification> (tuplets) ///////////////////////////////////////
   printTimeModification(out, indent, si, divisions);

   // <stem> //////////////////////////////////////////////////////////////
   if (si->hasStem()) {
      printIndent(out, indent, "<stem");
      if (si->stemUp()) {
         out << ">up";
      } else {
         out << ">down";
      }
      out << "</stem>\n";
   }

   // <beam> //////////////////////////////////////////////////////////////

   // <notations> /////////////////////////////////////////////////////////
   printNoteNotations(out, si, indent, notetype);

   printLyrics(out, si, indent);

   printIndent(out, --indent, "</note>\n");

   if (si->isPrimaryChordNote()) {
      vectorSIp* cns = si->getChordNotes();
      if (cns == NULL) {
         return;
      }
      vectorSIp& chordnotes = *cns;
      if (chordnotes.size() < 2) {
         return;
      }
      for (int i=1; i<chordnotes.size(); i++) {
         printNote(out, chordnotes[i], partstaff, indent, divisions, measureP3);
      }
   }

}



//////////////////////////////
//
// printTimeModification --
//

ostream& printTimeModification(ostream& out, int indent, ScoreItem* si, 
      int divisions) {
   RationalDuration rd = si->getRationalDuration();
   if (rd.isPowerOfTwo()) {
      return out;
   }

   RationalNumber rn = rd.getDurationPrimary();
   int exponent = ceil(log(rn.getFloat())/log(2)-0.0001);
   RationalNumber regular = 1;
   if (exponent > 0) {
      regular = 1 << exponent;
   } else if (exponent < 0) {
      regular /= (1 << (-exponent));
   } 
   
   RationalNumber tuplet = rn / regular;
   // printIndent(out, indent, "<!-- tuplet ");
   // out << tuplet << " -->\n";
   printIndent(out, indent++, "<time-modification>\n");
   printIndent(out, indent, "<actual-notes>");
   out <<  tuplet.getDenominator() << "</actual-notes>\n";
   printIndent(out, indent, "<normal-notes>");
   out <<  tuplet.getNumerator() << "</normal-notes>\n";
   printIndent(out, --indent, "</time-modification>\n");
  
   return out;
}
   


//////////////////////////////
//
// printLyrics --
//
// <lyrics>
//    <lyric default-y="-80" number="1">
//       <syllabic>begin</syllabic>
//       <text>A</text>
//    </lyric>
//    <lyric default-y="-97" number="2">
//       <syllabic>begin</syllabic>
//       <text>F</text>
//    </lyric>
// <lyrics>
//

void printLyrics(ostream& out, ScoreItem* si, int indent) {
   if (!si->isPrimaryChordNote()) {
      // Only attach lyrics to primary notes of chords (also single notes
      // are considered primary chord notes)
      return;
   }
   vectorSIp* testlyrics = si->getLyricsGroup();
   if (testlyrics == NULL) {
      // no lyrics attached to this note.
      return;
   }

   vectorSIp& lyrics = *testlyrics;
   int number;
   vectorSIp notes;
   vectorVSIp verses(100);
   int count = 0;
   int i, j;
   // first extract the verse lines:
   for (i=0; i<lyrics.size(); i++) {
      if (lyrics[i]->isNoteItem()) {
         notes.push_back(lyrics[i]);
      }
      if (!lyrics[i]->isTextItem()) {
         continue;
      }
      number = lyrics[i]->getParameterInt(ns_auto, np_verseLine);
      if ((number > 0) && (number < verses.size())) {
         verses[number].push_back(lyrics[i]);
         count++;
      }
   }

   if (count == 0) {
      // nothing to do
      return;
   }

   int before;
   int after;

   SCORE_FLOAT p4, defaulty;
   SCORE_FLOAT staffscale = si->getStaffScale();

   for (i=1; i<verses.size(); i++) {
      if (verses[i].size() == 0) {
         continue;
      }
      printIndent(out, indent++, "<lyric");
      out << " number=\"" << i << "\"";

      if (verses[i].size() > 0) {
         p4 = verses[i][0]->getVPos();
         defaulty = (p4 - 11.0) * 5.0 * staffscale;
         if (defaulty != 0.0) {
            out << " default-y=\"" << defaulty << "\"";
         }
      }

      out << ">\n";

      for (j=0; j<verses[i].size(); j++) {
         // <syllabic>
         before = verses[i][j]->getParameterBool(ns_auto, np_hyphenBefore);
         after  = verses[i][j]->getParameterBool(ns_auto, np_hyphenAfter);

         if (j<verses[i].size()-1) {
            printIndent(out, indent, "<syllabic>single</syllabic>\n");
         } else {
            if (before && after) {
               printIndent(out, indent, "<syllabic>middle</syllabic>\n");
            } else if (before) {
               printIndent(out, indent, "<syllabic>end</syllabic>\n");
            } else if (after) {
               printIndent(out, indent, "<syllabic>begin</syllabic>\n");
            } else {
               printIndent(out, indent, "<syllabic>single</syllabic>\n");
            }
         }
 
         // <text>
         // only considering initial lyric attached
         // have to figure out what happens with multiple syllables
         // of the same verse number.
         printIndent(out, indent, "<text");
         out << " font-size=\"" 
             << verses[i][j]->getFontSizeInPoints(staffscale) << "\"";
         if (verses[i][j]->isItalic()) {
            out << " font-style=\"italic\"";
         }
         if (verses[i][j]->isBold()) {
            out << " font-weight=\"bold\"";
         }
         out << ">";
         out << verses[i][j]->getTextNoFontXmlEscapedUTF8();
         out << "</text>\n";

         if (j<verses[i].size()-1) {
            printIndent(out, indent, "<elision> </elision>\n");
         } else {
            // <extend>
            if (verses[i][j]->getParameterBool(ns_auto, np_wordExtension)) {
               printIndent(out, indent, "<extend type=\"start\"/>\n");
            }
         }
      }

      printIndent(out, --indent, "</lyric>\n");
   }

}



//////////////////////////////
//
// getNoteType --
//


string getNoteType(ScoreItem* si) {
   if (si->isRestItem()) {
      return getRestType(si);
   }
   int headtype = si->getP6Int();
   if (rhythmicScalingQ && (headtype > 0)) {
      headtype += Scaling;
   }
   switch (headtype) {
      case 1:   return "half";
      case 2:   return "whole";
      case 3:   return "breve";
      case 0:   // quarter note or smaller, determine below
         break;
   }
   if (headtype > 0) {
      return "";      // unknown type (diamond, X, invisible notehead)
   }
   double duration = si->getDuration();
   if (duration <= 0) {
      // deal with grace note notehead types later...
      return "";
   }

   double exp = log(duration)/log(2.0);
   // int type = floor(exp-0.0001);
   int type = floor(exp+0.0001);
   if (rhythmicScalingQ) {
      type += Scaling;
   }

   switch (type) {
      case 5:   return "maxima";
      case 4:   return "long";
      case 3:   return "breve";
      case 2:   return "whole";
      case 1:   return "half";
      case 0:   return "quarter";
      case -1:  return "eighth";
      case -2:  return "16th";
      case -3:  return "32nd";
      case -4:  return "64th";
      case -5:  return "128th";
   }
 
   // unknown type
   return "";
}



//////////////////////////////
//
// getRestType --
//

string getRestType(ScoreItem* si) {
   if (si->isNoteItem()) {
      return getNoteType(si);
   }
   int headtype = si->getP5Int();
   if (rhythmicScalingQ) {
      headtype -= Scaling;
   }
   switch (headtype) {
      case  0:  return "quarter";
      case  1:  return "eighth";
      case  2:  return "16th";
      case  3:  return "32nd";
      case  4:  return "64th";
      case -1:  return "half";
      case -2:  return "whole";
      case -3:  return "breve";
   }

   // unknown type
   return "";
}



