#ifndef BatchLab_HDF5_Tools_h
#define BatchLab_HDF5_Tools_h

#include <string>
#include <vector>

#include <hdf5.h>
#include <H5LT.h>

#include <Lib/Sizes.h>

namespace BatchLab {

namespace HDF5 {

  class Helper {
  public:
    static bool checkSizes(){
      if(sizeof(bool)!=1) return false;
      if(LIB_SIZE_CHAR!=1) return false;
      if(LIB_SIZE_SHORT!=2) return false;
      if(LIB_SIZE_INT!=4) return false;
      if(LIB_SIZE_FLOAT!=4) return false;
      if(LIB_SIZE_DOUBLE!=8) return false;
      return true;
    }

    static hid_t string_datatype(unsigned int aSize){
      // aSize should include the trailing null char.
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
    
    static hid_t basic_mem_type(hid_t aFileType){
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

    static hid_t compound_mem_type(hid_t aFileType){
      // FIXME : In principle H5T_get_native_type should do the
      //         job but it crashes.
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

    // std::string :
    static bool read_string(hid_t,const std::string&,
                            std::string&);
    static bool read_array_string(hid_t,const std::string&,
                                  std::vector<std::string>&);
    static bool put_array_string(const std::vector<std::string>&,char*);
    static bool put_array_string(unsigned int,char*,
                                 std::vector<std::string>&);

    // atbs :
    static bool read_atb(hid_t aID, //data set
                  const std::string& aName,std::string& aData,
                  unsigned int aSize = 100){
      // From H5LT.c/H5LTget_attribute_string.
      char* b = new char[aSize];
      if(H5LT_get_attribute_disk(aID,aName.c_str(),b)<0) {
        delete [] b;
        return false;
      }
      aData = std::string(b);
      delete [] b;
      return true;
    }

    static bool read_atb(hid_t,const std::string&,int&);
    static bool read_atb(hid_t,const std::string&,unsigned int&);

    // Else :
    static void dump(hid_t aID,const std::string& aName){
      H5Giterate(aID,aName.c_str(), NULL,file_info,0);
    }
  public:
    Helper(int aLevel = 1)
    :fCompress(1),fScalar(Failure),fCompact(Failure)
    ,fSimple(Failure),fCPT(Failure){
      if(aLevel<0) aLevel = 0;
      if(aLevel>9) aLevel = 9;
      fCompress = aLevel;

      fScalar = H5Screate(H5S_SCALAR);
      if(fScalar<0) fScalar = Failure;
    
      fCompact = H5Pcreate(H5P_DATASET_CREATE);
      if(fCompact<0) {
        fCompact = Failure;
      } else {
        if(fCompress) {
          if(H5Pset_layout(fCompact,H5D_COMPACT)<0) { 
            //FIXME
            H5Pclose(fCompact);
            fCompact = Failure;
          }
        }
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
    
    virtual ~Helper(){
      if(fScalar>=0) H5Sclose(fScalar);
      if(fCompact>=0) H5Pclose(fCompact);
      if(fSimple>=0) H5Sclose(fSimple);
      if(fCPT>=0) H5Pclose(fCPT);
    }
  public:
    bool write_atb(hid_t aDS,
                   const std::string& aName,const std::string& aData){
      // From H5LT.c/H5LTset_attribute_string.
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
    
    bool write_atb(hid_t,const std::string&,int);
    bool write_atb(hid_t,const std::string&,unsigned int);
    bool write_object(hid_t,const std::string&,hid_t,unsigned int,char*);
    bool read_object(hid_t,const std::string&,unsigned int&,char*&);

    bool write_int(hid_t,const std::string&,int);
    bool write_uint(hid_t,const std::string&,unsigned int);
    bool write_double(hid_t,const std::string&,double);
    bool write_bool(hid_t,const std::string&,bool);
    bool write_float(hid_t,const std::string&,float);
    bool write_string(hid_t,const std::string& dset,const std::string&);
    bool write_array_string(hid_t,const std::string&,
                            const std::vector<std::string>&);
    bool write_array_int(hid_t,const std::string&,
                         unsigned int,const int []);
    bool write_array_int(hid_t,const std::string&,
                         const std::vector<int>&);
    bool write_array_uchar(hid_t,const std::string&,
                           const std::vector<unsigned char>&);
    bool write_array_double(hid_t,const std::string&,
                            unsigned int,const double[]);
    bool write_array_double(hid_t,const std::string&,
                            const std::vector<double>&);
    bool write_array_float(hid_t,const std::string&,
                           unsigned int,const float[]);
    bool write_array_float(hid_t,const std::string&,
                           const std::vector<float>&);

    template <class T>
      bool write_struct(hid_t aLocation,const std::string& aName, 
                        hid_t aCreateType,hid_t aWriteType,const T& aData) {
        hid_t dataset = 
          H5Dcreate(aLocation,aName.c_str(),aCreateType,fScalar,fCompact);
        if(dataset<0) return false;
        if(H5Dwrite(dataset,aWriteType,H5S_ALL,H5S_ALL,H5P_DEFAULT,&aData)<0) {
          H5Dclose(dataset);                 
          return false;
        }
        H5Dclose(dataset);                 
        return true;
      }
  private:
    static const int Failure = -1;

    static herr_t file_info(hid_t aID,const char* aName,void*){
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

  private:
    int fCompress;
    hid_t fScalar;
    hid_t fCompact;
    hid_t fSimple;
    hid_t fCPT;
  };

  bool read_array_int(hid_t,const std::string&,unsigned int&,int*&);
  bool read_int(hid_t,const std::string&,int&);
  bool read_uint(hid_t,const std::string&,unsigned int&);
  bool read_bool(hid_t,const std::string&,bool&);
  bool read_array_int(hid_t,const std::string&,std::vector<int>&);
  bool read_array_uchar(hid_t,const std::string&,std::vector<unsigned char>&);
  bool read_array_double(hid_t,const std::string&,
                         unsigned int&,double*&);
  bool read_double(hid_t,const std::string&,double&);
  bool read_array_double(hid_t,const std::string&,std::vector<double>&);
  bool read_array_float(hid_t,const std::string&,
                         unsigned int&,float*&);
  bool read_float(hid_t,const std::string&,float&);
  bool read_array_float(hid_t,const std::string&,std::vector<float>&);

} //HDF5

} //BatchLab

#endif
