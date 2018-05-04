// this :
#include <BatchLab/Rio/MemberWriter.h>

#include <Rio/Interfaces/IBuffer.h>

#include <Slash/Store/IArray.h>
#include <Slash/Store/IStorable.h>

#define MAXIMUM(a,b) ((a)>(b)?(a):(b)) 

#include <cstdio> //printf

BatchLab::Rio::MemberWriter::MemberWriter(::Rio::IBuffer& aBuffer)
:fBuffer(aBuffer)
{}

BatchLab::Rio::MemberWriter::~MemberWriter(){}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool BatchLab::Rio::MemberWriter::begin(
 const Slash::Store::IStorable&
,const std::string&
,Slash::Store::IConstVisitor::Local
){
  return true;
}

bool BatchLab::Rio::MemberWriter::end(const Slash::Store::IStorable&){
  return true;
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,bool aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,char aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,short aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,int aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,Slash::int64){
  ::printf("debug : BatchLab::Rio::MemberWriter::visit(int64) : dummy\n");
  return false; //FIXME
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,Slash::uint64){
  ::printf("debug : BatchLab::Rio::MemberWriter::visit(uint64) : dummy\n");
  return false; //FIXME
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,unsigned int aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,float aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,double aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<bool>& aData
){
  unsigned int number = aData.size();
  std::vector<unsigned char> data(number);
  for(unsigned int index=0;index<number;index++) {
    data[index] = (unsigned char)aData[index];  
  }
  return ::Rio::writeArray<unsigned char>(fBuffer,data);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<char>& aData
){
  return ::Rio::writeArray<char>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<short>& aData
){
  return ::Rio::writeArray<short>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<int>& aData
){
  return ::Rio::writeArray<int>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<Slash::int64>& //aData
){
  //FIXME return ::Rio::writeArray<Slash::int64>(fBuffer,aData);
  ::printf("debug : BatchLab::Rio::MemberWriter::visit(vector<int64>) : dummy\n");
  return false;
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<float>& aData
){
  return ::Rio::writeArray<float>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<double>& aData
){
  return ::Rio::writeArray<double>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<unsigned char>& aData
){
  return ::Rio::writeArray<unsigned char>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector< std::vector<double> >& aData
){
  return ::Rio::writeArray2<double>(fBuffer,aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::vector<std::string>& aData
){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const std::string& aData
){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit(const std::string&,const char* aData){
  return fBuffer.write(aData);
}

bool BatchLab::Rio::MemberWriter::visit_double(
 const std::string&
,const Slash::Store::IArray& aData
){ 
  Slash::Store::IArray::Orders orders = aData.orders();
  if(!::Rio::writeArray<unsigned int>(fBuffer,orders)) return false;

  unsigned int array_sz = 1;
 {unsigned int dimn = orders.size();
  for(unsigned int dimi=0;dimi<dimn;dimi++) array_sz *= orders[dimi];
  array_sz *= sizeof(double);}

  char*& pos = fBuffer.currentPosition();
  char* end = fBuffer.maximumPosition();

 {int bufSize = fBuffer.bufferSize();
  if((pos+array_sz)>end) {
    if(!fBuffer.expand(MAXIMUM(2*bufSize,bufSize+int(array_sz)))) 
      return false;
    end = fBuffer.maximumPosition();
  }}

  if(!aData.write(pos,end)) return false;
  return true;  
}

bool BatchLab::Rio::MemberWriter::visit(
 const std::string&
,const Slash::Store::IStorable& aData
){ 
  // WARNING : can't be called in a object.visit()
  //   over a aData being a "super" of "object" .
  if(!aData.visit(*this)) return false;
  return true;  
}
