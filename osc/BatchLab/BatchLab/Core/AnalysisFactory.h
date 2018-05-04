#ifndef BatchLab_AnalysisFactory_h
#define BatchLab_AnalysisFactory_h

// inheritance :
#include <Slash/AIDA/IAnalysisFactory.h>
//#include <AIDA/IAnalysisFactory.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <Slash/Core/ILibrary.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/LibraryManager.h>
#include <Slash/Tools/Dictionary.h>

#include <Lib/FunctionManager.h>

#include <BatchLab/Core/TreeFactory.h>
#include <BatchLab/Core/TupleFactory.h>
#include <BatchLab/Core/HistogramFactory.h>
#include <BatchLab/Core/FunctionFactory.h>
#include <BatchLab/Core/DataPointSetFactory.h>
#include <BatchLab/Core/FitFactory.h>
#include <BatchLab/Core/HistogramAllocator.h>

#include <AIDA/ITree.h>

//for allocators :
//FIXME : is it the right place ?
#include <BatchLab/Core/Histogram.h> //FIXME
#include <BatchLab/Core/Cloud.h>
#include <BatchLab/Core/DataPointSet.h>
#include <BatchLab/Core/Function.h>
#include <BatchLab/Core/MemoryTuple.h>

namespace BatchLab {

// A factory manager.

class AnalysisFactory 
:public virtual Slash::Core::IManager
//,public virtual AIDA::IAnalysisFactory {
,public virtual Slash::AIDA::IAnalysisFactory {
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::AnalysisFactory");return s_v;
  }
public: //AIDA/IAnalysisFactory

  virtual AIDA::ITreeFactory* createTreeFactory(
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    const std::string& = "" //aOptions
#endif
  ){return new TreeFactory(fSession);}

  virtual AIDA::ITupleFactory* createTupleFactory(AIDA::ITree& aTree
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ){
    Slash::Store::ITree* baseTree = INLIB_CAST(aTree,Slash::Store::ITree);
    if(!baseTree) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createTupleFactory :"
          << " received tree is not a Slash::Store::ITree." << std::endl;
      return 0;
    }
    return new BatchLab::TupleFactory(fSession,*baseTree);
  }
  
  virtual AIDA::IHistogramFactory* createHistogramFactory(AIDA::ITree& aTree
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ){
    Slash::Store::ITree* baseTree = INLIB_CAST(aTree,Slash::Store::ITree);
    if(!baseTree) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createHistogramFactory :"
          << " received tree is not a Slash::Store::ITree." << std::endl;
      return 0;
    }
    return new BatchLab::HistogramFactory(fSession,*baseTree);
  }

  virtual AIDA::IFunctionFactory* createFunctionFactory(AIDA::ITree& aTree
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ){
    Slash::Store::ITree* baseTree = INLIB_CAST(aTree,Slash::Store::ITree);
    if(!baseTree) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createFunctionFactory :"
          << " received tree is not a Slash::Store::ITree." << std::endl;
      return 0;
    }
    return new BatchLab::FunctionFactory(fSession,*baseTree);
  }

  virtual AIDA::IPlotterFactory* createPlotterFactory(
    int aArgc = 0,char** aArgv = 0
    ,const std::string& aImplementation = ""
    ,const std::string& aLibrary = ""
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ){
    if( (aImplementation.size()) && (aLibrary.size()) ) {
      // Get a plotter factory from another implementation :
      std::string entry = aImplementation + "_createPlotterFactory";
  
      Slash::Core::ILibraryManager* libraryManager = 
        Slash::libraryManager(fSession);
      if(!libraryManager) {
        std::ostream& out = fSession.cout();
        out << "BatchLab::AnalysisFactory::createPlotterFactory :"
            << " LibraryManager not found." << std::endl;
        return 0;
      }
      Slash::Core::ILibrary* dll = 
        libraryManager->addLibrary(aLibrary,aLibrary,"");
      if(!dll) {
        std::ostream& out = fSession.cout();
        out << "BatchLab::AnalysisFactory::createPlotterFactory :"
            << " unable to add a DLL" << std::endl;
        return 0;
      }
      typedef AIDA::IPlotterFactory*(*GetFactoryProc)(int,char**);
      GetFactoryProc p = (GetFactoryProc)dll->findProcedure(entry);
      if(!p) {
        std::ostream& out = fSession.cout();
        out << "BatchLab::AnalysisFactory::createPlotterFactory :"
            << " cannot get the entry point \"" << entry << "\"" 
            << "in \"" << aLibrary << "\"." << std::endl;
        return 0;
      }
      return p(aArgc,aArgv);
    } else {
      return 0;
    }
  }

  virtual AIDA::IDataPointSetFactory* createDataPointSetFactory(
    AIDA::ITree& aTree
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ){
    Slash::Store::ITree* baseTree = INLIB_CAST(aTree,Slash::Store::ITree);
    if(!baseTree) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createDataPointSetFactory :"
          << " received tree is not a Slash::Store::ITree." << std::endl;
      return 0;
    }
    return new BatchLab::DataPointSetFactory(fSession.cout(),*baseTree);
  }

  virtual AIDA::IFitFactory* createFitFactory(
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    const std::string& = "" //aOptions
#endif
  ){
    Slash::Store::ITree* memoryTree = Slash::memoryTree(fSession);
    if(!memoryTree) {
      TreeFactory tf(fSession);
      tf.create();
      memoryTree = Slash::memoryTree(fSession);
    }
    if(!memoryTree) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createFitFactory :"
          << " MemoryTree not found." 
          << std::endl;
      return 0;
    }
    if(!fFunctionFactory) {
      fFunctionFactory = new BatchLab::FunctionFactory(fSession,*memoryTree);
    }
    if(!fFunctionFactory) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createFitFactory :"
          << " can't get a FunctionFactory." 
          << std::endl;
      return 0;
    }
    if(!fTupleFactory) {
      fTupleFactory = new BatchLab::TupleFactory(fSession,*memoryTree);
    }
    if(!fTupleFactory) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::AnalysisFactory::createFitFactory :"
          << " can't get a TupleFactory." 
          << std::endl;
      return 0;
    }
    return new BatchLab::FitFactory
                 (fSession.cout(),*fFunctionFactory,*fTupleFactory);
  }

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else

  virtual AIDA::IGenericFactory* createGenericFactory(const std::string& factoryType, const std::string& options = "");
  virtual AIDA::IGenericFactory* createManagedObjectGenericFactory(const std::string& factoryType,AIDA::ITree& tree,const std::string& options = "");

  virtual AIDA::IGenericFactory* createGenericFactory(const std::string& /*aFactoryType*/,const std::string& = ""){
  ){return 0;}
  virtual AIDA::IGenericFactory* createManagedObjectGenericFactory(const std::string& /*factoryType*/,AIDA::ITree&,const std::string& = ""){return 0;}

#endif
public: // Slash::Core::IManager :
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    // inheritance :
    INLIB_IF_CAST(AIDA::IAnalysisFactory)
    else INLIB_IF_CAST(Slash::AIDA::IAnalysisFactory)
    else INLIB_IF_CAST(Slash::Core::IManager)
    // this :
    else INLIB_IF_CAST(BatchLab::AnalysisFactory)
    else return 0;
  }

public:
  AnalysisFactory(Slash::Core::ISession& aSession)
  :fSession(aSession)
  ,fName(Slash_AnalysisFactory)
  ,fFunctionFactory(0)
  ,fTupleFactory(0)
  {
    Lib::Debug::increment(AnalysisFactory::s_class());

    if(!fSession.findManager("LibraryManager")) {
      fSession.addManager
        (new Slash::LibraryManager(fSession,"LibraryManager"));
    }
    if(!fSession.findManager("FunctionManager")) {
      fSession.addManager(new Lib::FunctionManager(fSession));
    }
    if(!fSession.findManager("BatchLab::HistogramAllocator")) {
      fSession.addManager(new BatchLab::HistogramAllocator(fSession.cout()));
    }
    // For the IO :
    if(!fSession.findManager("Lib::Dictionary")) {
      Slash::Dictionary* manager = 
        new Slash::Dictionary(fSession.cout(),"Lib::Dictionary");
      //FIXME : is it the right place ?
  
      manager->add(new Slash::Class(fSession.cout(),
        Histogram1D::s_class(),1,allocate<BatchLab::Histogram1D>));
      manager->add(new Slash::Class(fSession.cout(),
        Histogram2D::s_class(),1,allocate<BatchLab::Histogram2D>));
      manager->add(new Slash::Class(fSession.cout(),
        Histogram3D::s_class(),1,allocate<BatchLab::Histogram3D>));
  
      manager->add(new Slash::Class(fSession.cout(),
        Profile1D::s_class(),1,allocate<BatchLab::Profile1D>));
      manager->add(new Slash::Class(fSession.cout(),
        Profile2D::s_class(),1,allocate<BatchLab::Profile2D>));
  
      manager->add(new Slash::Class(fSession.cout(),
        Cloud1D::s_class(),1,allocate<BatchLab::Cloud1D>));
      manager->add(new Slash::Class(fSession.cout(),
        Cloud2D::s_class(),1,allocate<BatchLab::Cloud2D>));
      manager->add(new Slash::Class(fSession.cout(),
        Cloud3D::s_class(),1,allocate<BatchLab::Cloud3D>));
  
      manager->add(new Slash::Class(fSession.cout(),
        DataPointSet::s_class(),1,allocate<BatchLab::DataPointSet>));
  
      manager->add(new Slash::Class(fSession.cout(),
        MemoryTuple::s_class(),1,allocate<BatchLab::MemoryTuple>));
  
      manager->add(new Slash::Class(fSession.cout(),
        Function::Gaussian::s_class(),1,allocate<Function::Gaussian>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::BreitWigner::s_class(),1,allocate<Function::BreitWigner>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::ExpHBOOK::s_class(),1,allocate<Function::ExpHBOOK>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::ExpAIDA::s_class(),1,allocate<Function::ExpAIDA>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::Polynomial::s_class(),1,allocate<Function::Polynomial>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::Scripted::s_class(),1,allocate<Function::Scripted>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::Composite::s_class(),1,allocate<Function::Composite>));
      manager->add(new Slash::Class(fSession.cout(),
        Function::Compiled::s_class(),1,allocate<Function::Compiled>));
  
      fSession.addManager(manager);
    }
  }

  virtual ~AnalysisFactory() {
    delete fFunctionFactory;
    delete fTupleFactory;
    Lib::Debug::decrement(AnalysisFactory::s_class());
  }

protected:
  template <class T>
  inline static Slash::Meta::IObject* allocate(std::ostream& a_out){
    return new T(a_out);
  }
protected:
  Slash::Core::ISession& fSession;
private:
  std::string fName;
  AIDA::IFunctionFactory* fFunctionFactory;
  AIDA::ITupleFactory* fTupleFactory;
};

}

#endif
