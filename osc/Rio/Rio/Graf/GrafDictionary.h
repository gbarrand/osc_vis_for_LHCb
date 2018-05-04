#ifndef Rio_GrafDictionary_h
#define Rio_GrafDictionary_h

#include <Rio/Core/CoreDictionary.h>

namespace Rio {

class GrafDictionary : public CoreDictionary {
public: //Rio::IDictionary
  virtual bool initialize();

  virtual const IClass& opt_AttLine_Class() const;
  virtual const IClass& opt_AttFill_Class() const;
  virtual const IClass& opt_AttMarker_Class() const;
public:
  GrafDictionary(std::ostream&);
  virtual ~GrafDictionary();

private:
  IClass* f_opt_AttLine_Class;
  IClass* f_opt_AttFill_Class;
  IClass* f_opt_AttMarker_Class;
};

}

#endif

