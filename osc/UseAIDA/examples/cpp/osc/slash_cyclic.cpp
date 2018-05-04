
#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/Tools/Managers.h>

#include <Slash/Tools/UI.h>

#include <inlib/random>

//NOTE : we prefix all global things with "slash_cyclic_"
//       in case the code is included in the osc-plot
//       callbacks examples.

class slash_cyclic_cbk : public virtual Slash::UI::ICyclicCallback {
public:
  virtual void execute() {
    inlib::random::gauss rg(0,1);

    // change h1d :
    AIDA::IHistogram1D* h1d = m_h1d;
    if(h1d) {
      // the below fill will induce some 
      //   BatchLab::BaseObject::emitUpdate
      // and then some SoPlotter.touch()
      for(int index=0;index<1000;index++) h1d->fill(rg.shoot(),1);
    }

    // change the function :
   {AIDA::IFunction* f = m_f;
    if(f) {
      std::vector<double> params;
      params.push_back(h1d->binHeight(50));
      params.push_back(0);
      params.push_back(h1d->rms());
      //params.push_back(flat.shoot()*2);
      // the below setParameters will induce some 
      //   BatchLab::BaseObject::emitUpdate
      // and then some SoPlotter updateChildren and viewer update.
      f->setParameters(params);
    }}

    // change h2d :
   {AIDA::IHistogram2D* h2d = m_h2d;
    if(h2d) {
      // the below fill will induce some 
      //   BatchLab::BaseObject::emitUpdate
      // and then some SoPlotter.touch()
      for(int index=0;index<1000;index++) h2d->fill(rg.shoot(),rg.shoot(),1);
    }}
  }
public:
  slash_cyclic_cbk(AIDA::IHistogram1D* a_h1d,AIDA::IHistogram2D* a_h2d,AIDA::IFunction* a_f):m_h1d(a_h1d),m_h2d(a_h2d),m_f(a_f){}
  virtual ~slash_cyclic_cbk(){}
public:
  AIDA::IHistogram1D* m_h1d;
  AIDA::IHistogram2D* m_h2d;
  AIDA::IFunction* m_f;
};

#define slash_cyclic_name "slash_cyclic"

static void slash_cyclic_stop(Slash::UI::IUI& aUI) {
  Slash::UI::ICyclic* cyclic = aUI.findCyclic(slash_cyclic_name);
  if(!cyclic) return;
  cyclic->stop();
  aUI.removeCyclic(cyclic);
}

#define name_h1d "slash_cyclic_h1d"
#define name_h2d "slash_cyclic_h1d"
#define name_f   "slash_cyclic_f"

static void slash_cyclic(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {

  AIDA::ITreeFactory* trf = a_aida.createTreeFactory();
  if(!trf) return;
  AIDA::ITree* tree = trf->create();
  delete trf;
  if(!tree) return;

  if(tree->find(name_h1d)) tree->rm(name_h1d);
  if(tree->find(name_h2d)) tree->rm(name_h2d);
  if(tree->find(name_f)) tree->rm(name_f);

  AIDA::IHistogramFactory* hf = a_aida.createHistogramFactory(*tree);
  if(!hf) return;
  AIDA::IHistogram1D* h1d = hf->createHistogram1D(name_h1d,name_h1d,50,-3,3);
  if(!h1d) return;
  AIDA::IHistogram2D* h2d = hf->createHistogram2D(name_h2d,name_h2d,50,-3,3,50,-3,3);
  if(!h2d) return;
  delete hf;
      
  AIDA::IFunctionFactory* ff = a_aida.createFunctionFactory(*tree); 
  if(!ff) return;
  AIDA::IFunction* f = ff->createFunctionByName(name_f,"G");  
  delete ff;
  if(!f) return;
  f->setTitle(name_f);
  std::vector<double> params;
  params.push_back(400);
  params.push_back(0);
  params.push_back(1);
  f->setParameters(params);

  AIDA::IPlotterFactory* pf = a_aida.createPlotterFactory(0,0);
  if(pf) {
  AIDA::IPlotter* plotter = pf->create(a_plotter);
  delete pf;
  if(plotter) {
    plotter->setTitle("monitor");
    plotter->createRegions(1,2,0);
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("infosRegionVisible","TRUE");
    region.plot(*h1d);
    region.plot(*f);}
   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("infosRegionVisible","TRUE");
    h2d->annotation().addItem("plotter.modeling","solid");
    h2d->annotation().addItem("plotter.painting","violet_to_red");
    region.plot(*h2d);}

    // fill histos with a cyclic :   
    Slash::UI::IUI* ui = Slash::find_UI(a_session);     
    if(!ui) {
      a_out << "Slash::UI::IUI not found." << std::endl;
    } else {
      slash_cyclic_stop(*ui);
      Slash::UI::ICyclic* cyclic = ui->createCyclic(slash_cyclic_name,1,"","");
      if(cyclic) {
        cyclic->setDelay(1000);
        cyclic->setCallback(new slash_cyclic_cbk(h1d,h2d,f));
        //a_out << "Start cyclic..." << std::endl;
        cyclic->start();
      }
    }

    if(a_ui) {
      plotter->show();
      plotter->interact();

      slash_cyclic_stop(*ui);
    }

    delete plotter;
  }}
}

#undef name_h1d
#undef name_h2d
#undef name_f

//#include <Slash/AIDA/IAnalysisFactory.h>
//#define Cast(aObject,aClass) (aClass*)(aObject).cast(#aClass)

int main(int argc,char* argv[]) {

  //Slash::AIDA::IAnalysisFactory* aida = Slash_AIDA_createAnalysisFactory();
  //if(!aida) return EXIT_FAILURE;
  //Slash::Core::ISession* session = Cast(*aida,Slash::Core::ISession);
  //if(!session) {
  //  std::cout << "AIDA::IAnalysisFactory not a Slash::Core::ISession." 
  //            << std::endl;
  //  return EXIT_FAILURE;
  //}

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_slash_cyclic :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "AIDA::IAnalysisFactory not a Slash::Core::ISession." 
          << std::endl;
    return 1;
  }

  slash_cyclic(*session,*aida,std::cout,"",true);
  delete aida;
  return 0;
}
