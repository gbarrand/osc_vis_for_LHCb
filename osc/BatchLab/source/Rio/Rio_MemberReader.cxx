// this :
#include <BatchLab/Rio/MemberReader.h>

#include <Rio/Interfaces/IBuffer.h>

#include <Slash/Store/IArray.h>
#include <Slash/Store/IStorable.h>

#define MAXIMUM(a,b) ((a)>(b)?(a):(b)) 

#include <cstdio> //printf

BatchLab::Rio::MemberReader::MemberReader(
 Slash::Core::ISession& aSession
,::Rio::IBuffer& aBuffer
)
:fSession(aSession)
,fBuffer(aBuffer)
{}

BatchLab::Rio::MemberReader::~MemberReader(){}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Slash::Core::ISession& BatchLab::Rio::MemberReader::session(){return fSession;}

bool BatchLab::Rio::MemberReader::begin(Slash::Store::IStorable&){return true;}

bool BatchLab::Rio::MemberReader::end(Slash::Store::IStorable&){return true;}

bool BatchLab::Rio::MemberReader::visit(bool& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(char& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(short& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(int& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(Slash::int64& /*aData*/){
  ::printf("debug : BatchLab::Rio::MemberReader::visit(int64) : dummy\n");
  return false; //FIXME
}

bool BatchLab::Rio::MemberReader::visit(Slash::uint64& /*aData*/){
  ::printf("debug : BatchLab::Rio::MemberReader::visit(uint64) : dummy\n");
  return false; //FIXME
}

bool BatchLab::Rio::MemberReader::visit(unsigned int& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(float& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(double& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<bool>& aData){
  std::vector<unsigned char> data;
  if(!::Rio::readArray<unsigned char>(fBuffer,data)) {
    aData.clear();
    return false;
  }
  unsigned int number = data.size();
  aData.resize(number);
  for(unsigned int index=0;index<number;index++) {
    aData[index] = (data[index]==1?true:false);  
  }
  return true;
}

bool BatchLab::Rio::MemberReader::visit(std::vector<char>& aData){
  return ::Rio::readArray<char>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<short>& aData){
  return ::Rio::readArray<short>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<int>& aData){
  return ::Rio::readArray<int>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<Slash::int64>& /*aData*/){
  //FIXME return ::Rio::readArray<Slash::int64>(fBuffer,aData);
  ::printf("debug : BatchLab::Rio::MemberReader::visit(vector<int64>) : dummy\n");
  return false;
}

bool BatchLab::Rio::MemberReader::visit(std::vector<float>& aData){
  return ::Rio::readArray<float>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<double>& aData){
  return ::Rio::readArray<double>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<unsigned char>& aData){
  return ::Rio::readArray<unsigned char>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector< std::vector<double> >& aData){
  return ::Rio::readArray2<double>(fBuffer,aData);
}

bool BatchLab::Rio::MemberReader::visit(std::vector<std::string>& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit(std::string& aData){
  return fBuffer.read(aData);
}

bool BatchLab::Rio::MemberReader::visit_double(Slash::Store::IArray& aData){ 
  Slash::Store::IArray::Orders orders;
  if(!::Rio::readArray<unsigned int>(fBuffer,orders)) return false;

  char*& pos = fBuffer.currentPosition();
  char* end = fBuffer.maximumPosition();

  if(!aData.read(orders,pos,end)) return false;
  return true;  
}
