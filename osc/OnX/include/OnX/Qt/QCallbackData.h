#ifndef OnX_QCallbackData_h
#define OnX_QCallbackData_h 

#include <string>

class QCallbackData {
public:
  QCallbackData():x(0),y(0){}
public:
  std::string value;
  std::string event;
  int x;
  int y;  
};

#endif

