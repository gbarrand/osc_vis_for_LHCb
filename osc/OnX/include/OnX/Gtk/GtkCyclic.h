#ifndef OnX_GtkCyclic_h
#define OnX_GtkCyclic_h 

#include <OnX/Core/BaseCyclic.h>

#include <gtk/gtk.h>

namespace OnX {

class GtkCyclic : public BaseCyclic {
public: //Slash::UI::ICyclic
  virtual void start();
  virtual void stop();
  virtual bool started() const;
public:
  GtkCyclic(Slash::Core::ISession&,const std::string&,
            int,const std::string&,const std::string&);
  virtual ~GtkCyclic();
  void setID(int);
  bool stopIt() const;
private:
  int fId;
  bool fStopIt;
};

}

#endif

