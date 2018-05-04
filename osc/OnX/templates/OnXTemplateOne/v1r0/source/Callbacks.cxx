//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/UI/IUI.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnXTemplateOne_Hello_world(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aUI.echo("Hello world ! (from C++)");
}

} // extern "C"
