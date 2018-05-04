#ifndef OnX_Main_h
#define OnX_Main_h

// Inheritance :
#include <OnX/Core/Session.h>

#include <vector>
#include <string>

namespace Slash {namespace UI {class IUI;}}
namespace Slash {namespace Core {class ISession;}}

namespace inlib {class args;}

namespace OnX {

class Main : public OnX::Session {
public: //Slash::Core::ISession
  virtual void* cast(const std::string&) const;
public:
  Main(const std::vector<std::string>& args = std::vector<std::string>());
  // deprecated :
  Main(bool create_UI,bool show_UI,const std::vector<std::string>& args);
  virtual ~Main();
private:
  Main(const Main&);
  Main& operator=(const Main&);
private:
  // helpers :
  static std::string findDriver(inlib::args&,bool rm_from_args);
  static bool findFile(std::ostream&,inlib::args&,std::string&);
public:
  Slash::UI::IUI* createUI(const std::string& driver,
                           const std::string& file,
                           bool);
  Slash::UI::IUI* ui() const;
  bool loadInterpreter(const std::string&,bool aQuiet = false) const;
public: //deprecated :
  bool isValid() const;
  int steer();
  void show();
  //Slash::Core::ISession* session() const;
private:
  void initialize(const std::vector<std::string>&);
private:
  bool fBalance;
  bool fStatus;
  bool fIB;
  std::string fTerm;
  std::string fDefaultDriver;
  std::string fDefaultFile;
};

}

#endif
