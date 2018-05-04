
#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

#include <inlib/random>
#include <inlib/sys/sleep>
#include <inlib/sys/thread>

//NOTE : we prefix all global things with "slash_thread_cyclic_"
//       in case the code is included in the osc-plot
//       callbacks examples.

class slash_thread_cyclic_fill_args {
public:
  slash_thread_cyclic_fill_args(AIDA::IHistogram1D* a_h1d,AIDA::IHistogram2D* a_h2d,AIDA::IFunction* a_f)
  :m_h1d(a_h1d),m_h2d(a_h2d),m_f(a_f),m_continue(true),m_exit(0){}
public:
  AIDA::IHistogram1D* m_h1d;
  AIDA::IHistogram2D* m_h2d;
  AIDA::IFunction* m_f;
  bool m_continue;
  bool m_exit;
};

static void slash_thread_cyclic_fill(void* aTag) {
  slash_thread_cyclic_fill_args* args = 
    (slash_thread_cyclic_fill_args*)aTag;

  inlib::random::gauss rg(0,1);

  // change h1d :
  AIDA::IHistogram1D* h1d = args->m_h1d;
  if(h1d) {
    // the below fill will induce some 
    //   BatchLab::BaseObject::emitUpdate
    // and then some SoPlotter.touch()
    for(int index=0;index<1000;index++) h1d->fill(rg.shoot(),1);
  }

  // change the function :
 {AIDA::IFunction* f = args->m_f;
  if(f) {
    std::vector<double> params;
    int middle = h1d->axis().bins()/2;
    params.push_back(h1d->binHeight(middle));
    params.push_back(0);
    params.push_back(h1d->rms());
    // the below setParameters will induce some 
    //   BatchLab::BaseObject::emitUpdate
    // and then some SoPlotter updateChildren and viewer update.
    f->setParameters(params);
  }}

  // change h2d :
 {AIDA::IHistogram2D* h2d = args->m_h2d;
  if(h2d) {
    // the below fill will induce some 
    //   BatchLab::BaseObject::emitUpdate
    // and then some SoPlotter.touch()
    for(int index=0;index<1000;index++) h2d->fill(rg.shoot(),rg.shoot(),1);
  }}

}

static void* slash_thread_cyclic_thread_main(void* aTag) {
  slash_thread_cyclic_fill_args* args = 
    (slash_thread_cyclic_fill_args*)aTag;
  //unsigned int count = 0;
  while(args->m_continue) {
    //::printf("debug : %d\n",count);count++;
    slash_thread_cyclic_fill(aTag);    
    inlib::sleep_milli_secs(500);
  }
  args->m_exit = true;
  //std::cout << "thread_main end." << std::endl;
  return 0;
}

class slash_thread_cyclic_refresh_cbk : public virtual Slash::UI::ICyclicCallback {
public:
  virtual void execute() {m_plotter->refresh();}
public:
  slash_thread_cyclic_refresh_cbk(AIDA::IPlotter* a_plotter):m_plotter(a_plotter){}
  virtual ~slash_thread_cyclic_refresh_cbk() {delete m_plotter;}
public:
  AIDA::IPlotter* m_plotter;
};

static void slash_thread_cyclic_ask_stop(slash_thread_cyclic_fill_args* a_args) {
  a_args->m_continue = false;
}

static void slash_thread_cyclic_wait_stop(slash_thread_cyclic_fill_args* a_args) {
  while(!a_args->m_exit){inlib::sleep_milli_secs(100);}
}

static void slash_thread_cyclic_stop(Slash::UI::IUI& aUI){
  Slash::UI::ICyclic* cyclic = aUI.findCyclic("slash_thread_cyclic");
  if(!cyclic) return;
  cyclic->stop();
  aUI.removeCyclic(cyclic);
}

// below two statics used when called from osc-plot callback exa :
static slash_thread_cyclic_fill_args* s_slash_thread_cyclic_fill_args = 0;
static pthread_t* s_slash_thread_cyclic_thread = 0;

#define name_h1d "slash_thread_cyclic_h1d"
#define name_h2d "slash_thread_cyclic_h1d"
#define name_f   "slash_thread_cyclic_f"

static void slash_thread_cyclic(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
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
  std::string opts("thread_safe=true");
  AIDA::IHistogram1D* h1d = 
    hf->createHistogram1D(name_h1d,name_h1d,50,-3,3,opts);
  if(!h1d) return;
  AIDA::IHistogram2D* h2d = 
    hf->createHistogram2D(name_h2d,name_h2d,50,-3,3,50,-3,3,opts);
  if(!h2d) return;
  delete hf;
      
  AIDA::IFunctionFactory* ff = a_aida.createFunctionFactory(*tree); 
  if(!ff) return;
  //AIDA::IFunction* f = ff->createFunctionByName(name_f,"thread_safe:G");  
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
  AIDA::IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->setTitle("monitor");
    plotter->createRegions(1,2,0);
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("infosRegionVisible","TRUE");
    region.plot(*h1d,"emit=none"); // h1d.fill() will not do some graphics.
    region.plot(*f,"emit=none");}
   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("infosRegionVisible","TRUE");
    h2d->annotation().addItem("plotter.modeling","solid");
    h2d->annotation().addItem("plotter.painting","violet_to_red");
    region.plot(*h2d,"emit=none");}

    //FIXME : stop thread and cyclic in case we reexecute from osc-plot exas.

    // fill histos in a thread :
    slash_thread_cyclic_fill_args* args =
      new slash_thread_cyclic_fill_args(h1d,h2d,f);
    pthread_t* thread = new pthread_t;
    pthread_create(thread,0,slash_thread_cyclic_thread_main,(void*)args);

    // refresh the plotter by using a cyclic :   
    Slash::UI::IUI* ui = Slash::find_UI(a_session);     
    if(!ui) {
      std::cout << "Slash::UI::IUI not found." 
                << std::endl;
    } else {
      slash_thread_cyclic_stop(*ui);
      Slash::UI::ICyclic* cyclic = 
        ui->createCyclic("slash_thread_cyclic",1,"","");
      if(cyclic) {
        //std::cout << "Start cyclic..." << std::endl;
        cyclic->setDelay(2000); //do not put the GUI on knee.
        cyclic->setCallback(new slash_thread_cyclic_refresh_cbk(plotter));
        cyclic->start();
      }
    }

    if(a_ui) {
      plotter->show();
      plotter->interact();

      slash_thread_cyclic_stop(*ui);

      slash_thread_cyclic_ask_stop(args);
      slash_thread_cyclic_wait_stop(args);

      delete args;
      delete thread;

      std::cout << "exit..." << std::endl;

    } else {
      // called from osc-plot callback exa :
      s_slash_thread_cyclic_fill_args = args;
      s_slash_thread_cyclic_thread = thread;
    } 
  }}
}

#undef name_h1d
#undef name_h2d
#undef name_f


//#include <Slash/AIDA/IAnalysisFactory.h>
//#define Cast(aObject,aClass) (aClass*)(aObject).cast(#aClass)

int main(int argc,char* argv[]) {

  //Slash::AIDA::IAnalysisFactory* aida = Slash_AIDA_createAnalysisFactory();
  //if(!aida) return;
  //Slash::Core::ISession* session = Cast(*aida,Slash::Core::ISession);
  //if(!session) {
  //  std::cout << "AIDA::IAnalysisFactory not a Slash::Core::ISession." 
  //            << std::endl;
  //  return 1;
  //}

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_slash_thread_cyclic :" 
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

  slash_thread_cyclic(*session,*aida,std::cout,"",true);
  delete aida;
  return 0;
}
