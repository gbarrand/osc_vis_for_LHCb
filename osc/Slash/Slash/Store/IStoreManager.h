#ifndef Slash_Store_IStoreManager_h
#define Slash_Store_IStoreManager_h

#include <string>
#include <vector>

namespace Slash {namespace Store {class ITree;}}
namespace Slash {namespace Store {class IObject;}}

namespace Slash {

namespace Store {

class IStoreManager {
public:
  virtual ~IStoreManager() {}
public:
  virtual ITree* createTree(const std::string&,
                                bool = false, 
                                bool = false, 
                                const std::string& = "" ) = 0;
  virtual IObject* createObject(ITree&,
                                  const std::string&, //what/class
                                  const std::string&, //path
                                  const std::string&, //name
                                  const std::string&, //title
                                  const std::vector<std::string>&, //args
                                  const std::string& = "") = 0; //opts

  //To create multi dimensional histos.
  typedef std::pair<int, std::pair<double,double> > Range;
  virtual IObject* createObject(ITree&,
                                  const std::string&, //what/class
                                  const std::string&, //path
                                  const std::string&, //name
                                  const std::string&, //title
                                  const std::vector<Range>&, //args
                                  const std::string& = "") = 0; //opts
//public:
//  virtual IObjectReader* findReader(const std::string&) = 0;
//  virtual IObjectWriter* findWriter(const std::string&) = 0;
};

} //Store

} //Slash

#endif
