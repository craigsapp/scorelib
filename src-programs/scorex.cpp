//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Apr 12 20:10:32 PDT 2014
// Last Modified: Sun Apr 26 20:41:54 PDT 2015
// Filename:      scorex.cpp
// URL: 	  https://github.com/craigsapp/scorelib/blob/master/src-programs/scorex.cpp
// Documentation: http://scorelib.sapp.org/program/scorex
// Syntax:        C++ 11
//
// Description:   Extract pages, overlays or systems from multi-page input.
//

#include "scorelib.h"
#include "stdlib.h"

using namespace std;

void   processData          (ScorePage& infile, Options& opts);
void   extractBinaryPages   (ScorePageSet& infiles, Options& opts);
void   extractAsciiPages    (ScorePageSet& infiles, Options& opts);
void   extractSystems       (ScorePageSet& infiles, const string& filebase);
void   printSystem          (ScorePage& page, int sysnum, int syscount, 
                             int psysindex, int psyscount, 
                             const string& infilebase, const string& filebase);
void   extractMeasures      (ScorePageSet& infiles, const string& measures);
void   extractSingleMeasure (ScorePageSet& infiles, int page, int system, 
                             int measure);
void   printBarlineEdge     (ScoreItem* item, double left, double right);

int    autoQ = 1;

///////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
   Options opts;
   opts.define("A|no-auto=b", "don't print auto namespace parameters");
   opts.define("c|count=b", "count the number of input pages/segments");
   opts.define("pc|page-count=b", "count the number of input pages");
   opts.define("sc|segment-count=b", "count the number of input segments");
   opts.define("s|systems|extract-systems=b", 
         "extract systems to separate files");
   opts.define("i|info=b", "count input pages, overlays, and systems");
   opts.define("p|page=i:0", "Extract given page index (offset from 1)");
   opts.define("f|filebase=s:", "Optional filename base for data extraction");
   opts.define("m|measure|measures=s", "Extract measure or range of measures");
   opts.define("mus=b", "Extract pages into binary .MUS files");
   opts.define("pag=b", "Extract pages into binary .PAG files");
   opts.define("pmx=b", "Extract pages into ASCII .PMX files");
   opts.define("txt=b", "Extract pages into ASCII .TXT files");
   opts.process(argc, argv);

   autoQ = !opts.getBoolean("no-auto");

   ScorePageSet infiles(opts);

   if (opts.getBoolean("count")) {
      cout << "Pages:\t\t" << infiles.getPageCount() << endl;
      infiles.analyzeSegmentsByIndent();
      cout << "Segments:\t" << infiles.getSegmentCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("mus") || opts.getBoolean("pag")) {
      extractBinaryPages(infiles, opts);
      exit(0);
   }

   if (opts.getBoolean("pmx") || opts.getBoolean("txt")) {
      extractAsciiPages(infiles, opts);
      exit(0);
   }

   if (opts.getBoolean("page-count")) {
      cout << infiles.getPageCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("segment-count")) {
      infiles.analyzeSegmentsByIndent();
      cout << infiles.getSegmentCount() << endl;
      exit(0);
   }

   if (opts.getBoolean("extract-systems")) {
      extractSystems(infiles, opts.getString("filebase"));
      exit(0);
   }

   if (opts.getBoolean("measures")) {
      extractMeasures(infiles, opts.getString("measures"));
      exit(0);
   }

   if (opts.getBoolean("page")) {
      cout << infiles[opts.getInteger("page")-1];
      exit(0);
   }



   return 0;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// extractMeasures -- only extract one at a time for now.
//

void extractMeasures(ScorePageSet& infiles, const string& measures) {
   int measure = 0;
   sscanf(measures.data(), "%d", &measure);
   measure--;
   int msum = 0;
   int sysm;
   int mcount;
   int i, j;
   for (i=0; i<infiles.getPageCount(); i++) {
      for (j=0; j<infiles.getPage(i)->getSystemCount(); j++) {
         mcount = infiles.getPage(i)->getSystemMeasureCount(j) - 1;
         if (measure < msum + mcount) {
            sysm = measure - msum;
            extractSingleMeasure(infiles, i, j, sysm);
            return;
         }
         msum += mcount;
      }
   }
}



//////////////////////////////
//
// extractSingleMeasure --
//

void extractSingleMeasure(ScorePageSet& infiles, int page, int system, 
      int measure) {

   vectorSMp& sysm =  infiles.getPage(page)->getSystemMeasures(system);

   double leftpos   = sysm[measure]->getHPosLeft();
   double rightpos  = sysm[measure]->getHPosRight();

   ScoreItem sitem;

   vectorSIp& mitems = sysm[measure]->getItems();

   // First print the staff lines:
   int i;
   int staffcount = 0;
   for (i=0; i<(int)mitems.size(); i++) {
      if (!mitems[i]->isStaffItem()) {
         continue;
      }
      sitem = *mitems[i];
      sitem.setP3(leftpos);
      sitem.setP6(rightpos);
      cout << sitem;
      staffcount++;
   }

   if (staffcount == 0) {
      // can't find staves in current measure, then must be
      // found in an earlier measure on the system, probably the first
      // one, so just look there (could generalize later, but that
      // should be good enought for all non-degenerate cases).
      vectorSIp& zitems = sysm[0]->getItems();
      for (i=0; i<(int)zitems.size(); i++) {
         if (!zitems[i]->isStaffItem()) {
            continue;
         }
         sitem = *zitems[i];
         sitem.setP3(leftpos);
         sitem.setP6(rightpos);
         cout << sitem;
         staffcount++;
      }
   }

   // Print everything except staves and slurs.
   // Will need to process other items in a similar manner.
   // Unknown how to deal with tuplets, etc which are subitem of slurs.
   for (i=0; i<(int)mitems.size(); i++) {
      if (mitems[i]->isStaffItem()) {
         continue;
      } else if (mitems[i]->isSlurItem()) {
         continue;
      } else if (mitems[i]->isBarlineItem()) {
         printBarlineEdge(mitems[i], leftpos, rightpos);
         continue;
      }
      cout << mitems[i];
   }

   // Print slurs.  These have to be process throughout the entire 
   // system line since they could start, stop, or pass through the
   // measures being extracted.  Change the P3/P6 values to the
   // visual endpoints of the slurs (probably more accurate).
   // P14 of slurs is used to shorten the left side of a slur.
   // P15 of slurs is used to shorten the left side of a slur.
   // Also assuming that slurs are not placed backwards such that P6<P3.
   double visleft;
   double visright;
   double p14;
   double p15;
   double vislen; // visual length of slur
   double slen;   // full length of slur
   vectorSIp& sysitems = infiles.getPage(page)->getSystemItems(system);
   for (i=0; i<(int)sysitems.size(); i++) {
      if (!sysitems[i]->isSlurItem()) {
         continue;
      }
      visleft  = sysitems[i]->getVisualLeftPosition();
      visright = sysitems[i]->getVisualRightPosition();
      if ((visleft >= leftpos) && (visright <= rightpos)) {
         cout << sysitems[i];
      } else if ((visleft < leftpos) && (visright < leftpos)) {
         // don't print: before measure
      } else if ((visleft > rightpos) && (visright > rightpos)) {
         // don't print: after measure
      } else if ((visleft < leftpos) && (visright <= rightpos)) {
         // starts before measure; ends in measure
         vislen = visright - leftpos;
         slen = visright - visleft;
         p14 = 1.0 - vislen / slen;
         sitem = *sysitems[i];
         sitem.setP14(p14);
         cout << sitem;
      } else if ((visleft >= leftpos) && (visright > rightpos)) {
         // starts in measure; ends after measure
         vislen = rightpos - visleft;
         slen = visright - visleft;
         p15 = vislen / slen;
         sitem = *sysitems[i];
         sitem.setP15(p15);
         cout << sitem;
      } else if ((visleft < leftpos) && (visright > rightpos)) {
         // starts before measure; ends after measure
         sitem = *sysitems[i];
         vislen = visright - leftpos;
         slen   = visright - visleft;
         p14    = 1.0 - vislen / slen;
         sitem.setP14(p14);
         vislen = rightpos - visleft;
         slen = visright - visleft;
         p15   = vislen / slen;
         sitem.setP15(p15);
         cout << sitem;
      }
   }

   // print the ending bar, which is usually considered the beginning
   // of the next measure and not included in the current measure
   // (check what happens at the end of systems).  Also need to 
   // adjust barlines that have repeats on both sides to repeats
   // only on one side (within the measure);

   vectorSIp& endbars = sysm[measure]->getEndBarlines();
   for (i=0; i<(int)endbars.size(); i++) {
      printBarlineEdge(endbars[i], leftpos, rightpos);
      continue;
   }
}


//////////////////////////////
//
// printBarlineEdge --
//

void printBarlineEdge(ScoreItem* item, double left, double right) {
   ScoreItem newitem;
   double p3 = item->getP3();
   switch (item->getP5Int()) {
      case 3:  // left-pointing repeat marks
         if (p3 == left) {
            newitem = *item;
            newitem.setP5(0);
            cout << newitem;
         } else {
            cout << item;
         }
         break;
      case 4:  // right-pointing repeat marks
         if (p3 == right) {
            newitem = *item;
            newitem.setP5(0);
            cout << newitem;
         } else {
            cout << item;
         }
         break;
      case 5:  // double repeat marks, three lines
      case 6:  // double repeat marks, two lines
         if ((p3 == left) || (p3 == right)) {
            newitem = *item;
            newitem.setP5(0);
            cout << newitem;
         } else {
            cout << item;
         }
         break;
      default:
         cout << item;
   }
}



//////////////////////////////
//
// extractSystems --
//

void extractSystems(ScorePageSet& infiles, const string& filebase) {
   int syscount = 0;
   int i, j;
   for (i=0; i<infiles.getPageCount(); i++) {
      infiles[i][0].analyzeSystems();
      syscount += infiles[i][0].getSystemCount();
   }

   if (syscount <= 0) {
      // nothing to do.
      return;
   }

   string infilebase;
   int pagesys;
   int counter = 0;
   for (i=0; i<infiles.getPageCount(); i++) {
      infilebase = infiles.getPage(i)->getFilenameBase();
      pagesys = infiles.getPage(i)->getSystemCount();
      for (j=0; j<pagesys; j++) {
         printSystem(*infiles.getPage(i), ++counter, syscount, j, pagesys, 
               infilebase, filebase);
      }
   }
}



//////////////////////////////
//
// printSystem --
//

void printSystem(ScorePage& page, int sysnum, int syscount, int psysindex, 
	int psyscount, const string& infilebase, const string& filebase) {
   string myfile;
   int digits;
   int xdigits;
   int i;

   cout << "RS" << endl;
   if (filebase.size() > 0) {
      myfile = filebase;
      digits = log10(syscount);
      if (digits == 0) { digits = 1; }
      xdigits = log10(sysnum);
      if (xdigits == 0) { xdigits = 1; }
      for (i=0; i<digits-xdigits; i++) {
         myfile += "0";
      }
      myfile += to_string(sysnum);
   } else {
      myfile = infilebase;
      if (psyscount >= 10) {
         if (psysindex < 10) {
            myfile += "0";
         }
      }
      myfile += to_string(psysindex);
   }
   cout << "SA " << myfile << endl;
   cout << endl;

   vectorSIp& sysitems = page.getSystemItems(psysindex);
   // the staves could be shifed down to 1 possibly here...
   if (autoQ) {
      cout << sysitems;
   } else {
      printNoAuto(cout, sysitems);
   }

   cout << endl;
   cout << "SM" << endl;
}



//////////////////////////////
//
// extractBinaryPages -- extract file from set and write as binary .mus
//   files.  The filenames muse be present in the data (excluding optional
//   filename extensions).
//

void extractBinaryPages(ScorePageSet& infiles, Options& opts) {
   int pages = infiles.getPageCount();
   ScorePage* infile;
   string filename;
   for (int i=0; i<pages; i++) {
      infile = infiles.getPage(i);
      filename = infile->getFilename();
      if (filename.size() == 0) {
         cerr << "Error: Filename is empty for page " << i+1 << endl;
         exit(1);
      }
      // filename should not have an extension, maybe check, but for now
      // add ".mus" or ".pag" always:
      if (opts.getBoolean("mus")) {
         filename += ".mus";
      } else if (opts.getBoolean("pag")) {
         filename += ".pag";
      }
      infile->writeBinary(filename);
   }
}



//////////////////////////////
//
// extractAsciiPages -- extract file from set and write as ASCII .pmx
//   files.  The filenames muse be present in the data (excluding optional
//   filename extensions).
//

void extractAsciiPages(ScorePageSet& infiles, Options& opts) {
   int pages = infiles.getPageCount();
   ScorePage* infile;
   string filename;
   ofstream outfile;

   for (int i=0; i<pages; i++) {
      infile = infiles.getPage(i);
      filename = infile->getFilename();
      if (filename.size() == 0) {
         cerr << "Error: Filename is empty for page " << i+1 << endl;
         exit(1);
      }
      // filename should not have an extension, maybe check, but for now
      // add ".pmx" or ".txt" always:
      if (opts.getBoolean("pmx")) {
         filename += ".pmx";
      } else if (opts.getBoolean("txt")) {
         filename += ".txt";
      }
      outfile.open(filename.data());
      infile->printPmx(outfile);
      outfile.close();
   }
}



