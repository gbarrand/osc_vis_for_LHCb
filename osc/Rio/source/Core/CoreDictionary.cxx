// this :
#include <Rio/Core/CoreDictionary.h>

#include <Rio/Interfaces/IObject.h>

#include <Rio/Core/BaseClass.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Sizes.h>

namespace Rio {

class None_Class : public BaseClass {
public://Rio::IClass
  virtual Rio::IObject* create(const Arguments&) {return 0;}
  virtual unsigned int checkSum() const {return fCheckSum;}
public:
  None_Class()
  :BaseClass("Rio::None","TNone",0),fCheckSum(0){}
  virtual ~None_Class(){}
private:
  unsigned int fCheckSum;
};

}

//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary::CoreDictionary(
 std::ostream& aOut
) 
:fOut(aOut)
,fNoneClass(new None_Class())
,f_opt_File_Class(0)
,f_opt_Directory_Class(0)
,f_opt_Object_Class(0)
,f_opt_Named_Class(0)
,f_opt_Array_Class(0)
,f_opt_Array_int_Class(0)
,f_opt_Array_float_Class(0)
,f_opt_Array_double_Class(0)
,f_opt_List_Class(0)
,f_opt_ObjArray_Class(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::CoreDictionary");
}
//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary::CoreDictionary(
 const CoreDictionary& aFrom
) 
:fOut(aFrom.fOut)
,fNoneClass(0)
,f_opt_File_Class(0)
,f_opt_Directory_Class(0)
,f_opt_Object_Class(0)
,f_opt_Named_Class(0)
,f_opt_Array_Class(0)
,f_opt_Array_int_Class(0)
,f_opt_Array_float_Class(0)
,f_opt_Array_double_Class(0)
,f_opt_List_Class(0)
,f_opt_ObjArray_Class(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::CoreDictionary");
}
//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary::~CoreDictionary(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_CLASSES_MAP
 {std::map<std::string,IClass*>::iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    delete (*it).second;
  }
  fClasses.clear();}
  fClasses_store.clear();
#else
 {std::list<IClass*>::iterator it;
  for(it=fClasses.begin();it!=fClasses.end();it=fClasses.erase(it)) 
    delete (*it);}
#endif

 {std::list<IObject*>::iterator it;
  for(it=fStreamerInfos.begin();
      it!=fStreamerInfos.end();
      it=fStreamerInfos.erase(it)) 
    delete (*it);}

  delete fNoneClass;

  Debug::decrement("Rio::CoreDictionary");
}
//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary& Rio::CoreDictionary::operator=(
 const CoreDictionary&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::CoreDictionary::isClass(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_CLASSES_MAP
  std::map<std::string,IClass*>::const_iterator  mit(fClasses.find(aName));
  if(mit==fClasses.end()) return false;
  return true;  
#else
  std::list<IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    if((*it)->name()==aName) return true;
  }
  return false;
#endif
}
//////////////////////////////////////////////////////////////////////////////
Rio::IClass* Rio::CoreDictionary::findClass(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_CLASSES_MAP
  std::map<std::string,IClass*>::const_iterator  mit(fClasses.find(aName));
  if(mit==fClasses.end()) return 0;
  return mit->second;  
#else
  std::list<IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    if((*it)->name()==aName) return *it;
  }
  //Out out(fPrinter);
  //out << "Rio::CoreDictionary::findClass : \"" 
  //    << aName << "\" not found." << endl;
  return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
Rio::IClass* Rio::CoreDictionary::findInStoreClass(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_CLASSES_MAP
  std::map<std::string,IClass*>::const_iterator  
    mit(fClasses_store.find(aName));
  if(mit==fClasses_store.end()) return 0;
  return mit->second;  

  //std::map<std::string,IClass*>::const_iterator it;
  //for(it=fClasses.begin();it!=fClasses.end();++it) {
  //  if(it->second->inStoreName()==aName) return it->second;
  //}
  //return 0;
#else
  std::list<IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    if((*it)->inStoreName()==aName) return *it;
  }
  return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
const Rio::IClass& Rio::CoreDictionary::noneClass(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *fNoneClass;
}
//opt : begin
//////////////////////////////////////////////////////////////////////////////
const Rio::IClass& Rio::CoreDictionary::opt_File_Class() const { return *f_opt_File_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Directory_Class() const { return *f_opt_Directory_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Object_Class() const { return *f_opt_Object_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Named_Class() const { return *f_opt_Named_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Array_Class() const { return *f_opt_Array_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Array_int_Class() const { return *f_opt_Array_int_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Array_float_Class() const { return *f_opt_Array_float_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_Array_double_Class() const { return *f_opt_Array_double_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_List_Class() const { return *f_opt_List_Class;}
const Rio::IClass& Rio::CoreDictionary::opt_ObjArray_Class() const { return *f_opt_ObjArray_Class;}
//opt : end
//////////////////////////////////////////////////////////////////////////////
const std::list<Rio::IObject*>& Rio::CoreDictionary::streamerInfos(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStreamerInfos;
} 

#include <cstdio> //sprintf

namespace Rio {
inline std::string long_tos(const long aValue) {
  char s[32];
  ::sprintf(s,"%ld",aValue);
  return std::string(s);
}
}

void Rio::CoreDictionary::out() const {

 {std::list<Rio::IObject*>::const_iterator it;
  for(it=fStreamerInfos.begin();it!=fStreamerInfos.end();++it)
    (*it)->out(fOut);}
  
  fOut << "Dictionary has " << long_tos(fClasses.size()) 
       << " entries :" << std::endl;
#ifdef RIO_CLASSES_MAP
 {std::map<std::string,IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    fOut << " - class \"" << it->second->name() << "\"" 
         << ", storeName \"" << it->second->inStoreName() << "\"" 
         << ", version " << it->second->version() 
         << std::endl;
  }}
#else
 {std::list<Rio::IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    fOut << " - class \"" << (*it)->name() << "\"" 
        << ", storeName \"" << (*it)->inStoreName() << "\"" 
        << ", version " << (*it)->version() 
	 << std::endl;
  }}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::CoreDictionary::addClass(
 IClass* aClass                                
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_CLASSES_MAP
  //fOut << "Rio::CoreDictionary::addClass :"
  //    << " name \"" << aClass->name() << "\"" 
  //    << ", storeName \"" << aClass->inStoreName() << "\"." 
  //    << std::endl;

  fClasses[aClass->name()] = aClass;
  fClasses_store[aClass->inStoreName()] = aClass;
#else
  fClasses.push_back(aClass);
#endif
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>

#include "CoreClasses.icxx"
//////////////////////////////////////////////////////////////////////////////
bool Rio::CoreDictionary::initialize(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  ///////////////////////////////////////////////////////////////////////
  // Interfaces :
  ///////////////////////////////////////////////////////////////////////
  {
    IClass* iclass = new IDirectory_Class();
    addClass(iclass);
  }

  ///////////////////////////////////////////////////////////////////////
  // Classes :
  ///////////////////////////////////////////////////////////////////////

  //---------------------------------------------------------------------
  { 
    IClass* iclass = new Object_Class();
    addClass(iclass);
    f_opt_Object_Class = iclass;
    // Has no streamer info. 
  }

  // sizeof(TString) = 8   (4 (virtual ~) + 1 + 3 (align))
  // sizeof(TObject) = 12
  // sizeof(TNamed)  = 28
  // sizeof(TObjArray) = 40
  // sizeof(TArray) = 8
  // sizeof(TArrayI) = 12
  // sizeof(TArrayD) = 12
  // sizeof(TArrayF) = 12

  //---------------------------------------------------------------------
  {
    IClass* iclass = new ObjArray_Class(*this);
    f_opt_ObjArray_Class = iclass;
    addClass(iclass);
  }

  IClass* streamerInfoClass = new StreamerInfo_Class();
  addClass(streamerInfoClass);
  IClass* streamerElementClass = new StreamerElement_Class();
  addClass(streamerElementClass);
  IClass* streamerBaseClass = new StreamerBase_Class();
  addClass(streamerBaseClass);
  IClass* streamerStringClass = new StreamerString_Class();
  addClass(streamerStringClass);
  IClass* streamerBasicTypeClass = new StreamerBasicType_Class();
  addClass(streamerBasicTypeClass);
  IClass* streamerObjectPointerClass = new StreamerObjectPointer_Class();
  addClass(streamerObjectPointerClass);
  IClass* streamerSTLClass = new StreamerSTL_Class();
  addClass(streamerSTLClass);

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Named_Class();
    f_opt_Named_Class = iclass;
    addClass(iclass);
     
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TObject","Basic ROOT object",0,Object::is(*this).version())); //12
    streamerInfo->add(new StreamerString(*this,"fName","object identifier",12));//8
    streamerInfo->add(new StreamerString(*this,"fTitle","object title",20));//8

    //28
  }


  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerInfoClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TNamed","The basis for a named object (name, title)",0,Named::is(*this).version()));//28
    streamerInfo->add(new StreamerBasicType(*this,"fCheckSum","checksum of original class",28,Streamer_Info::UNSIGNED_INT,"UInt_t"));//4
    streamerInfo->add(new StreamerBasicType(*this,"fClassVersion","Class version identifier",32,Streamer_Info::INT,"Int_t"));//4
    //40 = 2*int+4*int_p+2*ulong_p+bool+p (bool=4 !)
    streamerInfo->add(new StreamerObjectPointer(*this,"fElements","Array of TStreamerElements",76,"TObjArray*"));//4

    //80
  }

  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerElementClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TNamed","The basis for a named object (name, title)",0,Named::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fType","element type",28,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fSize","sizeof element",32,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fArrayLength","cumulative size of all array dims",36,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerBasicType(*this,"fArrayDim","number of array dimensions",40,Streamer_Info::INT,"Int_t"));

    {StreamerElement* streamerElement = new StreamerBasicType(*this,"fMaxIndex","Maximum array index for array dimension \"dim\"",44,Streamer_Info::INT,"Int_t");
    streamerInfo->add(streamerElement);
    streamerElement->setArrayDimension(1);
    streamerElement->setMaxIndex(0,5);}

    streamerInfo->add(new StreamerString(*this,"fTypeName","Data type name of data member",72));
  }

  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerBaseClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fBaseVersion","version number of the base class",88,Streamer_Info::INT,"Int_t"));
  }

  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerStringClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));

  }

  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerBasicTypeClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
    
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new StreamerBasicPointer_Class();
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
    streamerInfo->add(new StreamerBasicType(*this,"fCountVersion","version number of the class with the counter",88,Streamer_Info::INT,"Int_t"));
    streamerInfo->add(new StreamerString(*this,"fCountName","name of data member holding the array count",92));
    streamerInfo->add(new StreamerString(*this,"fCountClass","name of the class with the counter",100));
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new StreamerObject_Class();
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
    
  }

  //---------------------------------------------------------------------
  {
    StreamerInfo* streamerInfo = new StreamerInfo(*this,*streamerObjectPointerClass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
    
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new StreamerObjectAny_Class();
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    streamerInfo->add(new StreamerBase(*this,"TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is(*this).version()));
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Directory_Class(fOut);
    f_opt_Directory_Class = iclass;
    addClass(iclass);
    // Has no streamer info. 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new List_Class(*this);
    f_opt_List_Class = iclass;
    addClass(iclass);
    // Has no streamer info. 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new File_Class();
    f_opt_File_Class = iclass;
    addClass(iclass);
    // Has no streamer info. 
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Array_Class();
    f_opt_Array_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;
    offset += Rio_size_VIRTUAL;

    streamerInfo->add(new StreamerBasicType(*this,
      "fN",
      "Number of array elements",
      offset,
      Streamer_Info::INT,"Int_t")); //4
    offset += Rio_size_INT;

    //8
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Array_int_Class(*this);
    f_opt_Array_int_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;

    streamerInfo->add(new StreamerBase(*this,
      "TArray",
      "Abstract array base class",
      offset,
      Array::is(*this).version()));
    offset += Rio_size_TArray;

    streamerInfo->add(new StreamerBasicPointer(*this,
      "fArray",
      "[fN] Array of fN integers",
      offset,
      Streamer_Info::INT,"fN","TArray",1,"Int_t*"));
    offset += Rio_size_POINTER;

    //12
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Array_double_Class(*this);
    f_opt_Array_double_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;

    streamerInfo->add(new StreamerBase(*this,
      "TArray",
      "Abstract array base class",
      offset,
      Array::is(*this).version()));
    offset += Rio_size_TArray; //8

    streamerInfo->add(new StreamerBasicPointer(*this,
      "fArray",
      "[fN] Array of fN integers",
      offset,
      Streamer_Info::DOUBLE,"fN","TArray",1,"Double_t*"));
    offset += Rio_size_POINTER;

    //12
  }

  //---------------------------------------------------------------------
  {
    IClass* iclass = new Array_float_Class(*this);
    f_opt_Array_float_Class = iclass;
    addClass(iclass);

    StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");
    fStreamerInfos.push_back(streamerInfo);
    // Elements :
    int offset = 0;

    streamerInfo->add(new StreamerBase(*this,
      "TArray",
      "Abstract array base class",
      offset,
      Array::is(*this).version()));
    offset += Rio_size_TArray;

    streamerInfo->add(new StreamerBasicPointer(*this,
      "fArray",
      "[fN] Array of fN integers",
      offset,
      Streamer_Info::FLOAT,"fN","TArray",1,"Float_t*"));
    offset += Rio_size_POINTER;

    //12
  }

#define DIC_VECTOR(aClass,aType,aSI_Type)\
 {IClass* iclass = new aClass(*this);\
  addClass(iclass);\
  StreamerInfo* streamerInfo = new StreamerInfo(*this,*iclass,"");\
  fStreamerInfos.push_back(streamerInfo);\
  streamerInfo->add(new StreamerSTL(*this,"This","Used to call the proper TStreamerInfo case",0,aSI_Type,std::string("vector<")+#aType+">"));}

  DIC_VECTOR(Rio::std_vector_char_Class,char,Streamer_Info::CHAR)
  DIC_VECTOR(Rio::std_vector_short_Class,short,Streamer_Info::SHORT)
  DIC_VECTOR(Rio::std_vector_int_Class,int,Streamer_Info::INT)
  DIC_VECTOR(Rio::std_vector_unsigned_char_Class,unsigned char,Streamer_Info::UNSIGNED_CHAR)
  DIC_VECTOR(Rio::std_vector_unsigned_short_Class,unsigned short,Streamer_Info::UNSIGNED_SHORT)
  DIC_VECTOR(Rio::std_vector_unsigned_int_Class,unsigned int,Streamer_Info::UNSIGNED_INT)
  DIC_VECTOR(Rio::std_vector_float_Class,float,Streamer_Info::FLOAT)
  DIC_VECTOR(Rio::std_vector_double_Class,double,Streamer_Info::DOUBLE)

  DIC_VECTOR(Rio::std_vector_bool_Class,bool,Streamer_Info::BOOL)

  return true;
}

/* ROOT tag v3-00-06 :
debug : TStreamerInfo : +++++ TNamed -68599943
debug : TStreamerInfo : +++++ TStreamerInfo 357609799
debug : TStreamerInfo : +++++ TStreamerBase -1623888782
debug : TStreamerInfo : +++++ TStreamerElement -1925148838
debug : TStreamerInfo : +++++ TStreamerBasicType -1293091330
debug : TStreamerInfo : +++++ TStreamerObject -2117510581
debug : TStreamerInfo : +++++ TStreamerObjectAny -1186087169
debug : TStreamerInfo : +++++ TStreamerString -1769387431
debug : TStreamerInfo : +++++ TStreamerBasicPointer 1587298059
debug : TStreamerInfo : +++++ TStreamerObjectPointer 720556968
*/

/* ROOT tag v5-14-00

StreamerInfo for class: TArray, version=1
  Int_t          fN              offset=  0 type= 3 Number of array elements

StreamerInfo for class: TArrayI, version=1
  TArray         BASE            offset=  0 type= 0 Abstract array base class
  Int_t*         fArray          offset=  0 type=43 [fN] Array of fN integers

StreamerInfo for class: TArrayD, version=1
  TArray         BASE            offset=  0 type= 0 Abstract array base class
  Double_t*      fArray          offset=  0 type=48 [fN] Array of fN doubles

StreamerInfo for class: TArrayF, version=1
  TArray         BASE            offset=  0 type= 0 Abstract array base class
  Float_t*       fArray          offset=  0 type=45 [fN] Array of fN doubles

StreamerInfo for class: vector<char>, version=4
  vector<char>   This            offset=  0 type=300 ,stl=1, ctype=1, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<unsigned char>, version=4
  vector<unsigned char> This            offset=  0 type=300 ,stl=1, ctype=11, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<short>, version=4
  vector<short>  This            offset=  0 type=300 ,stl=1, ctype=2, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<int>, version=4
  vector<int>    This            offset=  0 type=300 ,stl=1, ctype=3, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<float>, version=4
  vector<float>  This            offset=  0 type=300 ,stl=1, ctype=5, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<double>, version=4
  vector<double> This            offset=  0 type=300 ,stl=1, ctype=8, Used to call the proper TStreamerInfo case

StreamerInfo for class: vector<string>, version=4
  vector<string> This            offset=  0 type=300 ,stl=1, ctype=61, Used to call the proper TStreamerInfo case

*/
