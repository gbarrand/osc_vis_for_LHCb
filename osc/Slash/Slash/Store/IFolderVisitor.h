#ifndef Slash_Store_IFolderVisitor_h
#define Slash_Store_IFolderVisitor_h

namespace Slash {namespace Store {class IObject;}}
namespace Slash {namespace Store {class IFolder;}}

namespace Slash {

namespace Store {

class IFolderVisitor {
public:
  virtual ~IFolderVisitor() {}
  /*
   * @return False stop the traversal.
   */
  virtual bool visit(const IFolder&,const IObject&) = 0;
  virtual bool beginVisit(const IFolder&,const IFolder&) = 0;
  virtual bool endVisit(const IFolder&,const IFolder&) = 0;
  virtual bool isRecursive() const = 0;
};

} //Store

} //Slash

#endif
