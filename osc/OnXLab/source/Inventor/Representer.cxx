// this :
#include <OnXLab/Inventor/Representer.h>

// Slash :
#include <Slash/Core/IRelation.h>
#include <Slash/Core/IRelationManager.h>
#include <Slash/Core/IRelationVisitor.h>
#include <Slash/Core/ISession.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/UI/IUI.h> //for lock/unlock
#include <Slash/Tools/Relation.h>
#include <inlib/cast>

// Lib :
#include <Lib/Debug.h>

// BatchLab :
#include <BatchLab/Core/Histogram.h>
#include <BatchLab/Core/Function.h>
#include <BatchLab/Core/Cloud.h>
#include <BatchLab/Core/DataPointSet.h>

#include <OnXLab/Inventor/PlottableThings.h>

#include <HEPVis/nodekits/SoPlotter.h>

namespace OnXLab {

class BaseObject_SoPlotter_Relation : public virtual Slash::Core::IRelation {
public: // IRelation :
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::BaseObject_SoPlotter_Relation)
    else INLIB_IF_CAST(Slash::Core::IRelation)
    else return 0;
  }
  virtual bool schedule() {
    switch(fEmitMode) {
    case Representer::NONE:
      break;
    case Representer::RENDER:
      fUI.lock();
      if(fSoPlotter.isUpdateFlagRaised(fUpdateWhat)==FALSE) {
        fSoPlotter.raiseUpdateFlag(fUpdateWhat);
        fSoPlotter.updateChildren();
        render();
      }
      fUI.unlock();
      break;
    case Representer::TOUCH:
      fUI.lock();
      if(fSoPlotter.isUpdateFlagRaised(fUpdateWhat)==FALSE) {
        fSoPlotter.raiseUpdateFlag(fUpdateWhat);
        fSoPlotter.touch();
      }
      fUI.unlock();
      break;
    }
    return true;
  }
  virtual void unSchedule() {}
  virtual void* first() const { return (void*)fObject;}
  virtual void* second() const { return &fSoPlotter;}
public:
  BaseObject_SoPlotter_Relation
  (Slash::Core::IRelationManager& aRelationManager,
   const BatchLab::BaseObject* aObject,SoPlotter& aSoPlotter,Slash::UI::IUI& aUI,Slash::UI::IWidget& aWidget,Representer::EmitMode aEmitMode,SoPlotter::UpdateFlag aUpdateWhat)
  :fName("OnXLab::BaseObject_SoPlotter_Relation")
  ,fRelationManager(aRelationManager)
  ,fObject(aObject),fSoPlotter(aSoPlotter)
  ,fUI(aUI),fWidget(aWidget),fEmitMode(aEmitMode),fUpdateWhat(aUpdateWhat){
    Lib::Debug::increment("OnXLab::BaseObject_SoPlotter_Relation");
    fSoPlotter.ref();
    //printf("debug : OnXLab::BaseObject_SoPlotter_Relation::BaseObject_SoPlotter_Relation : %ld\n",this);
  }
  virtual ~BaseObject_SoPlotter_Relation(){
    //printf("debug : OnXLab::BaseObject_SoPlotter_Relation::~BaseObject_SoPlotter_Relation : %ld\n",this);
    fSoPlotter.unref();
    Lib::Debug::decrement("OnXLab::BaseObject_SoPlotter_Relation");
  }
private:
  void render() const {
    Slash::RV_Find visitor(&fWidget,&fSoPlotter);
    fRelationManager.visit(visitor);
    if(!visitor.fFound) return;
    Slash::UI::ISoViewer* viewer = INLIB_CAST(fWidget,Slash::UI::ISoViewer);
    if(!viewer) return;
    //printf("debug : viewer render\n");
    viewer->render(); // it freezes the GUI responsiveness.
  }
private:
  std::string fName;
  Slash::Core::IRelationManager& fRelationManager;
  const BatchLab::BaseObject* fObject;
protected:
  SoPlotter& fSoPlotter;
  Slash::UI::IUI& fUI; //for lock/unlock
  Slash::UI::IWidget& fWidget;  
  Representer::EmitMode fEmitMode;
  SoPlotter::UpdateFlag fUpdateWhat;
};

class Widget_SoPlotter_Relation : public virtual Slash::Core::IRelation {
public: // IRelation :
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::Widget_SoPlotter_Relation)
    else INLIB_IF_CAST(Slash::Core::IRelation)
    else return 0;
  }
  virtual bool schedule() {return true;}
  virtual void unSchedule() {}
  virtual void* first() const { return &fWidget;}
  virtual void* second() const { return &fSoPlotter;}
public:
  Widget_SoPlotter_Relation
  (Slash::UI::IWidget& aWidget,SoPlotter& aSoPlotter)
  :fName("OnXLab::Widget_SoPlotter_Relation")
  ,fWidget(aWidget)
  ,fSoPlotter(aSoPlotter){
    Lib::Debug::increment("OnXLab::Widget_SoPlotter_Relation");
    //fSoPlotter.ref();
    //printf("debug : OnXLab::Widget_SoPlotter_Relation::Widget_SoPlotter_Relation : %ld\n",this);
  }
  virtual ~Widget_SoPlotter_Relation(){
    //printf("debug : OnXLab::Widget_SoPlotter_Relation::~Widget_SoPlotter_Relation : %ld\n",this);
    //fSoPlotter.unref();
    Lib::Debug::decrement("OnXLab::Widget_SoPlotter_Relation");
  }
private:
  std::string fName;
  Slash::UI::IWidget& fWidget;  
  SoPlotter& fSoPlotter;
};

class Notifier : public virtual INotifier {
public: //INotifier
  virtual bool isValid() const {
    Slash::RV_First_Find visitor(fObject);
    fRelationManager.visit(visitor);
    return visitor.fFound;
  }
  //virtual void destroy() const {}
public:
  Notifier(Slash::Core::IRelationManager& aRelationManager,
           const BatchLab::BaseObject* aObject,
           SoPlotter& aSoPlotter)
  :fRelationManager(aRelationManager),fObject(aObject),fSoPlotter(aSoPlotter){
    Lib::Debug::increment("OnXLab::Notifier");
  }
  virtual ~Notifier(){
    // A SbPlottableObject had been deleted.
    Slash::RV_Remove visitor(fObject,&fSoPlotter);
    fRelationManager.remove(visitor);
    Lib::Debug::decrement("OnXLab::Notifier");
  }
private:
  Slash::Core::IRelationManager& fRelationManager;
  const BatchLab::BaseObject* fObject;
  SoPlotter& fSoPlotter;
};

class DummyNotifier : public virtual INotifier {
public: //INotifier
  virtual bool isValid() const { return true;}
  virtual void destroy() const {}
public:
  DummyNotifier(){Lib::Debug::increment("OnXLab::DummyNotifier");}
  virtual ~DummyNotifier(){Lib::Debug::decrement("OnXLab::DummyNotifier");}
};


INotifier*  get_notifier(const BatchLab::BaseObject* aObject,Slash::Core::IRelationManager& aRelationManager,SoPlotter& aSoPlotter,Slash::UI::IUI& aUI,Slash::UI::IWidget& aWidget,Representer::EmitMode aEmitMode,SoPlotter::UpdateFlag aUpdateWhat){
  const_cast<BatchLab::BaseObject*>(aObject)->
    setRelationManager(&aRelationManager);
  aRelationManager.add(new BaseObject_SoPlotter_Relation(aRelationManager,aObject,aSoPlotter,aUI,aWidget,aEmitMode,aUpdateWhat));
  aRelationManager.add(new Widget_SoPlotter_Relation(aWidget,aSoPlotter));
  return new Notifier(aRelationManager,aObject,aSoPlotter);
}


template <class T>  
INotifier* get_notifierT(const T& aObject,Slash::Core::IRelationManager& aRelationManager,SoPlotter& aSoPlotter,Slash::UI::IUI& aUI,Slash::UI::IWidget& aWidget,Representer::EmitMode aEmitMode,SoPlotter::UpdateFlag aUpdateWhat){
  if(const BatchLab::BaseObject* object = 
       INLIB_CONST_CAST(aObject,BatchLab::BaseObject)) {
    const_cast<BatchLab::BaseObject*>(object)->
      setRelationManager(&aRelationManager);
    aRelationManager.add(new BaseObject_SoPlotter_Relation(aRelationManager,object,aSoPlotter,aUI,aWidget,aEmitMode,aUpdateWhat));
    aRelationManager.add(new Widget_SoPlotter_Relation(aWidget,aSoPlotter));
    return new Notifier(aRelationManager,object,aSoPlotter);
  } else {
    return new DummyNotifier();
  }
}

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
SbPlottableObject* OnXLab::Representer::represent(
 const AIDA::IBaseHistogram& aObject
,Slash::Core::IRelationManager& aRelationManager
,SoPlotter& aSoPlotter
,Slash::UI::IUI& aUI //for lock/unlock
,Slash::UI::IWidget& aWidget
,std::ostream& a_out
,int aVerboseLevel
,bool aRetain
,EmitMode aEmitMode
)
//////////////////////////////////////////////////////////////////////////////
// Returned value deleted by the plotter.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  //////////////////
  // Histograms :
  //////////////////
  if(const BatchLab::Histogram1D* object = 
       INLIB_CONST_CAST(aObject,BatchLab::Histogram1D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableHistogram1D(object->name(),object,notifier,aRetain);
  } else if(const BatchLab::Histogram2D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Histogram2D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableHistogram2D(object->name(),object,notifier,aRetain);


  //////////////////
  // Profiles :
  //////////////////
  } else if(const BatchLab::Profile1D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Profile1D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableProfile1D(object->name(),object,notifier,aRetain);
  } else if(const BatchLab::Profile2D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Profile2D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableProfile2D(object->name(),object,notifier,aRetain);

  //////////////////
  // Clouds :
  //////////////////
  } else if(const BatchLab::Cloud1D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Cloud1D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier = 
        get_notifier(object,
                     aRelationManager,aSoPlotter,aUI,aWidget,
                     aEmitMode,SoPlotter::BINS);
      return new PlottableCloud1D(object->name(),object,notifier,aRetain);
    }
  } else if(const BatchLab::Cloud2D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Cloud2D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier = 
        get_notifier(object,
                     aRelationManager,aSoPlotter,aUI,aWidget,
                     aEmitMode,SoPlotter::POINTS);
      return new PlottableCloud2D(object->name(),object,notifier,aRetain);
    }
  } else if(const BatchLab::Cloud3D* object = 
              INLIB_CONST_CAST(aObject,BatchLab::Cloud3D)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier = 
        get_notifier(object,
                     aRelationManager,aSoPlotter,aUI,aWidget,
                     aEmitMode,SoPlotter::POINTS);
      return new PlottableCloud3D(object->name(),object,notifier,aRetain);
    }

  //////////////////
  // DataPointSet :
  //////////////////
  } else if(const BatchLab::DataPointSet* object = 
              INLIB_CONST_CAST(aObject,BatchLab::DataPointSet)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableDataPointSet(object->name(),object,notifier,aRetain);
  } 

  /////////////////////////////////
  // Not a OnXLab object. 
  // For example a Zebra::Histogram is not a BatchLab::Histogram.
  // Try anyway :
  /////////////////////////////////

  if(aVerboseLevel) {
    a_out << "OnXLab::Representer::represent :"
          << " " << inlib::sout(aObject.title()) << " not a OnXLab object." 
          << std::endl;
  }

  if(const AIDA::IHistogram1D* object = 
       INLIB_CONST_CAST(aObject,AIDA::IHistogram1D)) {
    INotifier* notifier = 
      get_notifierT<AIDA::IBaseHistogram>
         (aObject,
          aRelationManager,aSoPlotter,aUI,aWidget,
          aEmitMode,SoPlotter::BINS);
    //FIXME : retain = true ? Suppress DummyNotifer ?
    // It will be more reliable.
    return new PlottableHistogram1D(object->title(),object,notifier,aRetain);
  } else if(const AIDA::IHistogram2D* object = 
              INLIB_CONST_CAST(aObject,AIDA::IHistogram2D)){
    //FIXME : retain = true ? Suppress DummyNotifer ?
    // It will be more reliable.
    INotifier* notifier = 
      get_notifierT<AIDA::IBaseHistogram>
         (aObject,
          aRelationManager,aSoPlotter,aUI,aWidget,
          aEmitMode,SoPlotter::BINS);
    return new PlottableHistogram2D(object->title(),object,notifier,aRetain);

  } else if(const AIDA::ICloud1D* object = 
              INLIB_CONST_CAST(aObject,AIDA::ICloud1D)) {
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier = 
        get_notifierT<AIDA::IBaseHistogram>
         (aObject,
          aRelationManager,aSoPlotter,aUI,aWidget,
          aEmitMode,SoPlotter::BINS);
      return new PlottableCloud1D(object->title(),object,notifier,aRetain);
    }
  } else if(const AIDA::ICloud2D* object = 
              INLIB_CONST_CAST(aObject,AIDA::ICloud2D)) {
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier = 
        get_notifierT<AIDA::IBaseHistogram>
         (aObject,
          aRelationManager,aSoPlotter,aUI,aWidget,
          aEmitMode,SoPlotter::POINTS);
      return new PlottableCloud2D(object->title(),object,notifier,aRetain);
    }
  } else if(const AIDA::ICloud3D* object = 
              INLIB_CONST_CAST(aObject,AIDA::ICloud3D)) {
    if(object->isConverted()) {
      return represent(object->histogram(),
                       aRelationManager,aSoPlotter,aUI,aWidget,a_out,
                       aVerboseLevel,aRetain,aEmitMode);
    } else {
      INotifier* notifier =
        get_notifierT<AIDA::IBaseHistogram>
          (aObject,
           aRelationManager,aSoPlotter,aUI,aWidget,
           aEmitMode,SoPlotter::POINTS);
      return new PlottableCloud3D(object->title(),object,notifier,aRetain);
    }
  } else {
    return 0;
  }

}
//////////////////////////////////////////////////////////////////////////////
SbPlottableObject* OnXLab::Representer::represent(
 const AIDA::IFunction& aObject
,Slash::Core::IRelationManager& aRelationManager
,SoPlotter& aSoPlotter
,Slash::UI::IUI& aUI //for lock/unlock
,Slash::UI::IWidget& aWidget
,std::ostream& a_out
,int aVerboseLevel
,bool aRetain
,EmitMode aEmitMode
)
//////////////////////////////////////////////////////////////////////////////
// Returned value deleted by the plotter.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(const BatchLab::BaseFunction* object = 
       INLIB_CONST_CAST(aObject,BatchLab::BaseFunction)){
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " function." << std::endl;
    }
    if(object->dimension()==1) {
      INotifier* notifier = 
        get_notifier(object,
                     aRelationManager,aSoPlotter,aUI,aWidget,
                     aEmitMode,SoPlotter::FUNCTION);
      return new PlottableFunction1D(object->name(),&aObject,notifier,aRetain);
    } else if(object->dimension()==2) {
      INotifier* notifier = 
        get_notifier(object,
                     aRelationManager,aSoPlotter,aUI,aWidget,
                     aEmitMode,SoPlotter::FUNCTION);
      if(aObject.codeletString()==BatchLab::Function::InsidePolygon::codelet()) { 
        return new PlottableInsidePolygon(object->name(),&aObject,notifier,aRetain);
      } else if(aObject.codeletString()==BatchLab::Function::InsideEllipse::codelet()) { 
        return new PlottableInsideEllipse(object->name(),&aObject,notifier,aRetain);
      } else {      
        return new PlottableFunction2D(object->name(),&aObject,notifier,aRetain);
      }
    } else {
      a_out << "OnXLab::Representer::represent :"
            << " function with dimension " << object->dimension()
            << " not handled."
            << std::endl;
      return 0;
    }
  } else {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " function (not a OnXLab one)." << std::endl;
    }
    if(aObject.dimension()==1) {
      INotifier* notifier = 
        get_notifierT<AIDA::IFunction>
          (aObject,
           aRelationManager,aSoPlotter,aUI,aWidget,
           aEmitMode,SoPlotter::FUNCTION);
      return new PlottableFunction1D("Function",&aObject,notifier,aRetain);
    } else if(aObject.dimension()==2) {

      INotifier* notifier = 
        get_notifierT<AIDA::IFunction>
          (aObject,
           aRelationManager,aSoPlotter,aUI,aWidget,
           aEmitMode,SoPlotter::FUNCTION);
      if(aObject.codeletString()==BatchLab::Function::InsidePolygon::codelet()) { 
        return new PlottableInsidePolygon("Function",&aObject,notifier,aRetain);
      } else if(aObject.codeletString()==BatchLab::Function::InsideEllipse::codelet()) { 
        return new PlottableInsideEllipse("Function",&aObject,notifier,aRetain);
      } else {
        return new PlottableFunction2D("Function",&aObject,notifier,aRetain);
      }
    } else {
      a_out << "OnXLab::Representer::represent :"
            << " function with dimension " << aObject.dimension()
            << " not handled."
            << std::endl;
      return 0;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
SbPlottableObject* OnXLab::Representer::represent(
 const AIDA::IDataPointSet& aObject
,Slash::Core::IRelationManager& aRelationManager
,SoPlotter& aSoPlotter
,Slash::UI::IUI& aUI //for lock/unlock
,Slash::UI::IWidget& aWidget
,std::ostream& a_out
,int aVerboseLevel
,bool aRetain
,EmitMode aEmitMode
)
//////////////////////////////////////////////////////////////////////////////
// Returned value deleted by the plotter.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(const BatchLab::DataPointSet* object = 
       INLIB_CONST_CAST(aObject,BatchLab::DataPointSet)) {
    if(aVerboseLevel) {
      a_out << "OnXLab::Representer::represent :"
            << " " << inlib::sout(object->name()) << "." 
            << std::endl;
    }
    INotifier* notifier = 
      get_notifier(object,
                   aRelationManager,aSoPlotter,aUI,aWidget,
                   aEmitMode,SoPlotter::BINS);
    return new PlottableDataPointSet(object->name(),&aObject,notifier,aRetain);
  } 

  // Not a OnXLab object. Try anyway.

  if(aVerboseLevel) {
    a_out << "OnXLab::Representer::represent :"
          << " " << inlib::sout(aObject.title()) << " not a OnXLab object."
          << std::endl;
  }

  if(const AIDA::IDataPointSet* object = 
       INLIB_CONST_CAST(aObject,AIDA::IDataPointSet)) {
    INotifier* notifier = 
      get_notifierT<AIDA::IDataPointSet>
        (aObject,
         aRelationManager,aSoPlotter,aUI,aWidget,
         aEmitMode,SoPlotter::BINS);
    return new PlottableDataPointSet(object->title(),object,notifier,aRetain);
  } else {
    return 0;
  }
}
