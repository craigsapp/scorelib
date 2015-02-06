---
layout: default
persistent: class
pagetitle:  Scorelib class overview 
---


The following diagram shows the primary class organization for
scorelib.  The classes can be divided into two groups: one for
dealing with a single SCORE item, and another group which manage
multiple items.  For individual items, the primary class is
[ScoreItem](ScoreItem) which inherits either [ScoreItemEdit](ScoreItemEdit)
or [ScoreItemBase](ScoreItemBase), depending on whether or not you
want to keep track of the edit history of the parameters.

<center>
<img style="width:400px;" src="/img/classoverview.png">
</center>


The [ScorePageBase](ScorePageBase) contains several helper classes
used to organize and process ScoreItems contained on the page.

* [DatabaseChord](DatabaseChord) is used to store an analysis of chords on the page.
* [DatabaseBeam](DatabaseBeam) is used to store an analysis of beams on the page.
* [DatabaseP3](DatabaseP3) is used to store an organization of [ScoreItems](ScoreItem) first by system, then by horizontal position.
* [PrintInfo](PrintInfo) is used to store page printing parameters.  The class is also used to calculate distances on the page in physical units and pixels.
* [StaffInfo](StaffInfo) is used to store an analysis of staves on the page, in particular how they are grouped into systems.
* [AnalysisInfo](AnalysisInfo) is used to keep track of analysis dependencies.  For example, analysis stored in [DatabaseP3 classes](DatabaseP3) first required the system analysis to be done (whose results are stored in [StaffInfo](StaffInfo).

<center>
<img style="width:500px;" src="/img/scorepagebase-classes.png">
</center>

ScorePageSet components
=======================

The [ScorePageSet](ScorePageSet) class consists of two organizing hierarchies.  The primary ones is by page, and a secondary one is by segment.  

Below is a schematic of the page organization within ScorePageSet.
Pages are two-dimensional arrays of [ScorePage](ScorePage) objects.
Each printed page is held in a [ScorePageOverlay](ScorePageOverlay)
object which consists of a list of [ScorePage](ScorePage) objects.
Each ScorePage in a ScorePageOverlay object is intended to be
displayed on the same page.  Use of the overlay system is dependend
on the implemention.  It can be used to store analytic markup,
variants, ossia, older versions of the page, etc.

<center>
<img style="width:500px;" src="/img/scorepageset-overlay.png">
</center>


The dual hierarchy of the ScorePageSet can be seen in the following
figure.  A musical score will be stored as printable pages within
an array of ScorePageOverlay objects.  A secondary array of
[ScoreSegment](ScoreSegment) objects marks the begin/end of musical
segments on the pages.  Segments may represent full works, movements
of works, or subsections of movements.  In the following figure,
the page segmentations are from page 0 to the middle of page 2 for
the first segment, from the middle of page 2 to the end of page 3
for the second segment, and page 4 for the third segment.

<center>
<img style="width:500px;" src="/img/scorepageset-overlaysegment.png">
</center>

The [ScoreSegment](ScoreSegment) class keeps track of parts and how
they are assigned to staves on [ScorePages](ScorePage).  A
[ScoreSegment](ScoreSegment) object contains a list of
[SegmentPart](SegmentPart) objects.  [SegmentParts](SegmentPart)
each contain a list of [SystemAddresses](SystemAddress) which locate
the staves for each part in the score.  The [SystemAddress](SystemAddress)
two dimensional list of system addresses.  When a part contains
more than one staff, additionay system addresses will be given on
a row in the [SystemAddress](SystemAddress) list.

<center>
<img style="width:500px;" src="/img/scorepageset-segment.png">
</center>

