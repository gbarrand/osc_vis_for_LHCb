//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/UI.h>
#include <inlib/cast>

// amanip :
#include <amanip/find.h>
#include <amanip/create.h>
#include <amanip/mo.h>

#include <Slash/Tools/Manager.h>

#include "inlib"

#include <inlib/args>

#define NotFound (-1)

namespace OnXLab {

class CloudExplorerManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::CloudExplorerManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  CloudExplorerManager(Slash::Core::ISession&)
  :fName("OnXLab_CloudExplorerManager"),fCloud(0),fRep(0){}
  virtual ~CloudExplorerManager(){
    delete fRep;
  }
  void setCloud(AIDA::ICloud* aCloud) { 
    fCloud = aCloud;
    delete fRep;
    fRep = 0;
  }
  AIDA::ICloud* cloud() const { return fCloud;}
  void setRep(AIDA::IBaseHistogram* aRep) {
    delete fRep;
    fRep = aRep;
  }
private:
  std::string fName;
  AIDA::ICloud* fCloud;
  AIDA::IBaseHistogram* fRep;
};
}

//////////////////////////////////////////////////////////////////////////////
static OnXLab::CloudExplorerManager* find_explorer(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return Slash_findManager
    (aUI.session(),"OnXLab_CloudExplorerManager",OnXLab::CloudExplorerManager);
}
//////////////////////////////////////////////////////////////////////////////
static AIDA::ICloud* getCloud(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnXLab::CloudExplorerManager* explorer = find_explorer(aUI);
  if(!explorer) return 0;
  return explorer->cloud();
}

#define CLOUD_HISTO(aCloud,aLower,aUpper,aValue) \
   {double xmin = 0;\
    double xmax = 0;\
    if(aXmax<=aXmin) { /* Guess the histogram booking parameters. */\
      xmin = aCloud->aLower();\
      xmax = aCloud->aUpper();\
      if(xmax==xmin) {\
        xmax = xmin + 1;\
      } else { /* Tilt xmax in order that max value is in the histo. */\
        xmax = xmax + (xmax-xmin) * 0.01;\
      }\
    } else {\
      xmin = aXmin;\
      xmax = aXmax;\
    }\
    /* WARNING : will be managed by the CloudExplorer. */\
    AIDA::IHistogram1D* histogram = \
      histogramFactory->createHistogram1D\
        (amanip::path_not_managed()+"/"+name,name,xnumber,xmin,xmax);\
    if(!histogram) {\
      delete histogramFactory;\
      return 0;\
    }\
    int n = aCloud->entries();\
    for(int index = 0;index<n;index++) {\
      histogram->fill(aCloud->aValue(index),aCloud->weight(index));\
    }\
    baseHistogram = histogram;}

//////////////////////////////////////////////////////////////////////////////
static AIDA::IBaseHistogram* represent_cloud(
 Slash::Core::ISession& aSession
,AIDA::ICloud& aCloud
,int aCoord
,int aXnumber
,double aXmin
,double aXmax
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aSession.cout();

  if(aCloud.dimension()<=0) {
    out << "represent_cloud :"
        << " cloud has fancy a dimension." << std::endl;
    return 0;
  }

  if( (aCoord<0) || (aCoord>=aCloud.dimension()) ) {
    out << "represent_cloud :"
        << " coordinate out of range ; given " << aCoord
        << " range [0," << aCloud.dimension() << "]." 
        << std::endl;
    return 0;
  }

  AIDA::IAnalysisFactory* aida = 
    Slash_findManager(aSession,Slash_AnalysisFactory,AIDA::IAnalysisFactory);
  if(!aida) return 0;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return 0;

  AIDA::IHistogramFactory* histogramFactory = 
    aida->createHistogramFactory(*memoryTree);
  if(!histogramFactory) return 0;

  AIDA::IBaseHistogram* baseHistogram = 0;

  std::string name = "tmp"; // it is ok for a not managed object.

  int xnumber = aXnumber<=0 ? 100 : aXnumber;

  if(AIDA::ICloud1D* cl1D = INLIB_CAST(aCloud,AIDA::ICloud1D)) {
   
    CLOUD_HISTO(cl1D,lowerEdge,upperEdge,value)

  } else if(AIDA::ICloud2D* cl2D = INLIB_CAST(aCloud,AIDA::ICloud2D)) {
   
    if(aCoord==0) {
      CLOUD_HISTO(cl2D,lowerEdgeX,upperEdgeX,valueX)
    } else if(aCoord==1) {
      CLOUD_HISTO(cl2D,lowerEdgeY,upperEdgeY,valueY)
    }

  } else if(AIDA::ICloud3D* cl3D = INLIB_CAST(aCloud,AIDA::ICloud3D)) {
   
    if(aCoord==0) {
      CLOUD_HISTO(cl3D,lowerEdgeX,upperEdgeX,valueX)
    } else if(aCoord==1) {
      CLOUD_HISTO(cl3D,lowerEdgeY,upperEdgeY,valueY)
    } else if(aCoord==2) {
      CLOUD_HISTO(cl3D,lowerEdgeZ,upperEdgeZ,valueZ)
    }

  }

  delete histogramFactory;

  return baseHistogram;
}
//////////////////////////////////////////////////////////////////////////////
static void cloud_histo_represent(
 Slash::UI::IUI& aUI
,AIDA::ICloud& aCloud
,int aCoord
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Find AIDA :
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
  if(!aida) return;
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
  if(!plotter) return;
  AIDA::IPlotterRegion& region = plotter->currentRegion();

  region.clear();

  if(aCloud.isConverted()) {

    if(AIDA::ICloud1D* cl1D = INLIB_CAST(aCloud,AIDA::ICloud1D)) {

      if(!region.plot(cl1D->histogram())) {
        std::ostream& out = aUI.session().cout();
        out << "cloud_histo_represent :"
            << " plotting of converted cloud failed." 
            << std::endl;
      }

    } else if(AIDA::ICloud2D* cl2D = INLIB_CAST(aCloud,AIDA::ICloud2D)) {

      if(!region.plot(cl2D->histogram())) {
        std::ostream& out = aUI.session().cout();
        out << "cloud_histo_represent :"
            << " plotting of converted cloud failed." 
            << std::endl;
      }

    } else if(AIDA::ICloud3D* cl3D = INLIB_CAST(aCloud,AIDA::ICloud3D)) {

      if(!region.plot(cl3D->histogram())) {
        std::ostream& out = aUI.session().cout();
        out << "cloud_histo_represent :"
            << " plotting of converted cloud failed." 
            << std::endl;
      }

    } else {
      std::ostream& out = aUI.session().cout();
      out << "cloud_histo_represent :"
          << " unknown kind of cloud." 
          << std::endl;
    }

  } else {

    OnXLab::CloudExplorerManager* explorer = find_explorer(aUI);
    if(explorer) {
      int nbin;
      if(Slash::parameter<int>(aUI,"OnXLab_CloudExplorer_xnumber.value",nbin)){
      double mn;
      if(Slash::parameter<double>(aUI,"OnXLab_CloudExplorer_xmin.value",mn)) {
      double mx;
      if(Slash::parameter<double>(aUI,"OnXLab_CloudExplorer_xmax.value",mx)) {
      AIDA::IBaseHistogram* rep = 
        represent_cloud(aUI.session(),aCloud,aCoord,nbin,mn,mx);
      if(rep) {
        explorer->setRep(rep);
        if(!region.plot(*rep)) {
          std::ostream& out = aUI.session().cout();
          out << "cloud_histo_represent :"
              << " plotting of not converted cloud failed." 
              << std::endl;
        }
      } else {
        std::ostream& out = aUI.session().cout();
        out << "cloud_histo_represent :"
            << " can't represent not converted cloud." 
            << std::endl;
      }

      }}}

    }

  }

  delete plotter;
}

extern "C" {

void OnXLab_CloudExplorer_list_represent(Slash::UI::IUI&);

//////////////////////////////////////////////////////////////////////////////
void OnXLab_CloudExplorer_update(Slash::UI::IUI& aUI) {

  OnXLab::CloudExplorerManager* explorer = find_explorer(aUI);
  if(!explorer) {
    explorer = new OnXLab::CloudExplorerManager(aUI.session());
    aUI.session().addManager(explorer);
  }

  std::string selection;
  std::string stree;
  if(!aUI.session().parameterValue("OnXLab.tree",stree)) return;
  if(!aUI.parameterValue(stree+".selection",selection)) return;  

  AIDA::ICloud* cloud = 0;
  AIDA::ITree* tree = 0;
  std::string mgr,path;
 {std::string basename;
  AIDA::IManagedObject* object = 
    find_aida_object(selection,aUI.session(),tree,mgr,path,basename);
  if(tree) {
    if(object) {
      cloud = INLIB_CAST(*object,AIDA::ICloud);
      if(!cloud) {
        aUI.echo(basename + " is not a cloud.");
      }
    } /*FIXME else {
      // May be a cloud in a Rio tree ?
      AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
      if(aida) {
        // Get the cloud from the rioTree :
        cloud = amanip::create_cloud(*aida,*tree,basename,"","");
        if(!cloud) {
          aUI.echo(basename + " not found.");
        }
      } 
    } */
  }}

  explorer->setCloud(cloud);
  if(cloud) {
    int n = cloud->dimension();
    std::string s;
    for(int index = 0;index<n;index++) {
      s += smanip_int2coord(index) + "\n";
    }
    aUI.setParameter("OnXLab_CloudExplorer_list.items",s);
    aUI.setParameter("OnXLab_CloudExplorer_tree.value",mgr);
    aUI.setParameter("OnXLab_CloudExplorer_cloud.value",path);
  } else {
    aUI.setParameter("OnXLab_CloudExplorer_list.items","");
    aUI.setParameter("OnXLab_CloudExplorer_tree.value","");
    aUI.setParameter("OnXLab_CloudExplorer_cloud.value","");
  }
  aUI.setParameter("OnXLab_CloudExplorer_xnumber.value","0");
  aUI.setParameter("OnXLab_CloudExplorer_xmin.value","0");
  aUI.setParameter("OnXLab_CloudExplorer_xmax.value","0");
  aUI.setParameter("OnXLab_CloudExplorer_xmin_scale.value","0");
  aUI.setParameter("OnXLab_CloudExplorer_xmax_scale.value","1000");

  OnXLab_CloudExplorer_list_represent(aUI);

}
//////////////////////////////////////////////////////////////////////////////
void OnXLab_CloudExplorer_infos(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string scoord;
  if(!aUI.parameterValue("OnXLab_CloudExplorer_list.value",scoord)) return;

  // Get the ICloud :
  AIDA::ICloud* cloud = getCloud(aUI);
  if(!cloud) return;

  std::ostream& out = aUI.session().cout();
  out << "/////////////////" << std::endl;
  std::string name = amanip::name<AIDA::ICloud>(*cloud);
  out << "name      : " << name << std::endl;
  out << "title     : " << cloud->title() << std::endl;
  out << "dimension : " << cloud->dimension() << std::endl;
  out << "entries   : " << cloud->entries() << std::endl;
  out << "converted : " << cloud->isConverted() << std::endl;

  if(AIDA::ICloud1D* cl1D = INLIB_CAST(*cloud,AIDA::ICloud1D)) {

    out << "min       : " << cl1D->lowerEdge() << std::endl;
    out << "max       : " << cl1D->upperEdge() << std::endl;
    out << "mean      : " << cl1D->mean() << std::endl;
    out << "rms       : " << cl1D->rms() << std::endl;

  } else if(AIDA::ICloud2D* cl2D = INLIB_CAST(*cloud,AIDA::ICloud2D)) {

    out << "x min     : " << cl2D->lowerEdgeX() << std::endl;
    out << "x max     : " << cl2D->upperEdgeX() << std::endl;
    out << "x mean    : " << cl2D->meanX() << std::endl;
    out << "x rms     : " << cl2D->rmsX() << std::endl;

    out << "y min     : " << cl2D->lowerEdgeY() << std::endl;
    out << "y max     : " << cl2D->upperEdgeY() << std::endl;
    out << "y mean    : " << cl2D->meanY() << std::endl;
    out << "y rms     : " << cl2D->rmsY() << std::endl;

  } else if(AIDA::ICloud3D* cl3D = INLIB_CAST(*cloud,AIDA::ICloud3D)) {

    out << "x min     : " << cl3D->lowerEdgeX() << std::endl;
    out << "x max     : " << cl3D->upperEdgeX() << std::endl;
    out << "x mean    : " << cl3D->meanX() << std::endl;
    out << "x rms     : " << cl3D->rmsX() << std::endl;

    out << "y min     : " << cl3D->lowerEdgeY() << std::endl;
    out << "y max     : " << cl3D->upperEdgeY() << std::endl;
    out << "y mean    : " << cl3D->meanY() << std::endl;
    out << "y rms     : " << cl3D->rmsY() << std::endl;

    out << "z min     : " << cl3D->lowerEdgeZ() << std::endl;
    out << "z max     : " << cl3D->upperEdgeZ() << std::endl;
    out << "z mean    : " << cl3D->meanZ() << std::endl;
    out << "z rms     : " << cl3D->rmsZ() << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab_CloudExplorer_list_represent(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Get the ICloud :
  AIDA::ICloud* cloud = getCloud(aUI);

  if(!cloud) return;

  std::string rep;
  if(!aUI.parameterValue("OnXLab_CloudExplorer_rep.value",rep)) return;

  if(rep=="points") {

    aUI.setParameter("OnXLab_CloudExplorer_xnumber.value","0");
    aUI.setParameter("OnXLab_CloudExplorer_xmin.value","0");
    aUI.setParameter("OnXLab_CloudExplorer_xmax.value","0");
    aUI.setParameter("OnXLab_CloudExplorer_xmin_scale.value","0");
    aUI.setParameter("OnXLab_CloudExplorer_xmax_scale.value","1000");

    // Find AIDA :
    AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
    if(!aida) {
      std::ostream& out = aUI.session().cout();
      out << "OnXLab_CloudExplorer_update :"
          << " can't find AIDA." 
          << std::endl;
      return;
    }
    AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
    if(!plotter) {
      std::ostream& out = aUI.session().cout();
      out << "OnXLab_CloudExplorer_update :"
          << " can't get an AIDA::IPlotter." 
          << std::endl;
      return;
    }
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.clear();
    if(!cloud->isConverted()) {
      if(!region.plot(*cloud)) {
        std::ostream& out = aUI.session().cout();
        out << "OnXLab_CloudExplorer_update :"
            << " plotting of not converted cloud failed." 
            << std::endl;
      }
    } else {
      std::ostream& out = aUI.session().cout();
      out << "OnXLab_CloudExplorer_update :"
          << " cloud is converted. No rep for the moment !" 
          << std::endl;
    }
    delete plotter;

  } else if(rep=="histo") {    

    int coord = 0; 
    if(cloud->isConverted()) {

      aUI.setParameter("OnXLab_CloudExplorer_xnumber.value","0");
      aUI.setParameter("OnXLab_CloudExplorer_xmin.value","0");
      aUI.setParameter("OnXLab_CloudExplorer_xmax.value","0");
      aUI.setParameter("OnXLab_CloudExplorer_xmin_scale.value","0");
      aUI.setParameter("OnXLab_CloudExplorer_xmax_scale.value","1000");

    } else {
      std::string s;
      if(!aUI.parameterValue("OnXLab_CloudExplorer_list.value",s)) return;
      if(!smanip_coord2int(s,coord)) return;

      double xmin = 0;
      double xmax = 0;

      if(AIDA::ICloud1D* cl1D = INLIB_CAST(*cloud,AIDA::ICloud1D)) {
        xmin = cl1D->lowerEdge();
        xmax = cl1D->upperEdge();
      } else if(AIDA::ICloud2D* cl2D = INLIB_CAST(*cloud,AIDA::ICloud2D)) {
        if(coord==0) {
          xmin = cl2D->lowerEdgeX();
          xmax = cl2D->upperEdgeX();
        } else if(coord==1) {
          xmin = cl2D->lowerEdgeY();
          xmax = cl2D->upperEdgeY();
        }
      } else if(AIDA::ICloud3D* cl3D = INLIB_CAST(*cloud,AIDA::ICloud3D)) {
        if(coord==0) {
          xmin = cl3D->lowerEdgeX();
          xmax = cl3D->upperEdgeX();
        } else if(coord==1) {
          xmin = cl3D->lowerEdgeY();
          xmax = cl3D->upperEdgeY();
        } else if(coord==2) {
          xmin = cl3D->lowerEdgeZ();
          xmax = cl3D->upperEdgeZ();
        }
      }

      Slash::set_parameter<double>(aUI,"OnXLab_CloudExplorer_xmin.value",xmin);
      Slash::set_parameter<double>(aUI,"OnXLab_CloudExplorer_xmax.value",xmax);

      // Save for the scale drag callback :
      Slash::Core::ISession& session = aUI.session();
      Slash::set_parameter<double>(session,"OnXLab_CloudExplorer_xmin",xmin);
      Slash::set_parameter<double>(session,"OnXLab_CloudExplorer_xmax",xmax);

      aUI.setParameter("OnXLab_CloudExplorer_xmin_scale.value","0");
      aUI.setParameter("OnXLab_CloudExplorer_xmax_scale.value","1000");

    }

    cloud_histo_represent(aUI,*cloud,coord);
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab_CloudExplorer_represent(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string rep;
  if(!aUI.parameterValue("OnXLab_CloudExplorer_rep.value",rep)) return;
  if(rep!="histo") return;
  AIDA::ICloud* cloud = getCloud(aUI);
  if(!cloud) return;
  std::string s;
  if(!aUI.parameterValue("OnXLab_CloudExplorer_list.value",s)) return;
  int i; 
  if(!smanip_coord2int(s,i)) return;
  cloud_histo_represent(aUI,*cloud,i);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLab_CloudExplorer_xmin_scale(Slash::UI::IUI& aUI) {
  std::ostream& out = aUI.session().cout();

  double value;
  if(!inlib::to<double>(out,aUI.callbackValue(),value)) return; //In [0,1000]

  Slash::Core::ISession& session = aUI.session();
  double xmin;
  if(!Slash::parameter<double>(session,"OnXLab_CloudExplorer_xmin",xmin))
    return;
  double xmax;
  if(!Slash::parameter<double>(session,"OnXLab_CloudExplorer_xmax",xmax))
    return;
  if(xmax<=xmin) return;

  double dx = xmax-xmin;
  dx *= value/1000.;
  double xmn = xmin + dx;
  Slash::set_parameter<double>(aUI,"OnXLab_CloudExplorer_xmin.value",xmn);

  OnXLab_CloudExplorer_represent(aUI);
}

void OnXLab_CloudExplorer_xmax_scale(Slash::UI::IUI& aUI) {
  std::ostream& out = aUI.session().cout();

  double value;
  if(!inlib::to<double>(out,aUI.callbackValue(),value)) return; //In [0,1000]

  Slash::Core::ISession& session = aUI.session();
  double xmin;
  if(!Slash::parameter<double>(session,"OnXLab_CloudExplorer_xmin",xmin))
    return;
  double xmax;
  if(!Slash::parameter<double>(session,"OnXLab_CloudExplorer_xmax",xmax))
    return;
  if(xmax<=xmin) return;

  double dx = xmax-xmin;
  dx *= value/1000.;
  double xmx = xmin + dx;
  Slash::set_parameter<double>(aUI,"OnXLab_CloudExplorer_xmax.value",xmx);

  OnXLab_CloudExplorer_represent(aUI);
}

} // extern "C"
