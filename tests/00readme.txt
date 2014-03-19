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
	Identify the pitch of each note (not tracking ties yet).

systeminfo.cpp
	Print information about each system on the page.

systemrhythm.cpp
	Analyze the rhythm of notes/rests on each system.

systemsort.cpp
	Sort data on page by system, and in particular by time order.

chordtest.cpp
	Do a chord analysis of notes, which will group notes into
	chords if: (1) a note does not posses a stem, and (2) there is
	another note at the same (or nearly same) P3 (horizontal position)
	which has a stem which passes by the stemless note.

beamtest.cpp
	Link notes (and rests) and beams.

chromate.cpp
	Give each pitch class a different color.

courtesy.cpp
	Mark or manipulate courtesy accidentals (accidentals which are
	not strictly necessary, but remind performers of the correct
	chromatic alteration.

extractsystem.cpp
	Extract a particular system from a page of music.


