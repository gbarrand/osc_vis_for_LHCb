#ifndef BatchLab_XML_AIDA_ML_h
#define BatchLab_XML_AIDA_ML_h

// Inheritance :
#include <exlib/raxml>

#include <BatchLab/Core/BaseTree.h>

//#ifdef HAS_ZLIB
#include <exlib/gzip>
//#endif

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

class AIDA_ML : public exlib::raxml {
public:
  AIDA_ML(inlib::xml::factory& aFactory,BaseTree& aTree)
  : exlib::raxml(aFactory,aTree.out(),aTree.verboseLevel()?true:false)
  ,fTree(aTree) {
//#ifdef HAS_ZLIB
    set_compressed_reader(new exlib::gzip_reader());
//#endif

    clear_readers();
    addDefaultReaders();
  
    std::vector<std::string> tags;
    tags.push_back("aida");
    tags.push_back("histogram1d");
    tags.push_back("histogram2d");
    tags.push_back("histogram3d");
    tags.push_back("profile1d");
    tags.push_back("profile2d");
    tags.push_back("axis");
    tags.push_back("statistics");
    tags.push_back("data1d");
    tags.push_back("data2d");
    tags.push_back("data3d");
    tags.push_back("tuple");
    tags.push_back("columns");
    tags.push_back("rows");
    tags.push_back("row");
    tags.push_back("entryITuple"); //aida.dtd spec.
    tags.push_back("entryTuple");  //not in aida.dtd ! Back comp.
    tags.push_back("cloud1d");
    tags.push_back("cloud2d");
    tags.push_back("cloud3d");
    tags.push_back("entries1d");
    tags.push_back("entries2d");
    tags.push_back("entries3d");
    tags.push_back("dataPointSet");
    tags.push_back("dataPoint");
    tags.push_back("function");
    tags.push_back("arguments");
    tags.push_back("argument");
    tags.push_back("parameters");
    tags.push_back("annotation");
    set_tags(tags);
  }

  virtual ~AIDA_ML(){}
public:
  /*
  virtual bool visit_end_element(inlib::xml::tree& aItemML,bool& aKeep) {
    m_objects.clear();
    m_read_tag = &(fTree.session());
    if(!exlib::raxml::visit_end_element(aItemML,aKeep)) return false;
    std::vector<exlib::raxml_out>::const_iterator it;
    for(it=m_objects.begin();it!=m_objects.end();++it) {
      Slash::Store::IObject* object = (Slash::Store::IObject*)(*it).object();
      const std::string& spath = (*it).path();
      if(object) {
        if(!fTree.mkdirs(spath) || !fTree.manageObject(object,spath)) {
          delete object;
        }
      }
    }  
    m_objects.clear();
    return true;
  }
  */
private:
  void addDefaultReaders();
private:
  inline static const std::string& s_name_counter() {
    static const std::string s_v("AIDAL_XML_name_counter");
    return s_v;
  }

  static std::string buildName(inlib::xml::tree&,Slash::Core::ISession&);
  static exlib::raxml_out read_histo(inlib::xml::tree&,
                     std::ostream&,bool,int,bool,void*);
  static exlib::raxml_out read_histo1d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static exlib::raxml_out read_histo2d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static exlib::raxml_out read_histo3d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static bool readTupleColumns(inlib::xml::tree&,bool&,std::string&,std::ostream&);
  static bool readTupleRows(inlib::xml::tree&,AIDA::ITuple&,
			    bool&,std::ostream&);
  static bool readCloudData(inlib::xml::tree&,BatchLab::Cloud1D&,
			    bool,std::ostream&);
  static bool readCloudData(inlib::xml::tree&,BatchLab::Cloud2D&,bool,std::ostream&);
  static bool readCloudData(inlib::xml::tree&,BatchLab::Cloud3D&,bool,std::ostream&);
  static bool readDataPointSetData(inlib::xml::tree&,AIDA::IDataPointSet&,
				   std::ostream&);
  class FunctionBooking {
  public:
    std::string fArgument;
    std::vector< std::pair<double,double> > fRanges;
    std::vector< std::pair<std::string,double> > fParameters;
  };

  static bool readFunctionBooking(inlib::xml::tree&,FunctionBooking&,std::ostream&);
  static exlib::raxml_out read_profil1d(inlib::xml::tree&,
					std::ostream&,bool,void*);
  static exlib::raxml_out read_profil2d(inlib::xml::tree&,
					std::ostream&,bool,void*);
  static exlib::raxml_out read_tuple(inlib::xml::tree&,std::ostream&,bool,void*);
  static exlib::raxml_out read_cloud(inlib::xml::tree&,
				     std::ostream&,bool,int,void*);
  static exlib::raxml_out read_cloud1d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static exlib::raxml_out read_cloud2d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static exlib::raxml_out read_cloud3d(inlib::xml::tree&,
				       std::ostream&,bool,void*);
  static exlib::raxml_out read_dps(inlib::xml::tree&,std::ostream&,bool,void*);
  static exlib::raxml_out read_function(inlib::xml::tree&,std::ostream&,bool,void*);
private:
  BaseTree& fTree;
};

}

#include "AIDA_ML.icc"

#endif
