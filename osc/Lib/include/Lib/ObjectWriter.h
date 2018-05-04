#ifndef Lib_ObjectWriter_h
#define Lib_ObjectWriter_h

// Inheritance :
#include <Slash/Store/IConstVisitor.h>

#include <Slash/Store/IStorable.h>
#include <Slash/Store/IArray.h>

#include <cstring>

namespace Lib {

class ObjectWriter : public virtual Slash::Store::IConstVisitor {
public: //Slash::Store::IConstVisitor
  virtual bool begin(const Slash::Store::IStorable&,const std::string&,
                     Slash::Store::IConstVisitor::Local){return true;}
  virtual bool end(const Slash::Store::IStorable&){return true;}

  virtual bool visit(const std::string&,bool aData){
    if((fPos+LIB_SIZE_CHAR)>fEnd) return false;
    unsigned char data = (aData?1:0);
    ::memcpy(fPos,&data,LIB_SIZE_CHAR);
    fPos += LIB_SIZE_CHAR;
    return true;
  }
  virtual bool visit(const std::string&,char aData){
    if((fPos+LIB_SIZE_CHAR)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_CHAR);
    fPos += LIB_SIZE_CHAR;
    return true;
  }
  virtual bool visit(const std::string&,short aData){
    if((fPos+LIB_SIZE_SHORT)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_SHORT);
    fPos += LIB_SIZE_SHORT;
    return true;
  }
  virtual bool visit(const std::string&,int aData){
    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(const std::string&,Slash::int64 aData){
    if((fPos+LIB_SIZE_INT64)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_INT64);
    fPos += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(const std::string&,Slash::uint64 aData){
    if((fPos+LIB_SIZE_INT64)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_INT64);
    fPos += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(const std::string&,unsigned int aData){
    if((fPos+LIB_SIZE_INT)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(const std::string&,float aData){
    if((fPos+LIB_SIZE_FLOAT)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_FLOAT);
    fPos += LIB_SIZE_FLOAT;
    return true;
  }
  virtual bool visit(const std::string&,double aData){
    if((fPos+LIB_SIZE_DOUBLE)>fEnd) return false;
    ::memcpy(fPos,&aData,LIB_SIZE_DOUBLE);
    fPos += LIB_SIZE_DOUBLE;
    return true;
  }
  virtual bool visit(const std::string&,const std::vector<bool>& aData){
    unsigned int number = aData.size();
    std::vector<unsigned char> data(number);
    for(unsigned int index=0;index<number;index++) {
      data[index] = (aData[index]?1:0);
    }
    return write<unsigned char>(fPos,fEnd,data,LIB_SIZE_CHAR);
  }
  virtual bool visit(const std::string&,const std::vector<char>& aData){
    return write<char>(fPos,fEnd,aData,LIB_SIZE_CHAR);
  }
  virtual bool visit(const std::string&,const std::vector<short>& aData){
    return write<short>(fPos,fEnd,aData,LIB_SIZE_SHORT);
  }
  virtual bool visit(const std::string&,const std::vector<int>& aData){
    return write<int>(fPos,fEnd,aData,LIB_SIZE_INT);
  }
  virtual bool visit(const std::string&,
                     const std::vector<Slash::int64>& aData){
    return write<Slash::int64>(fPos,fEnd,aData,LIB_SIZE_INT64);
  }
  virtual bool visit(const std::string&,const std::vector<float>& aData){
    return write<float>(fPos,fEnd,aData,LIB_SIZE_FLOAT);
  }
  virtual bool visit(const std::string&,const std::vector<double>& aData){
    return write<double>(fPos,fEnd,aData,LIB_SIZE_DOUBLE);
  }
  virtual bool visit(const std::string&,
                     const std::vector<unsigned char>& aData){
    return write<unsigned char>(fPos,fEnd,aData,LIB_SIZE_CHAR);
  }
  virtual bool visit(const std::string&,
                     const std::vector< std::vector<double> >& aData){
    unsigned int n = aData.size();
    unsigned int dim = n?aData[0].size():0;
    unsigned int number = n*dim;
    unsigned int array_sz = number*LIB_SIZE_DOUBLE;

    if((fPos+2*LIB_SIZE_INT+array_sz)>fEnd) return false;
  
    ::memcpy(fPos,&n,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    ::memcpy(fPos,&dim,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    if(number) {
      unsigned int array_sz = dim*LIB_SIZE_DOUBLE;
      for(unsigned int index=0;index<n;index++) {
        const std::vector<double>& v = aData[index];
        ::memcpy(fPos,&(v[0]),array_sz);
        fPos += array_sz;
      }
    }
  
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<std::string>& aData){
    unsigned int number = aData.size();
  
    unsigned int sz = 0;
    for(unsigned int index=0;index<number;index++) sz += aData[index].size()+1;
    sz++;
  
    if((fPos+LIB_SIZE_INT+sz)>fEnd) return false;
  
    ::memcpy(fPos,&sz,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    for(unsigned int index=0;index<number;index++) {
      const std::string& s = aData[index];
      unsigned int array_size = (s.size()+1)*LIB_SIZE_CHAR;
      const char* data = s.c_str();
      ::memcpy(fPos,data,array_size);
      fPos += array_size;
    }
    *fPos = '\0';
    fPos++;
  
    return true;
  }

  virtual bool visit(const std::string&,const std::string& aData){
    unsigned int number = aData.size();
    unsigned int array_sz = (number+1)*LIB_SIZE_CHAR;
  
    if((fPos+LIB_SIZE_INT+array_sz)>fEnd) return false;
  
    ::memcpy(fPos,&number,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    ::memcpy(fPos,aData.c_str(),array_sz);
    fPos += array_sz;
  
    return true;
  }

  virtual bool visit_double(const std::string&,
                            const Slash::Store::IArray& aData){ 
    if(!write<unsigned int>(fPos,fEnd,aData.orders(),LIB_SIZE_INT))
      return false;
    if(!aData.write(fPos,fEnd)) return false;
    return true;  
  }

  virtual bool visit(const std::string&,const Slash::Store::IStorable& aData){ 
    // WARNING : can't be called in a object.visit()
    //   over a aData being a "super" of "object" .
    if(!aData.visit(*this)) return false;
    return true;  
  }

  virtual bool visit(const std::string&,const char* aData){
    unsigned int number = ::strlen(aData);
    unsigned int array_sz = (number+1)*LIB_SIZE_CHAR;
  
    if((fPos+LIB_SIZE_INT+array_sz)>fEnd) return false;
  
    ::memcpy(fPos,&number,LIB_SIZE_INT);
    fPos += LIB_SIZE_INT;
  
    ::memcpy(fPos,aData,array_sz);
    fPos += array_sz;
  
    return true;
  }
public:
  ObjectWriter(char*& aPos,char* aEnd):fPos(aPos),fEnd(aEnd){}
  virtual ~ObjectWriter(){}
private:
  static const int LIB_SIZE_CHAR = 1;
  static const int LIB_SIZE_SHORT = 2;
  static const int LIB_SIZE_INT = 4;
  static const int LIB_SIZE_INT64 = 8;
  static const int LIB_SIZE_FLOAT = 4;
  static const int LIB_SIZE_DOUBLE = 8;
private:
  template <class T>
  static bool write(char*& aPos,char* aEnd,
                    const std::vector<T>& aData,unsigned int aSize){
    unsigned int number = aData.size();
    unsigned int array_sz = number*aSize;
    if((aPos+LIB_SIZE_INT+array_sz)>aEnd) return false;
    ::memcpy(aPos,&number,LIB_SIZE_INT);
    aPos += LIB_SIZE_INT;
    if(number) {
      const T* data = &(aData[0]);
      ::memcpy(aPos,data,array_sz);
      aPos += array_sz;
    }
    return true;
  }

private:
  char*& fPos;
  char* fEnd;
};

}

#endif
