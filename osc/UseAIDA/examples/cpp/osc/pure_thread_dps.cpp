
#include <AIDA/AIDA.h>

#include <inlib/random>
#include <inlib/sys/sleep>
#include <inlib/sys/thread>

//NOTE : we prefix all global things with "thread_"
//       in case the code is included in the osc-plot
//       callbacks examples.

class pure_thread_dps_args {
public:
  pure_thread_dps_args(AIDA::IDataPointSet& a_dps1d,
                   AIDA::IDataPointSet& a_dps2d,
                   AIDA::IPlotter* a_plotter)
  :f_dps1d(a_dps1d),f_dps2d(a_dps2d)
  ,f_plotter(a_plotter)
  ,f_continue(true),f_exit(false){}
public:
  AIDA::IDataPointSet& f_dps1d;
  AIDA::IDataPointSet& f_dps2d;
  AIDA::IPlotter* f_plotter;
  bool f_continue;
  bool f_exit;
};

static void* pure_thread_dps_main(void* aTag) {
  pure_thread_dps_args* args = (pure_thread_dps_args*)aTag;

  inlib::random::gauss r(0,1);
  inlib::random::bw bw(0,1);

  //unsigned int count = 0;
  while(args->f_continue) {
    //std::cout << "debug : " << count << std::endl;count++;

    for ( int i = 0; i<10; i++ ) {
      double x = r.shoot();
      double y = bw.shoot();

     {AIDA::IDataPoint* pt = args->f_dps1d.addPoint();
      if(pt) {
        pt->coordinate(0)->setValue(x);
        pt->coordinate(0)->setErrorPlus(x*0.1);
      }}

     {AIDA::IDataPoint* pt = args->f_dps2d.addPoint();
      if(pt) {
        pt->coordinate(0)->setValue(x);
        pt->coordinate(0)->setErrorPlus(x*0.1);

        pt->coordinate(1)->setValue(y);
        pt->coordinate(1)->setErrorPlus(y*0.2);
        pt->coordinate(1)->setErrorMinus(y*0.1);
      }}
    }

    inlib::sleep_milli_secs(1000);
  }


  args->f_exit = true;

  //std::cout << "pure_thread_dps_main end" << std::cout;

  return 0;
}

static void pure_thread_dps_ask_stop(pure_thread_dps_args* a_args){
  if(a_args->f_plotter) {
    delete a_args->f_plotter;
    a_args->f_plotter = 0;
  }
  a_args->f_continue = false;
}

static void pure_thread_dps_wait_stop(pure_thread_dps_args* a_args) {
  while(!a_args->f_exit){inlib::sleep_milli_secs(100);}
}

// below two statics used when called from osc-plot callback exa :
static pure_thread_dps_args* s_pure_thread_dps_args = 0;
static pthread_t* s_pure_thread_dps_thread = 0;

#define name_dps1d "pure_thread_dps1d"
#define name_dps2d "pure_thread_dps2d"

static void pure_thread_dps(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITreeFactory* trf = a_aida.createTreeFactory();
  if(!trf) return;
  AIDA::ITree* tree = trf->create();
  delete trf;
  if(!tree) return;

  if(tree->find(name_dps1d)) tree->rm(name_dps1d);
  if(tree->find(name_dps2d)) tree->rm(name_dps2d);

  AIDA::IDataPointSetFactory* dpsf = a_aida.createDataPointSetFactory(*tree);
  if(!dpsf) return;

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  //std::string opts("thread_safe=true");
  //NOTE : has we can't pass a string option, it is
  //enforced "thread_safe=true" in the implementation.
  AIDA::IDataPointSet* dps1d = dpsf->create(name_dps1d,"dps1d",1);
  AIDA::IDataPointSet* dps2d = dpsf->create(name_dps2d,"dps2d",2);
#else
  std::string opts("thread_safe=true"); //FIXME : how to pass that ?
  AIDA::IDataPointSet* dps1d = dpsf->create(name_dps1d,"dps1d",1.opts);
  AIDA::IDataPointSet* dps2d = dpsf->create(name_dps2d,"dps2d",2,opts);
#endif
  delete dpsf;
  if(!dps1d) return;
  if(!dps2d) return;
      
 {int argc = 1;
  static char* argv[] = {(char*)"-thread"};
  AIDA::IPlotterFactory* pf = a_aida.createPlotterFactory(argc,argv);
  if(pf) {
  AIDA::IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,2,0);
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
  //region.setParameter("infosRegionVisible","TRUE");
    region.plot(*dps1d);
    }//end region.

   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("plotter.gridStyle.visible","FALSE");

    region.setParameter("plotter.binsStyle[0].visible","TRUE");
    region.setParameter("plotter.binsStyle[0].color","0 0 1");
    region.setParameter("plotter.binsStyle[0].markerStyle","ASTERISK");
    region.setParameter("plotter.binsStyle[0].modeling","points");

    region.setParameter("plotter.errorsStyle[0].visible","TRUE");
    region.setParameter("plotter.errorsStyle[0].markerStyle","PLUS");
    region.setParameter("plotter.errorsStyle[0].color","1 0 0");

    region.plot(*dps2d);
    }//end region.

/*FIXME : reexecution.
    if(!a_ui) {
      //in case we reexecute (from osc-plot exas).
      if(s_thread_args) {
        s_thread_args->f_dps1d = dps1d;
        s_thread_args->f_dps2d = dps2d;
        s_thread_args->f_f = f;
      }
    }
*/

    pure_thread_dps_args* args = 
      new pure_thread_dps_args(*dps1d,*dps2d,plotter);

    pthread_t* thread = new pthread_t;
    pthread_create(thread,0,pure_thread_dps_main,(void*)args);

    if(a_ui) {
      plotter->show();
      plotter->interact();

      pure_thread_dps_ask_stop(args);
      pure_thread_dps_wait_stop(args);

      delete args;
      delete thread;

      a_out << "exit..." << std::endl;
    } else {
      // called from osc-plot callback exa :
      s_pure_thread_dps_args = args;
      s_pure_thread_dps_thread = thread;
    }

  }}}

}

#undef name_dps1d
#undef name_dps2d

int main(int argc,char* argv[]) {

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_pure_thread_dps :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  pure_thread_dps(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
