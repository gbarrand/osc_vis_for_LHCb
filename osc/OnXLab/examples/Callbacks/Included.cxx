// 
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//
// This callback file contains examples over AIDA only.
//


// AIDA :
#include <AIDA/AIDA.h>

// Slash :
#include <Slash/UI/IUI.h>
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/UI.h>

// amanip :
#include <amanip/find.h>
#include <amanip/create.h>

#include <inlib/random>
#include <inlib/args>

//////////////////////////////////////////////////////////////////////////////
/// Automatic inclusion of UseAIDA/examples/cpp/osc examples /////////////////
//////////////////////////////////////////////////////////////////////////////

#define AIDA_EXA(a_name) \
extern "C" {\
void OnXLabExas_aida_exa_##a_name(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;\
  Slash::Core::ISession& session = aUI.session();\
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);\
  if(!aida) {\
    std::ostream& out = session.cout();\
    out << "OnXLabExas_aida_exa_" << #a_name << " :" \
        << " aida not found." \
        << std::endl;\
    return;\
  }\
  a_name(*aida,session.cout(),aArgs[0],false);\
}}

// UseAIDA/examples/cpp/osc :
extern "C" {
  AIDA::IAnalysisFactory* AIDA_createAnalysisFactory_dummy(){return 0;}
}
#define AIDA_createAnalysisFactory AIDA_createAnalysisFactory_dummy

#define main banner_main_dummy
#include <banner.cpp>
AIDA_EXA(banner)
#undef main

#define main rio_main_dummy
#include <rio.cpp>
AIDA_EXA(rio)
#undef main

#define main fitting_main_dummy
#include <fitting.cpp>
AIDA_EXA(fitting)
#undef main

#define main hopera_main_dummy
#include <hopera.cpp>
AIDA_EXA(hopera)
#undef main

#define main root_th_main_dummy
#include <root_th.cpp>
AIDA_EXA(root_th)
#undef main

#define main func_2D_main_dummy
#include <func_2D.cpp>
AIDA_EXA(func_2D)
#undef main

#define main func_2D_curve_main_dummy
#include <func_2D_curve.cpp>
AIDA_EXA(func_2D_curve)
#undef main

#define main complex_and_func_main_dummy
#include <complex_and_func.cpp>
AIDA_EXA(complex_and_func)
#undef main

#define main ascii_tuple_main_dummy
#include <ascii_tuple.cpp>
AIDA_EXA(ascii_tuple)
#undef main

#define main animated_fit_main_dummy
#include <animated_fit.cpp>
AIDA_EXA(animated_fit)
#undef main

#define main animated_fit_2_main_dummy
#include <animated_fit_2.cpp>
AIDA_EXA(animated_fit_2)
#undef main

#define main xml_main_dummy
#include <xml.cpp>
AIDA_EXA(xml)
#undef main

#define main profile_main_dummy
#include <profile.cpp>
AIDA_EXA(profile)
#undef main

#define main cloud_slice_main_dummy
#include <cloud_slice.cpp>
AIDA_EXA(cloud_slice)
#undef main

#define main cloud_2D_main_dummy
#include <cloud_2D.cpp>
AIDA_EXA(cloud_2D)
#undef main

#define main lego_main_dummy
#include <lego.cpp>
AIDA_EXA(lego)
#undef main

#define main func_cut_main_dummy
#include <func_cut.cpp>
AIDA_EXA(func_cut)
#undef main

#define main on_the_fly_func_main_dummy
#include <on_the_fly_func.cpp>
AIDA_EXA(on_the_fly_func)
#undef main

#define main data_point_set_main_dummy
#include <data_point_set.cpp>
AIDA_EXA(data_point_set)
#undef main

#define main mandel_main_dummy
#include <mandel.cpp>
AIDA_EXA(mandel)
#undef main

#define main hippo_main_dummy
#include <hippo.cpp>
AIDA_EXA(hippo)
#undef main

#define main mag_main_dummy
#include <mag.cpp>
AIDA_EXA(mag)
#undef main

#define main func_to_cloud_main_dummy
#include <func_to_cloud.cpp>
AIDA_EXA(func_to_cloud)
#undef main

#define main reopen_main_dummy
#include <reopen.cpp>
AIDA_EXA(reopen)
#undef main

#define main pure_thread_main_dummy
#include <pure_thread.cpp>
AIDA_EXA(pure_thread)
#undef main

#define main pure_thread_dps_main_dummy
#include <pure_thread_dps.cpp>
AIDA_EXA(pure_thread_dps)
#undef main

extern "C" {
void aida_exa_pure_thread_stop(Slash::UI::IUI& aUI){
  if(s_pure_thread_args) {
    pure_thread_ask_stop(s_pure_thread_args);
  }
  if(s_pure_thread_dps_args) {
    pure_thread_dps_ask_stop(s_pure_thread_dps_args);
  }
}
}

#define main thread_and_auto_refresh_main_dummy
#include <thread_and_auto_refresh.cpp>
AIDA_EXA(thread_and_auto_refresh)
#undef main

extern "C" {
void aida_exa_thread_and_auto_refresh_stop(Slash::UI::IUI& aUI){
  if(s_thread_and_auto_refresh_args) {
    thread_and_auto_refresh_ask_stop(s_thread_and_auto_refresh_args);
  }
}
}

//////////////////////////////////////////////////////////////////////////////
/// Examples using Slash /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define SLASH_AIDA_EXA(a_name) \
extern "C" {\
void OnXLabExas_aida_exa_##a_name(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;\
  Slash::Core::ISession& session = aUI.session();\
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);\
  if(!aida) {\
    std::ostream& out = session.cout();\
    out << "OnXLabExas_aida_exa_" << #a_name << " :" \
        << " aida not found." \
        << std::endl;\
    return;\
  }\
  a_name(session,*aida,session.cout(),aArgs[0],false);\
}}

#define main slash_cyclic_main_dummy
#include <slash_cyclic.cpp>
SLASH_AIDA_EXA(slash_cyclic)
#undef main

extern "C" {
void aida_exa_slash_cyclic_stop(Slash::UI::IUI& aUI) {
  slash_cyclic_stop(aUI);
}
}

#define main slash_thread_cyclic_main_dummy
#include <slash_thread_cyclic.cpp>
SLASH_AIDA_EXA(slash_thread_cyclic)
#undef main

extern "C" {
void aida_exa_slash_thread_cyclic_stop(Slash::UI::IUI& aUI){
  if(s_slash_thread_cyclic_fill_args) {
    slash_thread_cyclic_ask_stop(s_slash_thread_cyclic_fill_args);
  }
}
}

//////////////////////////////////////////////////////////////////////////////
/// monitor examples exit ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define name_monitor_exit "aida_exa_monitor_exit"

extern "C" {
void aida_exa_monitor_wait_exit(Slash::UI::IUI& aUI) {
  bool slash_cyclic_stopped = true;

  bool thread_and_auto_refresh_stopped = false;
  if(s_thread_and_auto_refresh_args) {
    if(s_thread_and_auto_refresh_args->f_exit) {
      //thread stopped.
      delete s_thread_and_auto_refresh_args;
      delete s_thread_and_auto_refresh_thread;
      s_thread_and_auto_refresh_args = 0;
      s_thread_and_auto_refresh_thread = 0;
      thread_and_auto_refresh_stopped = true;
    }
  } else {
    thread_and_auto_refresh_stopped = true;
  }

  bool pure_thread_stopped = false;
 {if(s_pure_thread_args) {
    if(s_pure_thread_args->f_exit) {
      //thread stopped.
      delete s_pure_thread_args;
      delete s_pure_thread_thread;
      s_pure_thread_args = 0;
      s_pure_thread_thread = 0;
      pure_thread_stopped = true;
    }
  } else {
    pure_thread_stopped = true;
  }}


  bool pure_thread_dps_stopped = false;
 {if(s_pure_thread_dps_args) {
    if(s_pure_thread_dps_args->f_exit) {
      //thread stopped.
      delete s_pure_thread_dps_args;
      delete s_pure_thread_dps_thread;
      s_pure_thread_dps_args = 0;
      s_pure_thread_dps_thread = 0;
      pure_thread_dps_stopped = true;
    }
  } else {
    pure_thread_dps_stopped = true;
  }}

  bool slash_thread_cyclic_stopped = false;
  if(s_slash_thread_cyclic_fill_args) {
    if(s_slash_thread_cyclic_fill_args->m_exit) {
      //thread stopped.
      delete s_slash_thread_cyclic_fill_args;
      delete s_slash_thread_cyclic_thread;
      s_slash_thread_cyclic_fill_args = 0;
      s_slash_thread_cyclic_thread = 0;
      slash_thread_cyclic_stopped = true;
    }
  } else {
    slash_thread_cyclic_stopped = true;
  }

  //if(!slash_thread_cyclic_fill_args.m_exit) return;

  if(slash_cyclic_stopped 
     && thread_and_auto_refresh_stopped
     && pure_thread_stopped
     && pure_thread_dps_stopped
     && slash_thread_cyclic_stopped) {

   {Slash::UI::ICyclic* cyclic = aUI.findCyclic(name_monitor_exit);
    if(cyclic) {
      aUI.removeCyclic(cyclic);
    }}

    aUI.exit();
  }

}

void aida_exa_monitor_exit(Slash::UI::IUI& aUI) {
  //std::cout << "debug : exit 001..." << std::cout;

  // slash_cyclic.cpp example :
  slash_cyclic_stop(aUI);

  // thread_and_auto_refresh.cpp example :
  if(s_thread_and_auto_refresh_args) {
    thread_and_auto_refresh_ask_stop(s_thread_and_auto_refresh_args);
  }

  // pure_thread.cpp example :
  if(s_pure_thread_args) {
    pure_thread_ask_stop(s_pure_thread_args);
  }
  if(s_pure_thread_dps_args) {
    pure_thread_dps_ask_stop(s_pure_thread_dps_args);
  }

  // slash_thread_cyclic.cpp example :
  slash_thread_cyclic_stop(aUI);
  if(s_slash_thread_cyclic_fill_args) {
    slash_thread_cyclic_ask_stop(s_slash_thread_cyclic_fill_args);
  }

 {Slash::UI::ICyclic* cyclic = aUI.findCyclic(name_monitor_exit);
  if(!cyclic) {
    cyclic = aUI.createCyclic(name_monitor_exit,1,"","");
    if(cyclic) {
      cyclic->setDelay(1000);
      cyclic->setScript("DLD","OnXLabExas aida_exa_monitor_wait_exit");
      cyclic->start();
    }
  }}
}
}

#undef name_monitor_exit

#define main slash_shape_hits_collect_main_dummy
#include <slash_shape_hits_collect.cpp>
SLASH_AIDA_EXA(slash_shape_hits_collect)
#undef main

#define main slash_shape_collect_tuple_main_dummy
#include <slash_shape_collect_tuple.cpp>
SLASH_AIDA_EXA(slash_shape_collect_tuple)
#undef main

extern "C" {

// to demonstrate the regular filling of banner histo from a cyclic.
// See examples/OnX/AIDA.onx.
void OnXLabExas_h_fill(Slash::UI::IUI& aUI) {

  // Find the OnXLab banner h histo :

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
  if(!aida) return;
  AIDA::ITree* tree = amanip::find_mem(*aida);
  if(!tree) return;
  AIDA::IHistogram1D* h = 
    amanip::find_histogram_1D(*tree,"h",true,aUI.session().cout());
  if(!h) return;

  // fill h :
  inlib::random::gauss rg(0,1);
  for(int index=0;index<1000;index++) h->fill(rg.shoot(),1);
}

} // extern "C"

//////////////////////////////////////////////////////////////////////////////
/// Examples using Slash, Inventor and HEPVis ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define main slash_iv_regions_main_dummy
#include <slash_iv_regions.cpp>
SLASH_AIDA_EXA(slash_iv_regions)
#undef main

#define main slash_iv_plot_and_det_main_dummy
#include <slash_iv_plot_and_det.cpp>
SLASH_AIDA_EXA(slash_iv_plot_and_det)
#undef main

#define main slash_iv_plot_and_images_main_dummy
#include <slash_iv_plot_and_images.cpp>
SLASH_AIDA_EXA(slash_iv_plot_and_images)
#undef main

#define main slash_iv_aida_soplotter_main_dummy
#include <slash_iv_aida_soplotter.cpp>
SLASH_AIDA_EXA(slash_iv_aida_soplotter)
#undef main

/*
#define main xxx_main_dummy
#include <xxx.cpp>
AIDA_EXA(xxx)
#undef main
*/

#undef AIDA_createAnalysisFactory
