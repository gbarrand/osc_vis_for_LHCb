#ifndef Slash_Tools_NetManager_h
#define Slash_Tools_NetManager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Net/INetManager.h>

#include <inlib/cast>

namespace Slash {

class NetManager
:public virtual Slash::Core::IManager 
,public virtual Slash::Net::INetManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const  {
    INLIB_IF_CAST(Slash::Net::INetManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //Slash::UI::INetManager
  virtual inlib::net::base_socket& socket() const {return fSocket;}

public:

  inline NetManager(const std::string& aName,
                    std::ostream& a_out,
                    inlib::net::base_socket& aSocket)
  :fName(aName)
  ,fSocket(aSocket)
  {
    //Lib::Debug::increment("OnX::NetManager");
  }

  virtual ~NetManager() {
    //Lib::Debug::decrement("OnX::NetManager");
  }

private:
  NetManager(const NetManager& aFrom)
  :fSocket(aFrom.fSocket){}
  NetManager& operator=(const NetManager&){return *this;}
private:
  std::string fName;
  inlib::net::base_socket& fSocket;
};

}

#endif




