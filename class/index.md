---
layout: default
breadcrumbs: [['/doc','documentation']]
pagetitle:  Scorelib class overview 
---


<img style="float:right; margin-left:-20px; margin-bottom:20px; width:500px;" src="/img/classoverview.png">

Scorelib consists of two main sets of classes, one to manage
individual SCORE items, and another to manage collections of items.
The diagram on the left shows the relations between these classes:
one set for dealing with a single SCORE item (colored light blue),
and the other set that manages multiple items (colored
purple).  The three main classes to use directly are ScoreItem,
ScorePage, and ScorePageSet, while the others are used to deal with
lower-level maintence and abstraction.  The [ScorePageSet](ScorePageSet)
class is used to organize multiple pages; the [ScorePage](ScorePage)
class is used to manage a single page; and [ScoreItem](ScoreItem)
manages a single SCORE item.

For individual items, the primary class is ScoreItem
which inherits either ScoreItemEdit or
ScoreItemBase, depending on whether or not you
want to keep track of the edit history of the parameters.  ScoreItemBase
and ScoreItemEdit are not typically used directly, and instead hold
low-level maintenance of the parameters.

The ScoreItemEdit class is used to keep track of changes to the
parameters to facilitate undo/redo for interactive applications.
The ScoreItem class may inherit from either ScoreItemEdit or directly
from ScoreItemBase.  This is controlled at compile time by defining
the SCOREITEMEDIT preprocessor variable.  The Scorelib makefile
that creates the compiles code library compiles all code twice:
once with ScoreItem inheriting from ScoreItemEdit and a second time
with ScoreItem inheriting from ScoreItemBase. The library files
lib/scoreeditlib.a and lib/scorelib.a are created respectively for
the two inheritance cases, and you can link program to the one
containing the desired behavior.  Light-weight and batch-processing
programs should link to scorelib.a, and interactive programs
should link to scorelibedit.a.


ScorePageBase 
==============

The [ScorePageBase](ScorePageBase) class contains numerous helper classes
used to coordinate relationships between individual ScoreItems contained 
on the page.  These classes are not accessed directly, but are used to
facilite efficient higher-level manipulation of the items.  The main
helper classes for ScorePageBase are:

<ul style="margin-left:40px;">
<li> <a href="DatabaseChord">DatabaseChord</a>: keeps track of all notes sharing a common stem.</li>
<li> <a href="DatabaseBeam">DatabaseBeam</a>: keeps track of beams and notes attached to the beam.</li>
<li> <a href="DatabaseP3">DatabaseP3</a>: keeps track of items by horizontal position on each system of a page.</li>
<li> <a href="InfoPrint">InfoPrint</a>: stores page printing parameters and also calculates distances on the page in physical units and pixels.</li>
<li> <a href="InfoStaff">InfoStaff</a>: used information about staves on the page, such as how they are grouped into systems.</li>
<li> <a href="InfoAnalysis">InfoAnalysis</a>: is used to keep track of analysis dependencies.  For example, analysis stored in <a href="DatabaseP3">DatabaseP3 classes</a> first require systems to be identified.  This information is used to automatically calculate information needed for higher-level groupings.</li>
</ul>

<center>
<img style="margin-top:20px; width:700px;" src="/img/scorepagebase-classes.png">
</center>


ScorePageSet components
=======================


The [ScorePageSet](ScorePageSet) class consists of two organizing
hierarchies.  The primary ones is by page, and a secondary one is
by segment.  Segments consist of groups of systems which may span
multiple pages, and may only include a subset of systems on a
particular page.  Typically segments are used to define movements
in a work, and they contain a fixed number of instrumental parts.

<img style="float: right; width:500px;" src="/img/scorepageset-overlay.png">

A schematic of the page organization within ScorePageSet is shown
on the right.  Pages are two-dimensional arrays of [ScorePage](ScorePage)
objects.  Each printed page is held in a
[ScorePageOverlay](ScorePageOverlay) object which consists of a
list of [ScorePage](ScorePage) objects.  Each ScorePage in a
ScorePageOverlay object is intended to be displayed on the same
page.  Use of the overlay system is dependend on the implemention.
It can be used to store analytic markup, variants, ossia, older
versions of the page, etc.

The dual hierarchy of the ScorePageSet can be seen in the following
figure.  A musical score will be stored as printable pages within
an array of ScorePageOverlay objects.  A secondary array of
[ScoreSegment](ScoreSegment) objects marks the begin/end of musical
segments on the pages.  Segments may represent full works, movements
of works, or subsections of movements.  In the following figure,
the page segmentations are from page 0 to the middle of page 2 for
the first segment, from the middle of page 2 to the end of page 3
for the second segment, and page 4 for the third segment.

ScoreSegment class
==================

<img style="float:right; width:400px;" src="/img/scorepageset-overlaysegment.png">

The [ScoreSegment](ScoreSegment) class keeps track of parts and how
they are assigned to staves on ScorePages.  A
ScoreSegment object contains a list of
[SegmentPart](SegmentPart) objects.  SegmentParts
each contain a list of [AddressSystem](AddressSystem) which locate
the staves for each part in the score.  The AddressSystem
two dimensional list of system addresses.  When a part contains
more than one staff, additionay system addresses will be given on
a row in the AddressSystem list.

The ScorePageSet class contains a list of ScoreSegments that in turn consists
of a list of SegmentParts, which is a list of the instrumental parts in the
segment. Each SegmentPart contains a list of AddressSystem objects that keep
track of the staff assignment for each system for the instrument.  The schematic
below shows the relationship between thses classes:

<center>
<img style="width:600px;" src="/img/scorepageset-segment.png">
</center>

Class index
===========

<ul style="margin-left:50px;">
<li> <a href="AddressSystem">AddressSystem</a> </li>
<li> <a href="DatabaseBeam">DatabaseBeam</a> </li>
<li> <a href="DatabaseChord">DatabaseChord</a> </li>
<li> <a href="DatabaseP3">DatabaseP3</a> </li>
<li> <a href="InfoAnalysis">InfoAnalysis</a> </li>
<li> <a href="InfoPrint">InfoPrint</a> </li>
<li> <a href="InfoStaff">InfoStaff</a> </li>
<li> <a href="ScoreItem">ScoreItem</a> </li>
<li> <a href="ScoreItemBase">ScoreItemBase</a> </li>
<li> <a href="ScoreItemEdit">ScoreItemEdit</a> </li>
<li> <a href="ScorePage">ScorePage</a> </li>
<li> <a href="ScorePageBase">ScorePageBase</a> </li>
<li> <a href="ScorePageOverlay">ScorePageOverlay</a> </li>
<li> <a href="ScorePageSet">ScorePageSet</a> </li>
<li> <a href="ScoreSegment">ScoreSegment</a> </li>
<li> <a href="ScoreUtility">ScoreUtility</a> </li>
<li> <a href="SegmentPart">SegmentPart</a> </li>
</ul>




