#ifndef OnX_NSCyclic_h
#define OnX_NSCyclic_h 

#include <OnX/Core/BaseCyclic.h>

@class OnX_NSTimer;

namespace OnX {

class NSCyclic : public BaseCyclic {
public: //Slash::UI::ICyclic
  virtual void start();
  virtual void stop();
  virtual bool started() const;
public:
  NSCyclic(Slash::Core::ISession&,const std::string&,
                 int,const std::string&,const std::string&);
  virtual ~NSCyclic();
private:
  OnX_NSTimer* fTimer;
};

}

#endif

