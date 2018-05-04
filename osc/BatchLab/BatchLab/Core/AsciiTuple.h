#ifndef BatchLab_AsciiTuple_h
#define BatchLab_AsciiTuple_h

// Inheritance :
#include <BatchLab/Core/MemoryTuple.h>

#include <inlib/parse>
#include <inlib/system>

//#ifdef HAS_ZLIB
#include <exlib/gzip>
//#endif

namespace BatchLab {

class AsciiTuple : public MemoryTuple {
public: //AIDA/ITuple
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::AsciiTuple)
    else return BatchLab::MemoryTuple::cast(a_class);
  }
public: //Slash::Store::IObject
  virtual std::string storeClassName() const {return "BatchLab::AsciiTuple";}
public:
  AsciiTuple(std::ostream& a_out,
             const std::string& aName,const std::string& aTitle,
             const std::string& aFile)
  :MemoryTuple(a_out,aName)
  {
    setTitle(aTitle);
    if(aFile=="stdin") {
      if(!read((read_func_t)inlib::file::read_cstring,stdin)) clear();
    } else {
  
      std::string suffix = inlib::suffix(aFile);
      bool compressed = (suffix=="gz"?true:false);
  
      std::string name;
      inlib::file_name(aFile,name);
  
      if(compressed) {
//#ifdef HAS_ZLIB
        gzFile file = ::gzopen(name.c_str(),"rb");
        if(!file) {
          out() << "BatchLab::AsciiTuple::AsciiTuple :" 
              << " can't open file " << name << std::endl;
          return;
        } 
        if(!read((read_func_t)exlib::zread_cstring,file)) {
          out() << "BatchLab::AsciiTuple::AsciiTuple :" 
                << " problem reading " << name << std::endl;
          clear();
        }
        ::gzclose(file);
//#else
//      out() << "BatchLab::AsciiTuple :"
//            << " package not built with zlib." << std::endl;
//#endif
      } else {
        FILE* file = ::fopen(name.c_str(),"rb");
        if(!file) {
          out() << "BatchLab::AsciiTuple::AsciiTuple :" 
              << " can't open file " << name << std::endl;
          return;
        } 
        if(!read((read_func_t)inlib::file::read_cstring,file)) {
          out() << "BatchLab::AsciiTuple::AsciiTuple :" 
              << " problem reading " << name << std::endl;
          clear();
        }
        ::fclose(file);
      }
    }
  }

  virtual ~AsciiTuple(){}
private:
  typedef bool (*read_func_t)(void*,char*,unsigned int,size_t&);

  bool read(read_func_t a_func,void* a_tag) {
    const unsigned int BUFSIZE = 65536;
    char buffer[BUFSIZE+1];

    // Read header :
  
    //bool hippoStyle = false;
    //bool readFirstRow = false;
    //bool readSecondRow = false;

    // First line :
    size_t l;
    if(!a_func(a_tag,buffer,sizeof(buffer),l)) return false; //EOF.
  
    //printf("debug : BatchLab::AsciiTuple::read : line(1) : \"%s\"\n",buffer);
  
    std::vector<double> ds;
    // Guess the style of the file (ascii or hippodraw) :
    inlib::cstring_strtods(buffer,ds);
    unsigned int dn = ds.size();
    if(dn) {
      // First line is numbers. The style is assumed to be "ascii".
      std::string sbooking;
      for(unsigned int index=0;index<dn;index++) {
        std::string s;
        inlib::sprintf(s,32,"v%d",index);
        if(index) sbooking += " ";
        sbooking += s;
      }    
      std::list<Lib::Value> variables;
      if(!booking(out(),sbooking,variables)) {
        out() << "BatchLab::AsciiTuple::read :"
              << " booking failed."
              << " Booking string was " << inlib::sout(sbooking) << "."
              << std::endl;
        return false;
      }
      bool status = initialize(variables);
      Lib::Columns::deleteColumns(variables);
      if(!status) {
        out() << "BatchLab::AsciiTuple::read :"
              << " initialize failed."
              << std::endl;
        return false;
      }
  
    } else {
      // Expect hippodraw style, then a title line followed
      // with a line describing columns and then data.
      setTitle(buffer);
  
      // Read second line :
      if(!a_func(a_tag,buffer,sizeof(buffer),l)) return false; //EOF.
      // Second line is the columns describer :
      std::list<Lib::Value> variables;
      if(!booking(out(),buffer,variables)) {
        out() << "BatchLab::AsciiTuple::read :"
              << " booking failed."
              << " Booking string was " 
              << inlib::sout(std::string(buffer)) << "."
              << std::endl;
        return false;
      }
      bool status = initialize(variables);
      Lib::Columns::deleteColumns(variables);
      if(!status) {
        out() << "BatchLab::AsciiTuple::read :"
              << " initialize failed."
              << std::endl;
        return false;
      }
    }
  
    // At this point, the tuple is booked, then read data ; full speed...
    ds.resize(columns());
  
    while(true) {
      if(!a_func(a_tag,buffer,sizeof(buffer),l)) break; //EOF.
  
      //printf("debug : BatchLab::AsciiTuple::read :line(2): \"%s\"\n",buffer);
  
      inlib::cstring_strtods(buffer,ds,true); //the last true is important.
  
      if(!fill(ds)) {
        out() << "BatchLab::AsciiTuple::read :"
              << " can't fill." << std::endl;
        return false;
      }
  
    }
  
    //printf("debug : BatchLab::AsciiTuple::read : end : %d rows\n",rows());
  
    return true;
  }
};

}

#endif


