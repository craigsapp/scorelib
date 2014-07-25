
Named Parameters
==================

### global namespace


<dl>

<dt>
@courtesy: true
</dt>
<dd>
Indicates that the note should be displayed with a couresy accidental.  See also @analysis@courtesy.
</dd>

</dl>


### analysis namespace
<dl>

<dt>
@analysis@base40:  \d+
</dt>
<dd>
The base-40 pitch number of notes.  The ScorePage::analyzePitch() function will automatically calculate this parameter.
</dd>

<dt>
@analysis@courtesy: true
</dt>
<dd>
Indicates that an accidental is displayed, and the accidental is redundant when determining the pitch of the note.  The ScorePage::analyzePitch() function will automatically calculate this parameter.  See also @courtesy.
</dd>


</dl>



