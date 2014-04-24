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
                                     vectorVSIp curclef, vectorVSIp curkey,
                                     vectorVSIp curtime);

#define INDENT_STRING "\t"

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("s|segment=i:0", "segment to process");
   opts.define("f|filebase=s:filebase", 
          "base of filename for multiple segments");
   opts.process(argc, argv);
   
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
      printIndent(*partoutput[i], indent, "<part id=\"P");
      tempstring = to_string(i+1);
      *partoutput[i] << tempstring << "\">\n";
   }

   vectorVSIp current_clef(partcount);
   vectorVSIp current_keysig(partcount);
   vectorVSIp current_timesig(partcount);

   for (i=0; i<systemcount; i++) {
      printSystem(partoutput, infiles, segment, i, indent+1, current_clef, 
            current_keysig, current_timesig);
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
      int segment, int systemindex, int indent, vectorVSIp curclef, 
      vectorVSIp curkey, vectorVSIp curtime) {

   ScoreSegment& seg = infiles.getSegment(segment);
   SystemAddress sys = seg.getSystem(systemindex);

   


   // Iterate through each measure in the system.
   // ggg

}










