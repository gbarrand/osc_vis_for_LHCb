
#include <AIDA/AIDA.h>

#include <inlib/random>
#include <inlib/sys/sleep>
#include <inlib/sys/thread>

//NOTE : we prefix all global things with "thread_"
//       in case the code is included in the osc-plot
//       callbacks examples.

class thread_and_auto_refresh_args {
public:
  thread_and_auto_refresh_args(AIDA::IHistogram1D& a_h1d,
       AIDA::IHistogram2D& a_h2d,
       AIDA::IPlotter* a_plotter)
  :f_h1d(a_h1d),f_h2d(a_h2d),f_plotter(a_plotter),f_continue(true),f_exit(false){}
public:
  AIDA::IHistogram1D& f_h1d;
  AIDA::IHistogram2D& f_h2d;
  AIDA::IPlotter* f_plotter;
  bool f_continue;
  bool f_exit;
};

static void* thread_and_auto_refresh_main(void* aTag) {
  thread_and_auto_refresh_args* args = (thread_and_auto_refresh_args*)aTag;

  inlib::random::gauss r(0,1);

  //unsigned int count = 0;
  while(args->f_continue) {
    //std::cout << "debug : " << count << std::endl;count++;

    for (int i=0; i<10000; i++) {
      args->f_h1d.fill(r.shoot());
      args->f_h2d.fill(r.shoot(),r.shoot());
    }

    inlib::sleep_milli_secs(1000);
  }


  args->f_exit = true;

  //std::cout << "thread_and_auto_refresh_main end" << std::endl;

  return 0;
}

static void thread_and_auto_refresh_ask_stop(thread_and_auto_refresh_args* a_args){
  if(a_args->f_plotter) {

   {AIDA::IPlotterRegion& region = a_args->f_plotter->currentRegion();
    region.setParameter("auto_refresh","0");}
   {AIDA::IPlotterRegion& region = a_args->f_plotter->next();
    region.setParameter("auto_refresh","0");}

    delete a_args->f_plotter;
    a_args->f_plotter = 0;
  }
  a_args->f_continue = false;
}

static void thread_and_auto_refresh_wait_stop(thread_and_auto_refresh_args* a_args) {
  while(!a_args->f_exit){inlib::sleep_milli_secs(100);}
}

// below two statics used when called from osc-plot callback exa :
static thread_and_auto_refresh_args* s_thread_and_auto_refresh_args = 0;
static pthread_t* s_thread_and_auto_refresh_thread = 0;

#define name_h1d "thread_and_auto_refresh_h1d"
#define name_h2d "thread_and_auto_refresh_h1d"

static void thread_and_auto_refresh(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITreeFactory* trf = a_aida.createTreeFactory();
  if(!trf) return;
  AIDA::ITree* tree = trf->create();
  delete trf;
  if(!tree) return;

  if(tree->find(name_h1d)) tree->rm(name_h1d);
  if(tree->find(name_h2d)) tree->rm(name_h2d);

  AIDA::IHistogramFactory* hf = a_aida.createHistogramFactory(*tree);
  if(!hf) return;
  std::string opts("thread_safe=true");
  AIDA::IHistogram1D* h1d = hf->createHistogram1D(name_h1d,"h1d",50,-3,3,opts);
  AIDA::IHistogram2D* h2d = hf->createHistogram2D(name_h2d,"h2d",50,-3,3,50,-3,3,opts);
  delete hf;
  if(!h2d) return;
      
 {AIDA::IPlotterFactory* pf = a_aida.createPlotterFactory();
  if(pf) {
  AIDA::IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,2,0);
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("infosRegionVisible","TRUE");
    h1d->annotation().addItem("emit","none");
    region.plot(*h1d,"auto_refresh=1000");
    }//end region.
   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("infosRegionVisible","TRUE");
    region.plot(*h2d,"emit=none;auto_refresh=1000");
    }//end region.

/*FIXME : reexecution.
    if(!a_ui) {
      //in case we reexecute (from osc-plot exas).
      if(s_thread_args) {
        s_thread_args->f_h1d = h1d;
        s_thread_args->f_h2d = h2d;
        s_thread_args->f_f = f;
      }
    }
*/

    thread_and_auto_refresh_args* args = 
      new thread_and_auto_refresh_args(*h1d,*h2d,plotter);
    pthread_t* thread = new pthread_t;
    pthread_create(thread,0,thread_and_auto_refresh_main,(void*)args);

    if(a_ui) {
      plotter->show();
      plotter->interact();

      thread_and_auto_refresh_ask_stop(args);
      thread_and_auto_refresh_wait_stop(args);

      delete args;
      delete thread;

      a_out << "exit..." << std::endl;
    } else {
      // called from osc-plot callback exa :
      s_thread_and_auto_refresh_args = args;
      s_thread_and_auto_refresh_thread = thread;
    }

  }}}

}

#undef name_h1d
#undef name_h2d

int main(int argc,char* argv[]) {

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_thread_and_auto_refresh :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  thread_and_auto_refresh(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
