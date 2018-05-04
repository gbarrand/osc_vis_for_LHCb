
#include <BatchLab/Zebra/Tuple.h>

#include <inlib/tos>
#include <inlib/cast>

#include <BatchLab/Zebra/CHBOOK.h>

#include <cstdio> //sprintf
#include <cmath>

#define NotFound (-1)
#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

typedef unsigned char byte;

namespace BatchLab {

template <class T,class RT> class ZebraColumn : public BaseColumn {
public: //BaseColumn
  virtual unsigned int size() const {return 0;}
  virtual void reset() {}
  virtual void add() {}
  virtual BaseColumn* copy() const { return 0;/*FIXME*/}
  virtual void resetFilled() {/*FIXME*/};
  virtual std::string stringDefaultValue() const { 
    return inlib::tos(fDefault);
  }
  virtual bool isDefaultDefault() const { 
    return (fDefault==fDefaultDefault?true:false);
  }
  virtual bool append(void*) {/* NOT USED */ return false;}
  virtual bool read(Slash::Store::IVisitor&) {return false;}
  virtual bool value(int aIndex,Lib::Value& aVariable) const {
    T v;
    bool status = ZebraColumn::value(aIndex,v);
    aVariable.set(v);
    return status;
  }
public:
  ZebraColumn(std::ostream& a_out,const std::string& aLabel,const std::string& aType,const T& aDefaultDefault,const T& aDefault)
  :BaseColumn(a_out,aLabel,aType),fAddress(0)
  ,fDefaultDefault(aDefaultDefault)
  ,fDefault(aDefault){}
  virtual ~ZebraColumn() {}
  void setAddress(RT* aAddress) {
    fAddress = aAddress;
  }  
  virtual bool fill(T aValue) {
    //if(!fAddress) return false;
    *fAddress = (RT)aValue;
    return true;
  }
  virtual bool value(int /*aIndex*/,T& aValue) const {
    aValue = fDefault;
    return false;
  }
private:
  RT* fAddress;
protected:
  T fDefaultDefault;
  T fDefault;
};

//public:\
//  virtual std::string defaultValue() const {\
//    return inlib::tos(fDefault);\
//  }\
//

#define ZebraColumn_T(T,ST,CT,RT) \
class CT : public ZebraColumn<T,RT>  {\
public:\
  CT(std::ostream& a_out,const std::string& aLabel)\
  :ZebraColumn<T,RT>(a_out,aLabel,ST,0,0){}\
  virtual ~CT(){}\
};

ZebraColumn_T(float,"float",ZebraColumn_float,float)
ZebraColumn_T(double,"double",ZebraColumn_double,double)
ZebraColumn_T(int,"int",ZebraColumn_int,int)

//mapped on an int.
ZebraColumn_T(bool,"boolean",ZebraColumn_bool,int)
ZebraColumn_T(char,"char",ZebraColumn_char,int)
ZebraColumn_T(byte,"byte",ZebraColumn_byte,int)
ZebraColumn_T(short,"short",ZebraColumn_short,int)

class ZebraColumn_string : public BaseColumn {
public: //BaseColumn
  virtual unsigned int size() const { return 0;}
  virtual void reset() {}
  virtual void add() {}
  virtual BaseColumn* copy() const { return 0; /*FIXME*/}
  virtual void resetFilled() {/*FIXME*/}
  virtual std::string stringDefaultValue() const { 
    return fDefault;
  }
  virtual bool isDefaultDefault() const { 
    return (fDefault==fDefaultDefault?true:false);
  }
  virtual bool append(void*) {/* NOT USED */ return false;}
  virtual bool read(Slash::Store::IVisitor&) {return false;}
  virtual bool value(int aIndex,Lib::Value& aVariable) const {
    std::string v;
    bool status = ZebraColumn_string::value(aIndex,v);
    aVariable.set(v);
    return status;
  }
public:
  ZebraColumn_string(std::ostream& a_out,const std::string& aLabel)
  :BaseColumn(a_out,aLabel,"string") {
    fData[0] = '\0';
  }
  virtual ~ZebraColumn_string() {}
  void setAddress() { }
  virtual bool fill(const std::string& aValue) {
    //FIXME : should check size.
    ::sprintf(fData,"%s",aValue.c_str());
    return true;
  }
  virtual bool value(int /*aIndex*/,std::string& aValue) const {
    aValue = "";
    return false;
  }
private:
  char fData[512];
  std::string fDefaultDefault;
  std::string fDefault;
};

}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Tuple::Tuple(
 std::ostream& a_out
,const std::string& aPath
,int aID
,const std::string& aTitle
,const std::string& aBooking
,int aVerboseLevel
)
:BaseTuple(a_out,inlib::to<int>(aID),aTitle) 
,fVerboseLevel(aVerboseLevel)
,fPath(aPath)
,fID(aID)
,fBuffer(0)
,fPos(0)
,fIsValid(true)
//////////////////////////////////////////////////////////////////////////////
// To create a tuple in a file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBuffer = new char[500000];
  fPos = fBuffer;
  //FIXME : remove some existing aID in memory.
  //printf("debug : %d : create tuple %s\n",fID,aTitle.c_str());
  //printf("debug : +++++=== %d \"%s\"\n",fID,aPath.c_str());

  cd_beg();
  CHBNT(fID,aTitle," ");
  cd_end();

  std::list<Lib::Value> variables;
  if(!booking(out(),aBooking,variables)) return;
  std::list<Lib::Value>::const_iterator it;
  for(it=variables.begin();it!=variables.end();++it) {
    if(!addColumn((*it).label(),convertType((*it).type()))) {
      fIsValid = false;
      cd_beg();
      CHDELET(fID);
      cd_end();
      break;
    }
  }
  Lib::Columns::deleteColumns(variables);
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Tuple::~Tuple(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete [] fBuffer;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::isValid(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIsValid;
}

void BatchLab::Zebra::Tuple::cd_beg() const{
  INLIB_SELF(BatchLab::Zebra::Tuple);
  self.f_tmp = CHPWD();

  CHCDIR(fPath," ");

  // cd on file :
 {std::string s = fPath; //fPath is //PAWC/LUN<i>/....
  inlib::replace(s,"//PAWC","/");
  CHCDIR(s," ");}
}
void BatchLab::Zebra::Tuple::cd_end() const {CHCDIR(f_tmp," ");}

//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Tuple::copy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME return new BatchLab::Zebra::Tuple(aName,*this);
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Tuple::storeClassName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Zebra::Tuple";
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Zebra::Tuple::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  INLIB_IF_CAST(BatchLab::Zebra::Tuple)
  else return BaseTuple::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::setTitle(
 const std::string& //aTitle
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Tuple::rows(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::addRow(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHFNT(fID);
  cd_end();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::addColumn(
 const std::string& aName
,const std::string& aType
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = findColumn(aName);
  if(index!=NotFound) return true; //Done.
  if(aType=="int") { 
    ZebraColumn_int* array = new ZebraColumn_int(out(),aName);
    std::string type = aName+":I";
    // Data address :
    array->setAddress((int*)fPos);
    std::string block = aName.size()>8?aName.substr(0,8):aName;
    cd_beg();
    CHBNAME(fID,block,fPos,type);
    cd_end();
    fPos += 8;
    fColumns.push_back(array);

  } else if(aType=="float") { 
    ZebraColumn_float* array = new ZebraColumn_float(out(),aName);
    std::string type = aName+":R";
    // Data address :
    array->setAddress((float*)fPos);
    std::string block = aName.size()>8?aName.substr(0,8):aName;
    cd_beg();
    CHBNAME(fID,block,fPos,type);
    cd_end();
    // If putting 4 in the below ; on a 32bit Linux we get :  
    // ***** ERROR in HBNAME : Address not double-word aligned : ID=     100 
    fPos += 8;
    fColumns.push_back(array);

  } else if(aType=="double") { 
    ZebraColumn_double* array = new ZebraColumn_double(out(),aName);
    std::string type = aName+":R*8";
    // Data address :
    array->setAddress((double*)fPos);
    std::string block = aName.size()>8?aName.substr(0,8):aName;
    cd_beg();
    CHBNAME(fID,block,fPos,type);
    cd_end();
    fPos += 8;
    fColumns.push_back(array);

  } else if(aType=="boolean") { 
    ZebraColumn_bool* array = new ZebraColumn_bool(out(),aName);
    std::string type = aName+":I"; //mapped to an int.
    // Data address :
    array->setAddress((int*)fPos);
    std::string block = aName.size()>8?aName.substr(0,8):aName;
    cd_beg();
    CHBNAME(fID,block,fPos,type);
    cd_end();
    fPos += 8;
    fColumns.push_back(array);

  } else {
    out() << "BatchLab::Zebra::Tuple::addColumn :" 
        << " type " << inlib::sout(aType) << " not yet supported." 
        << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define GWRITE(T,ST,GT,MT) \
T BatchLab::Zebra::Tuple::GT(int aColumn) const{\
  BaseColumn* column = findColumn1(aColumn);\
  if(!column) {\
    out() << "BatchLab::Zebra::Tuple::get(" << ST << ") :"\
        << " column " << aColumn << " not found."\
        << std::endl;\
    return 0;\
  }\
  if(column->type()!=ST) {\
    out() << "BatchLab::Zebra::Tuple::get(" << ST << ") :"\
        << " type mismatch (" << inlib::sout(column->type()) << ")."\
        << std::endl;\
    return 0;\
  }\
  MT* col = (MT*)column;\
  T value;\
  if(!col->value(fRow,value)) {\
    out() << "BatchLab::Zebra::Tuple::get(" << ST << ") :"\
        << " can't get value."\
        << std::endl;\
    return 0;\
  }\
  return value;\
}

GWRITE(float,"float",getFloat,ZebraColumn_float)
GWRITE(double,"double",getDouble,ZebraColumn_double)
GWRITE(char,"char",getChar,ZebraColumn_char)
GWRITE(byte,"byte",getByte,ZebraColumn_byte)
GWRITE(short,"short",getShort,ZebraColumn_short)
GWRITE(int,"int",getInt,ZebraColumn_int)
GWRITE(bool,"boolean",getBoolean,ZebraColumn_bool)
//GWRITE(long,"long",getLong,ZebraColumn_long)
GWRITE(std::string,"string",getString,ZebraColumn_string)

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
//////////////////////////////////////////////////////////////////////////////
long BatchLab::Zebra::Tuple::getLong(
 int /*aColumn*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::getLong : dummy." << std::endl;
  return 0L; //FIXME
}
#else
//////////////////////////////////////////////////////////////////////////////
AIDA::int64 BatchLab::Zebra::Tuple::getLong(
 int /*aColumn*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::getLong : dummy." << std::endl;
  return 0; //FIXME
}
#endif
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITuple* BatchLab::Zebra::Tuple::getTuple(
 int
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::getTuple : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* BatchLab::Zebra::Tuple::getTuple(
 int
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::getTuple : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITupleEntry* BatchLab::Zebra::Tuple::getObject(
 int
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::getObject : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::ITuple* BatchLab::Zebra::Tuple::findTuple(
 int
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::findTuple : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* BatchLab::Zebra::Tuple::findTuple(
 int
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tuple::findTuple : dummy." << std::endl;
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define FWRITE(T,ST,FT,MT) \
bool BatchLab::Zebra::Tuple::FT(int aColumn,T aValue){\
  BaseColumn* column = findColumn1(aColumn);\
  if(!column) {\
    out() << "BatchLab::Zebra::Tuple::fill(" << ST << ") :"\
        << " column " << aColumn << " not found."\
        << std::endl;\
    return false;\
  }\
  if(column->type()!=ST) {\
    out() << "BatchLab::Zebra::Tuple::fill(" << ST << ") :"\
        << " type mismatch (" << inlib::sout(column->type()) << ")."\
        << std::endl;\
    return false;\
  }\
  MT* col = (MT*)column;\
  return col->fill(aValue);\
}

FWRITE(float,"float",fill,ZebraColumn_float)
FWRITE(double,"double",fill,ZebraColumn_double)
FWRITE(char,"char",fill,ZebraColumn_char)
FWRITE(byte,"byte",fill33,ZebraColumn_byte)
FWRITE(short,"short",fill,ZebraColumn_short)
FWRITE(int,"int",fill,ZebraColumn_int)
FWRITE(bool,"boolean",fill,ZebraColumn_bool)
//FWRITE(long,"long",ZebraColumn_long)

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
bool BatchLab::Zebra::Tuple::fill(int /*aColumn*/,long /*aValue*/){ 
  out() << "BatchLab::Zebra::Tuple::fill(long) : dummy." << std::endl;
  return false; //FIXME
}
#else
bool BatchLab::Zebra::Tuple::fill(int /*aColumn*/,AIDA::int64 /*aValue*/){ 
  out() << "BatchLab::Zebra::Tuple::fill(AIDA::int64) : dummy." << std::endl;
  return false; //FIXME
}
bool BatchLab::Zebra::Tuple::fill(int /*aColumn*/,unsigned char /*aValue*/){ 
  out() << "BatchLab::Zebra::Tuple::fill(unsigned char) : dummy." << std::endl;
  return false; //FIXME
}
#endif

bool BatchLab::Zebra::Tuple::fill(int aColumn,const std::string& aValue){ 
  BaseColumn* column = findColumn1(aColumn);
  if(!column) {
    out() << "BatchLab::Zebra::Tuple::fill(string) :"
        << " column " << aColumn << " not found."
        << std::endl;
    return false;
  }
  if(column->type()!="string") {
    out() << "BatchLab::Zebra::Tuple::fill(string) :"
        << " type mismatch (" << inlib::sout(column->type()) << ")."
        << std::endl;
    return false;
  }
  ZebraColumn_string* col = (ZebraColumn_string*)column;  
  return col->fill(aValue);
}
bool BatchLab::Zebra::Tuple::fill(int /*aColumn*/,const AIDA::ITupleEntry& /*aValue*/){ 
  out() << "BatchLab::Zebra::Tuple::fill(ITupleEntry) : dummy." << std::endl;
  return false; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::fill(
 const std::vector<double>& aRow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  int n = aRow.size();
  if(n!=columns()) {
    out() << "BatchLab::Zebra::Tuple::fill :"
        << " number of column mismatch (expected = " 
        << columns() << ", given = " << n << ")" << std::endl;
    return false;
  }
  for(int count=0;count<n;count++) {
    if(!fill(count,aRow[count])) return false;
  }
  return addRow();
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::fill(
 const std::vector<float>& aRow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  int n = aRow.size();
  if(n!=columns()) {
    out() << "BatchLab::Zebra::Tuple::fill :"
        << " number of column mismatch (expected = "
        << columns() << ", given = " << n << ")" << std::endl;
    return false;
  }
  for(int count=0;count<n;count++) {
    if(!fill(count,aRow[count])) return false;
  }
  return addRow();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define COLMINMAX(T,MT,OP) \
    MT* col = (MT*)column;\
    T value;\
    col->value(0,value);\
    T v = value;\
    for(int row=1;row<rown;row++) {\
      if(!col->value(row,value)) {\
        out() << "BatchLab::Zebra::Tuple::column[Min,Max] :"\
            << " can't get value."\
            << std::endl;\
        return (double)0;\
      }\
      v = OP(v,value);\
    }\
    return (double)v


//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Tuple::columnMin(
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
    COLMINMAX(float,ZebraColumn_float,MINIMUM);
  } else if(column->type()=="double") {
    COLMINMAX(double,ZebraColumn_double,MINIMUM);
  } else if(column->type()=="char") {
    COLMINMAX(char,ZebraColumn_char,MINIMUM);
  } else if(column->type()=="short") {
    COLMINMAX(short,ZebraColumn_short,MINIMUM);
  } else if(column->type()=="int") {
    COLMINMAX(int,ZebraColumn_int,MINIMUM);
  } else if(column->type()=="boolean") {
    COLMINMAX(bool,ZebraColumn_bool,MINIMUM);
  } else {
    out() << "BatchLab::Zebra::Tuple::columnMin :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Tuple::columnMax(
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
    COLMINMAX(float,ZebraColumn_float,MAXIMUM);
  } else if(column->type()=="double") {
    COLMINMAX(double,ZebraColumn_double,MAXIMUM);
  } else if(column->type()=="char") {
    COLMINMAX(char,ZebraColumn_char,MAXIMUM);
  } else if(column->type()=="short") {
    COLMINMAX(short,ZebraColumn_short,MAXIMUM);
  } else if(column->type()=="int") {
    COLMINMAX(int,ZebraColumn_int,MAXIMUM);
  } else if(column->type()=="boolean") {
    COLMINMAX(bool,ZebraColumn_bool,MAXIMUM);
  } else {
    out() << "BatchLab::Zebra::Tuple::columnMin :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Tuple::columnMean(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
#define COLMEAN(T,MT) \
    MT* col = (MT*)column;\
    double sv = 0;\
    T value = 0;\
    for(int row=0;row<rown;row++) {\
      if(!col->value(row,value)){\
        out() << "BatchLab::Zebra::Tuple::columnMean :"\
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
    COLMEAN(float,ZebraColumn_float);
  } else if(column->type()=="double") {
    COLMEAN(double,ZebraColumn_double);
  } else if(column->type()=="char") {
    COLMEAN(char,ZebraColumn_char);
  } else if(column->type()=="short") {
    COLMEAN(short,ZebraColumn_short);
  } else if(column->type()=="int") {
    COLMEAN(int,ZebraColumn_int);
  } else if(column->type()=="boolean") {
    COLMEAN(bool,ZebraColumn_bool);
  } else {
    out() << "BatchLab::Zebra::Tuple::columnMean :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Tuple::columnRms(
 int aColumn
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
#define COLRMS(T,MT) \
    MT* col = (MT*)column;\
    double sv = 0;\
    double sv2 = 0;\
    T value = 0;\
    for(int row=0;row<rown;row++) {\
      if(!col->value(row,value)){\
        out() << "BatchLab::Zebra::Tuple::columnRms :"\
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
    COLRMS(float,ZebraColumn_float);
  } else if(column->type()=="double") {
    COLRMS(double,ZebraColumn_double);
  } else if(column->type()=="char") {
    COLRMS(char,ZebraColumn_char);
  } else if(column->type()=="short") {
    COLRMS(short,ZebraColumn_short);
  } else if(column->type()=="int") {
    COLRMS(int,ZebraColumn_int);
  } else if(column->type()=="boolean") {
    COLRMS(bool,ZebraColumn_bool);
  } else {
    out() << "BatchLab::Zebra::Tuple::columnMean :"
        << " make no sense on column type " << column->type() << "."
        << std::endl;
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tuple::visit(
 const Slash::Store::IConstVisitor& //aVisitor
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
}
