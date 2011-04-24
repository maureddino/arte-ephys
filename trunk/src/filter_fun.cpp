#include "filter_fun.h"

void filter_buffer( Filtered_buffer * fb ){

  int c_in, c_out, samp;  // current sample, channel
  int in_pt, in_pt_c, in_pt_h1, in_pt_h2; // current input point, current, history1, history2
  int out_pt;
  int n_in_chans, n_in_samps; // copy values from Filt_buf to here for speed
  int n_out_chans, n_out_samps;  // copy from fb for speed
  int this_in, this_out;
  int p; // which second-order-section are we on?
  int c;
  int u_curs, f_curs, ff_curs;

  double *a; // denominator coefficients
  double *b; // numerator   coefficients

  rdata_t * daq_buf = *(fb->ptr_to_raw_stream); // daq's buffer
  rdata_t * u_buf  = fb->u_buf;                 // unfilt buffer
  rdata_t * f_buf  = fb->f_buf;                 // filt buffer
  rdata_t * ff_buf = fb->ff_buf;                // filtfilt buffer

  // copy data into local vars
  n_in_chans =  fb->stream_n_chans;
  n_in_samps =  fb->stream_n_samps_per_chan;
  n_out_chans = fb->n_chans;
  n_out_samps = fb->buf_len;

  u_curs = fb->u_curs;
  f_curs = fb->f_curs;
  ff_curs= fb->ff_curs;

  a = fb->my_filt.denom_coefs;
  b = fb->my_filt.num_coefs;

  // first copy data from in_stream to u_buf
  for(c_out = 0; c_out < n_out_chans; c_out++){
    c_in = fb->channels[c_out];
    for(samp = 0; samp < n_out_samps; samp++){
      this_out = (samp * n_out_chans) + c_out; // (samp num * n_chans) + chan_num
      this_in =  (samp * n_in_chans)  + c_in;  // (samp num * 
      u_buf[this_out] = daq_buf[this_in];
    }
  }

  if((fb->my_filt.filt_num_sos == 1)){
    // This is the place where FIR filtering will be implemented
    std::cout << "Tell Greg to implement the fir filter." << std::endl;
    exit(EXIT_FAILURE);
  }

  for(p = 0; p < fb->my_filt.filt_num_sos; p++){
    
    // this is the p'th filtering step
    // first compute f_buf
    for(samp = u_curs; samp < u_curs + n_in_samps; samp++){
      out_pt =  samp * n_out_chans;
      in_pt_c = samp * n_out_chans;
      in_pt_h1 = CBUF( (-1 + samp), n_out_samps  ) * n_out_chans;
      in_pt_h2 = CBUF( (-2 + samp), n_out_samps  ) * n_out_chans;
      for( c = 0; c < n_out_chans; c++){
	f_buf[ out_pt + c_out ] =
	  (rdata_t) ( u_buf[in_pt_c+c]* b[p*3+0] * fb->my_filt.input_gains[p] +
		      u_buf[in_pt_h1+c]*b[p*3+1] * fb->my_filt.input_gains[p] +
		      u_buf[in_pt_h2+c]*b[p*3+2] * fb->my_filt.input_gains[p] -
		      f_buf[in_pt_h1+c]*a[p*3+1] -
		      f_buf[in_pt_h2+c]*a[p*3+2] );
      }
    }
    // then copy f_buf back into u_buf to prep for the next run
    for(samp = u_curs; samp < u_curs + n_in_samps; samp++){
      out_pt =  samp * n_out_chans;
      in_pt_c = samp * n_out_chans;
      for( c = 0; c < n_out_chans; c++){
	u_buf[ out_pt+c ] = f_buf[ in_pt_c + c ];
      }
    }

  }
  // now the loop over sos'es has ended.  f_buf should be ready.
  // move the cursors to indicate this
  ((fb->u_curs)) += n_in_samps;
  ((fb->f_curs)) += n_in_samps;
  // and reset them to the beginning if it's time to do so.
  if( (fb->u_curs) > n_out_samps ){
    ((fb->u_curs)) = 0;
    ((fb->f_curs)) = 0;
  }

}
