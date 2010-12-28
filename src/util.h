#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include "assert.h"
#include <stdio.h>


template <class T> 
void parse_line_for_nums(std::string the_line,int n_elem, T *t){
  
  using namespace std;

  istringstream iss;

  iss.str(the_line);

  int n = 0;
  while(! iss.eof()){
    iss >> t[n];
    n+=1;
  }

  if(n != n_elem){
    std::cerr << "While parsing nums from a string, found " << n << " numbers when " << n_elem << " were requested." << std::endl;
    std::cerr << "Maybe an error in arteopt, or in the xml .conf files.  If n_chans, check for n thresh values and n window_heights, etc." << std::endl;
    std::cerr << "Input string:" << the_line << std::endl;
    std::cerr << "(watch out for white spaces?)" << std::endl;
    for(int a = 0; a < n; a++)
      std::cout << "array element is: " << t[a] << std::endl;
  }
  assert(n == n_elem);

}






#endif
