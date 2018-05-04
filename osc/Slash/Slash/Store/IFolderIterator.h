#ifndef Slash_Store_IFolderIterator_h
#define Slash_Store_IFolderIterator_h

namespace Slash {namespace Store {class IFolder;}}

namespace Slash {

namespace Store {

class IFolderIterator {
public:
  virtual ~IFolderIterator() {}
  virtual IFolder* folder() = 0;
  virtual void next() = 0;
};

} //Store

} //Slash

#endif
