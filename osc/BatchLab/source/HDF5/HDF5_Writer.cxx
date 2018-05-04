// this :
#include <BatchLab/HDF5/Writer.h>

#include <Slash/Store/IStorable.h>

#include <Lib/ObjectWriter.h>

#include <BatchLab/HDF5/HDF5O.h>
#include <BatchLab/HDF5/MemberInserter.h>

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::hdf5_writer(
 hid_t aID
,const std::string& aName
,const Slash::Store::IStorable& aObject
,BatchLab::HDF5::HDF5O& aHDF5O
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  BatchLab::HDF5::MemberInserter mInserter;
  if(!aObject.visit(mInserter)) return false;
  hid_t file_type = mInserter.compound();
  if(file_type<0) return false;
  unsigned int sz = mInserter.offset();
  if(!sz) {
    H5Tclose(file_type);
    return false;
  }

  char* buffer = new char[sz];
  char* p = buffer;
  char* end = buffer+sz;

  Lib::ObjectWriter mWriter(p,end);
  if(!aObject.visit(mWriter)) {
    H5Tclose(file_type);
    return false;
  }
  if(p!=end) {
    delete [] buffer;
    H5Tclose(file_type);
    return false;
  }
  if(!aHDF5O.write_object(aID,aName.c_str(),file_type,sz,buffer)) {
    delete [] buffer;
    H5Tclose(file_type);
    return false;
  }
  delete [] buffer;
  H5Tclose(file_type);
  return true;  
}
