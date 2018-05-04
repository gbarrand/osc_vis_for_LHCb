#ifndef Slash_Net_INetManager_h
#define Slash_Net_INetManager_h

namespace inlib {namespace net {class base_socket;}}

namespace Slash {

namespace Net {

class INetManager {
public:
  virtual ~INetManager() {};
public:
  virtual inlib::net::base_socket& socket() const = 0;
};

} //Net

} //Slash

#endif
