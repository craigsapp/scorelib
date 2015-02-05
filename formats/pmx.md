---
layout: default
persistent: formats/pmx
breadcrumbs: [['/formats','formats']]
javascript: ['js/turnonoff-sticky.html']
pagetitle:  SCORE PMX data format
grouptitle: ['/formats', 'Data formats']
group: [ 
   ['/formats/binary/', binary], 
   ['/formats/pmx/', PMX], 
   ['/formats/ppmx/', PPMX] 
]
---

"PMX" stands for "Parameter MatriX".  PMX files are ASCII versions
of binary SCORE data. Each line in a PMX file represents a SCORE
item, such as note, rest, barline, clef, key signature, etc. Each
line contains a list of floating-point values describing an item
item.  Text items are an exception: their lines start with "t"
rather than "16", and the text parameter immediate follows the
numeric parameter line.  Code 15 items also have a second line of
data which contains the name of the Encapsulated PostScript file
to include. PMX files can be read into the SCORE editor with the
RE command. The RE command also reads in macro files, so PMX data
can be mixed with SCORE commands.

The following notation example shows the corresponding PMX data underneath
it.  Move your mouse over the notation or the PMX data to highlight the
corresponding notation/PMX values.  For this example, the PMX data is time
sorted (sorted left to right spatially, interleaving all staves).

Ravel String Quartet in F major, IV, mm. 105&ndash;108.
{% include /examples/ravel-quartet/ravel-markup-svg.html %}

<p>
<div id="pmxlist" style="width:720px; height: 400px; white-space: nowrap; overflow-y:scroll; overflow-x: hidden;">
<div style="background-color:#f3f3f3; line-height: 1.10; padding: 5px; font-family:'Courier New',Courier,monospace; font-weight:600;">
{% include examples/ravel-quartet/ravel-markup.html %}
</div>
</div>




