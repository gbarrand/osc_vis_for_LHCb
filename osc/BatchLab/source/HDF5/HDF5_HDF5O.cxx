// this :
#include <BatchLab/HDF5/HDF5O.h>

#include <BatchLab/HDF5/Types.h>

#include <H5LT.h>

#include <cstdlib>

#define Failure (-1)


#ifdef WIN32  
// FIXME : 
//   The below is not exported in the DLL (internal methods).
//  We copy the code here and make them static.
static herr_t H5LT_get_attribute_mem( hid_t obj_id,
                           const char *attr_name,
                           hid_t mem_type_id,
                           void *data ) {
 hid_t attr_id;
 if ( ( attr_id = H5Aopen_name( obj_id, attr_name ) ) < 0 )  return -1;
 if ( H5Aread( attr_id, mem_type_id, data ) < 0 ) {
   H5Aclose( attr_id );
   return -1;
 }
 if ( H5Aclose( attr_id ) < 0 )  return -1;
 return 0;
}
static herr_t find_attr( hid_t loc_id, const char *name, void *op_data) {
 int ret = 0;
 char *attr_name = (char*)op_data;
 loc_id=loc_id;
 if( strcmp( name, attr_name ) == 0 ) ret = 1;
 return ret;
}
static herr_t H5LT_find_attribute( hid_t loc_id, const char* attr_name ) {
 unsigned int attr_num;
 attr_num = 0;
 return H5Aiterate( loc_id, &attr_num, find_attr, (void *)attr_name );
}
static herr_t H5LT_get_attribute_disk( hid_t loc_id,
                          const char *attr_name,
                          void *attr_out ) {
 hid_t      attr_id;
 if ( ( attr_id = H5Aopen_name( loc_id, attr_name ) ) < 0 )  return -1;

 hid_t      attr_type;
 if ( (attr_type = H5Aget_type( attr_id )) < 0 ) {
   H5Tclose( attr_type );
   H5Aclose( attr_id );
   return -1;
 }

 if ( H5Aread( attr_id, attr_type, attr_out ) < 0 ) {
   H5Tclose( attr_type );
   H5Aclose( attr_id );
   return -1;
 }

 if ( H5Tclose( attr_type )  < 0 ) {
   H5Tclose( attr_type );
   H5Aclose( attr_id );
   return -1;
 }

 if ( H5Aclose( attr_id ) < 0 )  return -1;;

 return 0;

}
#endif

//////////////////////////////////////////////////////////////////////////////
BatchLab::HDF5::HDF5O::HDF5O(
 int aLevel
)
:fCompress(1)
,fScalar(Failure)
,fSimple(Failure)
,fCPT(Failure)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aLevel<0) aLevel = 0;
  if(aLevel>9) aLevel = 9;
  fCompress = aLevel;

  fScalar = H5Screate(H5S_SCALAR);
  if(fScalar<0) {
    fScalar = Failure;
  }

  // For write_object :
 {hsize_t sz = 1;
  fSimple = H5Screate_simple(1,&sz,NULL);
  if(fSimple<0) {
     fSimple = Failure;
  }}

  if(fCompress) {
    fCPT = H5Pcreate(H5P_DATASET_CREATE);
    if(fCPT<0) {
      fCPT = Failure;
    } else {
      if(H5Pset_layout(fCPT,H5D_COMPACT)<0) { 
        H5Pclose(fCPT);
        fCPT = Failure;
      } else {
        hsize_t sz = 1;
        if(H5Pset_chunk(fCPT,1,&sz)<0) {
          H5Pclose(fCPT);
          fCPT = Failure;
        } else {
          if(H5Pset_deflate(fCPT,fCompress)<0) {
            H5Pclose(fCPT);
            fCPT = Failure;
          }
        }
      }
    }
  } else {
    fCPT = H5Pcopy(H5P_DEFAULT);
  }
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::HDF5::HDF5O::~HDF5O(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fScalar>=0) H5Sclose(fScalar);

  if(fSimple>=0) H5Sclose(fSimple);
  if(fCPT>=0) H5Pclose(fCPT);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::HDF5O::checkSizes(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(sizeof(bool)!=1) return false;
  if(sizeof(char)!=1) return false;
  if(sizeof(short)!=2) return false;
  if(sizeof(int)!=4) return false;
  if(sizeof(float)!=4) return false;
  if(sizeof(double)!=8) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
static herr_t file_info(
 hid_t aID
,const char* aName
,void* //aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  H5G_stat_t statbuf;
  H5Gget_objinfo(aID,aName,0,&statbuf);
  switch(statbuf.type) {
  case H5G_GROUP:{
    printf(" file_info : %d \"%s\"\n",aID,aName);
    printf(" Object with name %s is a group : begin\n", aName);
    //int retval = 
    H5Giterate(aID,aName, NULL,file_info,0);
    printf(" Object with name %s is a group : end\n", aName);
  }break;
  case H5G_DATASET: 
    printf(" file_info : %d \"%s\"\n",aID,aName);
    printf(" Object with name %s is a dataset \n", aName);
    break;
  case H5G_TYPE: 
    printf(" file_info : %d \"%s\"\n",aID,aName);
    printf(" Object with name %s is a named datatype \n", aName);
    break;
  default:
    printf(" file_info : %d \"%s\"\n",aID,aName);
    printf(" Unable to identify an object ");
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::HDF5::HDF5O::dump(
 hid_t aID
,const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  H5Giterate(aID,aName.c_str(), NULL,file_info,0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
hid_t BatchLab::HDF5::HDF5O::basic_mem_type(
 hid_t aFileType
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  H5T_class_t mclass = H5Tget_class(aFileType);
  size_t msize = H5Tget_size(aFileType);
  if(mclass==H5T_INTEGER) {
    H5T_sign_t msign = H5Tget_sign(aFileType);
    if(msize==1) {
      if(msign==H5T_SGN_NONE) {
        return H5Tcopy(H5T_NATIVE_UCHAR);
      } else {
        return H5Tcopy(H5T_NATIVE_CHAR);
      }
    } else if(msize==4) {
      if(msign==H5T_SGN_NONE) {
        return H5Tcopy(H5T_NATIVE_UINT);
      } else {
        return H5Tcopy(H5T_NATIVE_INT);
      }
    } else {
      return Failure;
    }
  } else if(mclass==H5T_FLOAT) {
    if(msize==4) {
      return H5Tcopy(H5T_NATIVE_FLOAT);
    } else if(msize==8) {
      return H5Tcopy(H5T_NATIVE_DOUBLE);
    } else {
      return Failure;
    }
  } else if(mclass==H5T_STRING) {
    return H5Tcopy(aFileType);
  }

  return Failure;
}
//////////////////////////////////////////////////////////////////////////////
hid_t BatchLab::HDF5::HDF5O::compound_mem_type(
 hid_t aFileType
)
//////////////////////////////////////////////////////////////////////////////
// FIXME : In principle H5T_get_native_type should do the job but it crashes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  H5T_class_t t_class = H5Tget_class(aFileType);
  if(t_class!=H5T_COMPOUND) return Failure;
  
  size_t sz = H5Tget_size(aFileType);
  //printf("debug : read_object : sz = %d\n",sz);

  hid_t memtype = H5Tcreate(H5T_COMPOUND,sz);
  if(memtype<0) return Failure;

  //FIXME : WARNING : is order the booked order ?

  int mn = H5Tget_nmembers(aFileType);
  std::vector<unsigned int> szs(mn);
  //printf("debug : members : %d\n",mn);
  for(int index=0;index<mn;index++) {
    char* mname = H5Tget_member_name(aFileType,index);
    size_t moffset = H5Tget_member_offset(aFileType,index);
    hid_t mtype = H5Tget_member_type(aFileType,index);
    //printf("debug : members :   %d (%d) : %s : begin\n",index,mn,mname);

   {H5T_class_t mclass = H5Tget_class(mtype);
    if( (mclass==H5T_INTEGER) || 
        (mclass==H5T_STRING)  ||
        (mclass==H5T_FLOAT)   ) {
      hid_t mmemtype = basic_mem_type(mtype);
      if(mmemtype<0) {
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      if(H5Tinsert(memtype,mname,moffset,mmemtype)<0) {
        H5Tclose(mmemtype);
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      H5Tclose(mmemtype);

    } else if(mclass==H5T_ARRAY) {
      int dimn = H5Tget_array_ndims(mtype); //Should be 1;
      hsize_t* dims = new hsize_t[dimn];
      int* perms = new int[dimn];
      if(H5Tget_array_dims(mtype,dims,perms)<0) {
        delete [] dims;
        delete [] perms;
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      hid_t base_type = H5Tget_super(mtype);
      if(base_type<0) {
        delete [] dims;
        delete [] perms;
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      hid_t mmemtype = basic_mem_type(base_type);
      if(mmemtype<0) {
        delete [] dims;
        delete [] perms;
        H5Tclose(base_type);
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      H5Tclose(base_type);
      hid_t array_type = H5Tarray_create(mmemtype,dimn,dims,perms);
      delete [] dims;
      delete [] perms;
      if(array_type<0) {
        H5Tclose(mmemtype);
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      H5Tclose(mmemtype);

      if(H5Tinsert(memtype,mname,moffset,array_type)<0) {
        H5Tclose(array_type);
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      H5Tclose(array_type);

    } else if(mclass==H5T_COMPOUND) {
      hid_t mmemtype = compound_mem_type(mtype);
      if(memtype<0) {
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      if(H5Tinsert(memtype,mname,moffset,mmemtype)<0) {
        H5Tclose(mmemtype);
        H5Tclose(mtype);
        if(mname) ::free(mname);
        H5Tclose(memtype);
        return Failure;
      }
      H5Tclose(mmemtype);
    } else {
      H5Tclose(mtype);
      if(mname) ::free(mname);
      H5Tclose(memtype);
      return Failure;
    }}
    H5Tclose(mtype);
    //printf("debug : members :   %d (%d) : %s : end\n",index,mn,mname);
    if(mname) ::free(mname);
  }

  return memtype;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
hid_t BatchLab::HDF5::HDF5O::string_datatype(
 unsigned int aSize
)
//////////////////////////////////////////////////////////////////////////////
// aSize should include the trailing null char.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t datatype = H5Tcopy(H5T_C_S1);
  if(datatype<0) return Failure;

  if(H5Tset_size(datatype,aSize)<0) {
    H5Tclose(datatype);
    return Failure;
  }

  if(H5Tset_strpad(datatype,H5T_STR_NULLTERM)<0) {
    H5Tclose(datatype);
    return Failure;
  }

  return datatype;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::HDF5O::write_atb( 
 hid_t aDS //data set
,const std::string& aName
,const std::string& aData
)
//////////////////////////////////////////////////////////////////////////////
// From H5LT.c/H5LTset_attribute_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int has_attr = H5LT_find_attribute(aDS,aName.c_str());
  if(has_attr==1)  {
    if(H5Adelete(aDS,aName.c_str())<0) return false;
  }

  hid_t datatype = string_datatype(aData.size()+1);
  if(datatype<0) return false;

  hid_t aid = H5Acreate(aDS,aName.c_str(),datatype,fScalar,H5P_DEFAULT);
  if(aid<0) {
    H5Tclose(datatype);
    return false;
  }

  if(H5Awrite(aid,datatype,aData.c_str())<0) {
    H5Aclose(aid);
    H5Tclose(datatype);
    return false;
  }

  H5Aclose(aid);
  H5Tclose(datatype);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::HDF5O::read_atb( 
 hid_t aID //data set
,const std::string& aName
,std::string& aData
,unsigned int aSize
)
//////////////////////////////////////////////////////////////////////////////
// From H5LT.c/H5LTget_attribute_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  char* b = new char[aSize];
  if(H5LT_get_attribute_disk(aID,aName.c_str(),b)<0) {
    delete [] b;
    return false;
  }
  aData = std::string(b);
  delete [] b;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::HDF5O::write_atb( 
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
bool BatchLab::HDF5::HDF5O::read_atb( 
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
bool BatchLab::HDF5::HDF5O::write_atb( 
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
bool BatchLab::HDF5::HDF5O::read_atb( 
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
bool BatchLab::HDF5::HDF5O::write_object( 
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
bool BatchLab::HDF5::HDF5O::read_object(
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
