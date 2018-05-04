
//FIXME : Qt : still crashy.

#include <AIDA/AIDA.h>

#include <inlib/random>
#include <inlib/sys/sleep>
#include <inlib/sys/thread>

//NOTE : we prefix all global things with "thread_"
//       in case the code is included in the osc-plot
//       callbacks examples.

class pure_thread_args {
public:
  pure_thread_args(AIDA::IHistogram1D& a_h1d,
                   AIDA::IHistogram2D& a_h2d,
                   AIDA::IPlotter* a_plotter)
  :f_h1d(a_h1d),f_h2d(a_h2d)
  ,f_plotter(a_plotter),f_continue(true),f_exit(false){}
public:
  AIDA::IHistogram1D& f_h1d;
  AIDA::IHistogram2D& f_h2d;
  AIDA::IPlotter* f_plotter;
  bool f_continue;
  bool f_exit;
};

static void* pure_thread_main(void* aTag) {
  pure_thread_args* args = (pure_thread_args*)aTag;

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

  //std::cout << "pure_thread_main end" << std::endl;

  return 0;
}

static void pure_thread_ask_stop(pure_thread_args* a_args){
  if(a_args->f_plotter) {
    delete a_args->f_plotter;
    a_args->f_plotter = 0;
  }
  a_args->f_continue = false;
}

static void pure_thread_wait_stop(pure_thread_args* a_args) {
  while(!a_args->f_exit){inlib::sleep_milli_secs(100);}
}

// below two statics used when called from osc-plot callback exa :
static pure_thread_args* s_pure_thread_args = 0;
static pthread_t* s_pure_thread_thread = 0;

#define name_h1d "pure_thread_h1d"
#define name_h2d "pure_thread_h2d"

static void pure_thread(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
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
  if(!h1d) return;
  if(!h2d) return;
      
 {int argc = 1;
  static char* argv[] = {(char*)"-thread"};
  AIDA::IPlotterFactory* pf = a_aida.createPlotterFactory(argc,argv);
  if(pf) {
  AIDA::IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,2,0);
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("infosRegionVisible","TRUE");
    region.plot(*h1d);
    }//end region.
   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("infosRegionVisible","TRUE");
    region.plot(*h2d);
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

    pure_thread_args* args = new pure_thread_args(*h1d,*h2d,plotter);

    pthread_t* thread = new pthread_t;
    pthread_create(thread,0,pure_thread_main,(void*)args);

    if(a_ui) {
      plotter->show();
      plotter->interact();

      pure_thread_ask_stop(args);
      pure_thread_wait_stop(args);

      delete args;
      delete thread;

      a_out << "exit..." << std::endl;
    } else {
      // called from osc-plot callback exa :
      s_pure_thread_args = args;
      s_pure_thread_thread = thread;
    }

  }}}

}

#undef name_h1d
#undef name_h2d

int main(int argc,char* argv[]) {

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_pure_thread :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  pure_thread(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
