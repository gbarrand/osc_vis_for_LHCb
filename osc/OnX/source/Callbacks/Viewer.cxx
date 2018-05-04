//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

#include <Slash/Tools/Inventor.h>
#include <Slash/Tools/Data.h>

#include <Slash/UI/ICallbackData.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Net/protocol>

#include <inlib/cast>
#include <inlib/args>
#include <inlib/system>
#include <inlib/net/base_socket>

#include "inlib"

extern "C" {

void OnX_viewer_read_scene(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args[0] widget name (or @current@)
  // args[1] file
  // args[2] format
  // args[3] placement
  if(!inlib::check_args(aArgs,4,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  std::string file;
  inlib::file_name(aArgs[1],file);
  if(!soViewer->readScene(file,aArgs[2],aArgs[3])) {
    aUI.echo("Can't read file "+file+".");
  }
}

void OnX_viewer_clear(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args[0] widget name (or @current@)
  // args[1] what. For example : all, staticScene, dynamicScene.
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->clear(aArgs[1]);
}

void OnX_viewer_view_all(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->viewAll();
}

void OnX_viewer_stop_animating(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->stopAnimating();
}

void OnX_viewer_set_title(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->setTitle(aArgs[1]);
}

void OnX_viewer_remove_manips(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->removeManips();
}

void OnX_viewer_detect_intersections(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args[0] : viewer
  // args[1] : stop at first (boolean).
  // args[2] : what (nil, scene, staticScene, dynmicScene, region)
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,3,out)) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  bool b;
  if(!inlib::to(out,aArgs[1],b)) return;
  soViewer->detectIntersections(b,aArgs[2]);
}

void OnX_viewer_set_background_color(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  //  args[0] widget
  //  args[1] color name.
  // or :
  //  args[0] widget
  //  args[1] red
  //  args[2] green
  //  args[3] blue
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_or_args(aArgs,2,4,out)) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  double r,g,b;
  if(aArgs.size()==2) {
    if(!ui_check_rgb(aUI,aArgs[1],r,g,b)) return;
  } else {
    if(!inlib::to<double>(out,aArgs[1],r)) return;
    if(!inlib::to<double>(out,aArgs[2],g)) return;
    if(!inlib::to<double>(out,aArgs[3],b)) return;
  }
  soViewer->setBackgroundColor(r,g,b);
}

#define VIEWER_SET(a_x,a_X) \
void OnX_viewer_set_##a_x(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
  std::ostream& out = aUI.session().cout();\
  if(!inlib::check_args(aArgs,2,out)) return;\
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);	\
  if(!soViewer) return;\
  bool v;\
  if(!inlib::to(out,aArgs[1],v)) return;	\
  soViewer->set##a_X(v);\
}

VIEWER_SET(decoration,Decoration)
VIEWER_SET(viewing,Viewing)
VIEWER_SET(head_light,Headlight)
VIEWER_SET(feedback_visibility,FeedbackVisibility)
VIEWER_SET(auto_clipping,AutoClipping)
VIEWER_SET(smoothing,Smoothing)
VIEWER_SET(animation,Animation)

void OnX_viewer_render(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;
  soViewer->render();
}

void OnX_viewer_collect( Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){

//    <collect exec="DLD">OnX data_collect SceneGraph( @cat@ @this@ @cat@ ) highlight==true</collect>
//    <collect exec="DLD">OnX data_dump</collect>
//    <collect exec="DLD">OnX data_filter name</collect>
//    <collect exec="DLD">OnX data_dump @{AccessorManager.dumpMode}@</collect>

  //if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  Slash::Core::ISession& session = aUI.session();

  std::string what("SceneGraph");
  if(aArgs.size()) {
    what += "("+aArgs[0]+")";  //For exa : SceneGraph(@current@)
  }

  Slash::Data::collect(session,what,"highlight==true");

  Slash::Net::INetManager* netManager = Slash::netManager(session);
  if(netManager) { //if netManager is here, then we are the server.
    //server asks data to the client.

    // get data string on the selected SoNodes and send
    // them to the client. With them the client will
    // initialize its AccessorManager handlers.

    inlib::net::base_socket& socket = netManager->socket();
    socket.send_string(inlib_net_protocol_selection);
    socket.send_string(Slash::Data::values(session,"name"));

    // then ask the client to execute a data_dump.
    // (It could have been done directly here by sending
    //  a inlib_net_protocol_cbk with the socket).
    Slash::UI::IUI::Options opts;
    opts.push_back(Slash::UI::IUI::Option("where","remote"));
    std::string svalue;
    session.parameterValue("AccessorManager.dumpMode",svalue);
    aUI.executeScript("DLD","OnX data_dump "+svalue,opts);

    //FIXME : but the output is done on the client side.
    //        It should be done by using the session.cout()
    //        of the server (for exa to dump in a GUI terminal).

  } else {
    Slash::Data::filter(session,"name");
    std::string svalue;
    session.parameterValue("AccessorManager.dumpMode",svalue);
    Slash::Data::dump(session,svalue);
  }

}
void OnX_viewer_rectangular_collect(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] widget name (or @current@)

  //std::ostream& out = aUI.session().cout();
  //out << "OnX_viewer_rectangular_collect :" 
  //    << " callback value : " << aUI.callbackValue()
  //    << std::endl;

  std::vector<std::string> words;
  inlib::words(aUI.callbackValue()," ",false,words);
  if(words.size()!=4) return;
  int x;
  if(!inlib::to<int>(words[0],x)) return;
  int y;
  if(!inlib::to<int>(words[1],y)) return;
  unsigned int w;
  if(!inlib::to<unsigned int>(words[2],w)) return;
  unsigned int h;
  if(!inlib::to<unsigned int>(words[3],h)) return;

  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;

  soViewer->collect(x,y,w,h);
}

void OnX_viewer_set_parameter(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] what
  // args[1,n-1] : partial value
  //  value is built with : args[1] + " " + args[2] + ..... + args[n-1]
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = aUI.currentWidget();
  if(!widget) return;
  if(!INLIB_CAST(*widget,Slash::UI::ISoViewer)) return;
  widget->setParameter(aArgs[0],last);
}

}

extern "C" {

#define ALIAS(a_alias,a_new) \
void a_alias(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
 a_new(aUI,aArgs);\
}


ALIAS(OnX_viewer_setParameter,OnX_viewer_set_parameter)
ALIAS(OnX_viewer_set_color,OnX_viewer_set_background_color)

}
