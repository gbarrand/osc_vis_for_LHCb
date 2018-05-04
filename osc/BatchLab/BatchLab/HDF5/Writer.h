#ifndef BatchLab_HDF5_Writer_h
#define BatchLab_HDF5_Writer_h

#include <exlib/hdf5/hdf5_h>

#include <string>

namespace Slash {namespace Store {class IStorable;}}
namespace BatchLab {namespace HDF5 {class HDF5O;}}

namespace BatchLab {

namespace HDF5 {

 bool hdf5_writer(hid_t,const std::string&,
                  const Slash::Store::IStorable&,
                  BatchLab::HDF5::HDF5O&);

}}

#endif
