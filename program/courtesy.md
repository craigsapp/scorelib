
The *courtesy* program can be used to identify courtesy/cautionary accidentals. The program can highlight notes with courtesy accidentals, hide these accidentals, or place the accidentals in parentheses.  Strictly speaking, courtesy accidentals do not need to be shown since the chromatic alteration of notes would not change if they are removed; however, such accidentals are used to clarify pitch spellings so that a musician sight-reading the music will less likely make a chromatic mistake.


Options
=======

|  option   |   argument   |   description
|-----------|--------------|----------------
|    -m     | *none*       | Mark courtesy accidentals with @courtesy named parameter.
|    -p     | *none*       | Place courtesy accidentals in parentheses.
|    -P     | *none*       | Remove parentheses around courtesy accidentals.
|    -r     | *none*       | Remove courtesy accidentals from notes.
|    -R     | *none*       | Redisplay courtesy accidentals (requires previous use of -k option).
|    -k     | *none*       | Keep courtesy accidental analyses on notes (use in particular with -r option to recover accidentals later).
|   -e      | *none*       | Convert cautionary accidentals to editorial accidentals.
|   -a      | *none*       | Include pitch-analysis parameters in output data.
|   --lj    | *none*       | Add LJ commands to end of output data for systems which have been altered.


Description
===========

The following music contains several unmarked courtesy accidentals which the *courtesy* program can identify and process.

<table><tr><td>[[https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.png]]</td></tr><tr><td valign=right><a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input data</a></td></tr></table>

The default behavior of the *courtesy* program when no options are given is to highlight notes with courtesy accidentals:

<table><tr><td>[[https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-default.png]]</td></tr><tr><td valign="right">courtesy <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input-data</a> &gt; <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-default.pmx>output-data</a></td></tr></table>

There are four courtesy accidentals identified for this passage of music:

1. G&#9838; at the start of the second measure.  This is used to warn of the cancellation of G&#9839; in the previous measure, caused by the barline.

2. C&#9838; in the middle of the second measure is used to clarify that the sharp on the previous C3 in the measure does not apply to C4 since they are in different octaves.

3. C&#9839; at the end the second measure is used to clarify that the sharp on the previous C4 in the measure does not apply to this C3 since they are in different octaves (the chromatic alteration of this note comes from the first C&#9839;in the measure).

4. The F&#9839; in the third measure is given for clarity that the natural on F4 in the top staff in third and fourth measures no longer applies.


### Marking courtesy accidentals ###

The <tt>-m</tt> opion can be used to mark courtesy accidentals in the data. 

<table><tr><td>
courtesy -m <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input-data</a> 
</td></tr>
<tr><td>
8 1 0 0 0.75 200<br/>
14 1 0 2 8<br/>
14 1 0 2<br/>
3 1 1.5 0 1<br/>
17 1 7.459 0 1 1<br/>
1 1 13.357 8 20 0 1<br/>
1 1 13.384 10 12 0 0.5 -1 1<br/>
1 1 19.17 11 10 0 1 -1.5<br/>
1 1 24.9 1 20 0 1 -1.5<br/>
1 1 30.506 10 10 0 0.5 -1 1<br/>
1 1 36.344 4 20 0 1 -1.5 0 0 -14<br/>
1 1 36.345 11 10 0 1 -1.5<br/>
1 1 44.567 4 20 0 1 -1.5<br/>
6 1 44.568 7 6.5 50.37 11<br/>
1 1 44.568 8 10 0 0.5 -1<br/>
1 1 50.372 7 12 0 0.5 -0.5<br/>
14 1 56.185 2<br/>
1 1 59.635 5 20 0 0.5 -1.5<br/>
6 1 59.635 6.5 7 65.44 21<br/>
1 1 59.635 10 13 0 1 -1.5<br/>
<b>@courtesy:	true</b><br/>
1 1 65.44 6 22 0 0.5 -1<br/>
5 1 70.323 6.5 6.5 82.77 -0.75 -2 -0.5 0 0 0 0.65<br/>
1 1 71.22 7 20 0 1<br/>
6 1 71.22 12.5 12 77 11<br/>
1 1 71.22 14 10 0 0.5 -1.5<br/>
1 1 77 13 13 0 0.5 -1<br/>
<b>@courtesy:	true</b><br/>
1 1 82.648 7 20 0 1<br/>
1 1 82.648 12 11 0 1 -1.5<br/>
1 1 93.86 6 22 0 1 -0.5<br/>
<b>@courtesy:	true</b><br/>
1 1 93.86 11 10 0 1 -1.5<br/>
14 1 105.279 2<br/>
1 1 108.729 7 20 0 1<br/>
1 1 108.729 11 10 0 1 -1.5<br/>
1 1 120.176 4 20 0 1 -1.5<br/>
1 1 120.176 10 10 0 1 -1.5<br/>
1 1 132.082 7 20 0 1 0 0 0 -14<br/>
1 1 132.082 9 12 0 1 -1<br/>
<b>@courtesy:	true</b><br/>
1 1 140.305 5 20 0 1 -1<br/>
1 1 140.305 9 10 0 1 -1<br/>
14 1 151.774 2<br/>
1 1 154.317 8 20 0 1<br/>
1 1 154.317 10 10 0 1 -1.5<br/>
1 1 165.637 7 20 0 1<br/>
1 1 165.637 14 10 0 1 -2.5<br/>
6 1 176.957 10 9.5 182.56 21<br/>
1 1 176.957 10 20 0 0.5<br/>
1 1 176.957 14 10 0 1 -2<br/>
1 1 182.563 9 20 0 0.5 -0.5<br/>
1 1 188.169 8 20 0 1<br/>
1 1 188.169 15 10 0 1 -1.5<br/>
14 1 200 2<br/>
8 2 0 2 0.75 200<br/>
3 2 1.5<br/>
17 2 7.459 0 1<br/>
1 2 13.357 3 20 0 1 -1.5<br/>
1 2 13.384 8 10 0 1 -0.5<br/>
1 2 24.9 2 20 0 1 -1.5<br/>
1 2 24.901 7 10 0 1<br/>
1 2 36.344 1 20 0 1 -1.5<br/>
1 2 36.345 6 10 0 1 0 0 0 14<br/>
1 2 44.567 6 20 0 1 -0.5<br/>
1 2 44.568 8 10 0 1 -0.5<br/>
1 2 59.635 2 20 0 0.5 -1.5<br/>
6 2 59.635 3.5 4 65.44 21<br/>
1 2 59.635 7 10 0 1<br/>
1 2 65.44 3 20 0 0.5 -1<br/>
1 2 71.22 4 23 0 1 -1.5<br/>
1 2 71.221 6 10 0 1<br/>
1 2 82.648 3 20 0 0.5 -1<br/>
6 2 82.648 4 3.5 88.25 21<br/>
1 2 82.648 5 10 0 1<br/>
1 2 88.254 2 20 0 0.5 -1.5<br/>
5 2 92.93 1 1 110.46 -1.3964 -2 0 0 0 0 0.65<br/>
1 2 93.86 3 20 0 1 -1.5<br/>
6 2 93.861 5 4.5 99.47 11<br/>
1 2 93.861 6 10 0 0.5 -1<br/>
1 2 99.466 5 10 0 0.5 -0.5<br/>
1 2 108.729 3 20 0 0.5 -1 1 10<br/>
1 2 108.729 4 13 0 1<br/>
1 2 114.57 2 20 0 1 -1.5<br/>
1 2 120.176 3 10 0 1<br/>
1 2 126.476 1 22 0 0.5 -1 1<br/>
1 2 132.082 2 20 0 1 -1.5 0 0 14<br/>
1 2 132.083 2 10 0 1<br/>
1 2 140.305 2 20 0 0.5 -1<br/>
6 2 140.305 3 2.5 146.09 21<br/>
1 2 140.307 2 10 0 1<br/>
1 2 146.085 1 23 0 0.5 -1.5<br/>
1 2 154.317 0 20 0 0.5 -2<br/>
6 2 154.317 2 0.5 171.24 21<br/>
1 2 154.317 5 10 0 1<br/>
1 2 159.923 -1 20 0 0.5 -2.51<br/>
1 2 165.637 -2 20 0 0.5 -2.99<br/>
1 2 165.637 6 10 0 1<br/>
1 2 171.243 -3 20 0 0.5 -3.5<br/>
1 2 176.957 -2 20 0 1 -2<br/>
1 2 176.957 7 10 0 1.5 0 10<br/>
1 2 188.169 5 20 0 1 -1<br/>
1 2 193.775 8 10 0 0.5 0 1<br/>
</td></tr>
</table>


### Removing courtesy accidentals ###

Using the <tt>-r</tt> option will remove cautionary accidentals.  The <tt>-k</tt> option can be added to keep the courtesy information so that the accidentals can be added back into the notation.

<table><tr><td>
[[https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-remove.png]]
</td></tr><tr><td valign=right>
courtesy -kr <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input-data</a> &gt; <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-remove.pmx>output-data</a>
</td></tr></table>


### Replacing courtesy accidentals ###

The <tt>-R</tt> option can be used to replace accidentals removed with the <tt>-r</tt> option.  This can only be done if the <tt>-k</tt> option was used at the same time as <tt>-r</tt>.  The <tt>-k</tt> option stores the cautionary accidental analysis as a global named parameter.  The <tt>-R</tt> option will force the display of an accidental on any note which has a @courtesy named parameter value set to true or non-zero.


### Parentheses ###

Using the <tt>-p</tt> option will cause cautionary accidentals to be placed in parentheses.

<table><tr><td>
[[https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-paren.png]]
</td></tr><tr><td valign=right>
courtesy -p <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input-data</a> &gt; <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-paren.pmx>output-data</a>
</td></tr></table>


### LJ adjustments ###

Notice the the parentheses added to courtesy accidentals sometimes touch the previous item in the music.  The <tt>--lj</tt> option can be used along with the <tt>-p</tt> option to append an LJ command macro for each system on the page which had parentheses added to accidentals.  When the data file is loaded into the SCORE editor with the RE command, the LJ commands will be run to add space around the added parentheses.

<table><tr><td>
[[https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-parenlj.png]]
</td></tr><tr><td valign=right>
courtesy -p --lj <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy.pmx>input-data</a> &gt; <a href=https://raw.github.com/wiki/craigsapp/scorelib/img/courtesy-parenlj.pmx>output-data</a>
</td></tr></table>


### Editorial accidentals ###

The <tt>-e</tt> option can be used to convert cautionary accidentals into editorial accidentals which are shown as small accidentals above the notes rather than in front of the notes.


Programming
=================

Courtesy/cautionary accidentals are automatically identified as a by-product of pitch analysis.  Here is a short program which will identify courtesy accidentals:

```C++
#include "scorelib.h"
int main(void) {
   ScorePage infile;
   infile.read("filename");
   infile.analyzePitch();
   cout << infile;
   return 0;
}
```

Pitch analyses are stored in the <tt>@analysis@base40</tt> named parameter, as can be seen in the processed output data on the right below.  Courtesy accidentals will have the value "true" for the named parameter <tt>@analysis@courtesy</tt>.

<table><tr valign="top"><td>
Given this input:
<table width=350>
<tr><td>
[[https://raw.github.com/wiki/craigsapp/scorelib/img/cf4-thumb-300px.png]]
</td>
<tr><td>
8  1  0.000 0  0.75 50<br/>
3  1  1.500<br/>
17 1  7.471 0  1.00<br/>
1  1 11.596 4 10.00  0 1<br/>
1  1 21.196 4 12.00  0 1<br/>
1  1 30.795 4 12.00  0 1<br/>
1  1 40.395 4 12.00  0 1<br/>
14 1 50.000 1<br/>
</td></tr>
</tr></table>

</td><td>
The above program produces this output:
<table width=350>
<tr><td>
[[https://raw.github.com/wiki/craigsapp/scorelib/img/cf4-caut-thumb-300px.png]]
</td></tr>
<tr><td>
8  1  0.000 0  0.75 50<br/>
3  1  1.500<br/>
17 1  7.471 0  1.00<br/>
1  1 11.596 4 10.00  0 1<br/>
@analysis@base40:	180	(f#)<br/>
1  1 21.196 4 12.00  0 1<br/>
<b>@analysis@courtesy:	true</b><br/>
@analysis@base40:	180	(f#)<br/>
1  1 30.795 4 12.00  0 1<br/>
<b>@analysis@courtesy:	true</b><br/>
@analysis@base40:	180	(f#)<br/>
1  1 40.395 4 12.00  0 1<br/>
<b>@analysis@courtesy:	true</b><br/>
@analysis@base40:	180	(f#)<br/>
14 1 50.000 1<br/>
</td></tr></table>

</td></tr></table>

Source Code
===========

[courtesy.cpp](https://github.com/craigsapp/scorelib/blob/master/src-programs/courtesy.cpp)

