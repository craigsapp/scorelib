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
                                     int segment, int indent);
ostream& printDefaults              (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ostream& printCredits               (ostream& out, ScorePageSet& infiles, 
                                     int segment, int indent);
ostream& printPartInfo              (ostream& out, ScorePageSet& infiles, 
                                     int segment, int part, int indent);
ostream& printXml                   (ostream& out, const string& text);
void     printSystem                (vector<stringstream*>& partouts, 
                                     ScorePageSet& infiles, int segment, 
                                     int systemindex, int indent, 
                                     vectorVSIp& curclef, vectorVSIp& curkey,
                                     vectorVSIp& curtime, int& mcounter,
                                     int divisions);
void     printPartMeasure           (stringstream& out, ScorePageSet& infiles, 
                                     ScorePage& page, 
                                     AddressSystem& partaddress,
                                     SystemMeasure& measureitems, 
                                     int sysindex, int measureindex, 
                                     int partindex, vectorVSIp& curclef, 
                                     vectorVSIp& curkey, vectorVSIp& curtime, 
                                     int mcounter, int indent, int divisions,
                                     int systemindex);
void     printNote                  (ostream& out, ScoreItem* si,
                                     int partstaff, int indent, int divisions);
void     printRest                  (ostream& out, ScoreItem* si, int partstaff, 
                                     int indent, int divisions);
void     printMeasureAttributes     (ostream& out, vectorSIp& items, int index, 
                                     int sysindex, int partindex, 
                                     int partstaff, int measureindex, 
                                     int divisions, int indent, 
                                     ScoreItem* currkey, ScoreItem* currtime, 
                                     ScoreItem* currclef);
int      printClefItem              (stringstream& stream, ScoreItem* item, 
                                     int indent);
int      printTimeSigItem           (stringstream& stream, ScoreItem* item, 
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


#define INDENT_STRING "\t"

int      locationQ        = 1;
int      debugQ           = 0;
int      rhythmicScalingQ = 0;
int      invisibleQ       = 0;
int      Scaling          = 0;
int      lyricsQ          = 1;
int      systemBreaksQ    = 1;
int      pageBreaksQ      = 1;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("L|no-location=b", 
         "Don't print location of each measure as comment");
   opts.define("s|segment=i:0", "segment to process");
   opts.define("f|filebase=s:filebase", 
         "Base of filename for multiple segments");
   opts.define("debug=b", "Print debugging information");
   opts.define("scale|scaling|rhythmic-scaling=i:0", 
         "Global rhythmic scaling of durations");
   opts.define("I|no-invisible=b", "Don't convert invisible rests/notes");
   opts.define("no-lyrics=b", "Don't process Lyrics");
   opts.define("no-system-breaks=b", "Don't print system break information");
   opts.define("no-page-breaks=b", "Don't print page break information");
   opts.define("dufay=b", "Use default options for Dufay translations");
   opts.process(argc, argv);

   locationQ        = !opts.getBoolean("no-location");
   debugQ           =  opts.getBoolean("debug");
   rhythmicScalingQ =  opts.getBoolean("rhythmic-scaling");
   invisibleQ       =  opts.getBoolean("no-invisible");
   Scaling          =  opts.getInteger("rhythmic-scaling");
   lyricsQ          = !opts.getBoolean("no-lyrics");
   systemBreaksQ    = !opts.getBoolean("no-system-breaks");
   pageBreaksQ      = !opts.getBoolean("no-page-breaks");
   if (opts.getBoolean("dufay")) {
      rhythmicScalingQ = 1;
      Scaling          = 1;
      invisibleQ       = 1;
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
   infiles.analyzeSegmentsByIndent();
   infiles.analyzePitch();
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
   string filename = filebase + "-"; to_string(segment);
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
   printIdentification(out, infiles, segment, indent);
   printDefaults(out, infiles, segment, indent);
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

   for (i=0; i<systemcount; i++) {
      printSystem(partoutput, infiles, segment, i, indent+1, current_clef, 
            current_keysig, current_timesig, measure_counter, divisions);
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

ostream& printCredits(ostream& out, ScorePageSet& infiles, 
      int segment, int indent) {
   // doing nothing for now
   return out;
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
   // doing nothing for now
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
      int segment, int indent) {
   printIndent(out, indent++, "<identification>\n");
   printIndent(out, indent++, "<encoding>\n");
   printIndent(out, indent, "<software>");
   out << "score2musicxml alpha version</software>\n";
   printIndent(out, indent, "<encoding-date>");
   auto now = chrono::system_clock::now();
   auto in_time = chrono::system_clock::to_time_t(now);
   out << put_time(localtime(&in_time), "%Y-%m-%d");
   out << "</encoding-date>\n";

   printIndent(out, --indent, "</encoding>\n");
   printIndent(out, --indent, "</identification>\n");
   return out;
}



//////////////////////////////
//
// printPartList -- Print the <part-list> element.
//

ostream& printPartList(ostream& out, ScorePageSet& infiles, 
    int segment, int indent) {
   printIndent(out, indent++, "<part-list>\n");

   int partcount = infiles.getSegment(segment).getPartCount();
   for (int i=0; i<partcount; i++) {
      printPartInfo(out, infiles, segment, i, indent);
   }

   printIndent(out, --indent, "</part-list>\n");
   return out;
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
   int part, int indent) {
   ScoreSegment& seg = infiles.getSegment(segment);
   printIndent(out, indent++, "<score-part id=\"P");
   out << to_string(part+1) << "\">\n";
   string partname = seg.getPartName(part);
   printIndent(out, indent, "<part-name");
   if (partname != "") {
      out << ">";
      printXml(out, partname);
      out << "</part-name>\n";
   } else {
      out << " print-object=\"no\">Part";
      out << to_string(part+1) << "\"</part-name>\n";
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
      vectorVSIp& curkey, vectorVSIp& curtime, int& mcounter, int divisions) {

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
               mcounter, indent, divisions, systemindex);
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
      int divisions, int systemindex) {

   ScoreItem* currentclef = curclef[partindex][0];
   ScoreItem* currentkey  = curkey[partindex][0];
   ScoreItem* currenttime = curtime[partindex][0];
   ScoreItem* nextclef = NULL;
   ScoreItem* nextkey  = NULL;
   ScoreItem* nexttime = NULL;
   
   double measuredur = measureitems.getDuration();
   int partstaff = page.getPageStaffIndex(partaddress);


   double width = measureitems.getP3Width();
   width = width * 7.0 / 6.0 * 5.0;
   width = ((int)(width * 10 + 0.5))/10.0;

   if (locationQ) {
      printIndent(out, indent, "<!--");
      out << " page=\"" << to_string(partaddress.getPageIndex()+1) << "\"";
      out << " system=\"" << to_string(partaddress.getSystemIndex()+1) << "\"";
      out << " systemstaff=\"" << to_string(partaddress.getSystemStaffIndex()+1);
      out << "\"";
      out << " pagestaff=\"" << to_string(partstaff) << "\"";
      out << " staffmeasure=\"" << to_string(measureindex+1) << "\"";
      out << " measuredur=\"" << measuredur << "\"";
      out << " -->\n";
   }

   printIndent(out, indent++, "<measure");
   out << " number=\"" << to_string(mcounter) << "\"";
   out << " width=\"" << width << "\">\n";

   vectorSIp& items = measureitems.getItems();
   int i;
   int staff;
   ScoreItem* si;

   // <barline>
   printForwardBarlineStyle(out, measureitems, partstaff, indent);

   // <print>
   if (pageBreaksQ && (measureindex == 0) && (partaddress.getSystemIndex() == 0) && (systemindex != 0)) {
      printIndent(out, indent, "<print new-page=\"yes\"/>\n");
   } else if (systemBreaksQ && (measureindex == 0) && (sysindex != 0)) {
      printIndent(out, indent, "<print new-system=\"yes\"/>\n");
   }

   // <attributes>
   printMeasureAttributes(out, items, 0, sysindex, partindex, partstaff, 
         measureindex, divisions, indent, currentkey, currenttime, currentclef);

   // <sound>

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
         nexttime = si;
      }

      if (si->isNoteItem()) {
         if (si->isSecondaryChordNote()) {
            // handled with primary notes.
            continue;
         }
         printNote(out, si, partstaff, indent, divisions);
      }
  
      if (si->isRestItem()) {
         if (!(invisibleQ && si->isInvisible())) {
            printRest(out, si, partstaff, indent, divisions);
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
      ScoreItem* currclef) {

   stringstream divisionstream;
   stringstream keystream;
   stringstream timestream;
   stringstream clefstream;

   indent++;
   int printing = 0;

   int i;
   for (i=index; i<items.size(); i++) {
      if (items[i]->getStaffNumber() != partstaff) {
         continue;
      }
      if (items[i]->hasDuration()) {
         break;
      }
      if (items[i]->isTimeSigItem()&&(!SU::equalTimeSigs(items[i], currtime))){
         printing += printTimeSigItem(timestream, items[i], indent);
      }
      if (items[i]->isClefItem() && (!SU::equalClefs(items[i], currclef))) {
         printing += printClefItem(clefstream, items[i], indent);
      }
   }

   if ((index == 0) && (sysindex == 0) && (measureindex == 0)) {
      printIndent(divisionstream, indent, "<divisions>");
      divisionstream << divisions << "</divisions>\n";
      printing = 1;
   }


   if (!printing) {
      return;
   }

   indent--;
   printIndent(out, indent, "<attributes>\n");

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

   // <clef>
   if (clefstream.rdbuf()->in_avail()) {
      out << clefstream.str();
   }

   printIndent(out, indent, "</attributes>\n");
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
         sign = 'G';
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
      int divisions) {

   double duration = si->getDuration();

   printIndent(out, indent++, "<note>\n");
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

   printIndent(out, --indent, "</note>\n");
}



//////////////////////////////
//
// printNoteNotations -- fermatas, ...
//

void printNoteNotations(ostream& out, ScoreItem* si, int indent, 
      string& notetype) {
   stringstream fermatastream;
   int printnotations = 0;

   // <fermata>
   if (si->hasFermata()) {
      printnotations = 1;
      printIndent(fermatastream, indent+1, "<fermata");
      if (si->getArticulation() > 0) {
         fermatastream << " type=\"upright\"";
      } else {
         fermatastream << " type=\"inverted\"";
      }

      // @default-y can be used to control vertical placement of fermata.

      fermatastream << "/>\n";
   }

   if (printnotations) {
      printIndent(out, indent, "<notations>\n");

      if (fermatastream.rdbuf()->in_avail()) {
         out << fermatastream.str();
      }
         
      printIndent(out, indent, "</notations>\n");
   }
}



//////////////////////////////
//
// printNote -- Print a note and any chord-notes attached to it.
//

void printNote(ostream& out, ScoreItem* si, int partstaff, int indent,
      int divisions) {

   printIndent(out, indent++, "<note>\n");

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
   out << to_string(octave) << "</octave>\n";
   printIndent(out, --indent, "</pitch>\n");

   // <duration> /////////////////////////////////////////////
   int notedivs = (int)(divisions * duration + 0.5);
   if (notedivs > 0) {  // dont't print if grace notes;
      if (rhythmicScalingQ) {
         notedivs = int(notedivs * pow(2.0, Scaling)+0.5);
      }
      printIndent(out, indent, "<duration>");
      out << notedivs << "</duration>\n";
   }

   // <voice> ////////////////////////////////////////////////

   // <type> /////////////////////////////////////////////////
   string notetype = getNoteType(si);
   if (notetype.size() > 0) {
      printIndent(out, indent, "<type>" + notetype + "</type>\n");
   }

   // <dot>
   int dotcount = si->getDotCount();
   for (int dot=0; dot<dotcount; dot++) {
      printIndent(out, indent, "<dot/>\n");
   }

   // <accidental> ///////////////////////////////////////////
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

   // <stem>
   if (si->hasStem()) {
      printIndent(out, indent, "<stem");
      if (si->stemUp()) {
         out << ">up";
      } else {
         out << ">down";
      }
      out << "</stem>\n";
   }

   // <beam>

   // <notations>
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
         printNote(out, chordnotes[i], partstaff, indent, divisions);
      }
   }

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

   for (i=1; i<verses.size(); i++) {
      if (verses[i].size() == 0) {
         continue;
      }
      printIndent(out, indent++, "<lyric");
      out << " number=\"" << to_string(i) << "\"";
      out << ">\n";

      for (j=0; j<verses[i].size(); j++) {
         // <syllabic>
         before = verses[i][j]->getParameterBool(ns_auto, np_hyphenBefore);
         after  = verses[i][j]->getParameterBool(ns_auto, np_hyphenAfter);
         if (j<verses[i].size()-1) {
            printIndent(out, indent, "<syllabic>single</syllabic>\n");
         } else {
            if (before && after) {
               printIndent(out, indent, "<syllabic>continue</syllabic>\n");
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
         printIndent(out, indent, "<text>");
         SU::printXmlTextEscapedUTF8(out, 
               verses[i][j]->getTextWithoutInitialFontCode());
         out << "</text>\n";

         if (j<verses[i].size()-1) {
            printIndent(out, indent, "<elision> </elision>\n");
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
   if (rhythmicScalingQ) {
      headtype += Scaling;
   }
   switch (headtype) {
      case 0:   // quarter note or smaller, determine below
         break;
      case 1:   return "half";
      case 2:   return "whole";
      case 3:   return "breve";
      default:  return "";      // unknown type (diamond, X, invisible notehead)
   }
   double duration = si->getDuration();
   if (duration <= 0) {
      // deal with grace note notehead types later...
      return "";
   }

   double exp = log(duration)/log(2.0);
   int type = ceil(exp-0.0001);
   if (rhythmicScalingQ) {
      type += Scaling;
   }

   switch (type) {
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



