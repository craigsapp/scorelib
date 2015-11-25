scorelib
========

SCORE data file processing library


Download
========
  git clone https://github.com/craigsapp/scorelib


Compiling
=========

The scorelib library is implemented using the C++11 standard.  Most code should compile with gcc 4.7, except for the use of regex which is fully implemented starting in gcc 4.9 (April 2014).  The makefiles for compiling the library and programs are set up to use clang 3.3 which already has a full implementation of the C++11 standard.  Microsoft Developer Studio 2013 may be able to compile the code, but this has not yet been tested.

The library has one compile-time define (SCOREITEMEDIT) which can be used to turn on the edit-history component of the ScoreItemEdit class.  This allows the ScoreItem objects to keep track of changes for undo/redo capabilities.  See the library makefile for its use.  


Documentation
=============

See the documentation website http://scorelib.sapp.org



