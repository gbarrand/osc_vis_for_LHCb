//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>

#include <inlib/args>

extern "C" {

void OnX_scene_handler_clear(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args[0] where.
  // args[1] what. For example : staticScene, dynamicScene.
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(aUI.session());
  if(!sh) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_scene_handler_clear :"
        << " scene handler not found."
        << std::endl;
    return;
  }

  sh->clear(aArgs[0],aArgs[1]);
}

}
