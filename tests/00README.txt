Test programs for scorelib.  Type "make all" to compile all programs
(after compiling the scorelib library).
===========================================================================

analyzestaves.cpp
	Test staff-analysis functions of ScorePage class.

extractstaff.cpp:
	Extract items from a particular staff.  The program also can report
	the highest staff on the page (using --max option), and can reposition
	extracted items to a different staff number (with -r option).

itemtest.cpp
	Test of manual creation of SCORE items, both fixed parameters
	and named parameters.  Output is printed as PMX data and XML
	data (without wrapping structure).  This program also checks
	storage of parameter change history.

readtest.cpp
	Test conversion of a PMX/binary file into PMX data.

scoreitems2xml.cpp
	This program converts SCORE PMX (ASCII) or MUS (binary) files
	into a list of SCORE items formatted as XML.  

staffdurations.cpp
	This program prints the duration (in quarter notes)
	for each staff position on the page up to the highest
	number staff item on the page.

staffoffsets.cpp
	Calculate the starting time of each item since the start of the staff.

stemtest.cpp
	Test flipping of stems (and beams).

analyzepitch.cpp

systeminfo.cpp

systemrhythm.cpp

systemsort.cpp

chordtest.cpp


