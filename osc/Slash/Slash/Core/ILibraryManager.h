#ifndef Slash_Core_ILibraryManager_h
#define Slash_Core_ILibraryManager_h

#include <string>

namespace Slash {namespace Core { class ILibrary;}}

namespace Slash {

namespace Core {

class ILibraryManager {
public:
  virtual ~ILibraryManager() {};
public:
  virtual ILibrary* addLibrary(const std::string&,
                               const std::string&,
                               const std::string&,
                               bool = false) = 0;
  virtual ILibrary* findLibrary(const std::string&) = 0;
};

} //Core

} //Slash

#endif
