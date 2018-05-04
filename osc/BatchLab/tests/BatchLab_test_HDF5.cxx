
// Lib :
#include <Lib/Debug.h>
#include <inlib/args>

// BatchLab :
#include "HDF5_Tools.h"
#include "HDF5_T_Tools.h"

// Compound datatype :
#define LDS 10
#define LSTRING 5
struct S_A {
  unsigned int a;
  int b;
  float c;
  double ds[LDS];
  char s[LSTRING+1];
};
struct S_B {
  unsigned int d;
  S_A e;
};

//////////////////////////////////////////////////////////////////////////////
bool build_S_B_file_type(
 hid_t& aDataType
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t S_A_type = H5Tcreate(H5T_COMPOUND,sizeof(S_A));
  if(S_A_type<0) {
    return false;
  }
  if(H5Tinsert(S_A_type,"a",HOFFSET(S_A,a),H5T_STD_U32LE)<0) {
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(S_A_type,"b",HOFFSET(S_A,b),H5T_STD_I32LE)<0) {
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(S_A_type,"c",HOFFSET(S_A,c),H5T_IEEE_F32LE)<0) {
    H5Tclose(S_A_type);
    return false;
  }

  hsize_t ds_dim[] = {LDS};
  hid_t ds_type = H5Tarray_create(H5T_IEEE_F64LE,1,ds_dim,NULL);
  if(ds_type<0) {
    H5Tclose(S_A_type);
    return false;
  }

  if(H5Tinsert(S_A_type,"ds",HOFFSET(S_A,ds),ds_type)<0) {
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  hid_t s_type = BatchLab::HDF5::Helper::string_datatype(LSTRING+1);
  if(s_type<0) {
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  if(H5Tinsert(S_A_type,"s",HOFFSET(S_A,s),s_type)<0) {
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  aDataType = H5Tcreate(H5T_COMPOUND,sizeof(S_B));
  if(aDataType<0) {
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(aDataType,"d",HOFFSET(S_B,d),H5T_STD_I32LE)<0) {
    H5Tclose(aDataType);
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(aDataType,"e",HOFFSET(S_B,e),S_A_type)<0) {
    H5Tclose(aDataType);
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  H5Tclose(s_type);
  H5Tclose(ds_type);
  H5Tclose(S_A_type);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool build_S_B_mem_type(
 hid_t& aDataType
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  hid_t S_A_type = H5Tcreate(H5T_COMPOUND,sizeof(S_A));
  if(S_A_type<0) {
    return false;
  }
  if(H5Tinsert(S_A_type,"a",HOFFSET(S_A,a),H5T_NATIVE_UINT)<0) {
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(S_A_type,"b",HOFFSET(S_A,b),H5T_NATIVE_INT)<0) {
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(S_A_type,"c",HOFFSET(S_A,c),H5T_NATIVE_FLOAT)<0) {
    H5Tclose(S_A_type);
    return false;
  }

  hsize_t ds_dim[] = {LDS};
  hid_t ds_type = H5Tarray_create(H5T_NATIVE_DOUBLE,1,ds_dim,NULL);
  if(ds_type<0) {
    H5Tclose(S_A_type);
    return false;
  }

  if(H5Tinsert(S_A_type,"ds",HOFFSET(S_A,ds),ds_type)<0) {
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  hid_t s_type = BatchLab::HDF5::Helper::string_datatype(LSTRING+1);
  if(s_type<0) {
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  if(H5Tinsert(S_A_type,"s",HOFFSET(S_A,s),s_type)<0) {
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  aDataType = H5Tcreate(H5T_COMPOUND,sizeof(S_B));
  if(aDataType<0) {
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(aDataType,"d",HOFFSET(S_B,d),H5T_NATIVE_INT)<0) {
    H5Tclose(aDataType);
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }
  if(H5Tinsert(aDataType,"e",HOFFSET(S_B,e),S_A_type)<0) {
    H5Tclose(aDataType);
    H5Tclose(s_type);
    H5Tclose(ds_type);
    H5Tclose(S_A_type);
    return false;
  }

  H5Tclose(s_type);
  H5Tclose(ds_type);
  H5Tclose(S_A_type);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
class C_A {
  unsigned int ui;
  float f;
  double* ds;
  int i;
};
  // The ds is a pointer to an array for which the size is not known
  // at compilation. We have then to recreate a "struct" in memory
  // with the right size, along with an offset mapping to the fields.

*/
struct C_A {
  unsigned int ui;
  float f;
  unsigned int lds;
  double* ds;
  int i;
};
//////////////////////////////////////////////////////////////////////////////
bool build_C_A_file_type(
 hid_t& aDataType
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int sz = 4+4+4+LDS*8+4;
  hid_t C_A_type = H5Tcreate(H5T_COMPOUND,sz);
  if(C_A_type<0) {
    return false;
  }
  unsigned int offset = 0;

  if(H5Tinsert(C_A_type,"ui",offset,H5T_STD_U32LE)<0) {
    H5Tclose(C_A_type);
    return false;
  }
  offset += sizeof(unsigned int);

  if(H5Tinsert(C_A_type,"f",offset,H5T_IEEE_F32LE)<0) {
    H5Tclose(C_A_type);
    return false;
  }
  offset += sizeof(float);

  if(H5Tinsert(C_A_type,"lds",offset,H5T_STD_U32LE)<0) {
    H5Tclose(C_A_type);
    return false;
  }
  offset += sizeof(unsigned int);

  hsize_t ds_dim[] = {LDS};
  hid_t ds_type = H5Tarray_create(H5T_IEEE_F64LE,1,ds_dim,NULL);
  if(ds_type<0) {
    H5Tclose(C_A_type);
    return false;
  }

  if(H5Tinsert(C_A_type,"ds",offset,ds_type)<0) {
    H5Tclose(ds_type);
    H5Tclose(C_A_type);
    return false;
  }
  offset += LDS*sizeof(double);

  if(H5Tinsert(C_A_type,"i",offset,H5T_STD_I32LE)<0) {
    H5Tclose(ds_type);
    H5Tclose(C_A_type);
    return false;
  }
  offset += sizeof(int);

  H5Tclose(ds_type);

  aDataType = C_A_type;

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

int main(int a_argc,char** a_argv) {
  inlib::args args(a_argc,a_argv);

  std::ostream& out = std::cout;

  if(args.number()<=1) {
      out << "BatchLab_test_HDF5.exe :" << std::endl;
      out << "  -write" << std::endl;
      out << "  -read" << std::endl;
      return 1;
  }

  std::string wtitle = "My histo";
  if(args.is_arg("-write")) {
    // Data to store :
    unsigned int n = 1000000;
    int* array = new int[n];
    for(unsigned int index=0;index<n;index++) array[index] = index;
    S_B my_struct;
    my_struct.d = 316;
    my_struct.e.a = 314;
    my_struct.e.b = -315;
    my_struct.e.c = 3.14F;
    my_struct.e.ds[5] = 3.16;
    ::strcpy(my_struct.e.s,"hello"); //LSTRING = 5

    C_A my_object;
    my_object.ui = 314;
    my_object.f = 315.0F;
    my_object.lds = LDS;
    my_object.ds = new double[LDS];
    my_object.ds[5] = 316;
    my_object.i = 317;

    // Create the file :
    hid_t file = 
      H5Fcreate("BatchLab_test_HDF5.hdf5", 
         H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(file<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create file."
          << std::endl;
      return 1;
    }

    // Create directories (HDF5 groups) in the file :
    hid_t stat = H5Gcreate(file,"stat",0);
    if(stat<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create group."
          << std::endl;
      return 1;
    }

    hid_t histos = H5Gcreate(stat,"histograms",0);
    if(histos<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create group."
          << std::endl;
      return 1;
    }
    BatchLab::HDF5::Helper helper;
    if(!helper.write_atb(histos,"type","Directory")) {
      out << "BatchLab_test_HDF5 :"
          << " can't write atb type."
          << std::endl;
      return 1;
    }
    int v = 314;
    if(!helper.write_atb(histos,"version",v)) {
      out << "BatchLab_test_HDF5 :"
          << " can't write atb version."
          << std::endl;
      return 1;
    }
  
    hid_t shistos = H5Gcreate(histos,"sub_histograms",0);
    if(shistos<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create group."
          << std::endl;
      return 1;
    }
  
    hid_t tuples = H5Gcreate(stat,"tuples",0);
    if(tuples<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create group."
          << std::endl;
      return 1;
    }
  
    // Write data in directories :
    if(!helper.write_array_int(histos,"my_histo",n,array)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when writing array int."
          << std::endl;
    }

    if(!helper.write_string(histos,"my_title",wtitle)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when writing std::string."
          << std::endl;
    }
  
    if(!helper.write_array_int(tuples,"my_tuple",n,array)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when writing array int."
          << std::endl;
    }
  
    std::vector<std::string> vs;
    vs.push_back("Line 1");
    vs.push_back("Line 2");
    vs.push_back("Line 3");
    vs.push_back("Line 4");
    if(!helper.write_array_string(histos,"my_strings",vs)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when writing array string."
          << std::endl;
    }
  
    hid_t S_B_file_type;
    if(!build_S_B_file_type(S_B_file_type)) {
      out << "BatchLab_test_HDF5 :"
          << " can't build type for S_B."
          << std::endl;
    } else {

      hid_t S_B_mem_type;
      if(!build_S_B_mem_type(S_B_mem_type)) {
        out << "BatchLab_test_HDF5 :"
            << " can't build type for S_B."
            << std::endl;
      } else {

        if(!helper.write_struct<S_B>
              (histos,"my_struct",S_B_file_type,S_B_mem_type,my_struct)) {
          out << "BatchLab_test_HDF5 :"
              << " problem when writing S_A."
              << std::endl;
        }
        H5Tclose(S_B_file_type);
        H5Tclose(S_B_mem_type);

      }
    }


    hid_t C_A_file_type;
    if(!build_C_A_file_type(C_A_file_type)) {
      out << "BatchLab_test_HDF5 :"
          << " can't build type for C_A."
          << std::endl;
    } else {

      unsigned int sz = 4+4+4+LDS*8+4;
      std::vector<unsigned int> mszs(5); //Not used at writing.
      mszs[0] = 4;
      mszs[1] = 4;
      mszs[2] = 4;
      mszs[3] = LDS*8;
      mszs[4] = 4;

      char* buffer = new char[sz];

      //Fill buffer :
      char* p = buffer;
      ::memcpy(p,&my_object.ui,sizeof(unsigned int));
      p+=sizeof(unsigned int);

      ::memcpy(p,&my_object.f,sizeof(float)); 
      p+=sizeof(float);

      ::memcpy(p,&my_object.lds,sizeof(unsigned int)); 
      p+=sizeof(unsigned int);

      ::memcpy(p,my_object.ds,LDS*sizeof(double));  
      p += LDS*sizeof(double);

      ::memcpy(p,&my_object.i,sizeof(int));
      p += sizeof(int);

      if(!helper.write_object(histos,"my_object",C_A_file_type,sz,buffer)) {
        out << "BatchLab_test_HDF5 :"
            << " problem when writing C_A."
            << std::endl;
      }
      delete [] buffer;
      H5Tclose(C_A_file_type);

    }
  
    // Release HDF5 resources and close the file :
    H5Gclose(tuples);
    H5Gclose(histos);
    H5Gclose(stat);
    H5Fclose(file);
  
    delete [] my_object.ds;
    delete [] array;
  }

  if(args.is_arg("-read"))  {
    // Open the file :
    hid_t file = 
      H5Fopen("BatchLab_test_HDF5.hdf5",H5F_ACC_RDONLY, H5P_DEFAULT);
    if(file<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create file."
          << std::endl;
      return 1;
    }

    // Open a directory (group) :
    hid_t histos = H5Gopen(file,"/stat/histograms");
    if(histos<0) {
      out << "BatchLab_test_HDF5 :"
          << " can't create group."
          << std::endl;
      return 1;
    }

    std::string stype;
    if(!BatchLab::HDF5::Helper::read_atb(histos,"type",stype)) {
      out << "BatchLab_test_HDF5 :"
          << " can't read atb type."
          << std::endl;
      return 1;
    }
    out << "BatchLab_test_HDF5 :"
        << " histograms atb type should be Directory  = " << inlib::sout(stype)
        << std::endl;
    int v;
    if(!BatchLab::HDF5::Helper::read_atb(histos,"version",v)) {
      out << "BatchLab_test_HDF5 :"
          << " can't read atb version."
          << std::endl;
      return 1;
    }
    out << "BatchLab_test_HDF5 :"
        << " histograms version should be 314  = " << v
        << std::endl;

    // Get a data in the directory :
    unsigned int n = 0;
    int* array = 0;
    if(!BatchLab::HDF5::read_array_int(histos,"my_histo",n,array)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when reading array int."
          << std::endl;
    }

    std::vector<std::string> vs;
    if(!BatchLab::HDF5::Helper::read_array_string(histos,"my_strings",vs)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when reading array string."
          << std::endl;
    }

    std::string rtitle;
    if(!BatchLab::HDF5::Helper::read_string(histos,"my_title",rtitle)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when reading std::string."
          << std::endl;
    }
  
    S_B s;
    hid_t S_B_mem_type;
    if(!build_S_B_mem_type(S_B_mem_type)) {
      out << "BatchLab_test_HDF5 :"
          << " can't build type for S_B."
          << std::endl;
    } else {
      if(!BatchLab::HDF5::read_struct<S_B>
            (histos,"my_struct",S_B_mem_type,s)) {
        out << "BatchLab_test_HDF5 :"
            << " problem when reading struct."
            << std::endl;
      }
      H5Tclose(S_B_mem_type);
    }


    C_A my_object;
    my_object.ds = 0;
  
    BatchLab::HDF5::Helper helper;
   {unsigned int sz = 0;
    char* buffer = 0;
    if(!helper.read_object(histos,"my_object",sz,buffer)) {
      out << "BatchLab_test_HDF5 :"
          << " problem when reading object."
          << std::endl;
    } else {
      char* p = buffer;

      ::memcpy(&my_object.ui,p,sizeof(unsigned int));    
      p += sizeof(unsigned int);

      ::memcpy(&my_object.f,p,sizeof(float));
      p += sizeof(float);
 
      ::memcpy(&my_object.lds,p,sizeof(unsigned int));    
      p += sizeof(unsigned int);

      my_object.ds = new double[my_object.lds];
      ::memcpy(my_object.ds,p,LDS*sizeof(double)); 
      p += LDS*sizeof(double);

      ::memcpy(&my_object.i,p,sizeof(int));
      p += sizeof(int);

      delete [] buffer;
    }}

    // Release HDF5 resources and close the file :
    H5Gclose(histos);
    H5Fclose(file);

    //////////////////////////////////////////
    //// Use the data ////////////////////////
    //////////////////////////////////////////
    out << "BatchLab_test_HDF5 :"
        << " should be 1000000 : "  << n
        << std::endl;

    int array_314 = array[314];
    out << "BatchLab_test_HDF5 :"
        << " should be 314  : "  << array_314
        << std::endl;

   {char* ba = (char*)&array_314;
    int array_314_swapped;
    char* bsa = (char*)&array_314_swapped;
    bsa[0] = ba[3];
    bsa[1] = ba[2];
    bsa[2] = ba[1];
    bsa[3] = ba[0];
    out << "BatchLab_test_HDF5 :"
        << " swapped is : "  << array_314_swapped
        << std::endl;}

    out << "BatchLab_test_HDF5 :"
        << " should be " << inlib::sout(wtitle) << "  : "  << rtitle
        << std::endl;

    out << "BatchLab_test_HDF5 :"
        << " should be 4 " << int(vs.size())
        << std::endl;
    out << "BatchLab_test_HDF5 :"
      //<< " should be \"\" " << inlib::sout(vs[0])
        << " should be \"Line 3\" " << inlib::sout(vs[2])
        << std::endl;

    out << "BatchLab_test_HDF5 :"
        << " S_B.d : should be 316 " << s.d
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " S_B.e.a : should be 314 " << s.e.a
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " S_B.e.b : should be -315 " << s.e.b
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " S_B.e.c : should be 3.14 " << s.e.c
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " S_B.e.ds[5] : should be 3.16 " << s.e.ds[5]
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " S_B.e.s : should be \"hello\" " << inlib::sout(s.e.s)
        << std::endl;

    out << "BatchLab_test_HDF5 :"
        << " C_A.ui : should be 314 " << my_object.ui
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " C_A.f : should be 315 " << my_object.f
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " C_A.ds[5] : should be 316 " 
        << (my_object.ds?my_object.ds[5]:0)
        << std::endl;
    out << "BatchLab_test_HDF5 :"
        << " C_A.i : should be 317 " << my_object.i
        << std::endl;

    delete [] my_object.ds;
    delete [] array;
  }

  Lib::Debug::balance(out);

  return 0;
}
