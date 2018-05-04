#ifndef amanip_axis_h
#define amanip_axis_h

#include <AIDA/IAxis.h>

#include <sstream>

namespace amanip {

inline std::string bin_to_string(int a_index) {
  if(a_index==AIDA::IAxis::UNDERFLOW_BIN) {
    return "UNDERFLOW";
  } else if(a_index==AIDA::IAxis::OVERFLOW_BIN) {
    return "OVERFLOW";
  } else {
    std::ostringstream strm;
    strm << a_index;
    return strm.str();
  }
}

}

#endif
