#ifndef Rio_Date_h
#define Rio_Date_h

namespace Rio {

class Date {
public:
  Date();
  Date(const Date&);
  virtual ~Date();
  Date& operator=(const Date&);
  // Get/set :
  unsigned int date() const;
  int recordSize() const;
  void setDate(unsigned int);
  // Else :
  void set();
private:
  unsigned int fDate; // Date (relative to 1995) + time.
};

}

#endif
