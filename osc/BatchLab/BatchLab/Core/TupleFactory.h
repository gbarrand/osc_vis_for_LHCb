#ifndef BatchLab_TupleFactory_h
#define BatchLab_TupleFactory_h

#include <BatchLab/Core/BaseFactory.h>
#include <AIDA/ITupleFactory.h>

#include <Slash/Core/ISession.h>
#include <Slash/Store/IStoreManager.h>
#include <Slash/Tools/Manager.h>

#include <BatchLab/Core/MemoryTuple.h>
#include <BatchLab/Core/ChainedTuple.h>
#include <BatchLab/Core/Filter.h>
#include <BatchLab/Core/Evaluator.h>

#include <inlib/path>

namespace BatchLab {

class TupleFactory 
:public BaseFactory
,public virtual AIDA::ITupleFactory {
public: //AIDA/ITupleFactory
  virtual AIDA::ITuple* create(const std::string& aPath,
                               const std::string& aTitle,
                               const std::vector<std::string>& aColumnNames,
                               const std::vector<std::string>& aColumnTypes,
                               const std::string& aOptions){
    if(!aColumnNames.size()) return 0;
    if(aColumnNames.size()!=aColumnTypes.size()) return 0;
    std::string s; 
    for(unsigned int index=0;index<aColumnNames.size();index++) {
      if(index) s += ",";
      s += aColumnTypes[index] + " " + aColumnNames[index];
    }
    return create(aPath,aTitle,s,aOptions);
  }

  virtual AIDA::ITuple* create(const std::string& aPath,
                               const std::string& aTitle,
                               const std::string& aBooking,
                               const std::string& aOptions){
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;

    Slash::Store::IObject* object = 0;
    if(fTree.storeType()=="BatchLab::Rio::Tree") {
      //FIXME : check if an "aPath" object already exists !
      Slash::Store::IStoreManager* storeManager = 
        Slash_findManager(fSession,"Rio_Manager",Slash::Store::IStoreManager);
      if(!storeManager) return 0;
      std::vector<std::string> args;
      args.push_back(aBooking);
      object = storeManager->createObject
                 (fTree,"AIDA::ITuple",path,name,aTitle,args,aOptions);
  
    } else if(fTree.storeType()=="BatchLab::Zebra::Tree") {
      //FIXME : check if an "aPath" object already exists !
      Slash::Store::IStoreManager* storeManager = 
       Slash_findManager(fSession,"Zebra_Manager",Slash::Store::IStoreManager);
      if(!storeManager) return 0;
      std::vector<std::string> args;
      args.push_back(aBooking);
      object = storeManager->createObject
                 (fTree,"AIDA::ITuple",path,name,aTitle,args,aOptions);
  
    } else if(fTree.storeType()=="BatchLab::AsciiTree") {
      //FIXME : check if an "aPath" object already exists !
      //NOTE : at commit, only one tuple put in file.
      object= 
        new BatchLab::MemoryTuple(f_out,name,aTitle,aBooking);
    } else if( (fTree.storeType()=="BatchLab::MemoryTree") || 
               (fTree.storeType()=="BatchLab::XML::Tree")  ){
      //FIXME : check if an "aPath" object already exists !
      object = 
        new BatchLab::MemoryTuple(f_out,name,aTitle,aBooking);
    } else if( (fTree.storeType()=="BatchLab::HDF5::Tree") || 
               (fTree.storeType()=="BatchLab::Zip::Tree")  ||
               (fTree.storeType()=="BatchLab::FS::Tree")  ){
      //FIXME : check if an "aPath" object already exists !
      //FIXME : optimize : have a store-paginated tuple.
      object = 
        new BatchLab::MemoryTuple(f_out,name,aTitle,aBooking);
    } else {
      f_out << "BatchLab::TupleFactory::create :" 
          << " Can't create tuple " << inlib::sout(aPath) 
          << " in tree with store " << inlib::sout(fTree.storeName())
          << std::endl;
    }
    if(!object) return 0;
    AIDA::ITuple* aobject = INLIB_CAST(*object,AIDA::ITuple);
    if(!aobject) {
      f_out << "BatchLab::TupleFactory::create :" 
          << " not a AIDA::ITuple." << std::endl;
      delete object;
      return 0;
    }
    if(!manage(aOptions)) return aobject;
    if(manageObject(object,path)) return aobject;
    delete object;
    return 0;
  }
  
  virtual AIDA::ITuple* createChained(const std::string& aPath,
                                      const std::string& aTitle,
                                  const std::vector<AIDA::ITuple*>& aTuples){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::ChainedTuple* object = new BatchLab::ChainedTuple(f_out,
                                                      name,aTitle,
                                                      aTuples);
    //if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }

  virtual AIDA::ITuple* createChained(const std::string& aPath,
                                      const std::string& aTitle,
                                      const std::vector<std::string>& aTuples){
    std::vector<AIDA::ITuple*> tuples;
    unsigned tuplen = aTuples.size();
    for(unsigned int index=0;index<tuplen;index++) {
      Slash::Store::IObject* bo = fTree.findObject(aTuples[index]);
      if(!bo) {
        f_out << "BatchLab::TupleFactory::createChained :" 
            << " object " << inlib::sout(aTuples[index]) 
            << " not found in tree with store "
            << inlib::sout(fTree.storeName())
            << std::endl;
        return 0;
      }
      AIDA::ITuple* tuple = INLIB_CAST(*bo,AIDA::ITuple);
      if(!tuple) {
        f_out << "BatchLab::TupleFactory::createChained :" 
            << " object " << inlib::sout(aTuples[index]) 
            << " not a tuple."
            << std::endl;
        return 0;
      }
      tuples.push_back(tuple);
    }
    return createChained(aPath,aTitle,tuples);
  }
  
  virtual AIDA::ITuple* createCopy(const std::string& aPath,
                                   const AIDA::ITuple& aFrom){  //AIDA-v3r3p0
    AIDA::ITuple* tuple = 
      create(aPath,aFrom.title(),aFrom.columnNames(),aFrom.columnTypes(),"");
    if(!tuple) {
      f_out << "BatchLab::TupleFactory::createCopy :" 
          << " Can't create tuple " << inlib::sout(aPath) 
          << " in tree with store " << inlib::sout(fTree.storeName())
          << std::endl;
      return 0;
    }
    if(!amanip::copy(f_out,const_cast<AIDA::ITuple&>(aFrom),*tuple)) {
      f_out << "BatchLab::TupleFactory::createCopy :" 
          << " cannot copy tuple content."
          << std::endl;
      tuple->reset();
    }
    return tuple;
  }

  virtual AIDA::ITuple* createFiltered(const std::string& aPath,
                                       AIDA::ITuple& aFrom,
                                       AIDA::IFilter& aFilter){
    AIDA::ITuple* tuple = 
      create(aPath,aFrom.title(),aFrom.columnNames(),aFrom.columnTypes(),"");
    if(!tuple) {
      f_out << "BatchLab::TupleFactory::createFiltered :" 
          << " Can't create tuple " << inlib::sout(aPath) 
          << " in tree with store " << inlib::sout(fTree.storeName())
          << std::endl;
      return 0;
    }
    if(!amanip::copy(f_out,const_cast<AIDA::ITuple&>(aFrom),*tuple,aFilter)) {
      f_out << "BatchLab::TupleFactory::createFiltered :" 
          << " cannot copy tuple content."
          << std::endl;
      tuple->reset();
    }
    return tuple;
  }

  virtual AIDA::ITuple* createFiltered(const std::string& aPath,
                                       AIDA::ITuple& aFrom,
                                       AIDA::IFilter& aFilter,
                                     const std::vector<std::string>& aColumns){
    static const int NotFound = -1;
    unsigned int coln = aColumns.size();
    std::vector<int> colis;
    std::vector<std::string> types;
    for(unsigned int index=0;index<coln;index++) {
      int coli = aFrom.findColumn(aColumns[index]);
      if(coli==NotFound) {
        f_out << "BatchLab::TupleFactory::createFiltered :" 
            << " " << inlib::sout(aColumns[index]) 
            << " not a column of tuple with title " 
            << inlib::sout(aFrom.title())
            << std::endl;
        return 0;
      }
      colis.push_back(coli);
      types.push_back(aFrom.columnTypes()[coli]);
    }  
    AIDA::ITuple* tuple = create(aPath,aFrom.title(),aColumns,types,"");
    if(!tuple) {
      f_out << "BatchLab::TupleFactory::createFiltered :" 
          << " Can't create tuple " << inlib::sout(aPath) 
          << " in tree with store " << inlib::sout(fTree.storeName())
          << std::endl;
      return 0;
    }
    aFrom.start();
    while(aFrom.next()) {
      if(!aFilter.accept()) continue;
      bool status = true;
      for(int index=0;index<(int)coln;index++) {
        int fromi = colis[index];
        std::string type = aFrom.columnType(fromi);
        if(type=="float") {
          if(!tuple->fill(index,aFrom.getFloat(fromi))) {status = false;break;}
        } else if(type=="double") {
          if(!tuple->fill(index,aFrom.getDouble(fromi))) {
            status = false;break;
          }
        } else if(type=="char") {
          if(!tuple->fill(index,aFrom.getChar(fromi))) {status = false;break;}
        } else if(type=="short") {
          if(!tuple->fill(index,aFrom.getShort(fromi))) {status = false;break;}
        } else if(type=="int") {
          if(!tuple->fill(index,aFrom.getInt(fromi))) {status = false;break;}
        } else if(type=="long") {
          if(!tuple->fill(index,aFrom.getLong(fromi))) {status = false;break;}
        } else if(type=="boolean") {
          if(!tuple->fill(index,aFrom.getBoolean(fromi))) {
            status = false;break;
          }
        } else if(type=="string") {
          if(!tuple->fill(index,aFrom.getString(fromi))) {
            status = false;break;
          }
        } else if(type=="ITuple") {
          if(!tuple->fill(index,*aFrom.getObject(fromi))) {
            status = false;break;
          }
        } else {
          f_out << "BatchLab::TupleFactory::createFiltered :" 
              << " column type " << type << " not handled."
              << std::endl;
          status = false;
          break;
        }
      }
      if(!status) {
        f_out << "BatchLab::TupleFactory::createFiltered :" 
            << " problem when filling tuple."
            << std::endl;
        tuple->reset();
        break;
      }
      tuple->addRow();
    }
    return tuple;
  }
  
  virtual AIDA::IFilter* createFilter(const std::string& aScript){
    AIDA::ITree* atree = INLIB_CAST(fTree,AIDA::ITree);
    if(!atree) return 0;
    return new BatchLab::Filter(f_out,aScript,*atree);
  }

  virtual AIDA::IFilter* createFilter(const std::string& aScript,int,int = 0){
    AIDA::ITree* atree = INLIB_CAST(fTree,AIDA::ITree);
    if(!atree) return 0;
    //FIXME
    return new BatchLab::Filter(f_out,aScript,*atree);
  }

  virtual AIDA::IEvaluator* createEvaluator(const std::string& aScript){
    AIDA::ITree* atree = INLIB_CAST(fTree,AIDA::ITree);
    if(!atree) return 0;
    return new BatchLab::Evaluator(f_out,aScript,*atree);
  }
public:
  virtual AIDA::ITuple* createCopy(const std::string& aPath,
                                   AIDA::ITuple& aFrom){
    AIDA::ITuple* tuple = 
      create(aPath,aFrom.title(),aFrom.columnNames(),aFrom.columnTypes(),"");
    if(!tuple) {
      f_out << "BatchLab::TupleFactory::createCopy :" 
          << " Can't create tuple " << inlib::sout(aPath) 
          << " in tree with store " << inlib::sout(fTree.storeName())
          << std::endl;
      return 0;
    }
    if(!amanip::copy(f_out,const_cast<AIDA::ITuple&>(aFrom),*tuple)) {
      f_out << "BatchLab::TupleFactory::createCopy :" 
          << " cannot copy tuple content."
          << std::endl;
      tuple->reset();
    }
    return tuple;
  }
public:
  TupleFactory(Slash::Core::ISession& aSession,Slash::Store::ITree& aTree)
  :BaseFactory(aSession.cout(),aTree)
  ,fSession(aSession){
    Lib::Debug::increment("BatchLab::TupleFactory");
  }
  virtual ~TupleFactory(){
    Lib::Debug::decrement("BatchLab::TupleFactory");
  }
private:
  Slash::Core::ISession& fSession;
};

}

#endif
