#ifndef BatchLab_HistogramFactory_h
#define BatchLab_HistogramFactory_h

// Inheritance :
#include <AIDA/IHistogramFactory.h>
#include <BatchLab/Core/BaseFactory.h>

namespace Slash {namespace Core {class ISession;}}

namespace Slash {namespace Store {class ITree;}}

#include <BatchLab/Core/BaseObject.h>

namespace BatchLab {

class HistogramAllocator;

class HistogramFactory 
:public BaseFactory
,public virtual AIDA::IHistogramFactory {
public: //AIDA/AIDA::IHistogramFactory
  virtual AIDA::IHistogram1D* createHistogram1D(const std::string&,
                                                const std::string&,
                                                int,double,double,
                                                const std::string&);
  virtual AIDA::IHistogram1D* createHistogram1D(const std::string&, 
                                                const std::string&, 
                                                const std::vector< double >&, 
                                                const std::string&);
  virtual AIDA::IHistogram1D * createHistogram1D(const std::string&, 
                                                 int,double,double);
  
  virtual AIDA::IHistogram2D* createHistogram2D(const std::string&,
                                                const std::string&,
                                                int,double,double,
                                                int,double,double,
                                                const std::string&);
  virtual AIDA::IHistogram2D* createHistogram2D(const std::string&,
                                                const std::string&,
                                                const std::vector<double>&,
                                                const std::vector<double>&,
                                                const std::string&);
  virtual AIDA::IHistogram2D* createHistogram2D(const std::string&, 
                                                int,double,double,
                                                int,double,double);
  
  virtual AIDA::IHistogram3D* createHistogram3D(const std::string&,
                                                const std::string&,
                                                int,double,double,
                                                int,double,double,
                                                int,double,double,
                                                const std::string&);
  virtual AIDA::IHistogram3D* createHistogram3D(const std::string&,
                                                const std::string&,
                                                const std::vector<double>&,
                                                const std::vector<double>&,
                                                const std::vector<double>&,
                                                const std::string&);
  virtual AIDA::IHistogram3D* createHistogram3D(const std::string&, 
                                                int,double,double,
                                                int,double,double,
                                                int,double,double);
  
  virtual AIDA::IHistogram1D* add(const std::string&,
                                  const AIDA::IHistogram1D&,
                                  const AIDA::IHistogram1D&);
  virtual AIDA::IHistogram1D* subtract(const std::string&,
                                       const AIDA::IHistogram1D&,
                                       const AIDA::IHistogram1D&);
  virtual AIDA::IHistogram1D* multiply(const std::string&,
                                       const AIDA::IHistogram1D&,
                                       const AIDA::IHistogram1D&);
  virtual AIDA::IHistogram1D* divide(const std::string&,
                                     const AIDA::IHistogram1D&,
                                     const AIDA::IHistogram1D&);
  
  virtual AIDA::IHistogram2D* add(const std::string&,
                                  const AIDA::IHistogram2D&,
                                  const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram2D* subtract(const std::string&,
                                       const AIDA::IHistogram2D&,
                                       const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram2D* multiply(const std::string&,
                                       const AIDA::IHistogram2D&,
                                       const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram2D* divide(const std::string&,
                                     const AIDA::IHistogram2D&,
                                     const AIDA::IHistogram2D&);

  virtual AIDA::IHistogram3D* add(const std::string&,
                                  const AIDA::IHistogram3D&,
                                  const AIDA::IHistogram3D&);
  virtual AIDA::IHistogram3D* subtract(const std::string&,
                                       const AIDA::IHistogram3D&,
                                       const AIDA::IHistogram3D&);
  virtual AIDA::IHistogram3D* multiply(const std::string&,
                                       const AIDA::IHistogram3D&,
                                       const AIDA::IHistogram3D&);
  virtual AIDA::IHistogram3D* divide(const std::string&,
                                     const AIDA::IHistogram3D&,
                                     const AIDA::IHistogram3D&);

  virtual AIDA::IHistogram1D* sliceX(const std::string&,
                                     const AIDA::IHistogram2D&,int,int);
  virtual AIDA::IHistogram1D* sliceY(const std::string&,
                                     const AIDA::IHistogram2D&,int,int);
  virtual AIDA::IHistogram1D* sliceX(const std::string&,
                                     const AIDA::IHistogram2D&,int);
  virtual AIDA::IHistogram1D* sliceY(const std::string&,
                                     const AIDA::IHistogram2D&,int);
  virtual AIDA::IHistogram1D* projectionX(const std::string&,
                                          const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram1D* projectionY(const std::string&,
                                          const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram2D* sliceXY(const std::string&,
                                      const AIDA::IHistogram3D&,int,int);
  virtual AIDA::IHistogram2D* sliceXZ(const std::string&,
                                      const AIDA::IHistogram3D&,int,int);
  virtual AIDA::IHistogram2D* sliceYZ(const std::string&,
                                      const AIDA::IHistogram3D&,int,int);
  virtual AIDA::IHistogram2D* projectionXY(const std::string&,
                                           const AIDA::IHistogram3D&);
  virtual AIDA::IHistogram2D* projectionXZ(const std::string&,
                                           const AIDA::IHistogram3D&);
  virtual AIDA::IHistogram2D* projectionYZ(const std::string&,
                                           const AIDA::IHistogram3D&);

  virtual AIDA::ICloud1D* createCloud1D(const std::string&,
                                        const std::string&,
                                        int=-1,
                                        const std::string& = "");
  virtual AIDA::ICloud1D* createCloud1D(const std::string&);

  virtual AIDA::ICloud2D* createCloud2D(const std::string&,
                                        const std::string&,
                                        int=-1,
                                        const std::string& = "");
  virtual AIDA::ICloud2D* createCloud2D(const std::string&);
  
  virtual AIDA::ICloud3D* createCloud3D(const std::string&,
                                        const std::string&,
                                        int=-1,
                                        const std::string& = "");
  virtual AIDA::ICloud3D* createCloud3D(const std::string&);


  virtual bool destroy(AIDA::IBaseHistogram*);
  virtual AIDA::ICloud1D* createCopy(const std::string&,
                                     const AIDA::ICloud1D&);
  virtual AIDA::ICloud2D* createCopy(const std::string&,
                                     const AIDA::ICloud2D&);
  virtual AIDA::ICloud3D* createCopy(const std::string&,
                                     const AIDA::ICloud3D&);
  virtual AIDA::IHistogram1D* createCopy(const std::string&,
                                         const AIDA::IHistogram1D&);
  virtual AIDA::IHistogram2D* createCopy(const std::string&,
                                         const AIDA::IHistogram2D&);
  virtual AIDA::IHistogram3D* createCopy(const std::string&,
                                         const AIDA::IHistogram3D&);
  virtual AIDA::IProfile1D* createCopy(const std::string&,
                                       const AIDA::IProfile1D&);
  virtual AIDA::IProfile2D* createCopy(const std::string&,
                                       const AIDA::IProfile2D&);


  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            const std::string&,
                                            int,double,double,
                                            const std::string& = "");
  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            const std::string&,
                                            int,double,double,
                                            double,double,
                                            const std::string& = "");
  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            const std::string&,
                                            const std::vector<double>&,
                                            const std::string& = "");
  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            const std::string&,
                                            const std::vector<double>&,
                                            double,double,
                                            const std::string& = "");
  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            int,double,double);
  virtual AIDA::IProfile1D* createProfile1D(const std::string&,
                                            int,double,double,
                                            double,double);

  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            const std::string&,
                                            int,double,double,
                                            int,double,double,
                                            const std::string& = "");
  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            const std::string&,
                                            int,double,double,
                                            int,double,double,
                                            double,double,
                                            const std::string& = "");

  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            const std::string&,
                                            const std::vector<double>&,
                                            const std::vector<double>&,
                                            const std::string& = "");
  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            const std::string&,
                                            const std::vector<double>&,
                                            const std::vector<double>&,
                                            double,double,
                                            const std::string& = "");
  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            int,double,double,
                                            int,double,double);
  virtual AIDA::IProfile2D* createProfile2D(const std::string&,
                                            int,double,double,
                                            int,double,double,
                                            double,double);

public:
  HistogramFactory(Slash::Core::ISession&,Slash::Store::ITree&);
  virtual ~HistogramFactory();
private:
  inline bool CHECK_ALLOCATOR(const std::string& aCMT) {
    if(!fAllocator) {
      f_out << aCMT << " :"
          << " null allocator."
          << std::endl;
      return false;
    }
    return true;
  }

  inline bool THREAD_SAFETY(Slash::Store::IObject& a_object,
                            const std::string& aOptions,
                            const std::string& aCMT) {
    inlib::args args(aOptions,";",true);	
    bool thread_safe;
    if(args.find("thread_safe",thread_safe)) {
      BaseObject* bobject = INLIB_CAST(a_object,BatchLab::BaseObject);
      if(!bobject) {
        f_out << aCMT << " :"
            << " Slash::Store::IObject not a BatchLab::BaseObject."
            << std::endl;
        return false;
      }
      if(!bobject->setThreadSafe(thread_safe)){
        f_out << aCMT << " :"
              << " unable to set thread safety."
              << std::endl;
        return false;
      }
    }
    return true;
  }
private:
  Slash::Core::ISession& fSession;
  BatchLab::HistogramAllocator* fAllocator;
};

}

#include "HistogramFactory.icc"

#endif
