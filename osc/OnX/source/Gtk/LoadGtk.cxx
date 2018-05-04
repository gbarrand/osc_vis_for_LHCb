// this :
#include <OnX/Gtk/GtkUI.h>

extern "C" {

Slash::UI::IUI* OnX_createGtkUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs) {
  return new OnX::GtkUI(aSession,aArgs);
}

}
