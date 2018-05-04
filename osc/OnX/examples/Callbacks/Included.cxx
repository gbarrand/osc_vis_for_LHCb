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
#include <Slash/UI/IUI.h>

#include <Slash/Tools/UI.h>

#include <inlib/args>

//////////////////////////////////////////////////////////////////////////////
/// Automatic inclusion of OnX/examples/cpp examples /////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define ONX_EXA(a_name) \
extern "C" {\
void OnXExas_##a_name(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;\
  Slash::Core::ISession& session = aUI.session();\
  a_name(session,session.cout(),aArgs[0],false);\
}}

// OnX/examples/cpp :
extern "C" {
  Slash::Core::ISession* Slash_create_session_dummy(const std::vector<std::string>& args = std::vector<std::string>()){return 0;}
}
#define Slash_create_session Slash_create_session_dummy

// mire to check compilation only :
#define main mire_main_dummy
#include <mire.cpp>
ONX_EXA(mire)
#undef main

// create_ui to check compilation only :
#define main create_ui_main_dummy
#include <create_ui.cpp>
ONX_EXA(create_ui)
#undef main

// create_ui to check compilation only :
#define main obatch_main_dummy
#include <obatch.cpp>
ONX_EXA(obatch)
#undef main

#define main detector_visualize_main_dummy
#include <detector_visualize.cpp>
ONX_EXA(detector_visualize)
#undef main

#define OnXExas_include  //FIXME : have local cbks in exas.

#define main opengl_cube_main_dummy
#include <opengl_cube.cpp>
ONX_EXA(opengl_cube)
#undef main

#define main opengl_contour_main_dummy
#include <opengl_contour.cpp>
ONX_EXA(opengl_contour)
#undef main

#define main plotter_example_main_dummy
#include <plotter_example.cpp>
ONX_EXA(plotter_example)
#undef main

#undef Slash_create_session
