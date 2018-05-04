#ifndef BatchLab_XML_WAXML_h
#define BatchLab_XML_WAXML_h

#include <Slash/Store/IObject.h>
#include <BatchLab/XML/WriteContext.h>

namespace AIDA {
  class ITuple;
  class IDataPointSet;
}
namespace BatchLab {
  class Cloud1D;
  class Cloud2D;
  class Cloud3D;
}

namespace BatchLab {

class WAXML {
public:
  typedef bool (*Writer)(const Slash::Store::IObject&,XML::WriteContext&);
  typedef std::vector< std::pair<std::string,Writer> > Writers;
public:
  inline WAXML(std::ostream& aOut):fOut(aOut){addDefaultWriters();}
  virtual ~WAXML(){}
public:
  bool write(std::ostream& a_writer,const std::string& aPath,const Slash::Store::IObject& aObject){
    std::string s = aObject.storeClassName();
    Writer writer = findWriter(s);
    if(!writer) {
      fOut << "BatchLab::AIDA_XML::write :" 
           << " writer not found for class " << inlib::sout(s)
           << std::endl;
      return false;
    }
  
    BatchLab::XML::WriteContext ctx(fOut,
                                    a_writer,
                                    aPath,aObject.name());
    if(!writer(aObject,ctx)) {
      fOut << "BatchLab::AIDA_XML::write :" 
           << " writing failed for " << inlib::sout(aObject.name())
           << std::endl;
      return false;
    }
    return true;
  }
  
private:
  inline void addWriter(const std::string& a_class,
                        Writer aWriter,bool aCheck = false){
    if(aCheck==true) {if(findWriter(a_class)) return;}
    fWriters.push_back(std::pair<std::string,Writer>(a_class,aWriter));
  }

  inline Writer findWriter(const std::string& a_class){
    unsigned int number = fWriters.size();
    for(unsigned int index=0;index<number;index++) {
      if(a_class==fWriters[index].first) return fWriters[index].second;
    }
    return 0;
  }


  virtual void addDefaultWriters();
private:
  static bool get_booking(std::ostream&,const AIDA::ITuple&,std::string&);

  static void writeTupleRows(std::ostream&,AIDA::ITuple&,
			     const std::string&,std::ostream&);
  static bool write_Histogram1D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Histogram2D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Histogram3D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Profile1D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Profile2D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Tuple(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Cloud1D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Cloud2D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Cloud3D(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_DataPointSet(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
  static bool write_Function(const Slash::Store::IObject&,BatchLab::XML::WriteContext&);
private:
  std::ostream& fOut;
  Writers fWriters;
};

}

#include "WAXML.icc"

#endif
