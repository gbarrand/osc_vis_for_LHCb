#ifndef BatchLab_Rio_ReadContext_h
#define BatchLab_Rio_ReadContext_h

namespace Rio {
  class IDirectory;
  class IObject;
}

#include <string>

namespace BatchLab {

namespace Rio {

class ReadContext {
public:
  ReadContext(std::ostream&,::Rio::IObject&,const std::string&,int);
  virtual ~ReadContext();
public:
  std::ostream& out() const;
  ::Rio::IObject& object() const;
  const std::string& SID() const;
  int verboseLevel() const;
private:
  std::ostream& f_out;
  ::Rio::IObject& fObject;
  std::string fSID;
  int fVerboseLevel;
};

} //Rio

} //BatchLab

#endif
