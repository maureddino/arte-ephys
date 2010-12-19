// trode_drawer class
// should this and eeg_drawer both inherit from a sig_drawer class?
// every trode object will have a single trode_drawer class
// keeping track of
// From input:
//  * a list of the most recent spike waveforms
//  * a list of peaks (or other waveform features?) for all past spikes 
//    (possibly just bitmap renderings of projections, w/ a white dot added at each new spike)
// From config:
// * a list of the drawing areas (openGL or matlab) to draw to
// * a list of which channels to render waveforms for (probably all)
// * a list of which projections to draw
// * other preferences (line color, overlay mode, etc)
//
// Because the caller might not yet have a handle to the drawing area when
// this object is constructed (during reading of the conf files), we'll probably
// offload that part of initialization from the constructor to a fn called
// after reading the conf file is finished (when we'll have a full count of trodes
// and drawing areas needed)

#include <iostream>
#include "opts.h"
#include "spike.h"
// include opengl stuff

class TrodeDrawer{

 public:
  TrodeDrawer();
  ~TrodeDrawer();

  void remove_all_drawingareas();
  void attach_drawingarea();

  void new_spike(Spike); // Make a spike struct 


 private:
  n_chans;
  n_samps_per_chan;

  // add vars for display options

  // a list of channel waveform plots

  // somehow manage a list of projection plots, their x/y lims
  // an array of bitmaps (to collect spikes)

};
