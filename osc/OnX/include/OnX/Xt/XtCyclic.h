#ifndef OnX_XtCyclic_h
#define OnX_XtCyclic_h 

#include <OnX/Core/BaseCyclic.h>
#include <X11/Intrinsic.h>

namespace OnX {

class XtCyclic : public BaseCyclic {
public: //Slash::UI::ICyclic
  virtual void start();
  virtual void stop();
  virtual bool started() const;
public:
  XtCyclic(Slash::Core::ISession&,const std::string&,
           int,const std::string&,const std::string&,XtAppContext);
  virtual ~XtCyclic();
private:
  static void startTimer(XtCyclic*,XtIntervalId*);
private:
  XtAppContext fXtAppContext;
  XtIntervalId fIntervalId;
  bool fStopIt;
};

}

#endif

