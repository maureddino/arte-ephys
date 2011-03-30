#ifndef GLOBAL_DEFS_H_
#define GLOBAL_DEFS_H_

#include <string>
#include <iostream>
#include <NIDAQmx.h>
#include <map>
#include <stdint.h>

#define MAX_THREADS 32
#define THREADED_

#include <pthread.h>

//typedef float64 rdata_t;
typedef int16_t rdata_t;
const rdata_t RDATA_MIN = INT16_MIN;
const rdata_t RDATA_MAX = INT16_MAX;

extern pthread_t my_threads[MAX_THREADS];

const int MAX_TRODE_NAME_LENGTH = 50;
const std::string default_setup_config_filename ("/home/greghale/arte-ephys/conf/arte_setup_default.conf");
const std::string default_session_config_filename ("/home/greghale/arte-ephys/conf/arte_session_default.conf");

const int MAX_NEURAL_DAQ_N_CHANS = 32;
const int MAX_NEURAL_DAQ_N_SAMPS_PER_CHAN = 320;
const int MAX_NEURAL_DAQ_BUFFER = MAX_NEURAL_DAQ_N_CHANS * MAX_NEURAL_DAQ_N_SAMPS_PER_CHAN;

const int MAX_TRODE_N_CHANS = 32;      // 32 chanels
const int MAX_TRODE_BUFFER_LEN = 320; // 3200 samps = about 0.01 seconds
const int MAX_N_INTERMEDIATE_BUFFERS = 10;
const int MAX_TRODE_BUFFER = MAX_TRODE_N_CHANS * MAX_TRODE_BUFFER_LEN * (MAX_N_INTERMEDIATE_BUFFERS + 2);

const int MAX_LFP_BANK_N_CHANS = 32;
const int MAX_LFP_BANK_BUFFER_LEN = 320;
const int MAX_LFP_BANK_N_INTERMEDIATE_BUFFERS = 10;
const int MAX_LFP_BANK_BUFFER = MAX_LFP_BANK_N_CHANS * MAX_LFP_BANK_BUFFER_LEN * MAX_LFP_BANK_N_INTERMEDIATE_BUFFERS;

const double NEURAL_DAQ_V_MAX = 10.0;
const double NEURAL_DAQ_V_MIN = -10.0;

const int MAX_FILT_COEFS = MAX_TRODE_BUFFER_LEN;

struct neural_daq{
  int id;
  std::string dev_name;
  uint16_t n_samps_per_buffer;
  uint16_t n_chans;
  std::string in_filename;
  FILE *in_file;
  std::string raw_dump_filename;
  FILE *out_file;
  rdata_t data_buffer[MAX_NEURAL_DAQ_BUFFER];
  rdata_t *data_ptr;
  TaskHandle task_handle;
  int total_samp_count;
  int size_bytes;
  int status;
  double buffer_time_interval;  // inter-buffer-interval (sec)
  uint32_t buffer_count;
  uint32_t this_buffer;
};

extern bool daqs_reading; // Daq's read in unison.  No simultaneous mixing between file and card in allowed.
extern bool daqs_writing; // <-- prob won't be used.  Some daq's may write while others don't.

extern std::map <int, neural_daq> neural_daq_map;



#endif
