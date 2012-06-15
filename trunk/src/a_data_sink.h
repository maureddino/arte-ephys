#ifndef A_DATA_SINK_H_
#define A_DATA_SINK_H_

#include <memory> // shared_ptr
#include <boost/variant.hpp>
#include <boost/bind.hpp>
#include "a_data_source.h"  //ListenerKey typedef
#include "nidaq_data_source.h"
#include "global_state.h" // data_mutex

// ListenerKey typedef is in a_data_source.h

class BoolDefaultsFalse{
 public:
  BoolDefaultsFalse(){
    my_state = false;
  }
  void operator()(bool input_bool){
    my_state = input_bool;
  }
  bool operator()(){ return my_state; }
 private:
  bool my_state;
};


template < class SourceDataType, class SinkDataType >
class ADataSink{

 public:

  ADataSink();
  ADataSink( typename ADataSource<SourceDataType>::SourcePtr my_data_source, ArteGlobalState *gs );
  ~ADataSink();
  typedef typename PtrTo< ADataSink <SourceDataType, SinkDataType> >::Type SinkPtr;
  typedef typename PtrTo< ADataSink <SourceDataType, SinkDataType> >::ListType SinkList;
 
  // derived classes override operator() to do sink-specific work
  // no need to do any thread stuff. (threads handled by
  // the data source and data_sink_worker) Just ask the data
  // source we own for data by data_source.get_data(&data);
  // then do whatever with it
  virtual void operator()();
  
  SourceDataType *source_buffer;
  SinkDataType   data;

  typename ADataSource<SourceDataType>::SourcePtr my_data_source;

  void get_data_from_source();
  bool read_smudge(){ return smudge; };
  void set_smudge(bool new_smudge) {smudge = new_smudge;};

 protected:
  
  void register_self();

  bool smudge;
  
  //  ListenerKey my_key;

  BoolDefaultsFalse registration_done;
  ArteGlobalState *global_state_p;

  static SinkList data_sink_list;

};

template <class SourceDataType, class SinkDataType>
  ADataSink<SourceDataType,SinkDataType>::ADataSink()
{
  std::cout << "In abstract datasink constructor no args.\n"; fflush(stdout);
}

template <class SourceDataType, class SinkDataType>
  ADataSink<SourceDataType,SinkDataType>::ADataSink( typename ADataSource<SourceDataType>::SourcePtr the_source, ArteGlobalState *gs)
    :my_data_source (the_source)
    ,global_state_p (gs)
{
  // New the bool to be shared with the data source marking dirty data, and then pass it to the data source
  std::cout << "In CONSTRUCTOR for abstract sink\n"; fflush(stdout);

  std::cout << "smudge_ptr is " << &smudge << std::endl;
  register_self();
}
						     

template <class SourceDataType, class SinkDataType>
  ADataSink<SourceDataType, SinkDataType>::~ADataSink() {
  std::cout << "Sink Destructor!\n";
  // deregister self from listener list
  my_data_source->unregister_listener( &smudge );
};

// define the sink list
template <class SourceDataType, class SinkDataType>
  typename  ADataSink <SourceDataType, SinkDataType>::SinkList
  ADataSink<SourceDataType,SinkDataType>::data_sink_list;

template < class SourceDataType, class SinkDataType >
  void ADataSink <SourceDataType, SinkDataType>::operator()(){
  std::cout << "ADataSink operator();" << std::endl;
  std::cout << "Want to call () of concrete type instead; this one knows how to copy "
	    << "the source data into the store buffer.";
  // Mark data as 'processed'
  smudge = false;
};

template < class SourceDataType, class SinkDataType >
void ADataSink <SourceDataType, SinkDataType>::register_self(){
  std::cout << "about to assign &smudge = " << &smudge << std::endl;;
  std::cout << "my_data_source addy is " << my_data_source << std::endl;
  my_data_source->register_listener( &smudge );
  std::cout << "finished assign\n";
  /* old way
  my_key = data_sink_list.size();
  data_sink_list.push_back( SinkPtr (this) );
  my_data_source->register_listener( my_key );
  */

  registration_done(true);

};


template <class SourceDataType, class SinkDataType>
  void ADataSink<SourceDataType,SinkDataType>::get_data_from_source(){
  data = my_data_source->get_data();
};


#endif
