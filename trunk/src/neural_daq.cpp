#include "trode.h"
#include "neural_daq.h"
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <stdint.h>

bool acquiring;
int master_id;
int buffer_count = 0;
int32 buffer_size;

std::map <int, neural_daq> neural_daq_map;

bool daqs_reading = false;
bool daqs_writing = false;

pthread_t my_threads[MAX_THREADS];

void neural_daq_init(boost::property_tree::ptree &setup_pt){

  acquiring = false;
  master_id = 0; // arbitrarily pick a card to be master. maybe later user can choose this through conf file?
  buffer_count = 0;

  // temp vars for settings import
  neural_daq this_neural_daq;
  boost::property_tree::ptree ndaq_pt;

  // temp vars for nidaqmx initialization
  int samp_rate = (int)1e6/32;  // this is the card's max: 31250 Hz per channel, 32 channels
  int buffer_samps_per_chan = 32; // hard-coded for now, input data chunk width: 32 samples
  //int buffer_size;
  int32 daqErr = 0;
  char clk_src[256], channel_name[256], trig_name[256]; // strings to be set by nidaqmx fn's
  float64 clkRate;
  neural_daq this_nd;
  neural_daq master_daq;
  int n_completed = 0;
  bool master_completed = false;

  // import the settings
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
		setup_pt.get_child("options.setup.neural_daq_list")){
    // bring in properties from the config file
    ndaq_pt= v.second;
    assign_property <int> ("id", &(this_nd.id), ndaq_pt, ndaq_pt, 1);
    assign_property <uint16_t> ("n_samps_per_buffer", &(this_nd.n_samps_per_buffer), ndaq_pt, ndaq_pt, 1);
    assign_property <uint16_t> ("n_chans", &(this_nd.n_chans), ndaq_pt, ndaq_pt, 1);
    assign_property <std::string> ("dev_name", &(this_nd.dev_name), ndaq_pt, ndaq_pt, 1);
    assign_property <std::string> ("in_filename", &(this_nd.in_filename), ndaq_pt, ndaq_pt, 1);
    assign_property <std::string> ("raw_dump_filename", &(this_nd.raw_dump_filename), ndaq_pt, ndaq_pt, 1);
    // for timing purposes, all daqs must be in agreement over whether reading happens
    if( ! (this_nd.in_filename).empty() )
      daqs_reading = true;

    // but this isn't true of data dumps.  Cards might do that independently
    //if( ! (daq_pt->raw_dump_filename).empty() )
    //  daqs_writing = true;

    // TODO: Error-checking - make sure all daqs in conf specify unique in_filename's and/or unique raw_dump_filename's
    //       And that if we're in read mode (or write mode), that every daq has an in_filename (or raw_dump_filename)

    this_nd.total_samp_count = this_nd.n_chans * this_nd.n_samps_per_buffer;
    // set up a buffer to use as this daq's input stream
    
    //this_nd.data_ptr = new float64 [this_nd.total_samp_count];
    this_nd.data_ptr = this_nd.data_buffer;
    // WHY don't we call new in the above line?  b/c we're replacing dynamic memory with pre-allocated arrays.
    // See global_defs.h
    init_array <float64>(this_nd.data_ptr, 4.0, (this_nd.n_chans * this_nd.n_samps_per_buffer) );
    //this_nd.copy_flexptr = this_nd.data_ptr_copy;
    this_nd.size_bytes = this_nd.total_samp_count * sizeof(this_nd.data_ptr[0]);
    this_nd.buffer_count = 0;
    this_nd.this_buffer = 0;
    this_nd.buffer_time_interval = 0.001;
    neural_daq_map.insert( std::pair<int,neural_daq> (this_nd.id, this_nd) );
  }

  int n_daq = neural_daq_map.size();
  int n = master_id;
  std::map<int, neural_daq>::iterator it;
 
  if( !daqs_reading ){   // that is, if we're really using the cards, as opposed to taking data from raw_dump files

    while(n_completed < n_daq){
      if(!master_completed){   // we want to init the master card first
	n = master_id;         // at end of that init, set n to 0,
      } else{                  // then iterate through the other cards
	if (n == master_id){
	  n++;}
      }
      it = neural_daq_map.find(n);
      this_nd = (*it).second;
      
      this_nd.task_handle = 0; // dunno why, but most examples do this 0 init
      buffer_size = buffer_samps_per_chan * this_nd.n_chans;
      daq_err_check( (DAQmxCreateTask(this_nd.dev_name.c_str(), &(this_nd.task_handle))) );
      for (int c = 0; c < this_nd.n_chans; c++){
	sprintf(channel_name, "%s/ai%d", this_nd.dev_name.c_str(), c);
	daq_err_check ( DAQmxCreateAIVoltageChan(this_nd.task_handle,channel_name,"",DAQmx_Val_RSE,-10.0,10.0,DAQmx_Val_Volts,NULL) );
      }
      
      daq_err_check ( DAQmxCfgSampClkTiming(this_nd.task_handle, "", samp_rate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, buffer_size) );
      
      if( n == master_id ){
	std::cout << "Processing master daq" << std::endl;
	master_daq = this_nd;
	daq_err_check ( DAQmxSetRefClkSrc( this_nd.task_handle, "OnboardClock") );
	if(n_daq > 1)
	  daq_err_check ( DAQmxGetRefClkSrc( master_daq.task_handle, clk_src, 256) );
	daq_err_check ( DAQmxGetRefClkRate( master_daq.task_handle, &clkRate) );
	if(n_daq > 1)
	  daq_err_check ( GetTerminalNameWithDevPrefix(master_daq.task_handle, "ai/StartTrigger", trig_name) );
	daq_err_check ( DAQmxRegisterEveryNSamplesEvent( master_daq.task_handle, DAQmx_Val_Acquired_Into_Buffer, 32,0,EveryNCallback,(void *)&neural_daq_map) );
	daq_err_check ( DAQmxRegisterDoneEvent(master_daq.task_handle, 0, DoneCallback, (void *)&master_daq) );
	
	master_completed = true;
	n = 0;
      } else {
	std::cout << "Processing a slave daq." << std::endl;
	daq_err_check ( DAQmxSetRefClkSrc( this_nd.task_handle, clk_src) );
	daq_err_check ( DAQmxSetRefClkRate(this_nd.task_handle, clkRate) );
	daq_err_check ( DAQmxCfgDigEdgeStartTrig( this_nd.task_handle, trig_name, DAQmx_Val_Rising) );
	// Register every n samples?  No.  we only want the master card to do this.
	daq_err_check ( DAQmxRegisterDoneEvent(this_nd.task_handle, 0, DoneCallback, (void *)&this_nd) );
      }
      this_nd.status = 0;
      neural_daq_map[this_nd.id] = this_nd; // we gained a task handle for each nd. must re-insert into the map for that value to persist
      n_completed++;
    }
  } else {   // daqs are reading, so open the in_file's
    for (int n = 0; n < neural_daq_map.size(); n++){
      neural_daq *nd = &(neural_daq_map[n]);
      nd->in_file = try_fopen( nd->in_filename.c_str(), "rb" );
    }
  }
  
  // Open files for writing, on individual daq-by-daq basis:
  for(int n = 0; n < neural_daq_map.size(); n++){
    neural_daq *nd = &(neural_daq_map[n]);
    if( ! (nd->raw_dump_filename.empty()) ){
      nd->out_file = try_fopen( nd->raw_dump_filename.c_str(), "wb" );
    }
  }
}

void neural_daq_start_all(void){

  if (! daqs_reading){
    // start all slave tasks first, so that they don't miss the start trigger from the master
    std::map<int, neural_daq>::iterator it;
    buffer_count = 0;
    for(int n = 0; n < neural_daq_map.size(); n++){
      neural_daq *nd = &( neural_daq_map[n] );
      if(nd->id != master_id){
	daq_err_check ( DAQmxStartTask( nd->task_handle ) );
	nd->status= 1;
      }
      
      if(nd->out_file != NULL){
	// then we're dumping out output to disk
	try_fwrite<uint32_t>( &(nd->buffer_count),       1, nd->out_file);
	try_fwrite<uint16_t>( &(nd->n_chans),            1, nd->out_file);
	try_fwrite<uint16_t>( &(nd->n_samps_per_buffer), 1, nd->out_file);
      }
      
    }
    // start the master task last -> 
    acquiring = true;      
    daq_err_check ( DAQmxStartTask( neural_daq_map[master_id].task_handle) );
  } else {  // then daqs are getting their data from files
    printf("Reading from file %s\n", neural_daq_map[0].in_filename.c_str());
  }

}
void neural_daq_stop_all(void){
  acquiring = false;
  sleep(1);  // why sleep?  b/c for some reason hitting immediately after running program hangs the computer (threads' fault?)
  neural_daq *nd;

  if(! daqs_reading){
    std::map<int, neural_daq>::iterator it;
    
    bool32 isDone;
    for(int n = 0; n < neural_daq_map.size(); n++){
      //for(it = neural_daq_map.begin(); it != neural_daq_map.end(); it++){
      nd = &(neural_daq_map[n]);
      std::cout << "About to try to stop task_handle: " << nd->task_handle << " on dev: " << nd->dev_name << std::endl;
      daq_err_check ( DAQmxIsTaskDone(nd->task_handle, &isDone) );
      std::cout << "In stop task loop.  Task handle " << nd->task_handle << " is done: " << isDone << std::endl;
      daq_err_check ( DAQmxStopTask( nd->task_handle ) );
      
      
      daq_err_check ( DAQmxIsTaskDone(nd->task_handle, &isDone) );
      std::cout << "In stop task loop.  Task handle: " << nd->task_handle << " is done: " << isDone << std::endl;
      std::cout << "About to clear task: " << nd->task_handle << std::endl;
      
      daq_err_check ( DAQmxClearTask( nd->task_handle) );

      if( nd->out_file != NULL){
	printf("Finalizing file %s with buffer_count %d\n", nd->raw_dump_filename.c_str(), nd->buffer_count);
	rewind (nd->out_file);
	try_fwrite<uint32_t>( &(nd->buffer_count), 1, nd->out_file );
	fclose( nd->out_file );
      }
      
    }
  } else { // we're reading from files, so finalize the files and close them.

  }
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNSamplesEventType, uInt32 nSamples, void *callbackData){
  if(acquiring){
    neural_daq *nd;
    int32 read;
    int rc;
    int n;
    buffer_count++;
    for(int n = 0; n < neural_daq_map.size(); n++){
      //for(std::map<int,neural_daq>::iterator it = neural_daq_map.begin(); it != neural_daq_map.end(); it++){
      nd = &(neural_daq_map[n]);
      daq_err_check ( DAQmxReadAnalogF64( nd->task_handle, 32, 10.0, DAQmx_Val_GroupByScanNumber, nd->data_ptr, buffer_size, &read,NULL) );
     
      if( nd->out_file != NULL){
	try_fwrite<float64>( nd->data_ptr, buffer_size, nd->out_file);
      }
      nd->buffer_count = nd->buffer_count + 1;
	//memcpy( (*it).second.data_ptr_copy, (*it).second.data_ptr, (*it).second.size_bytes);
      //print_buffer( & (*it).second, 32, 32, 32 ); 
    }
    n = 0;
    for(std::map<std::string, Trode>::iterator it = trode_map.begin(); it != trode_map.end(); it++){
      Trode *this_trode = &((*it).second);
      
     
      //rc = pthread_create(&my_threads[n], NULL, trode_filter_data, this_trode);
      //if(rc){
      //printf("THREAD ERROR!");
      //exit(-1);
      //}

      trode_filter_data(this_trode);
      if( it == trode_map.begin() && (buffer_count % 37 == 0)){
	//this_trode->print_buffers(4, 97);
      }
      n++;

      //neural_daq_stop_all();
    }
  }
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData){
  std::cout << "DoneCallback Called." << std::endl;
  neural_daq nd = find_neural_daq_by_taskhandle(taskHandle);
  print_neural_daq(nd);
  fflush(stdout);
}

void print_neural_daq(neural_daq nd){
  std::cout << "id: " << nd.id << std::endl << "dev_name: " << nd.dev_name <<
    "status: " << nd.status << std::endl;
}

neural_daq find_neural_daq_by_taskhandle(TaskHandle taskhandle){
  std::map<int, neural_daq>::iterator it;
  for(it = neural_daq_map.begin(); it != neural_daq_map.end(); it++){
    if( (*it).second.task_handle = taskhandle){
      return (*it).second;
    }
  }
  std::cout << "Couldn't find the taskhandle: " << taskhandle << std::endl;
}

void print_buffer(neural_daq *ndp, int row_lim, int col_lim, int row_length){
  system("clear");
  std::cout << std::fixed << std::setprecision(1);
  for (int r = 0; r < row_lim; r++){
    for (int c = 0; c < row_lim; c++) {
      std::cout << std::setw(5) << ndp->data_ptr[ r*row_length + c ] << " ";
    }
    std::cout << std::endl;
  }
}
