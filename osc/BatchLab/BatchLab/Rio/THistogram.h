#ifndef BatchLab_Rio_THistogram_h
#define BatchLab_Rio_THistogram_h

// Inheritance :
#include <Rio/Core/Named.h>

namespace BatchLab {class BaseHistogram;}

namespace BatchLab {

namespace Rio {

class THistogram : public ::Rio::Named {
  RIO_META_HEADER(THistogram)
public:
public: //::Rio::IObject:
  virtual void* cast(const ::Rio::IClass&) const;
  virtual bool stream(::Rio::IBuffer&);
public:
  THistogram(const ::Rio::IDictionary&,std::ostream&);
  THistogram(const ::Rio::IDictionary&,const std::string&,
            const BatchLab::BaseHistogram&);
  virtual ~THistogram();
public:
  BatchLab::BaseHistogram* data() const;
private:
  std::ostream& f_out;
  BatchLab::BaseHistogram* fData;
};

} //Rio
} //BatchLab

#endif
