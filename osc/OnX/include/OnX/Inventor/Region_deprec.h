#ifndef OnX_Region_h
#define OnX_Region_h

#include <OnX/Interfaces/IRegion_deprec.h>

class SoRegion;

namespace OnX {

class Region : public virtual IRegion {
public: // IRegion :
  virtual void* cast(const std::string&) const;
  virtual bool setSize(double,double);
  virtual bool setPosition(double,double);
  virtual bool setSizePixel(unsigned int,unsigned int);
  virtual bool setPositionPixel(int,int);
  virtual bool size(double&,double&) const;
  virtual bool position(double&,double&) const;
  virtual bool clear(const std::string& = "");
  virtual bool refresh();
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
  virtual bool toggleParameter(const std::string&);
  virtual bool addRepresentation(IRepresentation*,const std::string&);
  virtual IRepresentation* findRepresentation(const std::string&,
                                              const std::string&);
  virtual void undo();
  virtual void resetUndo();
  virtual bool connectFrom(const IRegion&);
  virtual void* nativeRegion() const;
  virtual std::string nativeClass() const;
public:
  Region(SoRegion*);
  Region(const Region&);
  Region& operator=(const Region&);
  virtual ~Region();
private:
  SoRegion* fSoRegion;
};

}

#endif
