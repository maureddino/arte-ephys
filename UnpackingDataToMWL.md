# Introduction #

Arte data files are made of a sequence of header-buffer pairs (old way), or a globed sequence of google protocol buffers (new way).  Spikes, LFPs, event strings, and all other types of buffers are packed into a single file.  The program **arte\_to\_mwl** acts like adextract used to - searching an input file for a single type of buffer (for example: tetrode 03's spikes), and produces either .tt or .eeg files.  .tt files can be converted to .pyxabw just as they were under the AD system, using spikeparms2 and a .p file from AD's tracker.

A higher-level python script called **arte\_extract.py** can take a list of input tetrodes and output names and sequentially call arte\_to\_mwl, building up the usual directory tree for MWL spike data and filling the tetrode folders with .tt and .pxyabw files.


# Details #

General instructions for unpacking:
  * Grab the code or the executables from the svn site or from the svn repository or from Greg.  Data collected before July 2012 has to be extracted by the older version of the binary.
  * **arte\_to\_mwl** and **arte\_extract.py** live in arte-ephys/src/extract/ until further notice
  * Arte raw data can live together with raw AD data in /expr\_dir/raw/ (expr\_dir = for example /051412 directory for recording session from may 14 2012)
  * Let's say tetrode names 0 through 7 (defined in arte\_session.conf) correspond to tetrodes 23 24 25 26 01 02 03 04 from your drive, and you use the naming convention ##%% for a tetrode's directory, ## tetrode id digits, %% day of the month digits.
  * Move all raw data from AD and arte into expr\_dir/raw/
  * Run your normal AD extraction first (to produce .tt and .pxyaby files from AD spike files, and to produce .pos and .p files from the tracker)
  * Run arte\_extract.py like this, from within expr\_dir/:
` python ~/path/to/arte/arte-ephys/src/extract/arte_extract.py -trodenames 0 1 2 3 4 5 6 7 -trodelabels 2314 2414 2514 2614 0114 0214 0314 0414 -i raw/arte051412.data`
  * The code is a little buggy, so the order of the flags you pass is important.
  * Check the .pxyabw files produced in xclust
  * If you run _new_ arte\_extract.py on _old_ data, you will get errors and no data.
  * If you run _old_ arte\_extract.py on _new_ data, arte\_to\_mwl will probably silently run through the file and claim to find no spikes, or segfault.  Either way you definitely won't get data out.
  * The old/new boundary will probably be something like July 2012.  When it happens, old arte\_extract will be moved to a new directory called old\_extract, and renamed as old\_arte\_to\_mwl and old\_arte\_extract.py