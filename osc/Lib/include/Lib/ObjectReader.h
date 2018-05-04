#ifndef Lib_ObjectReader_h
#define Lib_ObjectReader_h

// Inheritance :
#include <Slash/Store/IVisitor.h>

#include <Slash/Store/IArray.h>

#include <cstring> //memcpy

namespace Lib {

class ObjectReader : public virtual Slash::Store::IVisitor {
public: //Slash::Store::IVisitor
  virtual bool begin(Slash::Store::IStorable&){return true;}
  virtual bool end(Slash::Store::IStorable&){return true;}

  virtual bool visit(bool& aData){
    if((fPos+LIB_SIZE_CHAR)>fEnd) return false;
    unsigned char data;
    ::memcpy(&data,fPos,LIB_SIZE_CHAR);
    fPos += LIB_SIZE_CHAR;
    aData = (data==1?true:false);
    return true;
  }
  virtual bool visit(char& aData){
    if((fPos+LIB_SIZE_CHAR)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_CHAR);
    fPos += LIB_SIZE_CHAR;
    return true;
  }
  virtual bool visit(short& aData){
    if((fPos+LIB_SIZE_SHORT)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_SHORT);
    fPos += LIB_SIZE_SHORT;
    return true;
  }
  virtual bool visit(int& aData){
    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(Slash::int64& aData){
    if((fPos+LIB_SIZE_INT64)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_INT64);
    fPos += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(Slash::uint64& aData){
    if((fPos+LIB_SIZE_INT64)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_INT64);
    fPos += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(unsigned int& aData){
    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(float& aData){
    if((fPos+LIB_SIZE_FLOAT)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_FLOAT);
    fPos += LIB_SIZE_FLOAT;
    return true;
  }
  virtual bool visit(double& aData){
    if((fPos+LIB_SIZE_DOUBLE)>fEnd) return false;
    ::memcpy(&aData,fPos,LIB_SIZE_DOUBLE);
    fPos += LIB_SIZE_DOUBLE;
    return true;
  }
  virtual bool visit(std::vector<bool>& aData){ 
    std::vector<unsigned char> data;
    if(!read<unsigned char>(fPos,fEnd,data,LIB_SIZE_CHAR)) {
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
  virtual bool visit(std::vector<char>& aData){ 
    return read<char>(fPos,fEnd,aData,LIB_SIZE_CHAR);
  }
  virtual bool visit(std::vector<short>& aData){ 
    return read<short>(fPos,fEnd,aData,LIB_SIZE_SHORT);
  }
  virtual bool visit(std::vector<Slash::int64>& aData){ 
    return read<Slash::int64>(fPos,fEnd,aData,LIB_SIZE_INT64);
  }
  virtual bool visit(std::vector<int>& aData){ 
    return read<int>(fPos,fEnd,aData,LIB_SIZE_INT);
  }
  virtual bool visit(std::vector<float>& aData){ 
    return read<float>(fPos,fEnd,aData,LIB_SIZE_FLOAT);
  }
  virtual bool visit(std::vector<double>& aData){ 
    return read<double>(fPos,fEnd,aData,LIB_SIZE_DOUBLE);
  }
  virtual bool visit(std::vector<unsigned char>& aData){ 
    return read<unsigned char>(fPos,fEnd,aData,LIB_SIZE_CHAR);
  }
  virtual bool visit(std::vector< std::vector<double> >& aData){ 
    if((fPos+2*LIB_SIZE_INT)>fEnd) return false;
    unsigned int n;
    ::memcpy(&n,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;

    unsigned int dim;
    ::memcpy(&dim,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    unsigned int number = n*dim;
    unsigned int array_sz = number*LIB_SIZE_DOUBLE;
  
    if((fPos+array_sz)>fEnd) return false;
  
    aData.resize(n);
    if(number) {
      unsigned int array_sz = dim*LIB_SIZE_DOUBLE;
      for(unsigned int index=0;index<n;index++) {
        std::vector<double>& v = aData[index];
        v.resize(dim);
        ::memcpy(&(v[0]),fPos,array_sz);
        fPos += array_sz;
      }
    }
  
    return true;
  }

  virtual bool visit(std::vector<std::string>& aData){
    // Exa : if ab0cde00, then aData should contain two strings ab and cde.
    aData.clear();

    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    unsigned int sz;
    ::memcpy(&sz,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    if((fPos+sz)>fEnd) return false;
  
    char* begin = fPos;
    unsigned int number = sz-1;
    for(unsigned int index=0;index<number;index++) {
      if((*fPos)=='\0') {
        unsigned int l = fPos - begin;
        std::string s(l,0);
        char* data = (char*)s.c_str();
        ::memcpy(data,begin,l);
        //printf("debug : xx \"%s\"\n",s.c_str());
        aData.push_back(s);
        begin = fPos+1;
      }
      fPos++;
    }
    fPos++;
  
    return true;
  }

  virtual bool visit(std::string& aData){
    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    unsigned int number;
    ::memcpy(&number,fPos,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    if((fPos+number+1)>fEnd) return false;
  
    aData.resize(number);
    ::memcpy((char*)aData.c_str(),fPos,number);
    fPos += (number+1)*LIB_SIZE_CHAR;
  
    return true;
  }

  virtual bool visit_double(Slash::Store::IArray& aData){ 
    Slash::Store::IArray::Orders orders;
    if(!read<unsigned int>(fPos,fEnd,orders,LIB_SIZE_INT)) return false;
    if(!aData.read(orders,fPos,fEnd)) return false;
    return true;  
  }

  virtual Slash::Core::ISession& session(){return fSession;}
public:
  ObjectReader(Slash::Core::ISession& aSession,char*& aPos,char* aEnd)
  :fSession(aSession),fPos(aPos),fEnd(aEnd){}
  virtual ~ObjectReader(){}
private:
  static const int LIB_SIZE_CHAR = 1;
  static const int LIB_SIZE_SHORT = 2;
  static const int LIB_SIZE_INT = 4;
  static const int LIB_SIZE_INT64 = 8;
  static const int LIB_SIZE_FLOAT = 4;
  static const int LIB_SIZE_DOUBLE = 8;
private:
  template <class T>
  static bool read(char*& aPos,char* aEnd,
                   std::vector<T>& aData,unsigned int aSize){ 
    if((aPos+LIB_SIZE_INT)>aEnd) return false;
    unsigned int number;
    ::memcpy(&number,aPos,LIB_SIZE_INT);
    aPos += LIB_SIZE_INT;
  
    unsigned int array_sz = number*aSize;
    if((aPos+array_sz)>aEnd) return false;
  
    aData.resize(number);
    if(number) {
      T* data = &(aData[0]);
      ::memcpy(data,aPos,array_sz);
      aPos += array_sz;
    }

    return true;
  }
private:
  Slash::Core::ISession& fSession;
  char*& fPos;
  char* fEnd;
};

}

#endif
