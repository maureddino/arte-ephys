#ifndef TRODE_H_
#define TRODE_H_

#include <NIDAQmx.h>
#include <string>

enum trig_mode_t {CENTER_ON_THRESH=0, CENTER_ON_NEXT_MAX=1};

// not a method, but a regular function
// this way has less overhead.
// fn gets pointer to trode, so it has access
// to all the public member variables

//void trode_filter_data(Trode *, float64 *, int);  // trode_filter_data(trode_to_process, raw buffer, timestamp at buffer start)
//void trode_process_data(Trode *); // only need access to this trode's buffers (filtered data is there)

class Trode{

 public:
  // Constructor will be called by arteopt.cpp itself, to avoid this class having
  // an include for arteopt.h (prefer arte opt includes trode, for scope reasons;
  // arteopt now does all the initialization stuff, and needs a std::vector<Trode>
  Trode();
  Trode(std::string &name, int n_chans, float64 *thresholds, int samps_pre, int samps_post, int trig_mode,
	       std::string &filt_name);

  // The old idea: constructor just takes a tetrode name and the options structure. 
  //Trode(char *, opt *)   // override constructor, take just a name and opt object
                                // then let the trode finish initializing

  virtual ~Trode();

  void set_filter();
  void set_trig_params();
  void set_n_chans();
  void set_n_samps_per_chan();
  int get_n_chans;
  int get_n_samps_per_chan();
  
  // these are handled during initialization by arteopt
  // void remove_all_drawing_areas();
  // void add_drawing_area();

  //add_data(prt_to_array, n_chans, n_samps_per_chan, time_at_beginning_of_buffer)
  void add_data(float64 *,int,int,long int);
  // NB: we'll prob use the fn's trode_filter_data & trode_process_data instead of this method
  // to save overhead.  But let's keep this member fn and implement it so we can compare the execution time

  std::string trode_name;
  float64 *ptr_to_raw_stream;
  int buffer_mult_of_input; // how many input buffers do we keep for this trode?
  float64 *raw_buffer;
  float64 *filt_buffer;
  int raw_data_cursor;
  int filt_data_cursor;
  int raw_cursor_time;
  int filt_cursor_time;

  int n_chans;                // num of chans for this trode
  int n_samps_per_chan;       // num of samps per chan (derived from samps_after_trig & samps_before_chan)
  int *channels;              // ptr is to 32-chan signal.  Which rows belong to this trode?

  int daq_id;                 // id number of neural daq card that this trode sees
  //neural_daq_card daq_card; // data struct of that daq card
  //can't do that b/c neural_daq_card defined in arteopt, which includes trode.h
  int stream_samps_per_chan;  // how many samps come in with each raw buffer?
  int stream_n_chan;          // how many chans come in with each raw buffer? (this will almost always be 32)

  std::string spike_mode;     // instructions for centering spikes.  'thresh' means center spikes on xing. 'peak' means on next local max
  float64 *thresholds;        // array of threshold voltages, 1 per chan
  int samps_before_trig;      // how many samps to collect before trig ind
  int samps_after_trig;       // hom many samps to collect after trig ind (total samps is after + before + 1

  float64 *win_heights;       // array of display ranges. I know, we should separate this data structure from
                              // the visualization, but it's hugely convenient to store this tetrode-by-tetrode data here

};

void trode_filter_data(Trode *, float64 *, int); // trode_filter_data(trode_to_prosess, raw buffer, timestamp at buffer start)
void trode_process_data(Trode *); // only need access to this trode's buffers (filtered data is there)

#endif
