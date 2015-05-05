//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May 14 22:22:03 PDT 2014
// Last Modified: Sat May 17 19:14:16 PDT 2014
// Filename:      score2mei.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/score2mei.cpp
// Documentation: https://github.com/craigsapp/scorelib/wiki/score2mei
// Syntax:        C++ 11
//
// Description:   Converts a ScorePageSet into MEI.
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
ostream& convertAllSegmentsToMdivs  (ostream& out, ScorePageSet& infiles);

void     printToFile                (ScorePageSet& infiles, int segment,
                                     string& filebase);
void     convertAllSegmentsToSeparateFiles(ScorePageSet& infiles,
                                           string& filebase);
//
// Utility functions:
//

ostream& printIndent                (ostream& out, int indent, string text);
void     printIndent                (stringstream& out, int indent,
                                     string text);
ScoreItem* getComposer              (ScorePageSet& infiles, int segment);
ScoreItem* getTitle                 (ScorePageSet& infiles, int segment);
string   getDurAttributeValue       (ScoreItem* si);

//
// Element printing functions:
//

ostream& printMeiHeadElement        (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent,
                                     map<string, ScoreItem*>& infoText);
ostream& printEncodingDescElement   (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent,
                                     map<string, ScoreItem*>&  infoText);
ostream& printFileDescElement       (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent,
                                     map<string, ScoreItem*>& infoText);
ostream& printRevisionDescElement   (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent,
                                     map<string, ScoreItem*>&  infoText);
ostream& printSegmentElement        (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent, int divisions,
                                     map<string, ScoreItem*>& infoText);
ostream& printScoreDefElement       (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent, int divisions,
                                     map<string, ScoreItem*>& infoText);
ostream& printStaffGrpElement       (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent);
ostream& printSectionElement        (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent);
ostream& printChordElement          (ostream& out, ScoreItem* si, int partstaff,
                                     int indent, int divisions,
                                     SCORE_FLOAT measureP3);
ostream& printNoteElement           (ostream& out, ScoreItem* si,
                                     int partstaff, int indent, int divisions,
                                     SCORE_FLOAT measureP3);
ostream& printPgHeadElement         (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent,
                                     map<string, ScoreItem*>& infoText);
ostream& printRestElement           (ostream& out, ScoreItem* si,
                                     int partstaff, int indent,
                                     int divisions, SCORE_FLOAT measureP3);
ostream& printBeamElement           (ostream& out, ScoreItem* si,
                                     vectorSIp& items, int index,
                                     int partstaff, int indent, int divisions,
                                     SCORE_FLOAT measureP3);
ostream& printVerseElements         (ostream& out, ScoreItem* si, int indent);
ostream& printStaffElement          (ostream& out, ScorePageSet& infiles,
                                     ScorePage& page,
                                     SystemMeasure& measureitems,
                                     int sysindex, int measureindex,
                                     vectorVSIp& curclef, vectorVSIp& curkey,
                                     vectorVSIp& curtime, int mcounter,
                                     int indent, int divisions,
                                     int systemindex, vectorSF& staffsizes,
                                     int segmentindex, int partcount,
                                     int partindex);
ostream& printMeasureElement        (ostream&  out, ScorePageSet& infiles,
                                     int segment, int systemindex, int indent,
                                     vectorVSIp& curclef, vectorVSIp& curkey,
                                     vectorVSIp& curtime, int& mcounter,
                                     int divisions, vectorSF& staffsizes,
                                     int partcount);
ostream& printMeasureElementContents(ostream& out, ScorePageSet& infiles,
                                     ScorePage& page,
                                     SystemMeasure& measureitems,
                                     int sysindex, int measureindex,
                                     vectorVSIp& curclef, vectorVSIp& curkey,
                                     vectorVSIp& curtime, int mcounter,
                                     int indent, int divisions,
                                     int systemindex, vectorSF& staffsizes,
                                     int segmentindex, int partcount);
ostream& printStaffDefElement       (ostream& out, ScorePageSet& infiles,
                                     int segment, int indent, int partindex,
                                     int partcount);


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
int      dufayQ           = 0;

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
      convertAllSegmentsToMdivs(cout, infiles);
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
      out << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)"
          << "\n";

      out << R"(<?xml-model href="http://music-encoding.googlecode.com/svn/tags/MEI2013_v2.1.0/schemata/mei-all.rng")";
      out << "\n";
      out << R"(   type="application/xml" schematypens="http://relaxng.org/ns/structure/1.0"?>)";
      out << "\n";

      out << R"(<?xml-model href="http://music-encoding.googlecode.com/svn/tags/MEI2013_v2.1.0/schemata/mei-all.rng")";
      out << "\n";
      out << R"(   type="application/xml" schematypens="http://purl.oclc.org/dsdl/schematron"?>)";
      out << "\n";
   }

   int divisions = infiles.getLCMRhythm(segment);

   printIndent(out, indent++, "");
   out << R"(<mei xmlns="http://www.music-encoding.org/ns/mei")";
   out << "\n";

   printIndent(out, indent-1, "   ");
   out << R"(xmlns:xlink="http://www.w3.org/1999/xlink")";
   out << "\n";

   printIndent(out, indent-1, "   ");
   out << R"(meiversion="2013">)";
   out << "\n";

   map<string, ScoreItem*> infoText;

   infoText["title"] = getTitle(infiles, segment);
   infoText["composer"] = getComposer(infiles, segment);

   printMeiHeadElement(out, infiles, segment, indent, infoText);

   printIndent(out, indent++, "<music>\n");
   printIndent(out, indent++, "<body>\n");
   printIndent(out, indent++, "<mdiv>\n");
   printIndent(out, indent++, "<score>\n");

   printSegmentElement(out, infiles, segment, indent, divisions, infoText);

   printIndent(out, --indent, "</score>\n");
   printIndent(out, --indent, "</mdiv>\n");
   printIndent(out, --indent, "</body>\n");
   printIndent(out, --indent, "</music>\n");
   printIndent(out, --indent, "</mei>\n");

   return out;
}



//////////////////////////////
//
// printSegmentElement -- Print the musical content of a ScorePageSet segment.
//     The <mei> entry point is inside of <score> element.
//

ostream& printSegmentElement(ostream& out, ScorePageSet& infiles, int segment,
      int indent, int divisions, map<string, ScoreItem*>& infoText) {

   // <scoreDef>
   printScoreDefElement(out, infiles, segment, indent, divisions, infoText);

   // <section>
   printSectionElement(out, infiles, segment, indent);

   return out;
}



//////////////////////////////
//
// printScoreDefElement --
//

ostream& printScoreDefElement(ostream& out, ScorePageSet& infiles, int segment,
      int indent, int divisions, map<string, ScoreItem*>& infoText) {
   printIndent(out, indent++, "<scoreDef\n");
   printIndent(out, indent, "");
   out << "ppq=\"" << divisions << "\"\n";

   // scoreDef@meter.count
   // scoreDef@meter.unit
   // Default time signature: read the first time signature
   // found on first line of music in segment.
   ScoreSegment& seg        = infiles.getSegment(segment);
   const AddressSystem& sys = seg.getSystemAddress(0);
   ScorePage* page          = infiles.getPage(sys);
   int sysindex             = sys.getSystemIndex();
   int i;
   int timetop, timebot;
   vectorSIp& items         = page->getSystemItems(sysindex);
   for (i=0; i<(int)items.size(); i++) {
      if (!items[i]->isTimeSignatureItem()) {
         continue;
      }
      timetop = items[i]->getTimeSignatureTop();
      timebot = items[i]->getTimeSignatureBottom();

      if (rhythmicScalingQ) {
         timebot = (int)(timebot * pow(2.0, -Scaling));
         if (timebot < 1) {
            timebot = 1;
         }
      }

      printIndent(out, indent, "meter.count=\"");
      out << timetop << "\"\n";
      printIndent(out, indent, "meter.unit=\"");
      out << timebot << "\"\n";
      break;
   }

   // scoreDef@vu.height is the length of a "virtual unit" which is
   // the size of a diatonic step on the nominal staff size.
   SCORE_FLOAT vuheight = 1.11125;
   printIndent(out, indent, "vu.height=\"");
   out << vuheight << "mm\"\n";

   // page height is currently fixed to 8.5"x11" paper:
   printIndent(out, indent, "page.height=\"");
   out << 11 * 25.4 / vuheight << "\"\n";

   // page width is currently fixed to 8.5"x11" paper:
   printIndent(out, indent, "page.width=\"");
   out << 8.5 * 25.4 / vuheight << "\"\n";

   // default margins for 8.5"x11" paper:
   printIndent(out, indent, "page.leftmar=\"");
   out << 0.525 * 25.4 / vuheight << "\"\n";
   printIndent(out, indent, "page.rightmar=\"");
   out << 0.475 * 25.4 / vuheight << "\"\n";

   // top and bottom margins are set to 0
   printIndent(out, indent, "page.topmar=\"0\"\n");
   printIndent(out, indent, "page.botmar=\"0\"\n");

   // default width of systems.
   printIndent(out, indent, "system.leftmar=\"0\"\n");
   printIndent(out, indent, "system.rightmar=\"0\"\n");

   printIndent(out, indent-1, ">\n");

   // <pgHead>
   printPgHeadElement(out, infiles, segment, indent, infoText);

   // <pgHead2>

   // <pgFoot>

   // <pgFoot2>

   // <staffGrp>
   printStaffGrpElement(out, infiles, segment, indent);

   printIndent(out, --indent, "</scoreDef>\n");
   return out;
}



//////////////////////////////
//
// printPgHeadElement -- Identify the title and composer on the first system.
//

ostream& printPgHeadElement(ostream& out, ScorePageSet& infiles, int segment,
      int indent, map<string, ScoreItem*>& infoText) {

   indent++;
   stringstream tout;

   int p2;
   SCORE_FLOAT p3;
   SCORE_FLOAT p4;
   SCORE_FLOAT scale;
   SCORE_FLOAT staffoffset;
   // SCORE_FLOAT topmargin = 11.0;
   SCORE_FLOAT fontsize = 10.0;
   ScoreItem* si;
   SCORE_FLOAT yposition;
   SCORE_FLOAT xposition;
   SCORE_FLOAT staffv;
   int pageindex;
   for (auto& it : infoText) {
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
      if (pageindex != 0) {
         continue;
         // only process first page items for now.
      }
      fontsize        = si->getFontSizeInPoints(scale);
      string function = si->getParameter(ns_auto, np_function);
      staffv = 0.8125 + 0.7875 * (p2 - 1) + staffoffset * scale * 0.0875 / 2.0;
      yposition = staffv + (p4 - 2.0) * scale * 0.0875 / 2.0;
      yposition = yposition * 25.4 * 40 / 8.89;
      // xposition = p3 / 200.0 * 7.5 * 25.4 * 40 / 8.89;
      xposition = p3 * 6.0 / 7.0;
      xposition += 0.525 * 25.4 * 40 / 8.89;

      printIndent(tout, indent++, "<anchoredText");
      if (function == "composer") {
         tout << " n=\"composer\"";
      } else if (function == "title") {
         tout << " n=\"title\"";
      }
      tout << " x=\"" << xposition << "\"";
      tout << " y=\"" << yposition << "\"";
      tout << ">\n";
      printIndent(tout, indent, "<rend");
      tout << " fontsize=\"" << fontsize << "\">";
      tout << SU::getTextNoFontXmlEscapedUTF8(text);
      tout << "</rend>\n";
      printIndent(tout, --indent, "</anchoredText>\n");
   }

   indent--;
   if (tout.rdbuf()->in_avail()) {
      printIndent(out, indent, "<pgHead>\n");
      out << tout.str();
      printIndent(out, indent, "</pgHead>\n");
   }

   return out;
}



//////////////////////////////
//
// printStaffGrpElement --
//

ostream& printStaffGrpElement(ostream& out, ScorePageSet& infiles,
      int segment, int indent) {

   ScoreSegment& seg = infiles.getSegment(segment);
   int partcount = seg.getPartCount();
   const AddressSystem& sys = seg.getSystemAddress(0);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   int i;
   vectorSIp& items     = page->getSystemItems(sysindex);
   if (items.size() == 0) {
      return out;
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
   for (i=0; i<(int)items.size(); i++) {
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

   printIndent(out, indent++, "<staffGrp");

   if (barlinestyle == 9) {
      // square bracket
      out << " symbol=\"bracket\"";
   }
   out << ">\n";

   // <scoreDef>/<staffGrp>/<staffDef>
   for (i=partcount-1; i>=0; i--) {
      printStaffDefElement(out, infiles, segment, indent, i, partcount);
   }

   printIndent(out, --indent, "</staffGrp>\n");
   return out;
}



//////////////////////////////
//
// printStaffDefElement -- Print the <part-list> element.
//

ostream& printStaffDefElement(ostream& out, ScorePageSet& infiles,
      int segment, int indent, int partindex, int partcount) {

   ScoreSegment& seg = infiles.getSegment(segment);
   printIndent(out, indent++, "<staffDef\n");

   // staffDef@n
   printIndent(out, indent, "n=\"");
   out << (partcount - partindex) << "\"\n";

   // staffDef@xml:id
   printIndent(out, indent, "xml:id=\"P");
   out << (partcount - partindex) << "\"\n";

   // staffDef@label
   string partname = seg.getPartName(partindex);
   if (partname.size() > 0) {
      printIndent(out, indent, "label=\"");
      out << SU::getTextNoFontXmlEscapedUTF8(partname);
      out << "\"\n";
   }

   // staffDef@lines, currently fixed to 5 lines on a staff.
   printIndent(out, indent, "lines=\"5\"\n");


   // Starting clef
   ScoreItem* initialclef = seg.getInitialClef(partindex);
   if (initialclef != NULL) {

      // staffDef@clef.line == staff line from bottom of staff.
      printIndent(out, indent, "clef.line=\"");
      out << initialclef->getStaffLine() << "\"\n";

      // staffDef@clef.shape == "G" for G clef, etc.
      printIndent(out, indent, "clef.shape=\"");
      out << initialclef->getClefLetter() << "\"\n";

      if (initialclef->soundsOctaveDown()) {
         // staffDef@clef.shape.dis == e.g. "8" if vocal tenor clef.
         printIndent(out, indent, "clef.dis=\"8\"\n");

         // staffDef@clef.shape.dis.place == location of 8 on clef ("below").
         printIndent(out, indent, "clef.dis.place=\"below\"\n");
      }
   }


   // staffDef@scale == scaling of staff from defaul
   SCORE_FLOAT staffscale = infiles.getPartScale(segment, partindex);
   if (staffscale != 1.0) {
      printIndent(out, indent, "scale=\"");
      out << staffscale * 100.0 << "%\"\n";
   }

   printIndent(out, --indent, "/>\n");
   return out;
}



//////////////////////////////
//
// printSectionElement -- print the music for a particular section.
//    <mei>/<music>/<body>/<mdiv>/<score>/<section>
//

ostream& printSectionElement(ostream& out, ScorePageSet& infiles, int segment,
      int indent) {
   printIndent(out, indent++, "<section>\n");

   ScoreSegment& seg = infiles.getSegment(segment);
   int partcount     = seg.getPartCount();
   int systemcount   = seg.getSystemCount();
   int i;

   vectorVSIp current_clef(partcount);
   vectorVSIp current_keysig(partcount);
   vectorVSIp current_timesig(partcount);
   for (i=0; i<partcount; i++) {
      current_clef[i].push_back(NULL);
      current_keysig[i].push_back(NULL);
      current_timesig[i].push_back(NULL);
   }

   int measure_counter = 1;
   int divisions = infiles.getLCMRhythm(segment);

   vectorSF staffsizes(partcount);
   fill(staffsizes.begin(), staffsizes.end(), 1.0);

   for (i=0; i<systemcount; i++) {
      printMeasureElement(out, infiles, segment, i, indent, current_clef,
            current_keysig, current_timesig, measure_counter, divisions,
            staffsizes, partcount);
   }

   printIndent(out, --indent, "</section>\n");
   return out;
}




//////////////////////////////
//
// printMeiHeadElement -- print <mei>/<meiHead> element.
//

ostream& printMeiHeadElement(ostream& out, ScorePageSet& infiles, int segment,
      int indent, map<string, ScoreItem*>& infoText) {

   printIndent(out, indent++, "<meiHead>\n");

   // <fileDesc>
   printFileDescElement(out, infiles, segment, indent, infoText);

   // <encodingDesc>
   printEncodingDescElement(out, infiles, segment, indent, infoText);

   // <revisionDesc>
   printRevisionDescElement(out, infiles, segment, indent, infoText);

   printIndent(out, --indent, "</meiHead>\n");

   return out;
}



///////////////////////////////
//
// printRevisionDescElement -- print <mei>/<meiHead>/<revisionDesc> element.
//

ostream&  printRevisionDescElement(ostream& out, ScorePageSet& infiles,
      int segment, int indent, map<string, ScoreItem*>&  infoText) {

   return out;
}



///////////////////////////////
//
// printEncodingDescElement -- print <mei>/<meiHead>/<encodingDesc> element.
//

ostream&  printEncodingDescElement(ostream& out, ScorePageSet& infiles,
      int segment, int indent, map<string, ScoreItem*>&  infoText) {

   return out;
}



//////////////////////////////
//
// printFileDescElement --  Print the <fileDesc> element.
//
//  <fileDesc>
//     <titleStmt>
//        <title label="movement">Adieu ces bons vins de Lannoys</title>
//        <respStmt>
//           <name role="composer">Guillaume Du Fay</name>
//        </respStmt>
//     </titleStmt>
//     <pubStmt/>
//     <sourceDesc>
//        <source>
//           <titleStmt>
//              <title label="movement">Adieu ces bons vins de Lannoys</title>
//              <respStmt>
//                 <name role="composer">Guillaume Du Fay</name>
//              </respStmt>
//           </titleStmt>
//           <pubStmt>
//              <availability>
//                 <useRestrict>Alejandro Planchart</useRestrict>
//              </availability>
//           </pubStmt>
//           <notesStmt>
//              <annot>Source SCOREFILE converted with score2mei alpha version on <date>2014-05-15</date>.</annot>
//           </notesStmt>
//        </source>
//     </sourceDesc>
//  </fileDesc>
//

ostream& printFileDescElement(ostream& out, ScorePageSet& infiles,
      int segment, int indent, map<string, ScoreItem*>& infoText) {

   printIndent(out, indent++, "<fileDesc>\n");

   // <titleStmt> /////////////////////////////////////////////////
   printIndent(out, indent++, "<titleStmt>\n");
   // <titleStmt>/<title>
   printIndent(out, indent, "<title label=\"movement\">");
   if (infoText.count("title")) {
      out << infoText["title"]->getTextNoFontXmlEscapedUTF8();
   }
   out << "</title>\n";
   // <titleStmt>/<respStmt>/<name>
   printIndent(out, indent++, "<respStmt>\n");
   printIndent(out, indent, "<name role=\"composer\">");
   if (infoText.count("composer")) {
      out << infoText["composer"]->getTextNoFontXmlEscapedUTF8();
   }
   out << "</name>\n";
   printIndent(out, --indent, "</respStmt>\n");
   printIndent(out, --indent, "</titleStmt>\n");
   // <pubStmt> ///////////////////////////////////////////////////
   printIndent(out, indent, "<pubStmt/>\n");

   // <sourceDesc> ////////////////////////////////////////////////
   printIndent(out, indent++, "<sourceDesc>\n");
   printIndent(out, indent++, "<source>\n");
   // <sourceDesc>/<source>/<titleStmt> //////////////////////////////
   printIndent(out, indent++, "<titleStmt>\n");
   // <titleStmt>/<title>
   printIndent(out, indent, "<title label=\"movement\">");
   if (infoText.count("title")) {
      out << infoText["title"]->getTextNoFontXmlEscapedUTF8();
   }
   out << "</title>\n";
   // <titleStmt>/<respStmt>/<name>
   printIndent(out, indent++, "<respStmt>\n");
   printIndent(out, indent, "<name role=\"composer\">");
   if (infoText.count("composer")) {
      out << infoText["composer"]->getTextNoFontXmlEscapedUTF8();
   }
   out << "</name>\n";
   printIndent(out, --indent, "</respStmt>\n");
   printIndent(out, --indent, "</titleStmt>\n");
   // <sourceDesc>/<source>/<pubStmt> ///////////////////////////////
   printIndent(out, indent++, "<pubStmt>\n");
   printIndent(out, indent++, "<availability>\n");
   printIndent(out, indent,   "<useRestrict>");
   if (infoText.count("copyright")) {
      out << infoText["copyright"]->getTextNoFontXmlEscapedUTF8();
   } else if (dufayQ) {
      out << "Alejandro Planchart";
   }
   out << "</useRestrict>\n";
   printIndent(out, --indent, "</availability>\n");
   printIndent(out, --indent, "</pubStmt>\n");
   // <sourceDesc>/<source>/<notesStmt> ////////////////////////////
   printIndent(out, indent++, "<notesStmt>\n");
   printIndent(out, indent,   "<annot>");
   out << "Source SCORE file converted with score2mei alpha version on <date>";
   auto now = chrono::system_clock::now();
   auto in_time = chrono::system_clock::to_time_t(now);
   out << put_time(localtime(&in_time), "%Y-%m-%d");
   out << "</date> (https://github.com/craigsapp/scorelib)";
   out << "</annot>\n";

   printIndent(out, --indent, "</notesStmt>\n");
   printIndent(out, --indent, "</source>\n");
   printIndent(out, --indent, "</sourceDesc>\n");

   printIndent(out, --indent, "</fileDesc>\n");
   return out;
}



//////////////////////////////
//
// getTitle --
//

ScoreItem* getTitle(ScorePageSet& infiles, int segment) {
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

   return candidate;
}



//////////////////////////////
//
// getComposer --
//

ScoreItem* getComposer(ScorePageSet& infiles, int segment) {
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

   return candidate;
}





//////////////////////////////
//
// printXml -- Escape various characters and make sure UTF-8.
//

ostream&  printXml(ostream& out, const string& text) {
   for (int i=0; i<(int)text.size(); i++) {
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
// convertAllSegmentsToMdivs --
//

ostream& convertAllSegmentsToMdivs(ostream& out, ScorePageSet& infiles) {
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
// printMeasureElement -- Print all parts on the given system.
//

ostream& printMeasureElement(ostream& out, ScorePageSet& infiles,
      int segment, int systemindex, int indent, vectorVSIp& curclef,
      vectorVSIp& curkey, vectorVSIp& curtime, int& mcounter, int divisions,
      vectorSF& staffsizes, int partcount) {

   ScoreSegment& seg = infiles.getSegment(segment);
   const AddressSystem& sys = seg.getSystemAddress(systemindex);
   ScorePage* page = infiles.getPage(sys);
   int sysindex = sys.getSystemIndex();
   int barcount = page->getSystemBarCount(sysindex);
   AddressSystem partaddress;
   partaddress = seg.getPartAddress(systemindex, 0);
   int i;
   for (i=0; i<barcount; i++) {
      SystemMeasure& measureitems = page->getSystemMeasure(sysindex, i);
      if (measureitems.getDuration() == 0.0) {
         continue;
      }

      SCORE_FLOAT width = measureitems.getP3Width();
      width = width * 6.0 / 7.0;

      double measuredur = measureitems.getDuration();
      out << "\n";
      if (locationQ) {
         printIndent(out, indent, "<!--");
         out << " page=\""          << partaddress.getPageIndex()+1 << "\"";
         out << " system=\""        << partaddress.getSystemIndex()+1 << "\"";
         out << " sysmeasure=\"" << i+1 << "\"";
         out << " mdur=\""          << measuredur << "\"";
         out << " -->\n";
      }

      printIndent(out, indent++, "<measure");
      out << " n=\"" << mcounter << "\"";
      out << " xml:id=\"m";
      out << (uint64_t)&measureitems << "\"";
      out << " width=\"" << width << "\"";
      out << ">\n";

      printMeasureElementContents(out, infiles, *page,
            measureitems, sysindex, i, curclef, curkey, curtime,
            mcounter, indent, divisions, systemindex, staffsizes,
            segment, partcount);

      printIndent(out, --indent, "</measure>\n");
      printIndent(out, indent,
         "<!--=======================================================-->\n");
      mcounter++;
   }

   return out;
}



//////////////////////////////
//
// printStaffElement -- print a single measure for the given part
//

ostream&  printStaffElement(ostream& out, ScorePageSet& infiles,
      ScorePage& page, SystemMeasure& measureitems,
      int sysindex, int measureindex, vectorVSIp& curclef,
      vectorVSIp& curkey, vectorVSIp& curtime, int mcounter, int indent,
      int divisions, int systemindex, vectorSF& staffsizes,
      int segmentindex, int partcount, int partindex) {

   ScoreSegment& seg = infiles.getSegment(segmentindex);
   AddressSystem partaddress = seg.getPartAddress(systemindex, partindex);

   printIndent(out, indent++, "<staff n=\"");
   out << (partcount-partindex);
   out << "\">\n";

   // currently only one layer being processed.
   printIndent(out, indent++, "<layer n=\"1\">\n");

   // ScoreItem* currentclef = curclef[partindex][0];
   // ScoreItem* currentkey  = curkey[partindex][0];
   // ScoreItem* currenttime = curtime[partindex][0];
   ScoreItem* nextclef = NULL;
   ScoreItem* nextkey  = NULL;
   ScoreItem* nexttime = NULL;

   // double measuredur = measureitems.getDuration();
   int partstaff = page.getPageStaffIndex(partaddress);

   SCORE_FLOAT measureP3 = measureitems.getP3();

   vectorSIp& items = measureitems.getItems();
   int i;
   int staff;
   ScoreItem* si;

   // <note>
   for (i=0; i<(int)items.size(); i++) {
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
         if (si->isFirstNoteInBeamGroup()) {
            printBeamElement(out, si, items, i, partstaff, indent, divisions,
                measureP3);
         }
         if (si->getChordNoteCount() > 1) {
            printChordElement(out, si, partstaff, indent, divisions, measureP3);
         }
         printNoteElement(out, si, partstaff, indent, divisions, measureP3);
      }

      if (si->isRestItem()) {
         if (!(invisibleQ && si->isInvisible())) {
            printRestElement(out, si, partstaff, indent, divisions, measureP3);
         }
      }

   }

   if (nextclef != NULL) {
      curclef[partindex][0] = nextclef;
   }
   if (nextkey != NULL) {
      curkey[partindex][0] = nextkey;
   }
   if (nexttime != NULL) {
      curtime[partindex][0] = nexttime;
   }

   printIndent(out, --indent, "</layer>\n");
   printIndent(out, --indent, "</staff>\n");

   return out;
}



//////////////////////////////
//
// printBeamElement -- returns the index number of the last note in the group.
//

ostream& printBeamElement(ostream& out, ScoreItem* si, vectorSIp& items, int index,
      int partstaff, int indent, int divisions, SCORE_FLOAT measureP3) {

   BeamGroup* bgp = si->getBeamGroup();
   if (bgp == NULL) {
      return out;
   }
   BeamGroup& bg = *bgp;

   printIndent(out, indent++, "<beam>\n");

   int i;
   for (i=0; i<(int)bg.notes.size(); i++) {
      if (si->isSecondaryChordNote()) {
         // handled with primary notes.
         continue;
      }
      if (si->getChordNoteCount() > 1) {
         printChordElement(out, bg.notes[i], partstaff, indent, divisions,
               measureP3);
      }
      printNoteElement(out, bg.notes[i], partstaff, indent, divisions,
            measureP3);
   }

   printIndent(out, --indent, "</beam>\n");
   return out;
}



//////////////////////////////
//
// printChordElement -- print a chord which is a set of two or notes attached
//      to a common stem.
//

ostream& printChordElement(ostream& out, ScoreItem* si, int partstaff,
      int indent, int divisions, SCORE_FLOAT measureP3) {

   int i;
   int notecount = si->getChordNoteCount();

   vectorSIp& chordnotes = *(si->getChordNotes());
   printIndent(out, indent++, "<chord>\n");

   for (i=0; i<notecount; i++) {
      printNoteElement(out, chordnotes[i], partstaff, indent, divisions,
            measureP3);
   }

   printIndent(out, --indent, "</chord>\n");

   return out;
}



//////////////////////////////
//
// printMeasureElementContents -- print a single measure for the given part
//

ostream&  printMeasureElementContents(ostream& out, ScorePageSet& infiles,
      ScorePage& page, SystemMeasure& measureitems,
      int sysindex, int measureindex, vectorVSIp& curclef,
      vectorVSIp& curkey, vectorVSIp& curtime, int mcounter, int indent,
      int divisions, int systemindex, vectorSF& staffsizes,
      int segmentindex, int partcount) {
   int i;
   for (i=partcount-1; i >= 0; i--) {
      printStaffElement(out, infiles, page, measureitems,
            sysindex, measureindex, curclef, curkey, curtime, mcounter, indent,
            divisions, systemindex, staffsizes, segmentindex, partcount, i);
   }
   return out;
}



//////////////////////////////
//
// printRestElement -- Print a rest.
//

ostream& printRestElement(ostream& out, ScoreItem* si, int partstaff,
      int indent, int divisions, SCORE_FLOAT measureP3) {

   printIndent(out, indent++, "<rest");

   string dur = getDurAttributeValue(si);
   if (dur.size() > 0) {
      out << " dur=\"" << dur << "\"";
   }

   stringstream versestream;
   // current rests not allowed to have verses:
   // printVerseElements(versestream, si, indent);
   if (versestream.rdbuf()->in_avail()) {
      out << ">\n";  // close <note> tag.
      out << versestream.str();
      printIndent(out, --indent, "</rest>\n");
   } else {
      out << "/>\n";  // close <note/> tag.
   }
   return out;
}



//////////////////////////////
//
// printNoteElement -- Print a note and any chord-notes attached to it.
//

ostream&  printNoteElement(ostream& out, ScoreItem* si, int partstaff,
      int indent, int divisions, SCORE_FLOAT measureP3) {

   printIndent(out, indent++, "<note\n");
   printIndent(out, indent, "xml:id=\"n");
   out << (uint64_t)si << "\"\n";

   string dur = getDurAttributeValue(si);
   if (dur.size() > 0) {
      printIndent(out, indent, "dur=\"");
      out << dur << "\"\n";
   }

   int  base40 = si->getParameterInt(ns_auto, np_base40Pitch);
   char step   = SU::base40ToLCDiatonicLetter(base40);
   int  alter  = SU::base40ToChromaticAlteration(base40);
   if (si->hasEditorialAccidental()) {
      alter = si->getEditorialAccidental();
   }
   int  octave = base40 / 40;

   // note@pname -- The diatonic pitch letter name of the note.
   printIndent(out, indent, "pname=\"");
   out << step << "\"\n";

   // note@accid -- The chromatic alteration of the note.
   switch (alter) {
      case -2: printIndent(out, indent, "accid=\"ff\"\n"); break;
      case -1: printIndent(out, indent, "accid=\"f\"\n");  break;
      case  1: printIndent(out, indent, "accid=\"s\"\n");  break;
      case  2: printIndent(out, indent, "accid=\"ss\"\n"); break;
   }

   // note@oct -- The octave of the note, middle-C being "4".
   printIndent(out, indent, "oct=\"");
   out << octave << "\"\n";

   // note@stem
   if (si->hasStem()) {
      if (si->stemUp()) {
         printIndent(out, indent, "stem.dir=\"up\"\n");
      } else {
         printIndent(out, indent, "stem.dir=\"down\"\n");
      }
   }


   // <note>/<verse>
   stringstream versestream;
   printVerseElements(versestream, si, indent);
   if (versestream.rdbuf()->in_avail()) {
      printIndent(out, indent-1, ">\n");  // close <note> tag.
      out << versestream.str();
      printIndent(out, --indent, "</note>\n");
   } else {
      printIndent(out, --indent, "/>\n");  // close <note/> tag.
   }

   return out;
}



//////////////////////////////
//
// printVerseElements --
//
//   <verse n="1">
//      <syl wordpos="i" con="d" fontsize="8.136">1.4.7.</syl>
//      <syl wordpos="i" con="d" fontsize="8.136">A</syl>
//   </verse>
//   <verse n="2">
//      <syl con="u" fontsize="8.136">3.</syl>
//      <syl con="u" fontsize="8.136">Je</syl>
//   </verse>
//   <verse n="3">
//      <syl con="u" fontsize="8.136">5.</syl>
//      <syl con="u" fontsize="8.136">De</syl>
//   </verse>
//

ostream& printVerseElements(ostream& out, ScoreItem* si, int indent) {
   if (!si->isPrimaryChordNote()) {
      // Only attach lyrics to primary notes of chords (also single notes
      // are considered primary chord notes)
      return out;
   }
   vectorSIp* testlyrics = si->getLyricsGroup();
   if (testlyrics == NULL) {
      // no lyrics attached to this note.
      return out;
   }

   vectorSIp& lyrics = *testlyrics;
   int number;
   vectorSIp notes;
   vectorVSIp verses(100);
   int count = 0;
   int i, j;
   // first extract the verse lines:
   for (i=0; i<(int)lyrics.size(); i++) {
      if (lyrics[i]->isNoteItem()) {
         notes.push_back(lyrics[i]);
      }
      if (!lyrics[i]->isTextItem()) {
         continue;
      }
      number = lyrics[i]->getParameterInt(ns_auto, np_verseLine);
      if ((number > 0) && (number < (int)verses.size())) {
         verses[number].push_back(lyrics[i]);
         count++;
      }
   }

   if (count == 0) {
      // nothing to do
      return out;
   }

   int before;
   int after;

   // SCORE_FLOAT p4, defaulty;
   SCORE_FLOAT staffscale = si->getStaffScale();

   for (i=1; i<(int)verses.size(); i++) {
      if (verses[i].size() == 0) {
         continue;
      }
      printIndent(out, indent++, "<verse");
      out << " n=\"" << i << "\"";

      if (verses[i].size() > 0) {
         //p4 = verses[i][0]->getVPos();
         //defaulty = (p4 - 11.0) * staffscale;
         //if (defaulty != 0.0) {
         //   out << " default-y=\"" << defaulty << "\"";
         //}
      }

      out << ">\n";

      for (j=0; j<(int)verses[i].size(); j++) {
         // <syl>
         before = verses[i][j]->hasHyphenBefore();
         after  = verses[i][j]->hasHyphenAfter();
         printIndent(out, indent, "<syl");

         // syl@wordpos -- position of the syllable within a word:
         //    i = initial (start of word)
         //    m = middle (middle of word)
         //    t = terminal (end of word)
         //    @wordpos is not given if the syllable is a complete word.
         if (j<(int)verses[i].size()-1) {
            // single syllable.  Caused by this being the start of a multi
            // syllable verse attached to a single note.
         } else {
            if (before && after) {
               out << " wordpos=\"m\""; // middle of word
            } else if (before) {
               out << " wordpos=\"t\""; // end of word (terminal)
            } else if (after) {
               out << " wordpos=\"i\""; // beginning of word (initial)
            } else {
               // a single-word syllable.
            }
         }

         // syl@con -- The continue attribute for a syllable.  What comes
         // after it to link to the next syllable in the verse:
         //    s = space is the connector to next syllable
         //    d = dash is the connector to next syllable
         //    u = underscore is the connector to next syllable
         //    t = tilde is the connector to next syllable (elision)
         if (j<(int)verses[i].size()-1) {
            // connect to next note with a space:
            out << " con=\"s\"";
         } else if (verses[i][j]->hasWordExtension()) {
            out << " con=\"u\"";
         } else if (after) {
            out << " con=\"d\"";
         }

         // syl@fontsize -- print the font size (in points):
         out << " font-size=\""
             << verses[i][j]->getFontSizeInPoints(staffscale) << "\"";
         out << ">";

         // print the child text of <syl>:
         out << verses[i][j]->getTextNoFontXmlEscapedUTF8();
         out << "</syl>\n";
      }

      printIndent(out, --indent, "</verse>\n");
   }

   return out;
}



//////////////////////////////
//
// getDurAttributeValue --
//

string getDurAttributeValue(ScoreItem* si) {
   double duration = si->getDuration();
   if (duration <= 0) {
      // deal with grace note notehead types later...
      return "";
   }

   double exp = log(duration)/log(2.0);
   int type = floor(exp+0.0001);
   if (rhythmicScalingQ) {
      type += Scaling;
   }

   switch (type) {
      case 5:   return "maxima";  // doesnot exist in MEI CMN
      case 4:   return "long";
      case 3:   return "breve";
      case 2:   return "1";
      case 1:   return "2";
      case 0:   return "4";
      case -1:  return "8";
      case -2:  return "16";
      case -3:  return "32";
      case -4:  return "64";
      case -5:  return "128";
      case -6:  return "512";
      case -7:  return "1024";
      case -8:  return "2048";
   }

   // unknown type
   return "";
}



