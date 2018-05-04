// this :
#include <OnXLab/Inventor/Plotter.h>

#include <AIDA/IHistogram.h>
#include <AIDA/IFunction.h>
#include <AIDA/ICloud.h>
#include <AIDA/IDataPointSet.h>
#include <AIDA/IAnnotation.h>

#include <Slash/Core/IMessage.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/Tools/Managers.h>
#include <inlib/cast>

#include <Lib/Debug.h>

#include <Inventor/errors/SoDebugError.h>

//#include <HEPVis/SbMutex.h>
#include <HEPVis/SbPlottableThings.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoTextRegion.h>
#include <HEPVis/nodekits/SoPage.h>

#include <OnXLab/Inventor/Representer.h>

#include <inlib/args>

#define NotFound (-1)

//namespace OnXLab {
//class Mutex : public virtual HEPVis::SbMutex {
//  virtual SbBool lock() const {return fMutex->lock()?TRUE:FALSE;}
//  virtual SbBool unlock() const {return fMutex->unlock()?TRUE:FALSE;}
//public:
//  Mutex():fMutex(0){fMutex = new Slash::Mutex();fMutex->initialize();}
//  virtual ~Mutex(){delete fMutex;}
//private:
//  Mutex(const Mutex&):fMutex(0){}
//  Mutex& operator=(const Mutex&){return *this;}
//private:
//  Slash::Mutex* fMutex;
//};
//}

namespace OnXLab {
class CyclicCbk : public virtual Slash::UI::ICyclicCallback {
public:
  virtual void execute() {
    fPlotter.raiseUpdateFlag(SoPlotter::ALL);
    fPlotter.updateChildren();
    fUI.synchronize();
  }
public:
  CyclicCbk(Slash::UI::IUI& aUI,SoPlotter& aPlotter)
  :fUI(aUI),fPlotter(aPlotter){
    Lib::Debug::increment("OnXLab::CyclicCbk");
    fPlotter.ref();
  }
  virtual ~CyclicCbk(){
    fPlotter.unref();
    Lib::Debug::decrement("OnXLab::CyclicCbk");
  }
public:
  Slash::UI::IUI& fUI;
  SoPlotter& fPlotter;
};
}

#define s_cyclic_prefix "OnXLab::Plotter::cyclic_"

static void create_cyclic(SoRegion* a_region,Slash::UI::IUI& aUI,unsigned int a_delay){
  if(a_region->isOfType(SoPlotterRegion::getClassTypeId())) {
    SoPlotter* soPlotter = ((SoPlotterRegion*)a_region)->getPlotter();
    std::string name(s_cyclic_prefix);
    name += inlib::to<unsigned long>((unsigned long)soPlotter);
    Slash::UI::ICyclic* cyclic = aUI.findCyclic(name);
    if(!cyclic) cyclic = aUI.createCyclic(name,1,"","");
    if(!cyclic) {
      aUI.session().cout() << "OnXLab::PlotterRegion::create_cyclic :"
                           << " can't create cyclic."
                           << std::endl;
    } else {
      cyclic->setDelay(a_delay);
      cyclic->setCallback(new OnXLab::CyclicCbk(aUI,*soPlotter));
      cyclic->start();
    }
  }
}

static void remove_cyclic(SoRegion* a_region,Slash::UI::IUI& aUI){
  if(a_region->isOfType(SoPlotterRegion::getClassTypeId())) {
    SoPlotter* soPlotter = ((SoPlotterRegion*)a_region)->getPlotter();
    std::string name(s_cyclic_prefix);
    name += inlib::to<unsigned long>((unsigned long)soPlotter);
    Slash::UI::ICyclic* cyclic = aUI.findCyclic(name);
    if(cyclic) aUI.removeCyclic(cyclic);
  }
}

#define OPTIONS \
  inlib::args args(aOptions,";",true);\
 {std::string value;\
  if(args.find("overall_style",value)) {\
    ((SoPlotterRegion*)fSoRegion)->setStyle(value.c_str());\
  }}\
  bool retain;\
  if(!args.find("retain",retain)) retain = false;\
  Representer::EmitMode emitMode = Representer::TOUCH;\
 {const AIDA::IAnnotation& annotation = aObject.annotation();\
  std::string value = annotation.value("emit");\
  if(!value.size()) args.find("emit",value);\
  if(value.size()) {\
         if(value=="none") {emitMode = Representer::NONE;}\
    else if(value=="render") {emitMode = Representer::RENDER;}\
    else if(value=="touch") {emitMode = Representer::TOUCH;}\
    else {\
      f_out << "OnXLab::PlotterRegion::plot :"\
            << " emit option value " << inlib::sout(value) << " unknown."\
            << std::endl;\
    }\
  }}\
/*{bool value;\
  if(args.find("thread_safe",value)) {\
    SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();\
    if(soPlotter->hasMutex()==FALSE) {\
      soPlotter->setMutex(new OnXLab::Mutex());\
    }\
  }}*/\
 {unsigned int value;\
  if(args.find("auto_refresh",value)) {\
    create_cyclic(fSoRegion,fUI,value);\
  }}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlotterRegion::PlotterRegion(
 std::ostream& a_out
,Slash::Core::IRelationManager& aRelationManager
,int aVerboseLevel
,SoRegion* aSoRegion
,Slash::UI::IUI& aUI //for lock/unlock
,Slash::UI::IWidget& aWidget
)
:f_out(a_out)
,fRelationManager(aRelationManager)
,fVerboseLevel(aVerboseLevel)
,fSoRegion(aSoRegion)
,fUI(aUI)
,fWidget(aWidget)
,fSetParameter(aSoRegion)
,fPlotterStyle(fSetParameter,"plotter")
,fLayout(fSetParameter,"plotter")
{
  Lib::Debug::increment("OnXLab::PlotterRegion");
}

//private:
OnXLab::PlotterRegion::PlotterRegion(const PlotterRegion& aRegion)
:AIDA::IPlotterRegion()
,f_out(aRegion.f_out)
,fRelationManager(aRegion.fRelationManager)
,fVerboseLevel(aRegion.fVerboseLevel)
,fSoRegion(aRegion.fSoRegion)
,fUI(aRegion.fUI)
,fWidget(aRegion.fWidget)
,fSetParameter(aRegion.fSoRegion)
,fPlotterStyle(fSetParameter,"plotter")
,fLayout(fSetParameter,"plotter")
{
  Lib::Debug::increment("OnXLab::PlotterRegion");
}

OnXLab::PlotterRegion::~PlotterRegion(){
  Lib::Debug::decrement("OnXLab::PlotterRegion");
}

//private:
OnXLab::PlotterRegion& OnXLab::PlotterRegion::operator=(const PlotterRegion& aRegion){return *this;}

SoRegion* OnXLab::PlotterRegion::soRegion() const{return fSoRegion;}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IBaseHistogram& aObject
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS

  SoPlotter* soPlotter = 0;
 {unsigned int value;
  if(args.find<unsigned int>("overlay",value)) {    
    soPlotter = ((SoPlotterRegion*)fSoRegion)->getOverlay(value);
  } else { //main plotter.
    soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  }}

  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable,soPlotter);
  fSoRegion->doIt(sb);
  return true;
}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IBaseHistogram& aObject
,const AIDA::IPlotterStyle&
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable);
  fSoRegion->doIt(sb);
  return true;
}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IFunction& aObject
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable);
  fSoRegion->doIt(sb);
  return true;
}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IFunction& aObject
,const AIDA::IPlotterStyle&
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable);
  fSoRegion->doIt(sb);
  return true;
}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IDataPointSet& aObject
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable);
  fSoRegion->doIt(sb);
  return true;
}

bool OnXLab::PlotterRegion::plot(
 const AIDA::IDataPointSet& aObject
,const AIDA::IPlotterStyle&
,const std::string& aOptions
){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  OPTIONS
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  SbPlottableObject* sbPlottable = 
    Representer::represent(aObject,
                           fRelationManager,*soPlotter,
                           fUI,fWidget,f_out,
                           fVerboseLevel,retain,emitMode);
  if(!sbPlottable) return false;
  SbPlotterDoIt sb(sbPlottable);
  fSoRegion->doIt(sb);
  return true;
}
/*FIXME
bool OnXLab::Plotter::plot(const AIDA::ICloud& aObject)
{
}
*/

bool OnXLab::PlotterRegion::remove(const AIDA::IBaseHistogram&){
  return false; //FIXME
}

bool OnXLab::PlotterRegion::remove(const AIDA::IFunction&){
  return false; //FIXME
}
bool OnXLab::PlotterRegion::remove(const AIDA::IDataPointSet&){
  return false; //FIXME
}
/*FIXME
bool OnXLab::Plotter::remove(const AIDA::ICloud&){
  return false;
}
*/

void OnXLab::PlotterRegion::clear(){
  remove_cyclic(fSoRegion,fUI);
  fSoRegion->clear("");
}

bool OnXLab::PlotterRegion::setParameter( const std::string& aParameter,const std::string& aValue){

  if(aParameter=="auto_refresh") {
    unsigned int value;
    if(inlib::to<unsigned int>(aValue,value)) {
      if(value) {
        create_cyclic(fSoRegion,fUI,value);
      } else {
        remove_cyclic(fSoRegion,fUI);
      }
      return true;
    } else {
      return false;
    }   
  }


  // If aString is a SoField then we do a 
  //   fSoRegion.set("<field> <value>")
  // If aString is a "part.field" then we do a 
  //   fSoRegion.set("<part>","<field> <value>")
  return (SoTools::setKitPart
            (fSoRegion,aParameter.c_str(),aValue.c_str())==TRUE?true:false);
}

std::string OnXLab::PlotterRegion::parameterValue(const std::string& aParameter
) const {  
  // aParameter = field then do a get("<field>")
  // aParameter = part.field then do a getPart("<part>") and then
  //  do a <field>.get("<value>")
  if(aParameter=="numberOfLegendRegions") { //OpenPAW/PAW_Manager::addLegend.
    if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return "";
    SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)fSoRegion;
    return inlib::to<int>(soPlotterRegion->getNumberOfLegendRegions());
  }
  SbString value;
  SoTools::getKitPart(fSoRegion,aParameter.c_str(),value);
  return std::string(value.getString());
}

AIDA::IPlotterStyle & OnXLab::PlotterRegion::style(){return fPlotterStyle;} 

// FIXME
bool OnXLab::PlotterRegion::setStyle(const AIDA::IPlotterStyle&) {return false;}
bool OnXLab::PlotterRegion::applyStyle(const AIDA::IPlotterStyle&) {return false;}

void OnXLab::PlotterRegion::setTitle(const std::string& aString){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  soPlotter->titleAutomated.setValue(FALSE);
  soPlotter->title.setValue(aString.c_str());
}

bool OnXLab::PlotterRegion::setXLimits(double aMin,double aMax){ 
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  soPlotter->xAxisAutomated.setValue(FALSE);
  soPlotter->xAxisMinimum.setValue((float)aMin);
  soPlotter->xAxisMaximum.setValue((float)aMax);
  return true;
}

bool OnXLab::PlotterRegion::setYLimits(double aMin,double aMax){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  soPlotter->yAxisAutomated.setValue(FALSE);
  soPlotter->yAxisMinimum.setValue((float)aMin);
  soPlotter->yAxisMaximum.setValue((float)aMax);
  return true;
}

bool OnXLab::PlotterRegion::setZLimits(double aMin,double aMax){
  if(!fSoRegion->isOfType(SoPlotterRegion::getClassTypeId())) return false;
  SoPlotter* soPlotter = ((SoPlotterRegion*)fSoRegion)->getPlotter();
  soPlotter->zAxisAutomated.setValue(FALSE);
  soPlotter->zAxisMinimum.setValue((float)aMin);
  soPlotter->zAxisMaximum.setValue((float)aMax);
  return true;
}

AIDA::IPlotterLayout& OnXLab::PlotterRegion::layout() { return fLayout;}
bool OnXLab::PlotterRegion::setLayout(const AIDA::IPlotterLayout&) {return false;}
AIDA::IInfo& OnXLab::PlotterRegion::info() { return fInfo;}
std::vector<std::string> OnXLab::PlotterRegion::availableParameterOptions(
 const std::string&) const { return std::vector<std::string>();}
std::vector<std::string> OnXLab::PlotterRegion::availableParameters() const {
  return std::vector<std::string>();
}

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
bool OnXLab::PlotterRegion::plot(
 const AIDA::IPlottable&
,const std::string&) {
  return false;
}
bool OnXLab::PlotterRegion::plot(
 const AIDA::IPlottable&
,const AIDA::IPlotterStyle&
,const std::string&) {
  return false;
}
bool OnXLab::PlotterRegion::remove(const AIDA::IPlottable&) { return false;}
bool OnXLab::PlotterRegion::refresh() { 
  fSoRegion->refresh();
  return true;
}
std::string OnXLab::PlotterRegion::title() const { return "";}
double OnXLab::PlotterRegion::xLimitMin() const { return 0;}
double OnXLab::PlotterRegion::xLimitMax() const { return 0;}
double OnXLab::PlotterRegion::yLimitMin() const { return 0;}
double OnXLab::PlotterRegion::yLimitMax() const { return 0;}
double OnXLab::PlotterRegion::zLimitMin() const { return 0;}
double OnXLab::PlotterRegion::zLimitMax() const { return 0;}
void* OnXLab::PlotterRegion::cast(const std::string&) const { return 0;}
#endif
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::Plotter::Plotter(
 std::ostream& a_out
,Slash::UI::IWidget& aWidget
,Slash::UI::IUI& aUI
,int aVerboseLevel
)
:f_out(a_out)
,fWidget(aWidget)
,fUI(aUI)
,fVerboseLevel(aVerboseLevel)
,fSoPage(0)
,fSetParameter(0)
,fTitleStyle(fSetParameter,"titleRegion.styles[0]")
,fRelationManager(0)
,fPlotterStyle(fSetParameter,"")
{
  Lib::Debug::increment("OnXLab::Plotter");
  fRelationManager = Slash::relationManager(aUI.session());
  if(!fRelationManager) {
    f_out << "OnXLab::Plotter::Plotter :"
          << " Lib::RelationManager not found.."
          << std::endl;
    return;
  }

  Slash::UI::ISoViewer* viewer = INLIB_CAST(fWidget,Slash::UI::ISoViewer);
  if(!viewer) {
    f_out << "OnXLab::Plotter::Plotter :"
          << " widget " << inlib::sout(fWidget.name()) 
          << " not a Slash::UI::ISoViewer." 
          << std::endl;
    return;
  }

  viewer->setBackgroundColor(1,1,1);

  SoNode* soNode = viewer->sceneGraph();
  if(!soNode) {
    f_out << "OnXLab::Plotter::Plotter :"
          << " Slash::UI::ISoViewer with null SoNode." 
          << std::endl;
    return;
  }
  if(!soNode->isOfType(SoPage::getClassTypeId())) {
    f_out << "OnXLab::Plotter::Plotter :"
          << " SoNode not a SoPage."
          << std::endl;
    return;
  }
  fSoPage = (SoPage*)soNode;    

  fSetParameter.setSoPage(fSoPage);

  SoRegion* soRegion = currentSoRegion();
  if(!soRegion) {
    f_out << "OnXLab::Plotter::Plotter :"
          << " no SoRegion available."
          << std::endl;
    return;
  }
  plotterRegion(soRegion); // Have at least one entry in fRegions.
}

OnXLab::Plotter::~Plotter(){
 {std::list<PlotterRegion*>::iterator it;
  for(it=fRegions.begin();it!=fRegions.end();it=fRegions.erase(it)) 
    delete (*it);}
  Lib::Debug::decrement("OnXLab::Plotter");
}

//private:
OnXLab::Plotter::Plotter(const OnXLab::Plotter& aFrom)
:f_out(aFrom.f_out)
,fWidget(aFrom.fWidget)
,fUI(aFrom.fUI)
,fVerboseLevel(aFrom.fVerboseLevel)
,fSoPage(0)
,fSetParameter(0)
,fTitleStyle(fSetParameter,"")
,fRelationManager(0)
,fPlotterStyle(fSetParameter,"")
{
  Lib::Debug::increment("OnXLab::Plotter");
}
OnXLab::Plotter& OnXLab::Plotter::operator=(const Plotter&){return *this;}

bool OnXLab::Plotter::isValid() const{
  if(!fSoPage) return false;
  if(fSoPage->getNumberOfRootRegions()<=0) return false;
  SoRegion* soRegion = currentSoRegion();
  if(!soRegion) return false;  
  return true;
}

void* OnXLab::Plotter::cast(const std::string& a_class) const{
  INLIB_IF_CAST(Slash::UI::IPlotter)
  else INLIB_IF_CAST(AIDA::IPlotter)
  else return 0;
}

AIDA::IPlotterRegion* OnXLab::Plotter::plotterRegion(
 SoRegion* aSoRegion //can be 0 ! See next()
) const{
  INLIB_SELF(OnXLab::Plotter);
  std::list<PlotterRegion*>::const_iterator it;
  for(it=self.fRegions.begin();it!=self.fRegions.end();++it) {
    if((*it)->soRegion()==aSoRegion) return (*it);
  }
  self.fRegions.push_back(new PlotterRegion
    (f_out,*fRelationManager,fVerboseLevel,aSoRegion,fUI,fWidget));
  return self.fRegions.back();
}

bool OnXLab::Plotter::createRegions(int aCols,int aRows,int aIndex){
  if(!fSoPage) return false;
  if((aCols<0)&&(aRows<0)) { //FIXME : beurk. Should have an AIDA method.
    SoRegion* soRegion = 
      fSoPage->createGridRegion("SoPlotterRegion",-aCols,-aRows,aIndex);
    if(!soRegion) return false;
    if(fSoPage->getNumberOfRootRegions()==1) 
      fSoPage->highlightRegion(soRegion);
    fSoPage->highlightRegion(soRegion);
  } else {
    fSoPage->createRegions("SoPlotterRegion",aCols,aRows,aIndex);
   {std::list<PlotterRegion*>::iterator it;
    for(it=fRegions.begin();it!=fRegions.end();it=fRegions.erase(it)) 
      delete (*it);}
    SoRegion* soRegion = currentSoRegion();
    if(!soRegion) return false;
    plotterRegion(soRegion); // Have an entry in fRegions.
  }
  return true;
}

AIDA::IPlotterRegion* OnXLab::Plotter::createRegion(
 double aX
,double aY
,double aWidth
,double aHeight                           
){
  if(!fSoPage) return 0;
  SoRegion* soRegion = fSoPage->createRegion
    ("SoPlotterRegion",(float)aX,(float)aY,(float)aWidth,(float)aHeight);
  if(!soRegion) return 0;
  if(fSoPage->getNumberOfRootRegions()==1) 
    fSoPage->highlightRegion(soRegion);
  return plotterRegion(soRegion);
}

void OnXLab::Plotter::clearRegions(){
  if(!fSoPage) return;
  fSoPage->clearRegions();
  // have to delete the auto_refresh cyclics.
  int n = fSoPage->getNumberOfRootRegions();
  for(int index=0;index<n;index++) {
    SoRegion* region = fSoPage->getRootRegion(index);
    remove_cyclic(region,fUI);
  }
}

bool OnXLab::Plotter::setParameter(
 const std::string& aParameter
,const std::string& aValue
){
  if(!fSoPage) return false;
  return (SoTools::setKitPart(fSoPage,aParameter.c_str(),aValue.c_str())==TRUE?true:false);
}

std::string OnXLab::Plotter::parameterValue(const std::string& aParameter) const{
  if(!fSoPage) return "";
  SbString value;
  SoTools::getKitPart((SoBaseKit*)fSoPage,aParameter.c_str(),value);
  return std::string(value.getString());
}

AIDA::IPlotterRegion& OnXLab::Plotter::next(){
  if(!fSoPage) { //FIXME : throw
    f_out << "OnXLab::Plotter::next :"
          << " no SoPage available !"
          << std::endl;

    return *plotterRegion(0);
  }

  SoRegion* highlighted = fSoPage->getHighlightedRegion();
  if(highlighted) {
    SoRegion* nextSoRegion = fSoPage->getNextRootRegion(highlighted);
    if(nextSoRegion) {
      fSoPage->highlightRegion(nextSoRegion);
    }
  }

  SoRegion* soRegion = currentSoRegion();
  if(!soRegion) {
    f_out << "OnXLab::Plotter::next :"
          << " no SoRegion available !"
          << std::endl;
  }
  return *plotterRegion(soRegion);
}

bool OnXLab::Plotter::setCurrentRegionNumber(int aIndex){
  if(!fSoPage) return false;
  SoRegion* soRegion = fSoPage->getRootRegion(aIndex);
  if(!soRegion) return false;
  fSoPage->highlightRegion(soRegion);
  return true;
}

AIDA::IPlotterRegion& OnXLab::Plotter::currentRegion() const{
  SoRegion* soRegion = currentSoRegion();
  if(!soRegion) {
    f_out << "OnXLab::Plotter::currentRegion :"
          << " no region available !"
          << std::endl;
  }
  return *plotterRegion(soRegion);
}

int OnXLab::Plotter::currentRegionNumber() const{
  if(!fSoPage) return NotFound;
  SoRegion* highlighted = fSoPage->getHighlightedRegion();
  if(!highlighted) return NotFound;
  return fSoPage->getRootRegionIndex(highlighted);
}

int OnXLab::Plotter::numberOfRegions() const{ 
  if(!fSoPage) return 0;
  return fSoPage->getNumberOfRootRegions();
}

bool OnXLab::Plotter::writeToFile(const std::string& aFile,const std::string& aFormat){  
  refresh();
  return fWidget.write(aFile,aFormat.empty()?"guessed":aFormat);
}

bool OnXLab::Plotter::show(){
  fWidget.show();
  fUI.synchronize();
  return true;
}

bool OnXLab::Plotter::hide(){
  fWidget.hide();
  fUI.synchronize();
  return true;
}

bool OnXLab::Plotter::refresh(){
  fUI.lock();
  if(fSoPage) fSoPage->refresh();
  fUI.synchronize();  
  //fViewer->render();
  fUI.unlock();
  return true;
}

bool OnXLab::Plotter::interact(){
  if(!fUI.steerLevel()) fUI.steer();
  return true;
}

void OnXLab::Plotter::setTitle(const std::string& aString){
  if(!fSoPage) return;
  // Assume a title on one line.
  fSoPage->title.set1Value(0,aString.c_str());
  if(aString.empty()) {
    fSoPage->setTitleVisible(FALSE);
  } else {
    fSoPage->setTitleVisible(TRUE);
  }
}

AIDA::ITitleStyle& OnXLab::Plotter::titleStyle(){return fTitleStyle;}

void OnXLab::Plotter::destroyRegions(){
  if(!fSoPage) return;
  fSoPage->deleteRegions();
}

void OnXLab::Plotter::setTitleStyle(const AIDA::ITitleStyle&) {}
std::vector<std::string> OnXLab::Plotter::availableParameterOptions(const std::string&) const { return std::vector<std::string>();}
std::vector<std::string> OnXLab::Plotter::availableParameters() const {
  return std::vector<std::string>();
}

AIDA::IPlotterRegion* OnXLab::Plotter::region(int aIndex) const{
  if(!fSoPage) return 0;
  SoRegion* soRegion = fSoPage->getRootRegion(aIndex);
  if(!soRegion) {
    f_out << "OnXLab::Plotter::region :"
          << " SoRegion " << aIndex << " not found."
          << std::endl;
    return 0;
  }
  return plotterRegion(soRegion);
}

SoRegion* OnXLab::Plotter::currentSoRegion() const{
  if(!fSoPage) return 0;
  SoRegion* highlighted = fSoPage->getHighlightedRegion();
  if(!highlighted) return 0;
  if(fSoPage->isRootRegion(highlighted)==FALSE) return 0;
  return highlighted;
}

//SoRegion* OnXLab::Plotter::getRootRegion(int aIndex) const{
//  if(!fSoPage) return 0;
//  return fSoPage->getRootRegion(aIndex);
//}

SoNode* OnXLab::Plotter::nodeOfInterest(const std::string& aWhat) const {
  //NOTE : similar to what is done in OnX/GuiViewer.h "interest".
  if(!fSoPage) return 0;
  if(aWhat=="page") return fSoPage;
  if(aWhat=="region") {
    SoRegion* soRegion = fSoPage->currentRegion();
    if(!soRegion) {
      SoDebugError::postInfo("OnXLab::Plotter::nodeOfInterest",
                             "Can't get region.");
      return 0;
    }
    return soRegion;
  } else {
    SoRegion* soRegion = fSoPage->currentRegion();
    if(!soRegion) {
      SoDebugError::postInfo("OnXLab::Plotter::nodeOfInterest",
                             "Can't get region.");
      return 0;
    }
    SoNode* node = soRegion->getNodeOfInterest(aWhat.c_str());
    if(!node) {
      SoDebugError::postInfo("OnXLab::Plotter::nodeOfInterest",
                             "Can't get interest node for \"%s\".",
                             aWhat.c_str());
      return 0;
    }
    return node;
  }
}

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
std::string OnXLab::Plotter::title() const { return "";}
AIDA::IPlotterStyle& OnXLab::Plotter::style() {
  return fPlotterStyle;
}
void OnXLab::Plotter::setStyle(const AIDA::IPlotterStyle&) {}
#endif
