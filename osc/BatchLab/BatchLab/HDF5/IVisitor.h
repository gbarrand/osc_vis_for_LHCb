#ifndef BatchLab_HDF5_IVisitor_h
#define BatchLab_HDF5_IVisitor_h

namespace BatchLab {
namespace HDF5 {

class IVisitor {
public:
  virtual ~IVisitor() {}
public:
  virtual bool dataSet(hid_t,const std::string&) = 0;
  virtual bool beginGroup(hid_t,const std::string&) = 0;
  virtual bool endGroup(hid_t,const std::string&) = 0;
};

}}

#endif
