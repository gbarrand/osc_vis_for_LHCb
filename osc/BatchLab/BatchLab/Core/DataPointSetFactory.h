#ifndef BatchLab_DataPointSetFactory_h
#define BatchLab_DataPointSetFactory_h

// Inheritance :
#include <BatchLab/Core/BaseFactory.h>
#include <AIDA/IDataPointSetFactory.h>
#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <BatchLab/Core/DataPointSet.h>

namespace Slash {namespace Store {class ITree;}}

namespace BatchLab {

class DataPointSetFactory 
:public BaseFactory
,public virtual AIDA::IDataPointSetFactory 
{
public: //AIDA/AIDA::IDataPointSetFactory
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual AIDA::IDataPointSet* create(const std::string& aPath,
                                      const std::string& aTitle,
                                      int aDimensionOfPoints){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::DataPointSet* object = new BatchLab::DataPointSet(f_out,
                                                      name,aTitle,
                                                      aDimensionOfPoints);
    //NOTE : enforce thread_safe = true for G.Ruprecht.
    std::string opts("thread_safe=true");
    if(!THREAD_SAFETY(*object,opts,"BatchLab::DataPointSetFactory::create")) {
      delete object;
      return 0;
    }
    //if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual AIDA::IDataPointSet* create(const std::string& aPath,
                                      int aDimensionOfPoints){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::DataPointSet* object = new BatchLab::DataPointSet(f_out,
                                                      name,name,
                                                      aDimensionOfPoints);
    //NOTE : enforce thread_safe = true for G.Ruprecht.
    std::string opts("thread_safe=true");
    if(!THREAD_SAFETY(*object,opts,"BatchLab::DataPointSetFactory::create")) {
      delete object;
      return 0;
    }
    //if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
#else
  virtual AIDA::IDataPointSet* create(const std::string& aPath,
                                      const std::string& aTitle,
                                      int aDimensionOfPoints,
                                      const std::string& aOptions = ""){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::DataPointSet* object = new BatchLab::DataPointSet(f_out,
                                                      name,aTitle,
                                                      aDimensionOfPoints);
    if(!THREAD_SAFETY(*object,aOptions,
                      "BatchLab::DataPointSetFactory::create")) {
      delete object;
      return 0;
    }
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual AIDA::IDataPointSet* create(const std::string& aPath,
                                      int aDimensionOfPoints,
                                      const std::string& aOptions = ""){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::DataPointSet* object = new BatchLab::DataPointSet(f_out,
                                                      name,name,
                                                      aDimensionOfPoints);
    if(!THREAD_SAFETY(*object,aOptions,
                      "BatchLab::DataPointSetFactory::create")) {
      delete object;
      return 0;
    }
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
#endif

  //FIXME
  virtual AIDA::IDataPointSet* createCopy(const std::string&,
                                        const AIDA::IDataPointSet&){return 0;}
  virtual bool destroy(AIDA::IDataPointSet*){return false;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::IHistogram1D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::IHistogram2D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::IHistogram3D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::ICloud1D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::ICloud2D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::ICloud3D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::IProfile1D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* create(const std::string&,
                                      const AIDA::IProfile2D&,
                                      const std::string&){return 0;}
  virtual AIDA::IDataPointSet* add(const std::string&,
                                   const AIDA::IDataPointSet&,
                                   const AIDA::IDataPointSet&){return 0;}
  virtual AIDA::IDataPointSet* subtract(const std::string&,
                                        const AIDA::IDataPointSet&,
                                        const AIDA::IDataPointSet&){return 0;}
  virtual AIDA::IDataPointSet* multiply(const std::string&,
                                        const AIDA::IDataPointSet&,
                                        const AIDA::IDataPointSet&){return 0;}
  virtual AIDA::IDataPointSet* divide(const std::string&,
                                      const AIDA::IDataPointSet&,
                                      const AIDA::IDataPointSet&){return 0;}
  virtual AIDA::IDataPointSet* weightedMean(const std::string&,
                                      const AIDA::IDataPointSet&,
				      const AIDA::IDataPointSet&){return 0;}

  virtual AIDA::IDataPointSet* createX(const std::string&,const std::string&, 
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createX(const std::string&,const std::string&, 
                                       const std::vector<double>&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createX(const std::string&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createX(const std::string&,
                                       const std::vector<double>&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}

  virtual AIDA::IDataPointSet* createY(const std::string&,const std::string&, 
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createY(const std::string&,const std::string&, 
                                       const std::vector<double>&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createY(const std::string&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createY(const std::string&,
                                       const std::vector<double>&,
                                       const std::vector<double>&,
                                       const std::vector<double>&){return 0;}

  virtual AIDA::IDataPointSet* createXY(const std::string&,const std::string&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXY(const std::string&,const std::string&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXY(const std::string&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXY(const std::string&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&){return 0;}

  virtual AIDA::IDataPointSet* createXYZ(const std::string&,const std::string&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
				        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXYZ(const std::string&,const std::string&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&,
				        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXYZ(const std::string&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
				        const std::vector<double>&){return 0;}
  virtual AIDA::IDataPointSet* createXYZ(const std::string&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&, 
                                        const std::vector<double>&,
                                        const std::vector<double>&,
                                        const std::vector<double>&,
				        const std::vector<double>&){return 0;}
public:
  DataPointSetFactory(std::ostream& a_out,Slash::Store::ITree& aTree)
  :BaseFactory(a_out,aTree){
    Lib::Debug::increment("BatchLab::DataPointSetFactory");
  }
  virtual ~DataPointSetFactory(){
    Lib::Debug::decrement("BatchLab::DataPointSetFactory");
  }
private:
  inline bool THREAD_SAFETY(DataPointSet& a_object,
                            const std::string& a__opts,
                            const std::string& a__CMT) {
    inlib::args args(a__opts,";",true);	
    bool thread_safe;
    if(args.find("thread_safe",thread_safe)) {
      BaseObject* bobject = INLIB_CAST(a_object,BatchLab::BaseObject);
      if(!bobject) {
        f_out << a__CMT << " :"
            << " Slash::Store::IObject not a BatchLab::BaseObject."
            << std::endl;
        return false;
      }
      if(!bobject->setThreadSafe(thread_safe)){
        f_out << a__CMT << " :"
            << " unable to set thread safety."
            << std::endl;
        return false;
      }
    }
    return true;
  }

};

}

#endif
