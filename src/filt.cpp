#include <boost/property_tree/exceptions.hpp>
#include <exception>
#include <string>
#include "filt.h"
#include "util.h"

#define S_F64 8                   // 8 bytes for a float64
#define S_ARRAY(R,C) R*C*S_F64   // Size of an array of float64s of row x col size 
//  wrap coordinate for circular buffer

int rel_pt(int pos, int curs, int buf_len){
  int r = (( curs + pos) % buf_len);
  if (r < 0)                    // funny situation - our implementation of c returns mod
    r += buf_len;               // with the same sign of the numerator.  tricky! 
  return r;
}

//Filt::FiltList filt_list;

Filt::FiltList Filt::build_filt_list( ArteSetupOptPb & setup_opt, int n_samps_per_source_chan ){
  Filt::FiltList filt_list;
  for(int i = 0; i < setup_opt.filters_size(); i++){
    ArteFilterOptPb this_filt_opt = setup_opt.filters(i);
    Filt this_filt ( this_filt_opt, n_samps_per_source_chan );
    filt_list.insert( std::pair <std::string, Filt> 
			    (this_filt_opt.filter_name(), this_filt) );
  }
  return filt_list;
}

void Filt::generate_coefs( ArteFilterOptPb &filt_opt ){
  std::cerr << "Not yet implemented.  Calculate your own damn coefficients!\n";
  //TODO: Filt::generate_coefs from filter options
}


Filt::Filt( ArteFilterOptPb & filt_opt, int n_samps_per_source_chan )
{

  filt_name = filt_opt.filter_name();
  
  bool calculate_coefs = false;
  if( filt_opt.has_regenerate_coefs() ){
    if( filt_opt.regenerate_coefs() ){
      generate_coefs( filt_opt );
    }
  } else
    {
      for ( int n = 0; n < filt_opt.numerators_size(); n++)
	numerator_coefs.push_back( filt_opt.numerators(n) );
      for ( int n = 0; n < filt_opt.denominators_size(); n++)
	denominator_coefs.push_back( filt_opt.denominators(n) );
      for ( int n = 0; n < filt_opt.multiplier_size(); n++)
	multipliers.push_back( filt_opt.multiplier(n) );
    }
  delay_direction        = filt_opt.delay_direction();
  make_sos               = filt_opt.make_sos();
  filtfilt_invalid_samps = ( delay_direction == 1 ) ?  
    0  : filt_opt.filtfilt_invalid_samps();

  if( make_sos ){
    if(!( (numerator_coefs.size() == denominator_coefs.size() &
	   numerator_coefs.size()/3 == multipliers.size() ) )){
      std::cerr << "FILT: Error constructing filter sections, "
		<< "number of numerator and denominator "
		<< "coefs must match, and be three times as "
		<< "many as number of multipliers.\n";
      // TODO: make exceptions for Filt - for example, coefs_mismatched_for_second_order_sections : public exception ?
      //      throw std::exception;
    }
    my_minimum_samps = n_samps_per_source_chan + 2;
    int n_sections = numerator_coefs.size()/3;
    for (int n = 0; n < n_sections; n++){
      bool this_forward_direction;
      if( (delay_direction == -1) | 
	  (delay_direction == 0 & n >= (n_sections / 2)) ){
	this_forward_direction = true;
      } else {
	this_forward_direction = false;
      }
      sections.push_back( Section ( numerator_coefs.begin()   + (3*n), numerator_coefs.begin()   + (3*n)+2,
				    denominator_coefs.begin() + (3*n), denominator_coefs.begin() + (3*n)+2,
				    multipliers       [n],
				    this_forward_direction ) );
    }
  }  else {
    my_minimum_samps = n_samps_per_source_chan + numerator_coefs.size() - 1;
    bool this_forward_direction = (delay_direction > -1) ? true : false;
    sections.push_back( Section (numerator_coefs.begin(), numerator_coefs.end(), 
				 denominator_coefs.begin(), denominator_coefs.end(), multipliers[0],
				 this_forward_direction ) );
  }


}

void Filt::init_raw_voltage_circular_buffer(raw_voltage_circular_buffer &buffer, 
					    int n_chans, int n_samps){
  buffer.clear();
  for(int c = 0; c < n_chans; c++){
    buffer.push_back( boost::circular_buffer <rdata_t> (n_samps) );
  }
}


//raw_voltage_circular_buffer * Filt::attach_buffers( raw_voltage_circular_buffer *_in_buffer ){
void Filt::attach_buffers (raw_voltage_circular_buffer *_in_buffer, raw_voltage_circular_buffer *_out_buffer){
  n_sections = sections.size();
  int n_chans = _in_buffer->size();
  int n_samps = (*_in_buffer)[0].size();
  
  init_raw_voltage_circular_buffer( output_buffer, n_chans, n_samps );
  
  intermediate_buffers.clear();
  for (int n = 0; n < n_sections; n++){
    intermediate_buffers.push_back( output_buffer ); // copy output buffer, which is 
                                                     // init to the right size already

    // set first sections's input buffer to the filter input buffer
    // all other sections input buffer is the previous intermediate buffer
    if( n == 0 ){
      sections[n].input_buffer = _in_buffer;
    } else {
      sections[n].input_buffer = &(intermediate_buffers[n-1]);
    }
    // set all sections' output buffer addys to intermediate buffers, but last
    // sections output is the filter output buffer
    if( n == (n_sections-1) ) {
      sections[n].output_buffer = &output_buffer;
    } else {
      //sections[n].output_buffer = &(intermediate_buffers[n]);
      sections[n].output_buffer = _out_buffer;
    }
  }

}
  
void Filt::operator()(){
  std::cout << "Implement Filt::operator()();\n";

}
// BIG NOTE: this functinon will now be defined in filtered_buffer.cpp
// This way, it will know what a Filtered_buffer is and can access its
// public members.  This drastically shortens the argument lis.t
void filter_data(rdata_t *in_buf, Filt *filt, neural_daq *nd, uint16_t *chans, uint16_t n_chans, int in_buf_len, 
		 uint16_t out_buf_len, int *u_curs_p, int *f_curs_p, int *ff_curs_p,  rdata_t *u_buf, rdata_t *f_buf, rdata_t *ff_buf){
 
  int u_curs = *u_curs_p;
  int f_curs = *f_curs_p;
  int ff_curs = *ff_curs_p;
  int out_pt; // an index to use in the for loop.
  int n_segs;
  int in_pt, in_pt_c, in_pt_h1, in_pt_h2;
  int in_pt_c_f, in_pt_h1_f, in_pt_h2_f, out_pt_f;  
  int h,n,c,p,s,i,i_offset_this, i_offset_last;  // various counters for for loops
  h = n = c = p = s = i = i_offset_this = i_offset_last = 0;
  double *a, *b;
  rdata_t value;
  int in_n_chans = nd->n_chans;

  a = filt->denom_coefs;
  b = filt->num_coefs;

  // up is just a loop to repeat the filtering multiple times, for testing CPU usage
  for(int up = 0; up < 1; up++){

    // *** Running with the f_buf -> u_buf copy step turned off, this copy block seems to be working *****    
    // first copy data into unfiltered buffer and initialize the corresponding out buffers
    for(s = 0; s < in_buf_len; s++){  //outer loop over n_samps. (n_chans, usually a tetrode)
      h = (s+u_curs) * n_chans; // here h will be the row (sample) offset for u_buf
      p = s * in_n_chans;  // p will offset for the longer rows of in_buf      
      for(c = 0; c < n_chans; c++){   // loop over chans
        u_buf[h + c] = in_buf[p + chans[c] ]; // in_buf is whe whole nidaq card, so we have to index into the chan through the 'chans' field
	                                      // which lists the channels to pick from in_buf
	f_buf[h + c] = 0;
      }
    }
    //  *****   End checked portion ***********
    
    for(s = 0; s < in_buf_len; s++){
      h = CBUF ( (s+u_curs+in_buf_len), out_buf_len ) * n_chans;
      for(c = 0; c < n_chans; c++){
	//u_buf[ h + c ] = 0.0;
	//f_buf[ h + c ] = 0.0;
      }
    } // a test to make sure indexing is working as expected


    // FOR NOW I'm BLOCKING the FIR section.  So I can test iir's with a single s.o.s. with the primary IIR code
    if((filt->filt_num_sos == 1) && (false)){  // THIS FIR STUFF ISN'T CAREFULLY CHECKED YET, NOR UPDATED FOR IN_BUF TRANSPOSE
     // then we most likely have an FIR.  This code will take care of FIR and 
     // single-segment IIR filters, too, as long as the first last denom
     // coefficient is set to 0.
      printf("/a");
      for(n = 0; n < in_buf_len; n++){                                           // process in_buf_len points
	out_pt = n + u_curs;
	for(p = 0; p <= filt->order; p++){                                        // iterate over history points (we need order of them)
	  h = -1*filt->order + p;                                 // history index should be -order, -order+1, ..., 0
	  in_pt = rel_pt(h, n+u_curs, in_buf_len);
	  for(c = 0; c < n_chans; c++){
	    f_buf[c*out_buf_len + out_pt] += 
	      (rdata_t)( in_buf[c*in_buf_len + in_pt]*filt->num_coefs[p]*filt->input_gains[0] -    // feedforward path
		f_buf[c*out_buf_len + in_pt]*filt->denom_coefs[p]);                       // feedback path
	  } // end loop over chans chan                                                                         // 
	} // end loop over history points
      } //end loop over point in out_buf to compute                                       // viola.  Check the sign conventions. 
    } // end if block for sos == 1
    
    else {
      // then we have biquad sections, and can dispense with the for loop over p
      // b/c we know the fixed history length.
      n_segs = filt->filt_num_sos;
      for(p = 0; p < n_segs; p++){
	i_offset_this = (p+1) * n_chans * out_buf_len;  // write to the (p+1)th part of the u_buf
        i_offset_last = (p)   * n_chans * out_buf_len;  // get vals from  (p)th part of the u_buf
	i_offset_this = n_chans * out_buf_len;
	i_offset_last = 0;

// 	if(p > 0){  // then copy an in_buf sized slice from f_buf (starting at f_curs) to u_buf (starting at u_curs)
// 	  //memcpy( (u_buf + S_ARRAY(u_curs,n_chans))+3, (f_buf + S_ARRAY(f_curs,n_chans)),  S_ARRAY(in_buf_len,n_chans) );
// 	  //memset( (u_buf + S_ARRAY(u_curs,n_chans))+3, 0, 3);
	  
// 	  //memcpy( (u_buf + u_curs*n_chans*8), (f_buf + f_curs*n_chans*8), in_buf_len*n_chans*8);
	  
// 	  //memset( (u_buf + u_curs*n_chans), 0, (1)*n_chans*8);
	  
// 	  for(s = -1*filt->order; s < 0; s++){
// 	    h = ( CBUF(u_curs - s, out_buf_len) * n_chans );
// 	    for(c = 0; c < n_chans; c++){
// 	      u_buf[h + c] = f_buf[h + c];
// 	    }
// 	  }
// 	  for(s = 0; s < in_buf_len; s++){
// 	    h = (s+u_curs)*n_chans;
// 	    for(c = 0; c < n_chans; c++){
// 	      u_buf[h + c] = f_buf[h + c];
// 	      if(u_buf[h + c] == f_buf[h + c]){
// 		printf("\a");
// 	      }
// 	      //u_buf[h + c] = f_buf[h + c];
// 	    }
// 	  } // end copy loop
// 	} // end copy if
	//printf("\a");
	
	for(s = 0; s < in_buf_len; s++){  // loop over samples
	  
	  out_pt =   (s+u_curs) * n_chans + i_offset_this; 
	  in_pt_c =  (s+f_curs) * n_chans + i_offset_last;
	  in_pt_h1 = (CBUF( -1+f_curs+s, out_buf_len )) * n_chans + i_offset_last;
	  in_pt_h2 = (CBUF( -2+f_curs+s, out_buf_len )) * n_chans + i_offset_last;
	  
	  out_pt_f = (s + u_curs) * n_chans;
	  in_pt_c_f = (s+f_curs) * n_chans;
	  in_pt_h1_f = ( CBUF( -1+f_curs+s, out_buf_len) ) * n_chans;
	  in_pt_h2_f = ( CBUF( -2+f_curs+s, out_buf_len) ) * n_chans;
	  
	  for(c = 0; c < n_chans; c++){ //loop over chans
	    
	    //if(p > 0){
	    //  u_buf[in_pt_c + c] = f_buf[out_pt + c];
	    //}

	    u_buf[out_pt + c] =
	      (rdata_t) ( u_buf[in_pt_c + c] * b[p*3 + 0] * filt->input_gains[p] +
		u_buf[in_pt_h1 +c] * b[p*3 + 1] * filt->input_gains[p] +
		u_buf[in_pt_h2 +c] * b[p*3 + 2] * filt->input_gains[p] -
		f_buf[in_pt_h1_f +c] * a[p*3 + 1] -
		f_buf[in_pt_h2_f +c] * a[p*3 + 2] );

	    f_buf[out_pt_f + c] = u_buf[out_pt + c];
	    //f_buf[out_pt + c] = 1.1;

	    //f_buf[c*out_buf_len + out_pt] = -100.0;  
	  } // end loop over chans
	} //end loop over points in this input chucnk
      }  // end loop over segs

      // copy the end of u_buf (the result of the run of the last SOS) to f_buf
      for(s = 0; s < in_buf_len; s++){
	h = (s+u_curs)*n_chans;
	for(c = 0; c  < n_chans; c++){
	  f_buf[h + c] = u_buf[h + c + i_offset_this];
	}
      }


    } // end if iir
  } // end 'up' loop

  //   std::cout << "Begin check." << std::endl;
  // std::cout << "Before change, *u_curs_p is " << *u_curs_p << std::endl;
  // std::cout << "in_buf_len is " << in_buf_len << std::endl;
  // std::cout << "out_buf_len is " << out_buf_len << std::endl;

  (*u_curs_p) = in_buf_len + (*u_curs_p);
  (*f_curs_p) = in_buf_len + (*f_curs_p);
  //std::cout << "Before wrap check, *u_curs_p is " << *u_curs_p << std::endl;
  //if( u_curs > out_buf_len || f_curs > out_buf_len){
  //  std::cout << "Strange: u_curs or f_curs is greater than it's supposed to be allowed to get." << std::endl;
  //}
  if((*u_curs_p) == out_buf_len){
    (*u_curs_p) = 0;
    //  std::cout << "Reset u_curs_p." << std::endl;
  }
  if((*f_curs_p) == out_buf_len)
    (*f_curs_p) = 0;

  //std::cout << "After all checking, *u_curs_p is " << (*u_curs_p) << std::endl;
  fflush(stdout);
  
  // Still need to do the filtfilt work
  //if((filt->count)++ == 4)
    //exit(1);
}

Filt::Filt(){
}

Filt::~Filt(){
}

void Filt::init(boost::property_tree::ptree &filt_pt){
  int n_coefs;
  filt_name = filt_pt.data();

  assign_property<name_string_t>("type", &filt_type, filt_pt, filt_pt, 1);
  assign_property<int>("order", &order, filt_pt, filt_pt, 1);
  assign_property<bool>("filtfilt", &filtfilt, filt_pt, filt_pt, 1);
  assign_property<int> ("filtfilt_wait_n_buffers", &filtfilt_wait_n_buffers, filt_pt, filt_pt, 1);

  data_cursor = 0;

  if(strcmp(filt_type,"fir") == 0){
    n_coefs = order;
    filt_num_sos = 1; // second-order-sections is the wrong term here; we mean one plain-old-section (full filter order)
  }
  else if( strcmp(filt_type,"iir") == 0 ){
    filt_num_sos = order / 2;
    n_coefs = filt_num_sos * 3;
  } else{
    std::cerr << "Can't use filt type: " << filt_type << std::endl;
  }
  assign_property<double>("num_coefs", num_coefs, filt_pt, filt_pt,n_coefs);
  assign_property<double>("denom_coefs", denom_coefs, filt_pt, filt_pt, n_coefs);
  if(strcmp(filt_type,"fir") == 0)  // in the fir case:
    assign_property<double>("input_gains", input_gains, filt_pt, filt_pt, 1);
  else  // in the iir case:
    assign_property<double>("input_gains", input_gains, filt_pt, filt_pt, filt_num_sos);
  count = 0; // temporary thing for debugging
}

void print_array(rdata_t *buf, int n_chans, int buf_len, int curs)
{
  std::cout.precision(2);
  for (int c = 0; c < n_chans; c++){
    std::cout << "c" << c << " ";
    for (int p = 0; p < buf_len; p++){
      if(p == curs)
	std::cout << " : ";
      std::cout << buf[c*buf_len + p] << " ";
    }
    std::cout << std::endl;
  }
}

