#ifndef Slash_Store_IFolder_h
#define Slash_Store_IFolder_h

#include <string>

namespace Slash {namespace Store {class IObjectIterator;}}
namespace Slash {namespace Store {class IFolderIterator;}}
namespace Slash {namespace Store {class IFolderVisitor;}}
namespace Slash {namespace Store {class IObject;}}
namespace Slash {namespace Store {class ITree;}}

namespace Slash {

namespace Store {

class IFolder {
public:
  virtual ~IFolder() {}
  virtual void* cast(const std::string&) const = 0;
public:
  virtual IFolder* parent() const = 0;
  virtual std::string name() const = 0;
  virtual void setName(const std::string&) = 0;
  virtual void add(IObject*) = 0;
  virtual void remove(IObject*) = 0;
  virtual void add(IFolder*) = 0;
  virtual void remove(IFolder*) = 0;
  virtual unsigned int numberOfObjects() const = 0;
  virtual unsigned int numberOfFolders() const = 0;
  virtual IObjectIterator* objects() const = 0;
  virtual IFolderIterator* folders() const = 0;
  virtual IObject* findInTree(const std::string&,IFolder*&) const = 0;
  virtual IObject* findObject(const std::string&) const = 0;
  virtual bool isValid(const IObject*) const = 0;
  virtual void clear() = 0;
  virtual IFolder* mkdir(const std::string&) = 0;
  virtual IFolder* findFolder(const std::string&) const = 0;
  virtual void ls(bool) const = 0;
  virtual std::string path() const = 0;
  virtual bool visit(IFolderVisitor&) const = 0;
  virtual void setParent(IFolder*) = 0;
  virtual ITree& tree() = 0;
  // Link methods :
  virtual bool link(IFolder*,const std::string&) = 0;
  virtual bool unlink(IFolder*) = 0;
  virtual IFolder* linkingFolder() const = 0;
  virtual void setLinkingFolder(IFolder*) = 0;
  virtual std::string linkingPath() const = 0;
};

} //Store

} //Slash

#endif
