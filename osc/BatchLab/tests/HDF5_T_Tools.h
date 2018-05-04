#ifndef BatchLab_HDF5_T_Tools_h
#define BatchLab_HDF5_T_Tools_h

#include <string>

#include <hdf5.h>

namespace BatchLab {

namespace HDF5 {

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool write_array(
 hid_t aLocation
,const std::string& aName
,hid_t aFileType
,hid_t aMemType
,int aCompress
,unsigned int aSize
,const T aArray[]
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int sz = aSize;
  int dimn = 1;
  hsize_t* dims = new hsize_t[dimn];
  dims[0] = sz;
  hid_t dataspace = H5Screate_simple(dimn,dims,NULL); 
  delete [] dims;
  if(dataspace<0) return false;

  hid_t cpt = -1;
  if(aCompress) {
    cpt = H5Pcreate(H5P_DATASET_CREATE);
    if(cpt<0) {
      H5Sclose(dataspace);
      return false;
    }
    if(H5Pset_layout(cpt,H5D_COMPACT)<0) { 
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
    hsize_t* cdims = new hsize_t[dimn];
    cdims[0] = (sz<=20?sz:20);
    if(H5Pset_chunk(cpt,dimn,cdims)<0) {
      delete [] cdims;
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
    delete [] cdims;
    if(H5Pset_deflate(cpt,aCompress)<0) {
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
  } else {
    cpt = H5P_DEFAULT;
  }

  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),aFileType,dataspace,cpt);
  if(dataset<0) {
    if(aCompress) H5Pclose(cpt);
    H5Sclose(dataspace);
    return false;
  }

  if(H5Dwrite(dataset,aMemType,H5S_ALL,H5S_ALL,H5P_DEFAULT,aArray)<0) {
    H5Dclose(dataset);                 
    if(aCompress) H5Pclose(cpt);
    H5Sclose(dataspace);
    return false;
  }

  H5Dclose(dataset);                 
  if(aCompress) H5Pclose(cpt);
  H5Sclose(dataspace);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool write_array(
 hid_t aLocation
,const std::string& aName
,hid_t aFileType
,hid_t aMemType
,int aCompress
,const std::vector<T>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int sz = aArray.size();
  int dimn = 1;
  hsize_t* dims = new hsize_t[dimn];
  dims[0] = sz;
  hid_t dataspace = H5Screate_simple(dimn,dims,NULL); 
  delete [] dims;
  if(dataspace<0) return false;

  hid_t cpt = -1;
  if(aCompress) {
    cpt = H5Pcreate(H5P_DATASET_CREATE);
    if(cpt<0) {
      H5Sclose(dataspace);
      return false;
    }
    if(H5Pset_layout(cpt,H5D_COMPACT)<0) { 
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
    hsize_t* cdims = new hsize_t[dimn];
    cdims[0] = (sz<=20?sz:20);
    if(H5Pset_chunk(cpt,dimn,cdims)<0) {
      delete [] cdims;
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
    delete [] cdims;
    if(H5Pset_deflate(cpt,aCompress)<0) {
      H5Pclose(cpt);
      H5Sclose(dataspace);
      return false;
    }
  } else {
    cpt = H5P_DEFAULT;
  }
  
  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),aFileType,dataspace,cpt);
  if(dataset<0) {
    if(aCompress) H5Pclose(cpt);
    H5Sclose(dataspace);
    return false;
  }

  const T* data = &(aArray[0]);
  if(H5Dwrite(dataset,aMemType,H5S_ALL,H5S_ALL,H5P_DEFAULT,data)<0) {
    H5Dclose(dataset);                 
    if(aCompress) H5Pclose(cpt);
    H5Sclose(dataspace);
    return false;
  }

  H5Dclose(dataset);                 
  if(aCompress) H5Pclose(cpt);
  H5Sclose(dataspace);
  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool write_array_struct(
 hid_t aLocation
,const std::string& aName
,hid_t aCreateType
,hid_t aWriteType
,unsigned int aSize
,const T aArray[]
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int dimn = 1;
  hsize_t* dims = new hsize_t[dimn];
  dims[0] = aSize;
  hid_t dataspace = H5Screate_simple(dimn,dims,NULL); 
  delete [] dims;
  if(dataspace<0) return false;

  hid_t dataset = 
    H5Dcreate(aLocation,aName.c_str(),aCreateType,dataspace,H5P_DEFAULT);
  if(dataset<0) {
    H5Sclose(dataspace);
    return false;
  }

  if(H5Dwrite(dataset,aWriteType,H5S_ALL,H5S_ALL,H5P_DEFAULT,aArray)<0) {
    H5Dclose(dataset);                 
    H5Sclose(dataspace);
    return false;
  }

  H5Dclose(dataset);                 
  H5Sclose(dataspace);

  return true;
}
*/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_scalar(
 hid_t aLocation
,const std::string& aName
,hid_t aMemType
,T& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) return false;

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    return false;
  }

  hid_t memspace = H5Screate(H5S_SCALAR); 
  if(memspace<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    return false;
  }

  if(H5Dread(dataset,aMemType,memspace,dataspace,H5P_DEFAULT,&aData)<0) {
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    return false;
  }


  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_array(
 hid_t aLocation
,const std::string& aName
,hid_t aMemType
,unsigned int& aSize
,T*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aSize = 0;
    aArray = 0;
    return false; // data set not found.
  }

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  int dimn = H5Sget_simple_extent_ndims(dataspace);
  if(dimn<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }
  if(dimn!=1) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }
  //printf("debug : read dimn %d\n",dimn);

  hsize_t* dims = new hsize_t[dimn];
 {int rdimn = H5Sget_simple_extent_dims(dataspace,dims,NULL);
  if(rdimn<0) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }}

  aSize = (unsigned int)dims[0];
  if(!aSize) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return true; //It is ok.
  }

  hid_t memspace = H5Screate_simple(dimn,dims,NULL); 
  delete [] dims;
  if(memspace<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  aArray = new T[aSize];
  if(H5Dread(dataset,aMemType,memspace,dataspace,H5P_DEFAULT,aArray)<0) {
    delete [] aArray;
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }


  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_sub_array(
 hid_t aLocation
,const std::string& aName
,hid_t aMemType
,unsigned int aOffset
,unsigned int aNumber
,unsigned int& aSize
,T*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aSize = 0;
    aArray = 0;
    return false; // data set not found.
  }

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  int dimn = H5Sget_simple_extent_ndims(dataspace);
  if(dimn<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }
  if(dimn!=1) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }
  //printf("debug : read dimn %d\n",dimn);

  hsize_t* dims = new hsize_t[dimn];
 {int rdimn = H5Sget_simple_extent_dims(dataspace,dims,NULL);
  if(rdimn<0) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }}

  unsigned int sz = (unsigned int)dims[0];
  if(!sz) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return true; //It is ok.
  }
  delete [] dims;
  //  abcdef
  //  012345 
  int remain = sz-aOffset;
  int number  = (aNumber<=remain) ? aNumber : remain;
  if(number<=0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return true; //It is ok.
  }

 {hsize_t offset[1];
  offset[0] = aOffset;
  hsize_t count[1];
  count[0] = number;
  if(H5Sselect_hyperslab(dataspace,H5S_SELECT_SET,offset,NULL,count,NULL)<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }}

  dims = new hsize_t[1];
  dims[0] = number;
  hid_t memspace = H5Screate_simple(1,dims,NULL); 
  delete [] dims;
  if(memspace<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  aArray = new T[number];
  if(H5Dread(dataset,aMemType,memspace,dataspace,H5P_DEFAULT,aArray)<0) {
    delete [] aArray;
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray = 0;
    return false;
  }

  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_array(
 hid_t aLocation
,const std::string& aName
,hid_t aMemType
,std::vector<T>& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aArray.clear();
    return false; // data set not found.
  }

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }

  int dimn = H5Sget_simple_extent_ndims(dataspace);
  if(dimn<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }
  if(dimn!=1) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }
  //printf("debug : read dimn %d\n",dimn);

  hsize_t* dims = new hsize_t[dimn];
 {int rdimn = H5Sget_simple_extent_dims(dataspace,dims,NULL);
  if(rdimn<0) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }}

  unsigned int sz = (unsigned int)dims[0];
  if(!sz) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return true; //It is ok.
  }

  hid_t memspace = H5Screate_simple(dimn,dims,NULL); 
  delete [] dims;
  if(memspace<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }

  aArray.resize(sz);
  T* data = (T*)&(aArray[0]);
  if(H5Dread(dataset,aMemType,memspace,dataspace,H5P_DEFAULT,data)<0) {
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aArray.clear();
    return false;
  }


  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_array_struct(
 hid_t aLocation
,const std::string& aName
,hid_t aReadType
,unsigned int& aSize
,T*& aArray
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) {
    aSize = 0;
    aArray = 0;
    return false; // data set not found.
  }

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  int dimn = H5Sget_simple_extent_ndims(dataspace);
  if(dimn<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }
  //printf("debug : read dimn %d\n",dimn);

  hsize_t* dims = new hsize_t[dimn];
 {int rdimn = H5Sget_simple_extent_dims(dataspace,dims,NULL);
  if(rdimn<0) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }}

  hid_t memspace = H5Screate_simple(dimn,dims,NULL); 
  if(memspace<0) {
    delete [] dims;
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  aSize = (unsigned int)dims[0];
  delete [] dims;
  if(!aSize) {
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  aArray = new T[aSize];
  if(H5Dread(dataset,aReadType,memspace,dataspace,H5P_DEFAULT,aArray)<0) {
    delete [] aArray;
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    aSize = 0;
    aArray = 0;
    return false;
  }

  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);

  return true;
}
*/
//////////////////////////////////////////////////////////////////////////////
template <class T>
bool read_struct(
 hid_t aLocation
,const std::string& aName
,hid_t aReadType
,T& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t dataset = H5Dopen(aLocation,aName.c_str());
  if(dataset<0) return false;

  hid_t dataspace = H5Dget_space(dataset);
  if(dataspace<0) {
    H5Dclose(dataset);
    return false;
  }

  hid_t memspace = H5Screate(H5S_SCALAR);
  if(memspace<0) {
    H5Sclose(dataspace);
    H5Dclose(dataset);
    return false;
  }

  if(H5Dread(dataset,aReadType,memspace,dataspace,H5P_DEFAULT,&aData)<0) {
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    return false;
  }

  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);
  return true;
}
  
} //HDF5

} //BatchLab

#endif
