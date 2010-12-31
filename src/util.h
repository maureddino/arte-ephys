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

// Template functions can't have their interface and implementation separated.
// So please don't move these fns to util.cpp
// Thanks

// take a string which haz between 1 and n space-delimited values
// that need to get written into an array.
// pass the string to parse as arg 1, a pointer to the array to populate as t
// pass expected number of elements in n_elem, or -1 to tell the function not to check
template <class T> 
void parse_line_for_vals(std::string the_line,T *t, int n_elem){
  
  using namespace std;

  istringstream iss;

  iss.str(the_line);

  // I'm confused about this.  Should the caller do it
  // instead of having us do it here?
  // If we're doing it, how does the caller's scope know
  // how big this array should have been?
  if(n_elem > 0)
    t = new T [n_elem];
  else
    t = new T [1];

  // counting this way seems to be sensitive to white spaces at 
  // beginning and end of string.  Can this be fixed?
  // (try it with test_util.cpp)
  int n = 0;
  while(! iss.eof()){
    iss >> t[n];
    n+=1;
  }

  // only do this check if we care about the n_elem specification
  if(n_elem == -1){
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
}

// Populate a trode's fields with either the value given by the property tree,
// or, if property tree has no info for this trode, use the default value
// To specify that the trode being populated _IS_ the default trode, pass NULL
// as the last argument.
// var is a pointer to the member variable to be set.  this_trode_ptree is a ptree
// node corresponding to only this tetrode.  tree_key is a string key into the ptree
// to the target parameter. default_var is NULL or a pointer to the default value to use
// in the case that tree_key into ptree doesn't give a value.
template <class T>
int assign_trode_property(std::string tree_key, boost::property_tree::ptree this_trode_pt, T *t, boost::property_tree::ptree default_trode_pt, int n_elem){

  // How do you check for failure when searching a ptree?
  std::istringstream iss;
  try{
    iss.str(this_trode_pt.second.get<std::string>(tree_key));
  }
  catch(...){
    iss.str(default_trode_pt.second.get<std::string>(tree_key));
  }

  parse_line_for_vals(iss.str(), t, n_elem); 

  //iss.chr(this_trode_ptree

  //if(default_var == NULL){  // then we're popu

}




#endif