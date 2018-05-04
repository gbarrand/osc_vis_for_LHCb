// this :
#include "HDF5_Tools.h"

#include "HDF5_T_Tools.h"

#include <BatchLab/HDF5/Types.h>

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_atb( 
 hid_t aDS //data set
,const std::string& aName
,unsigned int aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int has_attr = H5LT_find_attribute(aDS,aName.c_str());
  if(has_attr==1)  {
    if(H5Adelete(aDS,aName.c_str())<0) return false;
  }

  hid_t aid = H5Acreate(aDS,aName.c_str(),H5T_STD_U32XX,fScalar,H5P_DEFAULT);
  if(aid<0) return false;

  if(H5Awrite(aid,H5T_NATIVE_UINT,&aData)<0) {
    H5Aclose(aid);
    return false;
  }

  H5Aclose(aid);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::read_atb( 
 hid_t aID //data set
,const std::string& aName
,unsigned int& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(H5LT_get_attribute_mem
    (aID,aName.c_str(),H5T_NATIVE_UINT,&aData)<0) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_atb( 
 hid_t aDS //data set
,const std::string& aName
,int aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int has_attr = H5LT_find_attribute(aDS,aName.c_str());
  if(has_attr==1)  {
    if(H5Adelete(aDS,aName.c_str())<0) return false;
  }

  hid_t aid = H5Acreate(aDS,aName.c_str(),H5T_STD_I32XX,fScalar,H5P_DEFAULT);
  if(aid<0) return false;

  if(H5Awrite(aid,H5T_NATIVE_INT,&aData)<0) {
    H5Aclose(aid);
    return false;
  }

  H5Aclose(aid);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::read_atb( 
 hid_t aID //data set
,const std::string& aName
,int& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(H5LT_get_attribute_mem
    (aID,aName.c_str(),H5T_NATIVE_INT,&aData)<0) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_object( 
 hid_t aLocation
,const std::string& aName
,hid_t aFileType
,unsigned int
,char* aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fSimple<0) return false;
  if(fCPT<0) return false;

  hid_t mem_type = compound_mem_type(aFileType);
  if(mem_type<0) return false;

  hid_t dataset = H5Dcreate(aLocation,aName.c_str(),aFileType,fSimple,fCPT);
  if(dataset<0) {
    H5Tclose(mem_type);                 
    return false;
  }

  if(H5Dwrite(dataset,mem_type,H5S_ALL,H5S_ALL,H5P_DEFAULT,aData)<0) {
    H5Dclose(dataset);                 
    H5Tclose(mem_type);                 
    return false;
  }

  H5Dclose(dataset);                 
  H5Tclose(mem_type);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::read_object(
 hid_t aLocation
,const std::string& aName
,unsigned int& aSize
,char*& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aSize = 0;
    aData = 0;
    return false;
  }

  hid_t filetype = H5Dget_type(dataset);
  if(filetype<0) {
    H5Dclose(dataset);
    aSize = 0;
    aData = 0;
    return false;
  }

  H5T_class_t t_class = H5Tget_class(filetype);
  if(t_class!=H5T_COMPOUND) {
    H5Tclose(filetype);
    H5Dclose(dataset);
    aSize = 0;
    aData = 0;
    return false;
  }
  
  size_t sz = H5Tget_size(filetype);

  hid_t memtype = compound_mem_type(filetype);
  if(memtype<0) {
    H5Tclose(filetype);
    H5Dclose(dataset);
    aSize = 0;
    aData = 0;
    return false;
  }

  H5Tclose(filetype);

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Tclose(memtype);
    H5Dclose(dataset);
    aSize = 0;
    aData = 0;
    return false;
  }

  char* buffer = new char[sz];
  if(H5Dread(dataset,memtype,fScalar,dataspace,H5P_DEFAULT,buffer)<0) {
    delete [] buffer;
    H5Sclose(dataspace);
    H5Tclose(memtype);
    H5Dclose(dataset);
    aSize = 0;
    aData = 0;
    return false;
  }

  H5Sclose(dataspace);
  H5Tclose(memtype);
  H5Dclose(dataset);

  aSize = sz;
  aData = buffer;  
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_int(
 hid_t aLocation
,const std::string& aName
,int aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),H5T_STD_I32XX,fScalar,fCompact);
  if(dataset<0) return false;

  if(H5Dwrite(dataset,H5T_NATIVE_INT,H5S_ALL,H5S_ALL,H5P_DEFAULT,&aData)<0) {
    H5Dclose(dataset);                 
    return false;
  }

  H5Dclose(dataset);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_uint(
 hid_t aLocation
,const std::string& aName
,unsigned int aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),H5T_STD_U32XX,fScalar,fCompact);
  if(dataset<0) return false;

  if(H5Dwrite(dataset,H5T_NATIVE_UINT,H5S_ALL,H5S_ALL,H5P_DEFAULT,&aData)<0) {
    H5Dclose(dataset);                 
    return false;
  }

  H5Dclose(dataset);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_double(
 hid_t aLocation
,const std::string& aName
,double aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),H5T_IEEE_F64XX,fScalar,fCompact);
  if(dataset<0) return false;

  if(H5Dwrite
       (dataset,H5T_NATIVE_DOUBLE,H5S_ALL,H5S_ALL,H5P_DEFAULT,&aData)<0) {
    H5Dclose(dataset);                 
    return false;
  }

  H5Dclose(dataset);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_float(
 hid_t aLocation
,const std::string& aName
,float aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),H5T_IEEE_F32XX,fScalar,fCompact);
  if(dataset<0) return false;

  if(H5Dwrite
       (dataset,H5T_NATIVE_FLOAT,H5S_ALL,H5S_ALL,H5P_DEFAULT,&aData)<0) {
    H5Dclose(dataset);                 
    return false;
  }

  H5Dclose(dataset);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_bool(
 hid_t aLocation
,const std::string& aName
,bool aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),H5T_STD_U8XX,fScalar,fCompact);
  if(dataset<0) return false;

  unsigned char data = aData?1:0;
  if(H5Dwrite
       (dataset,H5T_NATIVE_UCHAR,H5S_ALL,H5S_ALL,H5P_DEFAULT,&data)<0) {
    H5Dclose(dataset);                 
    return false;
  }

  H5Dclose(dataset);                 
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_string(
 hid_t aLocation
,const std::string& aName
,const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
// From H5LTmakge_dataset_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t filetype = string_datatype(aString.size()+1);
  if(filetype<0) return false;

  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),filetype,fScalar,fCompact);
  if(dataset<0) {
    H5Tclose(filetype);
    return false;
  }

  hid_t memtype = filetype;
  if(H5Dwrite
       (dataset,memtype,H5S_ALL,H5S_ALL,H5P_DEFAULT,aString.c_str())<0) {
    H5Dclose(dataset);                 
    H5Tclose(filetype);
    return false;
  }

  H5Dclose(dataset);                 
  H5Tclose(filetype);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_string(
 hid_t aLocation
,const std::string& aName
,const std::vector<std::string>& aArray
)
//////////////////////////////////////////////////////////////////////////////
// From H5LTmake_dataset_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = aArray.size();
  unsigned int sz = 0;
  for(unsigned int index=0;index<number;index++) 
    sz += aArray[index].size()+1;
  sz++;

  hid_t filetype = string_datatype(sz);
  if(filetype<0) return false;

  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),filetype,fScalar,H5P_DEFAULT);
  if(dataset<0) {
    H5Tclose(filetype);
    return false;
  }

  char* buffer = new char[sz];
  if(!put_array_string(aArray,buffer)) {
    delete [] buffer;
    H5Dclose(dataset);                 
    H5Tclose(filetype);
  }

  hid_t memtype = filetype;
  if(H5Dwrite(dataset,memtype,H5S_ALL,H5S_ALL,H5P_DEFAULT,buffer)<0) {
    delete [] buffer;
    H5Dclose(dataset);                 
    H5Tclose(filetype);
    return false;
  }

  delete [] buffer;
  H5Dclose(dataset);                 
  H5Tclose(filetype);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::read_string(
 hid_t aLocation
,const std::string& aName
,std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
// From H5LTread_dataset_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aString.clear();
    return false; // data set not found.
  }

  hid_t filetype = H5Dget_type(dataset);
  if(filetype<0) {
    H5Dclose(dataset);
    aString.clear();
    return false;
  }

  H5T_class_t t_class = H5Tget_class(filetype);
  if(t_class!=H5T_STRING) {
    H5Tclose(filetype);
    H5Dclose(dataset);
    aString.clear();
    return false;
  }
  
  size_t sz = H5Tget_size(filetype);
  H5Tclose(filetype);

  // We could have use filetype since, for string, 
  // file type is the same than memory type.
  hid_t memtype = string_datatype(sz);
  if(memtype<0) {
    H5Dclose(dataset);
    aString.clear();
    return false;
  }

  char* buff = new char[sz];
  herr_t stat = H5Dread(dataset,memtype,H5S_ALL,H5S_ALL,H5P_DEFAULT,buff);
  if(stat<0) {
    delete [] buff;
    H5Tclose(memtype);
    H5Dclose(dataset);
    aString.clear();
    return false;
  }

  size_t len = sz-1;
  aString.resize(len,0);
  for(unsigned int index=0;index<len;index++) aString[index] = buff[index];

  delete [] buff;
  H5Tclose(memtype);
  H5Dclose(dataset);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::read_array_string(
 hid_t aLocation
,const std::string& aName
,std::vector<std::string>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aArray.clear();
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) return false;

  hid_t filetype = H5Dget_type(dataset);
  if(filetype<0) {
    H5Dclose(dataset);
    return false;
  }

  H5T_class_t t_class = H5Tget_class(filetype);
  if(t_class!=H5T_STRING) {
    H5Tclose(filetype);
    H5Dclose(dataset);
    return false;
  }
  
  size_t sz = H5Tget_size(filetype);
  H5Tclose(filetype);

  // We could have use filetype since, for string, 
  // file type is the same than memory type.
  hid_t memtype = string_datatype(sz);
  if(memtype<0) {
    H5Dclose(dataset);
    return false;
  }

  char* buffer = new char[sz];
  herr_t stat = H5Dread(dataset,memtype,H5S_ALL,H5S_ALL,H5P_DEFAULT,buffer);
  if(stat<0) {
    delete [] buffer;
    H5Tclose(memtype);
    H5Dclose(dataset);
    return false;
  }

  if(!put_array_string(sz,buffer,aArray)) {
    delete [] buffer;
    H5Tclose(memtype);
    H5Dclose(dataset);
    return false;
  }

  delete [] buffer;
  H5Tclose(memtype);
  H5Dclose(dataset);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::put_array_string(
 const std::vector<std::string>& aArray
,char* aPos
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = aArray.size();
  for(unsigned int index=0;index<number;index++) {
    const std::string& s = aArray[index];
    unsigned int array_size = (s.size()+1)*LIB_SIZE_CHAR;
    const char* data = s.c_str();
    ::memcpy(aPos,data,array_size);
    aPos += array_size;
  }
  *aPos = '\0';
  aPos++;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::put_array_string(
 unsigned int aSize
,char* aPos
,std::vector<std::string>& aArray
)
//////////////////////////////////////////////////////////////////////////////
// Exa : if ab0cde00, then aArray should contain two strings ab and cde.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aArray.clear();
  char* begin = aPos;
  unsigned int number = aSize-1;
  for(unsigned int index=0;index<number;index++) {
    if((*aPos)=='\0') {
      unsigned int l = aPos - begin;
      std::string s(l,0);
      char* data = (char*)s.c_str();
      ::memcpy(data,begin,l);
      //printf("debug : xx \"%s\"\n",s.c_str());
      aArray.push_back(s);
      begin = aPos+1;
    }
    aPos++;
  }
  aPos++;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_int(
 hid_t aLocation
,const std::string& aName
,unsigned int aSize
,const int aArray[]
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<int>
    (aLocation,aName,H5T_STD_I32XX,H5T_NATIVE_INT,fCompress,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_int(
 hid_t aLocation
,const std::string& aName
,const std::vector<int>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<int>
    (aLocation,aName,H5T_STD_I32XX,H5T_NATIVE_INT,fCompress,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_uchar(
 hid_t aLocation
,const std::string& aName
,const std::vector<unsigned char>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<unsigned char>
    (aLocation,aName,H5T_STD_U8XX,H5T_NATIVE_UCHAR,fCompress,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_double(
 hid_t aLocation
,const std::string& aName
,unsigned int aSize
,const double aArray[]
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<double>
    (aLocation,aName,H5T_IEEE_F64XX,H5T_NATIVE_DOUBLE,fCompress,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_double(
 hid_t aLocation
,const std::string& aName
,const std::vector<double>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<double>
    (aLocation,aName,H5T_IEEE_F64XX,H5T_NATIVE_DOUBLE,fCompress,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_float(
 hid_t aLocation
,const std::string& aName
,unsigned int aSize
,const float aArray[]
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<float>
    (aLocation,aName,H5T_IEEE_F32XX,H5T_NATIVE_FLOAT,fCompress,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::Helper::write_array_float(
 hid_t aLocation
,const std::string& aName
,const std::vector<float>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return write_array<float>
    (aLocation,aName,H5T_IEEE_F32XX,H5T_NATIVE_FLOAT,fCompress,aArray);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_int(
 hid_t aLocation
,const std::string& aName
,unsigned int& aSize
,int*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<int>(aLocation,aName,H5T_NATIVE_INT,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_int(
 hid_t aLocation
,const std::string& aName
,std::vector<int>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<int>(aLocation,aName,H5T_NATIVE_INT,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_int(
 hid_t aLocation
,const std::string& aName
,int& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_scalar<int>(aLocation,aName,H5T_NATIVE_INT,aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_uint(
 hid_t aLocation
,const std::string& aName
,unsigned int& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_scalar<unsigned int>(aLocation,aName,H5T_NATIVE_UINT,aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_uchar(
 hid_t aLocation
,const std::string& aName
,std::vector<unsigned char>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<unsigned char>(aLocation,aName,H5T_NATIVE_UCHAR,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_bool(
 hid_t aLocation
,const std::string& aName
,bool& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned char value = 0;
  if(!read_scalar<unsigned char>(aLocation,aName,H5T_NATIVE_UCHAR,value)) {
    aValue = false;
    return false;
  }
  if((value!=0) && (value!=1)) {
    aValue = false;
    return false;
  }
  aValue = (value==1?true:false);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_double(
 hid_t aLocation
,const std::string& aName
,unsigned int& aSize
,double*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<double>(aLocation,aName,H5T_NATIVE_DOUBLE,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_double(
 hid_t aLocation
,const std::string& aName
,double& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_scalar<double>(aLocation,aName,H5T_NATIVE_DOUBLE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_double(
 hid_t aLocation
,const std::string& aName
,std::vector<double>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<double>(aLocation,aName,H5T_NATIVE_DOUBLE,aArray);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_float(
 hid_t aLocation
,const std::string& aName
,unsigned int& aSize
,float*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<float>(aLocation,aName,H5T_NATIVE_FLOAT,aSize,aArray);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_float(
 hid_t aLocation
,const std::string& aName
,float& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_scalar<float>(aLocation,aName,H5T_NATIVE_FLOAT,aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::read_array_float(
 hid_t aLocation
,const std::string& aName
,std::vector<float>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return read_array<float>(aLocation,aName,H5T_NATIVE_FLOAT,aArray);
}
