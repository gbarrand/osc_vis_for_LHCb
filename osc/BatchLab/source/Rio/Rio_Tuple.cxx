// this :
#include <BatchLab/Rio/Tuple.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Array_double.h>

#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BranchElement.h>
#include <Rio/Tree/Leaf.h>
#include <Rio/Tree/LeafString.h>
#include <Rio/Tree/LeafObject.h>
#include <Rio/Tree/LeafElement.h>

#include <inlib/tos>
#include <inlib/cast>

//#include <BatchLab/Rio/Vector.h>

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
#include <AIDA/ITupleColumn.h>
#endif

#include <stdio.h> //sprintf

#include <math.h>

#define NotFound (-1)
#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

typedef unsigned char byte;

namespace BatchLab {

template <class T,class LeafT> 
class RioColumn 
:public BaseColumn 
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
,public virtual AIDA::ITupleColumn<T>
#endif
{
public: //BaseColumn
  virtual unsigned int size() const {
    ::Rio::Branch& branch = fLeaf.branch();
    return (unsigned int)branch.entries();
  }
  virtual void reset() {
    ::Rio::Branch& branch = fLeaf.branch();
    branch.reset();
  }
  virtual void add() {} //NOT USED.
  virtual bool append(void*) { return false;} //NOT USED.
  virtual BaseColumn* copy() const {  //FIXME
    f_out << "BatchLab::RioColumn_T::copy : dummy." << std::endl;
    return 0;
  }
  virtual void resetFilled() { //FIXME
    f_out << "BatchLab::RioColumn_T::resetFilled : dummy." << std::endl;
  }
  virtual std::string stringDefaultValue() const { 
    return inlib::tos(fDefault);
  }
  virtual bool isDefaultDefault() const { 
    return (fDefault==fDefaultDefault?true:false);
  }
  virtual bool read(Slash::Store::IVisitor&) {return false;}
  virtual bool value(int aIndex,Lib::Value& aVariable) const {
    ::Rio::Branch& branch = fLeaf.branch();
    int n;
    if(!branch.findEntry(aIndex,n)) {
      aVariable.setNone();
      return false;
    }
    //aVariable.set(fLeaf.value()); //Does not handle std::vector.
    aVariable.set(*((T*)fLeaf.valuePointer())); //For std::vector.
    return true;
  }
public: //AIDA::ITupleColumn
  virtual bool fill(const T& aValue) {
    if(fBinded) return false;
    *fFilled = aValue;
    return true;
  }
  virtual T& value() const { //FIXME : should return const T&.
    INLIB_SELF(RioColumn);
    ::Rio::Branch& branch = fLeaf.branch();
    int number = (int)branch.entries();
    if((fRow<0)||(fRow>=number)) return self.fDefault;   
    int n;
    if(!branch.findEntry(fRow,n)) {
      return self.fDefault;
    }
    //self.fTmp = fLeaf.value();
    self.fTmp = *((T*)fLeaf.valuePointer()); //For std::vector.
    return self.fTmp;
  }
  virtual bool bind(T* aVariable) {
    if(!fBinded) delete fFilled;
    fFilled = aVariable;
    fBinded = true;
    fLeaf.setAddress(fFilled);
    return true;
  }
  virtual const T& defaultValue() const {
    return fDefault;
  }
  virtual T* fillableObject() const {
    return fFilled;
  }
public:
  RioColumn(std::ostream& a_out,LeafT& aLeaf,const std::string& aType,const T& aDefaultDefault,const T& aDefault)
  :BaseColumn(a_out,aLeaf.name(),aType)
  ,fLeaf(aLeaf)
  ,fFilled(0)
  ,fBinded(false)
  ,fDefaultDefault(aDefaultDefault)
  ,fDefault(aDefault){
    fFilled = new T;
    *fFilled = fDefault;
    fLeaf.setAddress(fFilled);
  }
  virtual ~RioColumn() {
    if(!fBinded) delete fFilled;
  }
  virtual bool value(int aIndex,T& aValue) const {
    ::Rio::Branch& branch = fLeaf.branch();
    int n;
    if(!branch.findEntry(aIndex,n)) {
      aValue = fDefault;
      return false;
    }
    //aValue = fLeaf.value();
    aValue = *((T*)fLeaf.valuePointer()); //For std::vector.
    return true;
  }
private:
  LeafT& fLeaf;
  T* fFilled;
  bool fBinded;
protected:
  T fDefaultDefault;
  T fDefault;
  T fTmp;
};

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2)
#define RIO_COLUMN_SOURCE(aClassName,aType,aLeafType,aSType,aDefaultDefault,aDefault) \
class aClassName : public RioColumn<aType,aLeafType>  {\
public:\
  virtual void* cast(const std::string& a_class) const {\
    INLIB_IF_CAST(BatchLab::aClassName)\
    else return BatchLab::BaseColumn::cast(a_class);\
  }\
public:\
  aClassName(std::ostream& a_out,aLeafType& aLeaf)\
  :RioColumn<aType,aLeafType>(a_out,aLeaf,#aSType,aDefaultDefault,aDefault){}\
  virtual ~aClassName(){}\
};

#else
// The only difference with the upper is in the cast
// method to cast on AIDA::ITupleColumn.

#define RIO_COLUMN_SOURCE(aClassName,aType,aLeafType,aSType,aDefaultDefault,aDefault) \
class aClassName : public RioColumn<aType,aLeafType>  {\
public:\
  virtual void* cast(const std::string& a_class) const {\
    INLIB_IF_CAST(BatchLab::aClassName)\
    else if(aClass==std::string("AIDA::ITupleColumn<")+#aType+">") {\
      return (void*)static_cast<const AIDA::ITupleColumn<aType> *>(this);\
    }\
    else return BatchLab::BaseColumn::cast(a_class);\
  }\
public:\
  aClassName(std::ostream& a_out,aLeafType& aLeaf)\
  :RioColumn<aType,aLeafType>(a_out,aLeaf,#aSType,aDefaultDefault,aDefault){}\
  virtual ~aClassName(){}\
};

#endif

RIO_COLUMN_SOURCE(RioColumn_char,char,::Rio::LeafCharacter,char,0,0)
//FIXME : should have a ::Rio::LeafByte
RIO_COLUMN_SOURCE(RioColumn_byte,byte,::Rio::LeafCharacter,byte,0,0)
RIO_COLUMN_SOURCE(RioColumn_short,short,::Rio::LeafShort,short,0,0)
RIO_COLUMN_SOURCE(RioColumn_int,int,::Rio::LeafInteger,int,0,0)
RIO_COLUMN_SOURCE(RioColumn_float,float,::Rio::LeafFloat,float,0,0)
RIO_COLUMN_SOURCE(RioColumn_double,double,::Rio::LeafDouble,double,0,0)

RIO_COLUMN_SOURCE(RioColumn_vector_char,std::vector<char>,::Rio::LeafElement,char[],std::vector<char>(),std::vector<char>())
RIO_COLUMN_SOURCE(RioColumn_vector_short,std::vector<short>,::Rio::LeafElement,short[],std::vector<short>(),std::vector<short>())
RIO_COLUMN_SOURCE(RioColumn_vector_int,std::vector<int>,::Rio::LeafElement,int[],std::vector<int>(),std::vector<int>())
RIO_COLUMN_SOURCE(RioColumn_vector_float,std::vector<float>,::Rio::LeafElement,float[],std::vector<float>(),std::vector<float>())
RIO_COLUMN_SOURCE(RioColumn_vector_double,std::vector<double>,::Rio::LeafElement,double[],std::vector<double>(),std::vector<double>())

//RIO_COLUMN_SOURCE(RioColumn_vector_std_string,std::vector<std::string>,::Rio::LeafElement,string[],std::vector<std::string>(),std::vector<std::string>())

// LeafString handles internally a char[] and not a std::string.
// In the below the fFilled is a char[].
//RIO_COLUMN_SOURCE(RioColumn_string,std::string,::Rio::LeafString,string,std::string(),std::string())

class RioColumn_string : public BaseColumn {
public: //BaseColumn
  virtual unsigned int size() const {
    ::Rio::Branch& branch = fLeaf.branch();
    return (unsigned int)branch.entries();
  }
  virtual void reset() {
    ::Rio::Branch& branch = fLeaf.branch();
    branch.reset();
  }
  virtual void add() {} //NOT USED.
  virtual bool append(void*) { return false;} //NOT USED.
  virtual BaseColumn* copy() const {  //FIXME
    f_out << "BatchLab::RioColumn_string::copy : dummy." << std::endl;
    return 0;
  }
  virtual void resetFilled() { //FIXME
    f_out << "BatchLab::RioColumn_string::resetFilled : dummy." << std::endl;
  }
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::RioColumn_string)
    else return BatchLab::BaseColumn::cast(a_class);
  }
  virtual std::string stringDefaultValue() const { 
    return fDefault;
  }
  virtual bool isDefaultDefault() const { 
    return (fDefault==fDefaultDefault?true:false);
  }
  virtual bool read(Slash::Store::IVisitor&) {return false;}
  virtual bool value(int aIndex,Lib::Value& aVariable) const {
    ::Rio::Branch& branch = fLeaf.branch();
    int n;
    if(!branch.findEntry(aIndex,n)) {
      aVariable.setNone();
      return false;
    }
    aVariable.set(fLeaf.value());
    return true;
  }
public:
  RioColumn_string(std::ostream& a_out,::Rio::LeafString& aLeaf)
  :BaseColumn(a_out,aLeaf.name(),"string"),fLeaf(aLeaf) {
    fFilled[0] = '\0';
    fLeaf.setAddress(fFilled);
  }
  virtual ~RioColumn_string() {}
  virtual bool fill(const std::string& aValue) {
    //FIXME : should check size.
    ::sprintf(fFilled,"%s",aValue.c_str());
    return true;
  }
  virtual bool value(int aIndex,std::string& aValue) const {
    ::Rio::Branch& branch = fLeaf.branch();
    int n;
    if(!branch.findEntry(aIndex,n)) {
      aValue = "";
      return false;
    }
    aValue = fLeaf.value();
    return true;
  }
private:
  ::Rio::LeafString& fLeaf;
  char fFilled[512];
  std::string fDefaultDefault;
  std::string fDefault;
};

class RioColumn_tuple : public BaseColumn {
public: //BaseColumn
  virtual unsigned int size() const {
    ::Rio::Branch& branch = fLeaf.branch();
    return (unsigned int)branch.entries();
  }
  virtual void reset() {
    ::Rio::Branch& branch = fLeaf.branch();
    branch.reset();
  }
  virtual void add() {} //NOT USED.
  virtual bool append(void*) { return false;} //NOT USED.
  virtual BaseColumn* copy() const {  //FIXME
    f_out << "BatchLab::RioColumn_tuple::copy : dummy." << std::endl;
    return 0;
  }
  virtual void resetFilled() { //FIXME
    f_out << "BatchLab::RioColumn_tuple::resetFilled : dummy." << std::endl;
  }
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::RioColumn_tuple)
    else return BatchLab::BaseColumn::cast(a_class);
  }
  virtual std::string stringDefaultValue() const { return "";}
  virtual bool isDefaultDefault() const { return true;}
  virtual bool read(Slash::Store::IVisitor&) {return false;}
  virtual bool value(int aIndex,Lib::Value& aVariable) const {
    AIDA::ITuple* v;
    bool status = RioColumn_tuple::value(aIndex,v);
    aVariable = Lib::Value((void*)v);
    return status;
  }
public:
  RioColumn_tuple(std::ostream& a_out,
                  ::Rio::LeafObject& aLeaf,
                  const std::list<Lib::Value>& aVariables,
                  ::Rio::Tree* aTree,
                  int aVerbose)
    :BaseColumn(a_out,aLeaf.name(),"ITuple")
    ,fLeaf(aLeaf)
    ,fVariables(aVariables)
    ,fRioTree(aTree)
    ,fVerbose(aVerbose)
    ,fTemplate(0) 
    ,fValue(0) {
  }
  virtual ~RioColumn_tuple() {
    if(fTemplate) delete fTemplate; //Will delete the fRioTree.
    else delete fRioTree;
    delete fValue;
    Lib::Columns::deleteColumns(fVariables);
  }
  AIDA::ITuple* getTuple() {
    if(!fRioTree) {
      f_out << "BatchLab::RioColumn_tuple::getTuple : no ::Rio::Tree !" 
          << std::endl;
      return 0;
    }
    if(!fTemplate) {
      std::string sname = name() + "_template";
      fTemplate = new Rio::Tuple(f_out,sname,fRioTree,fVariables,fVerbose);
    }
    fTemplate->reset();
    return fTemplate;
  }
  virtual bool value(int aIndex,AIDA::ITuple*& aValue) const {
    delete fValue;
    INLIB_SELF(RioColumn_tuple);
    self.fValue = 0;
    ::Rio::Branch& branch = fLeaf.branch(); //Contain ::Rio::Trees
    fLeaf.setAddress(0); //Compell the creation of a new ::Rio::Tree.
    int n;
    if(!branch.findEntry(aIndex,n)) {
      aValue = 0;
      return false;
    }
    ::Rio::IObject* obj = fLeaf.object();
    if(!obj) {
      aValue = 0;
      return false;
    }
    ::Rio::Tree* rioTree = ::Rio::cast< ::Rio::Tree>(*obj);
    if(!rioTree) {
      f_out << "BatchLab::RioColumn_tuple::value :"
          << " object not a ::Rio::Tree." 
          << std::endl;
      aValue = 0;
      return false;
    }
    rioTree->directory().removeObject(rioTree);
    //FIXME : handling fValue is not a great idea.
    // Someone may do twice call to value(...) in the same program....
    self.fValue = 
      new BatchLab::Rio::Tuple(f_out,rioTree->name(),rioTree,fVerbose);
    aValue = fValue;
    return true;
  }
private:
  ::Rio::LeafObject& fLeaf;
  std::list<Lib::Value> fVariables;
  ::Rio::Tree* fRioTree;
  int fVerbose;
  Rio::Tuple* fTemplate;
  Rio::Tuple* fValue;
};

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tuple::Tuple(
 std::ostream& a_out
,::Rio::IDirectory& aDirectory
,const std::string& aName
,const std::string& aTitle
,const std::string& aBooking
,int aVerboseLevel
)
:BaseTuple(a_out,aName,aTitle) 
,fRioTree(0)
,fVerboseLevel(aVerboseLevel)
//////////////////////////////////////////////////////////////////////////////
// To attach a tuple in a file or create one if not found.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Lib::Value> variables;
  if(!booking(out(),aBooking,variables)) return;
  create(aDirectory,aTitle,variables);
  Lib::Columns::deleteColumns(variables);
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tuple::Tuple(
 std::ostream& a_out
,::Rio::IDirectory& aDirectory
,const std::string& aName
,const std::string& aTitle
,const std::list<Lib::Value>& aVariables
,int aVerboseLevel
)
:BaseTuple(a_out,aName,aTitle) 
,fRioTree(0)
,fVerboseLevel(aVerboseLevel)
//////////////////////////////////////////////////////////////////////////////
// To attach a tuple in a file or create one if not found.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  create(aDirectory,aTitle,aVariables);
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Tuple::create(
 ::Rio::IDirectory& aDirectory
,const std::string& aTitle
,const std::list<Lib::Value>& aVariables
)
//////////////////////////////////////////////////////////////////////////////
// To attach a tuple in a file or create one if not found.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string sid = name();
  
  // Remove existing 'sid's in memory :
  // FIXME : Could delete a TObject in memory referenced elsewhere...
  // FIXME : Could delete a TObject with sid which is not a Tuple !
  /*
    while(1) {
    ::Rio::IObject* old = container->findInMemory(sid);
    if(!old) break;
    container->remove(sid); //FIXME
    }
  */
    
  //
  //  The ::Rio::Tree object in memory will be 
  // deleted when closeing the ::Rio::File. 
  //  We have to take care of the reference fRioTree.
  //
  ::Rio::IObject* rioObj = aDirectory.find(sid);
  if(!rioObj) { 
    
    // Not found. Create a new one :
    //printf("debug : BatchLab::Rio::Tuple::create : %ld \"%s\" new\n",
    //this,name().c_str());
    
    fRioTree = new ::Rio::Tree(aDirectory,sid,aTitle);
    if(!fRioTree) {
      out() << "BatchLab::Rio::Tuple::create :" 
          << " error creating a ::Rio::Tree object." << std::endl;
      return;
    }

    createColumns(aVariables);

  } else {

    //printf("debug : BatchLab::Rio::Tuple::create : %ld \"%s\" already in mem\n",
    //this,name().c_str());
      
    ::Rio::Tree* rioTree = ::Rio::cast< ::Rio::Tree>(*rioObj);
    if(!rioTree) {
      out() << "BatchLab::Rio::Tuple::create :"
          << " object \"" << sid << "\" not a ::Rio::Tree." << std::endl;
      aDirectory.removeObject(rioObj);
      delete rioObj;
      rioObj = 0;
    } else {
      fRioTree = rioTree;
      BaseTuple::setTitle(fRioTree->title());
      createColumns(); //FIXME : check variable matching ?
    }
  }

}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tuple::Tuple(
 std::ostream& a_out
,const std::string& aName
,::Rio::Tree* aRioTree
,const std::list<Lib::Value>& aVariables
,int aVerboseLevel
)
:BaseTuple(a_out,aName,aRioTree->title()) 
,fRioTree(aRioTree)
,fVerboseLevel(aVerboseLevel)
//////////////////////////////////////////////////////////////////////////////
// Used by the BatchLab::RioColumn_tuple constructor.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  createColumns(aVariables);
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tuple::Tuple(
 std::ostream& a_out
,const std::string& aName
,::Rio::Tree* aRioTree
,int aVerboseLevel
)
:BaseTuple(a_out,aName,aRioTree->title()) 
,fRioTree(aRioTree)
,fVerboseLevel(aVerboseLevel)
//////////////////////////////////////////////////////////////////////////////
// Used by the BatchLab::Rio::read_Tree reader.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  createColumns();
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tuple::~Tuple(
)
//////////////////////////////////////////////////////////////////////////////
// The ::Rio::Tree object in memory will be deleted when 
// closing the ::Rio::File.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //disconnect the tuple from the store.
  if(fRioTree) {
    fRioTree->directory().removeObject(fRioTree);
    delete fRioTree;
    fRioTree = 0;
  }


 {std::vector< ::Rio::IObject** >::iterator it;
  for(it=fObjectHolders.begin();it!=fObjectHolders.end();++it) delete *it;}
 {std::vector< std::vector<double>* >::iterator it;
  for(it=fVectors.begin();it!=fVectors.end();++it) delete *it;}

}

bool BatchLab::Rio::Tuple::close(){
  //disconnect the tuple from the store.
  if(fRioTree) {
    fRioTree->directory().removeObject(fRioTree);
    delete fRioTree;
    fRioTree = 0;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Tuple::createColumns(
)
//////////////////////////////////////////////////////////////////////////////
// Used by the BatchLab::Rio::read_Tree reader.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Build the BatchLab::RioColumns:
  std::vector< ::Rio::BaseLeaf*>& leaves = fRioTree->leaves();
  std::vector< ::Rio::BaseLeaf*>::const_iterator it;
  for(it=leaves.begin();it!=leaves.end();++it) {
    if(::Rio::LeafFloat* lf = ::Rio::cast< ::Rio::LeafFloat>(*(*it))) {
      fColumns.push_back(new RioColumn_float(out(),*lf));
    } else if(::Rio::LeafDouble* ld = 
                 ::Rio::cast< ::Rio::LeafDouble>(*(*it))) {
      fColumns.push_back(new RioColumn_double(out(),*ld));
    } else if(::Rio::LeafCharacter* lc = 
                ::Rio::cast< ::Rio::LeafCharacter>(*(*it))) {
      fColumns.push_back(new RioColumn_char(out(),*lc));
    } else if(::Rio::LeafShort* lsh = 
                ::Rio::cast< ::Rio::LeafShort>(*(*it))) {
      fColumns.push_back(new RioColumn_short(out(),*lsh));
    } else if(::Rio::LeafInteger* li = 
                 ::Rio::cast< ::Rio::LeafInteger>(*(*it))) {
      fColumns.push_back(new RioColumn_int(out(),*li));
    } else if(::Rio::LeafString* ls = 
                ::Rio::cast< ::Rio::LeafString>(*(*it))) {
      fColumns.push_back(new RioColumn_string(out(),*ls));
    } else if(::Rio::LeafObject* lo = 
                ::Rio::cast< ::Rio::LeafObject>(*(*it))) {
      ::Rio::IClass* objClass = lo->objectClass();
      if(!objClass) {
        out() << "BatchLab::Rio::Tuple::createColumns :" 
            << " ::Rio::LeafObject without an object class."
            << std::endl;
      } else {
        if(objClass->name()=="Rio::Tree") {
          std::list<Lib::Value> vars;
          fColumns.push_back
            (new RioColumn_tuple(out(),*lo,vars,0,fVerboseLevel));
        } else {
          out() << "BatchLab::Rio::Tuple::createColumns :" 
              << " ::Rio::LeafObject object class "
              << inlib::sout(objClass->name()) << " not handled."
              << std::endl;
        }
      }
    } else if(::Rio::LeafElement* le = 
                ::Rio::cast< ::Rio::LeafElement>(*(*it))) {
      ::Rio::Branch& branch = le->branch();
      ::Rio::BranchElement* be = ::Rio::cast< ::Rio::BranchElement>(branch);
      if(!be) {
        out() << "BatchLab::Rio::Tuple::createColumns :" 
            << " branch is not a Rio::BranchElement."
            << std::endl;
      } else {
        if( (be->className()=="std::vector<double>") ||
            (be->className()=="vector<double>") ) {
          std::vector<double>* array_double = new std::vector<double>;
          fVectors.push_back(array_double);
          if(!be->setAddress(array_double)) {
            out() << "BatchLab::Rio::Tuple::createColumns :" 
                << " can't set address on the Rio::BranchElement."
                << std::endl;
          } else {
            fColumns.push_back(new RioColumn_vector_double(out(),*le));
          }
        } else {
          out() << "BatchLab::Rio::Tuple::createColumns :" 
              << " class "
              << inlib::sout(be->className()) << " not handled."
              << std::endl;
        }
      }
    } else {
      out() << "BatchLab::Rio::Tuple::createColumns :" 
          << " unhandle leaf type " << inlib::sout((*it)->isA().name()) << "."
          << std::endl;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::isValid(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fRioTree?true:false;
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::Tuple::copy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Rio::Tuple::copy : dummy." << std::endl;
  //FIXME return new BatchLab::Rio::Tuple(aName,*this);
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tuple::storeClassName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Rio::Tuple";
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Rio::Tuple::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  INLIB_IF_CAST(BatchLab::Rio::Tuple)
  else return BaseTuple::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::setTitle(
 const std::string& aTitle
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioTree) return false;
  fRioTree->setTitle(aTitle);
  BaseTuple::setTitle(aTitle);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Tuple::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //BaseTuple::reset();
  if(!fRioTree) return;
  fRioTree->reset(); //Reset also branches.
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Rio::Tuple::rows(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioTree) return 0;
  return (int)fRioTree->entries();
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::addRow(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioTree) return false;
  int n;
  return fRioTree->fill(n);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::createColumns(
 const std::list<Lib::Value>& aVariables
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Lib::Value>::const_iterator it;
  for(it=aVariables.begin();it!=aVariables.end();++it) {
    std::string name = (*it).label();
    if(findColumn(name)!=NotFound) {
      out() << "BatchLab::Rio::Tuple::Rio::Tuple :"
          << " column " << name 
          << " already exists" << std::endl;
      clear();
      return false;
    }
    //const std::string& stype = ;
    //printf("debug : BatchLab::Rio::Tuple::createColumns : %ld \"%s\" \"%s\"\n",
    //this,name.c_str(),stype.c_str());

    switch((*it).type()) {
    case Slash::Core::IValue::INT:
    case Slash::Core::IValue::DOUBLE:
    case Slash::Core::IValue::BOOL:
    case Slash::Core::IValue::SHORT:
    case Slash::Core::IValue::INT64:
    case Slash::Core::IValue::FLOAT:
    case Slash::Core::IValue::UNSIGNED_CHAR:
    case Slash::Core::IValue::CHAR:

    case Slash::Core::IValue::ARRAY_DOUBLE:

    case Slash::Core::IValue::STRING:{
      std::list<Lib::Value> vars;
      addColumn(name,convertType((*it).type()),vars);
     }break;
    case Slash::Core::IValue::VOID_STAR:{ //Assum AIDA::ITuple.
      std::list<Lib::Value>* pvars = 
        (std::list<Lib::Value>*)(*it).get_void_star();
      std::list<Lib::Value> vars;
      Lib::Columns::copyColumns(*pvars,vars);
      addColumn(name,convertType((*it).type()),vars);
     }break;

    //case UNSIGNED_INTEGER:
    //case UNSIGNED_INT64:
    default:
      out() << "BatchLab::Rio::Tuple::createColumns :" 
          << " unhandle type " << inlib::sout((*it).stype()) 
          << " for variable with name " << inlib::sout(name) 
          << "." 
          << std::endl;
      clear();
      return false;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Tuple::addColumn(
 const std::string& aName
,const std::string& aType
,const std::list<Lib::Value>& aVariables
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aType=="float") { 
    std::string s = aName;
    s += "/F";
    float dummy;
    fRioTree->createBranch(aName,&dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a float branch" 
          << std::endl;
    } else if(::Rio::LeafFloat* lf = ::Rio::cast< ::Rio::LeafFloat>(*leaf)) {
      fColumns.push_back(new RioColumn_float(out(),*lf));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not a float one !" 
          << std::endl;
    }
  } else if(aType=="double") { 
    std::string s = aName;
    s += "/D";
    double dummy;
    fRioTree->createBranch(aName,&dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a double branch" 
          << std::endl;
    } else if(::Rio::LeafDouble* ld = 
                ::Rio::cast< ::Rio::LeafDouble>(*leaf)) {
      fColumns.push_back(new RioColumn_double(out(),*ld));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not a double one !" 
          << std::endl;
    }
  } else if(aType=="char") { 
    std::string s = aName;
    s += "/B";
    char dummy;
    fRioTree->createBranch(aName,&dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a char branch" 
          << std::endl;
    } else if(::Rio::LeafCharacter* lc = 
                ::Rio::cast< ::Rio::LeafCharacter>(*leaf)) {
      fColumns.push_back(new RioColumn_char(out(),*lc));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not a char one !" 
          << std::endl;
    }
  } else if(aType=="short") { 
    std::string s = aName;
    s += "/S";
    short dummy;
    fRioTree->createBranch(aName,&dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a short branch" 
          << std::endl;
    } else if(::Rio::LeafShort* lsh = 
                ::Rio::cast< ::Rio::LeafShort>(*leaf)) {
      fColumns.push_back(new RioColumn_short(out(),*lsh));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not an short one !" 
          << std::endl;
    }
  } else if(aType=="int") { 
    std::string s = aName;
    s += "/I";
    int dummy;
    fRioTree->createBranch(aName,&dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a int branch" 
          << std::endl;
    } else if(::Rio::LeafInteger* li = 
                ::Rio::cast< ::Rio::LeafInteger>(*leaf)) {
      fColumns.push_back(new RioColumn_int(out(),*li));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not an int one !" 
          << std::endl;
    }
  } else if(aType=="string") {
    std::string s = aName;
    s += "/C";
    char* dummy = 0;
    fRioTree->createBranch(aName,dummy,s);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(aName);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a string branch" 
          << std::endl;
    } else if(::Rio::LeafString* ls = 
                ::Rio::cast< ::Rio::LeafString>(*leaf)) {
      fColumns.push_back(new RioColumn_string(out(),*ls));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not a string one !" 
          << std::endl;
    }

  } else if(aType=="double[]") {
    std::string sname = aName;
    ::Rio::IClass* std_vector_double_class = 
      fRioTree->dictionary().findClass("std::vector<double>");
    if(!std_vector_double_class) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " can't find class for std::vector<double>."
          << std::endl;
    } else {
      std::vector<double>* obj = new std::vector<double>;
      fVectors.push_back(obj);
      fRioTree->createBranchElement(sname,*std_vector_double_class,obj);
      ::Rio::BaseLeaf* leaf = fRioTree->leaf(sname);
      if(!leaf) {
        out() << "BatchLab::Rio::Tuple::addColumn :" 
            << " unable to create a double[] branch" 
            << std::endl;
        //delete pobj;
        //delete branch;
      } else if(::Rio::LeafElement* le = 
                  ::Rio::cast< ::Rio::LeafElement>(*leaf)) {
        fColumns.push_back(new RioColumn_vector_double(out(),*le));
      } else { 
        out() << "BatchLab::Rio::Tuple::addColumn :" 
            << " created branch is not a LeafElement one !" 
            << std::endl;
        //delete pobj;
        //delete branch;
        //delete rioTree;
      }
    }

  } else if(aType=="ITuple") {
    std::string sname = aName;
    ::Rio::Tree* rioTree = new ::Rio::Tree(fRioTree->directory(),sname,sname);
    //Deleted by the RioColumn_tuple fTemplate Rio::Tuple.
    if(!rioTree) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a ::Rio::Tree for the column." 
          << std::endl;
      return;
    }
    ::Rio::IObject* obj = static_cast< ::Rio::IObject*>(rioTree);
    // Remove the sub tree from the IDirectory, else its going 
    // to be saved in the file. Only the main ::Rio::Tree should appear
    // in the file.
    fRioTree->directory().removeObject(obj);

    typedef ::Rio::IObject* Holder;
    ::Rio::IObject** pobj = new Holder;
    *pobj = obj;
    fObjectHolders.push_back(pobj);

    //Rio::Branch* branch = 
    fRioTree->createBranch(sname,::Rio::Tree::is(fRioTree->dictionary()),pobj);
    ::Rio::BaseLeaf* leaf = fRioTree->leaf(sname);
    if(!leaf) {
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " unable to create a ITuple branch" 
          << std::endl;
      //delete pobj;
      //delete branch;
    } else if(::Rio::LeafObject* ls = 
                ::Rio::cast< ::Rio::LeafObject>(*leaf)) {
      fColumns.push_back(
        new RioColumn_tuple(out(),*ls,aVariables,rioTree,fVerboseLevel));
    } else { 
      out() << "BatchLab::Rio::Tuple::addColumn :" 
          << " created branch is not a LeafObject one !" 
          << std::endl;
      //delete pobj;
      //delete branch;
      //delete rioTree;
    }
  } else {
    out() << "BatchLab::Rio::Tuple::addColumn :" 
        << " type " << inlib::sout(aType) << " not yet supported." 
        << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define GWRITE(T,ST,GT,MT) \
T BatchLab::Rio::Tuple::GT(int aColumn) const{\
  BaseColumn* column = findColumn1(aColumn);\
  if(!column) {\
    out() << "BatchLab::Rio::Tuple::get(" << ST << ") :"	\
          << " column " << aColumn << " not found."\
          << std::endl;\
    return 0;\
  }\
  MT* col = INLIB_CAST(*column,MT);\
  if((column->type()!=ST)||(!col)) {\
    out() << "BatchLab::Rio::Tuple::get(" << ST << ") :"		\
          << " type mismatch (" << inlib::sout(column->type()) << ")"\
          << " for column " << aColumn << " "\
          << inlib::sout(column->name()) << "."\
          << std::endl;\
    return 0;\
  }\
  T value;\
  if(!col->value(fRow,value)) {\
    out() << "BatchLab::Rio::Tuple::get(" << ST << ") :"	\
          << " can't get value."\
          << std::endl;\
    return 0;\
  }\
  return value;\
}

GWRITE(float,"float",getFloat,BatchLab::RioColumn_float)
GWRITE(double,"double",getDouble,BatchLab::RioColumn_double)
//GWRITE(bool,"boolean",getBoolean,BatchLab::RioColumn_bool)
GWRITE(char,"char",getChar,BatchLab::RioColumn_char)
GWRITE(byte,"byte",getByte,BatchLab::RioColumn_byte)
GWRITE(short,"short",getShort,BatchLab::RioColumn_short)
GWRITE(int,"int",getInt,BatchLab::RioColumn_int)
//GWRITE(long,"long",getLong,BatchLab::RioColumn_long)
GWRITE(std::string,"string",getString,BatchLab::RioColumn_string)

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
//////////////////////////////////////////////////////////////////////////////
long BatchLab::Rio::Tuple::getLong(
 int /*aColumn*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Rio::Tuple::getLong : dummy." << std::endl;
  return 0L; //FIXME
}
#else
//////////////////////////////////////////////////////////////////////////////
AIDA::int64 BatchLab::Rio::Tuple::getLong(
 int /*aColumn*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Rio::Tuple::getLong : dummy." << std::endl;
  return 0L; //FIXME
}
#endif

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::getBoolean(
 int /*aColumn*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Rio::Tuple::getBoolean : dummy." << std::endl;
  return false; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITuple* BatchLab::Rio::Tuple::findTuple(
 int
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Rio::Tuple::findTuple const : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* BatchLab::Rio::Tuple::findTuple(
 int aColumn
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  BaseColumn* column = findColumn1(aColumn);
  if(!column) {
    out() << "BatchLab::Rio::Tuple::findTuple :"
        << " column " << aColumn << " not found."
        << std::endl;
    return 0;
  }
  BatchLab::RioColumn_tuple* col = INLIB_CAST(*column,BatchLab::RioColumn_tuple);
  if((column->type()!="ITuple")||(!col)) {
    out() << "BatchLab::Rio::Tuple::findTuple : :"
        << " type mismatch (" << inlib::sout(column->type()) << ")"
        << " for column " << aColumn << " "
        << inlib::sout(column->name()) << "."
        << std::endl;
    return 0;
  }
  return col->getTuple();
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITuple* BatchLab::Rio::Tuple::getTuple(
 int aColumn
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return findTuple(aColumn);
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* BatchLab::Rio::Tuple::getTuple(
 int aColumn
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return findTuple(aColumn);
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITupleEntry* BatchLab::Rio::Tuple::getObject(
 int aColumn
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  BaseColumn* column = findColumn1(aColumn);
  if(!column) {\
    out() << "BatchLab::Rio::Tuple::getObject :"
        << " column " << aColumn << " not found."
        << std::endl;
    return 0;
  }
  RioColumn_tuple* col = INLIB_CAST(*column,BatchLab::RioColumn_tuple);
  if((column->type()!="ITuple")||(!col)) {
    out() << "BatchLab::Rio::Tuple::getObject :"
        << " type mismatch (" << inlib::sout(column->type()) << ")"
        << " for column " << aColumn << " " 
        << inlib::sout(column->name()) << "."
        << std::endl;
    return 0;
  }
  AIDA::ITuple* value = 0;
  if(!col->value(fRow,value)) {
    out() << "BatchLab::Rio::Tuple::getObject :"
        << " can't get value."
        << std::endl;
    return 0;
  }
  if(!value) return 0;
  return (const AIDA::ITupleEntry*)INLIB_CAST(*value,AIDA::ITupleEntry);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define FWRITE(T,ST,FT,MT) \
bool BatchLab::Rio::Tuple::FT(int aColumn,T aValue){\
  BaseColumn* column = findColumn1(aColumn);\
  if(!column) {\
    out() << "BatchLab::Rio::Tuple::fill(" << ST << ") :"	\
          << " column " << aColumn << " not found."\
          << std::endl;\
    return false;\
  }\
  MT* col = INLIB_CAST(*column,MT);\
  if((column->type()!=ST)||(!col)) {\
    out() << "BatchLab::Rio::Tuple::fill(" << ST << ") :"		\
          << " type mismatch (" << inlib::sout(column->type()) << ")"\
          << " for column " << aColumn << " "\
          << inlib::sout(column->name()) << "."\
          << std::endl;\
    return false;\
  }\
  return col->fill(aValue);\
}

FWRITE(float,"float",fill,BatchLab::RioColumn_float)
FWRITE(double,"double",fill,BatchLab::RioColumn_double)
//FWRITE(bool,"boolean",fill,BatchLab::RioColumn_bool)
FWRITE(char,"char",fill,BatchLab::RioColumn_char)
FWRITE(short,"short",fill,BatchLab::RioColumn_short)
FWRITE(int,"int",fill,BatchLab::RioColumn_int)
//FWRITE(long,"long",BatchLab::RioColumn_long)

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
bool BatchLab::Rio::Tuple::fill(int /*aColumn*/,long /*aValue*/){ 
  out() << "BatchLab::Rio::Tuple::fill(long) : dummy." << std::endl;
  return false; //FIXME
}
#else
bool BatchLab::Rio::Tuple::fill(int /*aColumn*/,AIDA::int64 /*aValue*/){ 
  out() << "BatchLab::Rio::Tuple::fill(AIDA::int64) : dummy." << std::endl;
  return false; //FIXME
}
FWRITE(byte,"byte",fill,BatchLab::RioColumn_byte)
#endif

bool BatchLab::Rio::Tuple::fill(int /*aColumn*/,bool /*aValue*/){ 
  out() << "BatchLab::Rio::Tuple::fill(bool) : dummy." << std::endl;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::fill(
 int aColumn
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  BaseColumn* column = findColumn1(aColumn);
  if(!column) {
    out() << "BatchLab::Rio::Tuple::fill(string) :"
        << " column " << aColumn << " not found."
        << std::endl;
    return false;
  }
  RioColumn_string* col = INLIB_CAST(*column,BatchLab::RioColumn_string);  
  if((column->type()!="string")||(!col)) {
    out() << "BatchLab::Rio::Tuple::fill(string) :"
        << " type mismatch (" << inlib::sout(column->type()) << ")"
        << " for column " << aColumn << " "
        << inlib::sout(column->name()) << "."
        << std::endl;
    return false;
  }
  return col->fill(aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::fill(
 int //aColumn
,const AIDA::ITupleEntry& //aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  out() << "BatchLab::Rio::Tuple::fill(ITupleEntry) : dummy." << std::endl;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::fill(
 const std::vector<double>& aRow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  unsigned int n = aRow.size();
  if(n!=fColumns.size()) {
    out() << "BatchLab::Rio::Tuple::fill :" 
        << " number of column mismatch (expected = " 
        << columns() << ", given = " << n << ")" << std::endl;
    return false;
  }
  for(unsigned int count=0;count<n;count++) {
    if(!fill(count,aRow[count])) return false;
  }
  return addRow();
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tuple::fill(
 const std::vector<float>& aRow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  unsigned int n = aRow.size();
  if(n!=fColumns.size()) {
    out() << "BatchLab::Rio::Tuple::fill : number of column mismatch (expected = "
        << columns() << ", given = " << n << ")" << std::endl;
    return false;
  }
  for(unsigned int count=0;count<n;count++) {
    if(!fill(count,aRow[count])) return false;
  }
  return addRow();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define COLMINMAX(T,MT,OP) \
    MT* col = INLIB_CAST(*column,MT);\
    T value;\
    col->value(0,value);\
    T v = value;\
    for(int row=1;row<rown;row++) {\
      if(!col->value(row,value)) {\
        out() << "BatchLab::Rio::Tuple::column[Min,Max] :"	\
              << " can't get value."\
              << std::endl;\
        return (double)0;\
      }\
      v = OP(v,value);\
    }\
    return (double)v


//////////////////////////////////////////////////////////////////////////////
double BatchLab::Rio::Tuple::columnMin(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  BaseColumn* column = findColumn1(aColumn);
  if(!column) return 0;
  int rown = rows();
  if(rown<=0) return 0;
  if(column->type()=="float") {
    COLMINMAX(float,BatchLab::RioColumn_float,MINIMUM);
  } else if(column->type()=="double") {
    COLMINMAX(double,BatchLab::RioColumn_double,MINIMUM);
  } else if(column->type()=="char") {
    COLMINMAX(char,BatchLab::RioColumn_char,MINIMUM);
  } else if(column->type()=="short") {
    COLMINMAX(short,BatchLab::RioColumn_short,MINIMUM);
  } else if(column->type()=="int") {
    COLMINMAX(int,BatchLab::RioColumn_int,MINIMUM);
/*
  } else if(column->type()=="long") {
    COLMINMAX(long,RioColumn_long,MINIMUM);
  } else if(column->type()=="boolean") {
    RioColumn_bool* col = INLIB_CAST(*column,BatchLab::RioColumn_bool);
    bool value;
    col->value(0,value);
    double v = value;
    double b;
    for(int row=1;row<rown;row++) {
      if(!col->value(row,value)) {
      }
      b = value?1:0;
      v = MINIMUM(v,b);
    }
    return v;
*/
  } else {
    out() << "BatchLab::Rio::Tuple::columnMin :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Rio::Tuple::columnMax(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  BaseColumn* column = findColumn1(aColumn);
  if(!column) return 0;
  int rown = rows();
  if(rown<=0) return 0;
  if(column->type()=="float") {
    COLMINMAX(float,BatchLab::RioColumn_float,MAXIMUM);
  } else if(column->type()=="double") {
    COLMINMAX(double,BatchLab::RioColumn_double,MAXIMUM);
  } else if(column->type()=="char") {
    COLMINMAX(char,BatchLab::RioColumn_char,MAXIMUM);
  } else if(column->type()=="short") {
    COLMINMAX(short,BatchLab::RioColumn_short,MAXIMUM);
  } else if(column->type()=="int") {
    COLMINMAX(int,BatchLab::RioColumn_int,MAXIMUM);
/*
  } else if(column->type()=="long") {
    COLMINMAX(long,BatchLab::RioColumn_long,MAXIMUM);
  } else if(column->type()=="boolean") {
    RioColumn_bool* col = (RioColumn_bool*)column;
    bool value;
    col->value(0,value);
    double v = value;
    double b;
    for(int row=1;row<rown;row++) {
      if(!col->value(row,value)) {
      }
      b = value?1:0;
      v = MAXIMUM(v,b);
    }
    return v;
*/
  } else {
    out() << "BatchLab::Rio::Tuple::columnMin :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Rio::Tuple::columnMean(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
#define COLMEAN(T,MT) \
    MT* col = INLIB_CAST(*column,MT);\
    double sv = 0;\
    T value = 0;\
    for(int row=0;row<rown;row++) {\
      if(!col->value(row,value)){\
        out() << "BatchLab::Rio::Tuple::columnMean :"	\
              << " can't get value."\
              << std::endl;\
        return (double)0;\
      }\
      sv += value;\
    }\
    return sv/rown

  BaseColumn* column = findColumn1(aColumn);
  if(!column) return 0;
  int rown = rows();
  if(rown<=0) return 0;
  if(column->type()=="float") {
    COLMEAN(float,BatchLab::RioColumn_float);
  } else if(column->type()=="double") {
    COLMEAN(double,BatchLab::RioColumn_double);
  } else if(column->type()=="char") {
    COLMEAN(char,BatchLab::RioColumn_char);
  } else if(column->type()=="short") {
    COLMEAN(short,BatchLab::RioColumn_short);
  } else if(column->type()=="int") {
    COLMEAN(int,BatchLab::RioColumn_int);
/*
  } else if(column->type()=="long") {
    COLMEAN(long,BatchLab::RioColumn_long);
  } else if(column->type()=="boolean") {
    RioColumn_bool* col = INLIB_CAST(*column,BatchLab::RioColumn_bool);
    double sv = 0;
    bool value = false;
    double b;
    for(int row=0;row<rown;row++) {
      col->value(row,value);
      b = value?1:0;
      sv += b;
    } 
    double mean = sv/((double)rown);
    return mean;
*/
  } else {
    out() << "BatchLab::Rio::Tuple::columnMean :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Rio::Tuple::columnRms(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
#define COLRMS(T,MT) \
    MT* col = INLIB_CAST(*column,MT);\
    double sv = 0;\
    double sv2 = 0;\
    T value = 0;\
    for(int row=0;row<rown;row++) {\
      if(!col->value(row,value)){\
        out() << "BatchLab::Rio::Tuple::columnRms :"	\
              << " can't get value."\
              << std::endl;\
        return (double)0;\
      }\
      sv += (double)value;\
      sv2 += (double)value * (double)value;\
    }\
    double mean = sv/((double)rown);\
    double rms = ::sqrt(::fabs(sv2/((double)rown) - mean * mean));\
    return rms

  BaseColumn* column = findColumn1(aColumn);
  if(!column) return 0;
  int rown = rows();
  if(rown<=0) return 0;
  if(column->type()=="float") {
    COLRMS(float,BatchLab::RioColumn_float);
  } else if(column->type()=="double") {
    COLRMS(double,BatchLab::RioColumn_double);
  } else if(column->type()=="char") {
    COLRMS(char,BatchLab::RioColumn_char);
  } else if(column->type()=="short") {
    COLRMS(short,BatchLab::RioColumn_short);
  } else if(column->type()=="int") {
    COLRMS(int,BatchLab::RioColumn_int);
/*
  } else if(column->type()=="long") {
    COLRMS(long,BatchLab::RioColumn_long);
  } else if(column->type()=="boolean") {
    RioColumn_bool* col = INLIB_CAST(*column,BatchLab::RioColumn_bool);
    double sv = 0;
    double sv2 = 0;
    bool value = false;
    double b;
    for(int row=0;row<rown;row++) {
      col->value(row,value);
      b = value?1:0;
      sv += b;
      sv2 += b * b;
    } 
    double mean = sv/((double)rown);
    double rms = ::sqrt(::fabs(sv2/((double)rown) - mean * mean));
    return rms;
*/
  } else {
    out() << "BatchLab::Rio::Tuple::columnMean :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
