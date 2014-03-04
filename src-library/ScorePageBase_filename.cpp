//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Mar  3 16:13:33 PST 2014
// Last Modified: Mon Mar  3 16:13:36 PST 2014
// Filename:      ScorePageBase_filename.cpp
// URL:           https://github.com/craigsapp/scorelib/master/src-library/ScorePageBase_filename.cpp
// Syntax:        C++11
//
// Description:   Filename processing functions for ScorePageBase.
//

#include "ScorePageBase.h"
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////
//
// Filename management functions for ScorePageBase.
//

//////////////////////////////
//
// ScorePageBase::getFilename -- Returns the filename base and the extension.
//

string& ScorePageBase::getFilename(string& output) { 
   output.clear();
   output = filename_base;
   if (filename_extension.size() > 0) {
      output += ".";
      output += filename_extension;
   }
   return output;
}



//////////////////////////////
//
// ScorePageBase::getFilenameWithPath -- Returns the filename 
//     base and the extension. Prefixed by the directory name (if it
//     is not empty).
//

string& ScorePageBase::getFilenameWithPath(string& output) { 
   output.clear();
   if (filename_path.size() > 0) {
      output = filename_path;
      if (output.back() != '/') { 
         output += "/";
      }
   }
   output += filename_base;
   if (filename_extension.size() > 0) {
      output += ".";
      output += filename_extension;
   }
   return output; 
}



//////////////////////////////
//
// ScorePageBase::getFilenameBase --
//

const string& ScorePageBase::getFilenameBase(void) { 
   return filename_base;
}



//////////////////////////////
//
// ScorePageBase::getFilenameExtension --
//

const string& ScorePageBase::getFilenameExtension(void) { 
   return filename_extension;
}



//////////////////////////////
//
// ScorePageBase::getFilenamePath --
//

const string& ScorePageBase::getFilenamePath(void) { 
   return filename_path;
}



//////////////////////////////
//
// ScorePageBase::setFilename -- The filename, in the form:
//     path/base.ext
//

void ScorePageBase::setFilename(const string& filename) { 
   auto location = filename.rfind("/");

   // first split the path from the filename.
   string path;
   string rest;
   if (location != string::npos) {
      path = filename.substr(0, location);
      if (path.size() == 0) {
         path += '/';
      }
      string rest = filename.substr(location+1);
   } else {
      rest = filename;
   }

   setFilenamePath(path);

   // split file extension from filename
   location = rest.rfind("/");
   string ext;
   string base;
   if (location != string::npos) {
      ext = rest.substr(location+1);
      if (ext.size() == 0) {
         base = rest;
      }
   } else {
      base = rest;
   }
   setFilenameBase(base);
   setFilenameExtension(ext);
}



//////////////////////////////
//
// ScorePageBase::setFilenameBase --
//

const string& ScorePageBase::setFilenameBase(const string& filebase) { 
   filename_base = filebase;
   return removeCharacter(filename_extension, '/');
}



//////////////////////////////
//
// ScorePageBase::setFilenameExtension --
//

const string& ScorePageBase::setFilenameExtension(const string& fileext) { 
   if ((fileext.size() > 0) && (fileext[0] == '.')) {
      filename_extension = fileext.substr(1);
   } else {
      filename_extension = fileext;
   }
   return removeCharacter(filename_extension, '/');
}



//////////////////////////////
//
// ScorePageBase::setFilenamePath -- Set the path for the file.  Any
//    trailing slash(es) will be removed from the path name.  But if the
//    path name is only "/", then do not remove the slash.
//

const string& ScorePageBase::setFilenamePath(const string& path) { 
   filename_path = path;
   while (filename_path.size() > 1 && (filename_path.back() == '/')) {
      filename_path.resize(filename_path.size()-1);
   }
   return filename_path;
}



//////////////////////////////
//
// ScorePageBase::removeCharacter -- remove all characters of a specific type
//    in the input string.
//

string& ScorePageBase::removeCharacter(string& out, char character) {
   out.erase(remove_if(out.begin(), out.end(), 
         [&character](char ch){ return ch==character; }));
   return out;
}



