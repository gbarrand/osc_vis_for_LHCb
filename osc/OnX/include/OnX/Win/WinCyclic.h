#ifndef OnX_WinCyclic_h
#define OnX_WinCyclic_h 

#include <OnX/Core/BaseCyclic.h>

#include <windows.h>

namespace OnX {

class WinCyclic : public BaseCyclic {
public: //Slash::UI::ICyclic
  virtual void start();
  virtual void stop();
  virtual bool started() const;
public:
  WinCyclic(Slash::Core::ISession&,const std::string&,
            int,const std::string&,const std::string&);
  virtual ~WinCyclic();
private:
  static void CALLBACK timerProc(HWND,UINT,UINT_PTR,DWORD);
private:
  UINT_PTR fId;
};

}

#endif

