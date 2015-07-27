# Introduction #

There's a lot of state to save between sessions:

> ## Session settings constant within rat ##
    * channel -> trode assignment
    * trode filter settings
    * channel gains
    * channel thresholds
    * channel colors
    * channel 'cursors' (ie a little marker in amplitude space for vis only)
    * (potentially) cluster bounds for closed-loop stuff
    * channel -> eeg assignment
    * eeg stacking order
    * eeg trace color
    * eeg filter settings

> ## Settings that may change within day, at user's whim ##
    * Trode -> display window assignment
    * Trode magnification
    * Ordering displayed trodes within a window
    * Show/hide metadata

> ## Back-end settings probably essentially constant over life of the system ##
    * pin\_in -> channel assignments
    * Daq input buffer sizes
    * Rules for default output file names

> ## One-off, single-day or dubugging mode ##
    * Get input from card, or from saved card buffer dump series?
    * No displays?

How can we implement all this?

# Proposal #

2 config files, each in xml, each in 2 possible places

  * arte\_backend.conf
  * arte\_session.conf

Living in:

  * pwd from which arte was launched, if not found there, then search
  * ~/.arte/

Rough sketch of a config file:

```
<session_config>
 <output_file>data.aep</output_file>
 <trodes>
  <trode>TT01
   <name>"trode01"</name>
   <filter>"IIR_600_6000_N4"</filter>
   <input_channels>
    <channel>01</channel>
    <channel>02</channel>
    <channel>03</channel>
   <input_channels>
  </trode>
  <trode>TT02
   <name>"trode02"</name>
   <filter>"IIR_600_6000_N4"</filter>
   <channels>
    <channel>04</channel>
   </channels>
  </trode>
 </trodes>
 <eegbanks>
  <eegbank>Hippocampus
   <filter>"IIR_1_375_N4"</filter>
   <eeg>EEG01
    <channel>01</channel>
    <color>Hot Pink</color>
   </eeg>
   <eeg>EEG02
    <channel>05</channel>
    <color>Sea Monkey</color>
   </eeg>
  </eegbank>
  <eegbank>Cortex
   <filter>"IIR_p1_200_N4"</filter>
   <eeg>EEG03
    <channel>09</channel>
    <color>Turquoise</color>
   </eeg>
  </eegbank>
 </eegbanks>
</session_config>
```

(admittedly I don't know how to write a clear xml file yet)

Parse xml files using [Boost Property Tree](http://www.boost.org/doc/libs/1_45_0/doc/html/property_tree.html)

We can write to xml, too, when the tree changes due to user input.