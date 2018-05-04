// this :
#include <BatchLab/HDF5/MemberInserter.h>

// Slash :
#include <Slash/Store/IArray.h>
#include <Slash/Store/IStorable.h>

#include <Lib/ObjectSizeGetter.h>

#include <BatchLab/HDF5/HDF5O.h>
#include <BatchLab/HDF5/Types.h>

#define LIB_SIZE_CHAR   1
#define LIB_SIZE_SHORT  2
#define LIB_SIZE_INT    4
#define LIB_SIZE_INT64  8
#define LIB_SIZE_FLOAT  4
#define LIB_SIZE_DOUBLE 8

#define NotFound (-1)

#define VISIT_INSERT(aFile,aMem) \
  if(fCurrent.fCompound<0) return false;\
  if(H5Tinsert(fCurrent.fCompound,\
               aName.c_str(),\
               fCurrent.fOffset,aFile)<0) {\
    /*fprintf(stderr,"debug : BatchLab::HDF5::MemberInserter::visit : \"%s\" : failed\n",aName.c_str());*/\
    clear();\
    return false;\
  }\
  fCurrent.fOffset += aMem;\
  return true;

//////////////////////////////////////////////////////////////////////////////
template <class T>
bool insert(hid_t aCompound,
            const std::string& aName,
            unsigned int& aOffset,
            unsigned int aNumber,
            hid_t aFileType,
            unsigned int aSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string name_num = aName + "_num";
  if(H5Tinsert(aCompound,name_num.c_str(),aOffset,H5T_STD_U32XX)<0) {
    return false;
  }
  aOffset += LIB_SIZE_INT;

  unsigned int number = aNumber;
  if(number) {
    hsize_t dims[1];
    dims[0] = number;
    hid_t array_type = H5Tarray_create(aFileType,1,dims,NULL);
    if(array_type<0) return false;
    if(H5Tinsert(aCompound,aName.c_str(),aOffset,array_type)<0) {
      H5Tclose(array_type);
      return false;
    }
    H5Tclose(array_type);
    aOffset += number*aSize;
  }
  return true;
}

#define VISIT_INSERT_VECTOR(aType,aFile,aMem) \
  if(fCurrent.fCompound<0) return false;\
  if(!insert<aType>(fCurrent.fCompound,\
                   aName,\
                   fCurrent.fOffset,\
                   aData.size(),aFile,aMem)) {\
    /*fprintf(stderr,"debug : BatchLab::HDF5::MemberInserter::visit_vec : \"%s\" : failed\n",aName.c_str());*/\
    clear();\
    return false;\
  }\
  return true;

#define VISIT_ARRAY(aFile,aMem) \
  if(fCurrent.fCompound<0) return false;\
  Slash::Store::IArray::Orders orders = aData.orders();\
  if(!insert<unsigned int>(fCurrent.fCompound,\
                           aName+"_orders",\
                           fCurrent.fOffset,\
                           orders.size(),H5T_STD_U32XX,LIB_SIZE_INT)) {\
    clear();\
    return false;\
  }\
  unsigned int array_sz = 1;\
  unsigned int dimn = orders.size();\
  hsize_t* dims = new hsize_t[dimn];\
  for(unsigned int dimi=0;dimi<dimn;dimi++) {\
    array_sz *= orders[dimi];\
    dims[dimi] = orders[dimi];\
  }\
  array_sz *= aMem;\
  if(array_sz) {\
    hid_t array_type = H5Tarray_create(aFile,dimn,dims,NULL);\
    delete [] dims;\
    if(array_type<0) {\
      clear();\
      return false;\
    }\
    if(H5Tinsert\
         (fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,array_type)<0) {\
      H5Tclose(array_type);\
      clear();\
      return false;\
    }\
    H5Tclose(array_type);\
    fCurrent.fOffset += array_sz;\
  }\
  return true;

//////////////////////////////////////////////////////////////////////////////
BatchLab::HDF5::MemberInserter::MemberInserter(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::HDF5::MemberInserter::~MemberInserter(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::HDF5::MemberInserter::clear(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Item>::iterator it;
  for(it=fStack.begin();it!=fStack.end();++it) {
    if((*it).fCompound>=0) {
      H5Tclose((*it).fCompound);
    }
  }
  fStack.clear();
  fReturned.fCompound = NotFound;
  fReturned.fOffset = 0;
}
//////////////////////////////////////////////////////////////////////////////
hid_t BatchLab::HDF5::MemberInserter::compound(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fReturned.fCompound;
}
//////////////////////////////////////////////////////////////////////////////
unsigned int BatchLab::HDF5::MemberInserter::offset(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fReturned.fOffset;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::begin(
 const Slash::Store::IStorable& aObject
,const std::string& aClassName
,Slash::Store::IConstVisitor::Local aLocal
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  // Use aLocal to be able to work in a object.visit()
  // over a aObject being a "super" of "object" .
  Lib::ObjectSizeGetter mSize;
  if(!aLocal(aObject,mSize)) {
    clear();
    return false;
  }
  unsigned int sz = mSize.size();

  fStack.push_front(fCurrent);

  fCurrent.fParent = &(fStack.front());
  fCurrent.fSize = sz;
  if(sz) {
    fCurrent.fCompound = H5Tcreate(H5T_COMPOUND,sz);
    if(fCurrent.fCompound<0) {
      clear();
      return false;
    }
  } else {
    // This may happen if a super class has no members.
    fCurrent.fCompound = NotFound;
  }
  fCurrent.fOffset = 0;
  fCurrent.fName = aClassName;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::end(
 const Slash::Store::IStorable&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  if( fCurrent.fParent && 
      (fCurrent.fParent->fCompound>=0) && 
      (fCurrent.fCompound>=0) ) {
    if(H5Tinsert(fCurrent.fParent->fCompound,
                 fCurrent.fName.c_str(),
                 fCurrent.fParent->fOffset,
                 fCurrent.fCompound)<0) {
      clear();
      return false;
    }
    if(fStack.size()>1) {
      H5Tclose(fCurrent.fCompound);
      fCurrent.fCompound = NotFound;
      fCurrent.fOffset = 0;
    }
    fCurrent.fParent->fOffset += fCurrent.fSize;
  }

  fReturned = fCurrent;

  fCurrent = fStack.front();

  fStack.pop_front();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,bool
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_U8XX,LIB_SIZE_CHAR)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,char
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_I8XX,LIB_SIZE_CHAR)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,short
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_I16XX,LIB_SIZE_SHORT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,int
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_I32XX,LIB_SIZE_INT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,Slash::int64
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_I64XX,LIB_SIZE_INT64)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,Slash::uint64
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_U64XX,LIB_SIZE_INT64)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,unsigned int
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_STD_U32XX,LIB_SIZE_INT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,float
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_IEEE_F32XX,LIB_SIZE_FLOAT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,double
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT(H5T_IEEE_F64XX,LIB_SIZE_DOUBLE)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::string& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  if(fCurrent.fCompound<0) return false;
  std::string name_num = aName + "_num";
  if(H5Tinsert(fCurrent.fCompound,
               name_num.c_str(),
               fCurrent.fOffset,H5T_STD_U32XX)<0) {
    clear();
    return false;
  }
  fCurrent.fOffset += LIB_SIZE_INT;

  hid_t array_type = BatchLab::HDF5::HDF5O::string_datatype(aData.size()+1);
  if(array_type<0) {
    clear();
    return false;
  }

  if(H5Tinsert
       (fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,array_type)<0) {
    H5Tclose(array_type);
    clear();
    return false;
  }

  H5Tclose(array_type);
  fCurrent.fOffset += aData.size()+1;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<std::string>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  if(fCurrent.fCompound<0) return false;
  std::string name_num = aName + "_num";
  if(H5Tinsert(fCurrent.fCompound,
               name_num.c_str(),
               fCurrent.fOffset,H5T_STD_U32XX)<0) {
    clear();
    return false;
  }
  fCurrent.fOffset += LIB_SIZE_INT;

  unsigned int number = aData.size();
  unsigned int sz = 0;
  for(unsigned int index=0;index<number;index++) sz += aData[index].size()+1;
  sz++;

  hid_t array_type = BatchLab::HDF5::HDF5O::string_datatype(sz);
  if(array_type<0) {
    clear();
    return false;
  }

  if(H5Tinsert(fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,array_type)<0) {
    H5Tclose(array_type);
    clear();
    return false;
  }

  H5Tclose(array_type);
  fCurrent.fOffset += sz;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<bool>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{   
  VISIT_INSERT_VECTOR(unsigned char,H5T_STD_U8XX,LIB_SIZE_CHAR)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<char>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(char,H5T_STD_I8XX,LIB_SIZE_CHAR)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<short>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(short,H5T_STD_I16XX,LIB_SIZE_SHORT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<Slash::int64>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(Slash::int64,H5T_STD_I64XX,LIB_SIZE_INT64)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<int>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  VISIT_INSERT_VECTOR(int,H5T_STD_I32XX,LIB_SIZE_INT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<float>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(float,H5T_IEEE_F32XX,LIB_SIZE_FLOAT)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<double>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(double,H5T_IEEE_F64XX,LIB_SIZE_DOUBLE)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector<unsigned char>& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_INSERT_VECTOR(unsigned char,H5T_STD_U8XX,LIB_SIZE_CHAR)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const std::vector< std::vector<double> >& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(fCurrent.fCompound<0) return false;
  std::string name_num = aName + "_num";
  if(H5Tinsert(fCurrent.fCompound,
               name_num.c_str(),
               fCurrent.fOffset,H5T_STD_U32XX)<0) {
    clear();
    return false;
  }
  fCurrent.fOffset += LIB_SIZE_INT;

  std::string name_dim = aName + "_dim";
  if(H5Tinsert(fCurrent.fCompound,
               name_dim.c_str(),
               fCurrent.fOffset,H5T_STD_U32XX)<0) {
    clear();
    return false;
  }
  fCurrent.fOffset += LIB_SIZE_INT;

  unsigned int n = aData.size();
  unsigned int dim = n?aData[0].size():0;
  unsigned int number = n * dim;

  if(number) {
    hsize_t dims[1];
    dims[0] = number;
    hid_t array_type = H5Tarray_create(H5T_IEEE_F64XX,1,dims,NULL);
    if(array_type<0) {
      clear();
      return false;
    }
    if(H5Tinsert
         (fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,array_type)<0) {
      H5Tclose(array_type);
      clear();
      return false;
    }
    H5Tclose(array_type);
    fCurrent.fOffset += number*LIB_SIZE_DOUBLE;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit_double(
 const std::string& aName
,const Slash::Store::IArray& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  VISIT_ARRAY(H5T_IEEE_F64XX,LIB_SIZE_DOUBLE)
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const Slash::Store::IStorable& aData
)
//////////////////////////////////////////////////////////////////////////////
// WARNING : can't be called in a object.visit()
//   over a aData being a "super" of "object" .
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(fCurrent.fCompound<0) return false;
  MemberInserter mInserter;
  if(!aData.visit(mInserter)) {
    clear();
    return false;
  }
  hid_t type = mInserter.compound();
  if(type<0) return true; //This is ok if member object has no data to store.
  if(H5Tinsert(fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,type)<0) {
    H5Tclose(type);
    clear();
    return false;
  }
  H5Tclose(type);
  fCurrent.fOffset += mInserter.offset();
  return true;  
}
#include <string.h>
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::HDF5::MemberInserter::visit(
 const std::string& aName
,const char* aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  if(fCurrent.fCompound<0) return false;
  std::string name_num = aName + "_num";
  if(H5Tinsert(fCurrent.fCompound,
               name_num.c_str(),
               fCurrent.fOffset,H5T_STD_U32XX)<0) {
    clear();
    return false;
  }
  fCurrent.fOffset += LIB_SIZE_INT;

  size_t ldata = ::strlen(aData);
  hid_t array_type = 
    BatchLab::HDF5::HDF5O::string_datatype(ldata+1);
  if(array_type<0) {
    clear();
    return false;
  }

  if(H5Tinsert
       (fCurrent.fCompound,aName.c_str(),fCurrent.fOffset,array_type)<0) {
    H5Tclose(array_type);
    clear();
    return false;
  }

  H5Tclose(array_type);
  fCurrent.fOffset += ldata+1;

  return true;
}
