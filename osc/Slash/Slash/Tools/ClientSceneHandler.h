#ifndef Slash_Tools_ClientSceneHandler_h
#define Slash_Tools_ClientSceneHandler_h

// inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/ISceneHandler.h>

#include <Slash/Core/ISession.h>

#include <inlib/cast>

#include <exlib/iv/send>

namespace Slash {

class ClientSceneHandler
:public virtual Slash::Core::IManager 
,public virtual Slash::UI::ISceneHandler {

public: //IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::ISceneHandler)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //ISceneHandler

  virtual bool setViewer(const std::string& aViewer) {
    fViewer = aViewer;
    return true;
  }

  virtual bool addScene(const std::string& aPlacement,SoNode* aNode) {
    return exlib::iv::send(fSocket,fSession.cout(),fViewer,aPlacement,aNode);
  }

  virtual bool clear(const std::string& aViewer,
                     const std::string& aPlacement) {
    if(!fSocket.send_string(inlib_net_protocol_clear)) {
      std::ostream& out = fSession.cout();
      out << "Slash::ClientSceneHandler::clear :"
          << " send_string() failed."
          << std::endl;
      return false;
    }

    if(!fSocket.send_string(aPlacement)) {
      std::ostream& out = fSession.cout();
      out << "Slash::ClientSceneHandler::clear :"
          << " send_string() failed."
          << std::endl;
      return false;
    }

    return true;
  }

  virtual SoNode* find(const std::string& aViewer,
                       const std::string& aPlacement) {
    return 0;
  }

public:
  inline ClientSceneHandler(Slash::Core::ISession& aSession,
                            const std::string& aName,
                            inlib::net::base_socket& aSocket)
  :fSession(aSession)
  ,fName(aName)
  ,fSocket(aSocket)
  {}

  virtual ~ClientSceneHandler() {}
protected:
  inline ClientSceneHandler(const ClientSceneHandler& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName),fSocket(aFrom.fSocket)
  {}
private:
  inline ClientSceneHandler& operator=(const ClientSceneHandler&){
    return *this;
  }
protected:
  Slash::Core::ISession& fSession;
  std::string fName;
  inlib::net::base_socket& fSocket;
  std::string fViewer;
};

}

#endif
