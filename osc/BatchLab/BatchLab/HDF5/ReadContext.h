#ifndef BatchLab_HDF5_ReadContext_h
#define BatchLab_HDF5_ReadContext_h

#include <exlib/hdf5/hdf5_h>

#include <string>
#include <ostream>

namespace BatchLab {namespace HDF5 {class HDF5O;}}

namespace BatchLab {

namespace HDF5 {

class ReadContext {
public:
  ReadContext(int,std::ostream&,
              hid_t,const std::string&,int,
              HDF5O&);
  virtual ~ReadContext();
public:
  int verboseLevel() const;
  std::ostream& out() const;
  hid_t id() const;
  std::string name() const;
  int version() const;
  HDF5O& helper() const;
private:
  int fVerboseLevel;
  std::ostream& f_out;
  hid_t fID;
  std::string fName;
  int fVersion;
  HDF5O& fHDF5O;
};

} //HDF5
 
} //BatchLab

#endif
