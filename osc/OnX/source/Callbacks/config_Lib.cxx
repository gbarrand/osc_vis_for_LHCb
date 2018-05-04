// Dummy file to help building the onx-config system.

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/Printer.h>

#include <inlib/sout>
#include <exlib/xml/parser>

#include <Lib/Session.h>

extern "C" {

void OnX_config_libutils(Slash::UI::IUI& aUI) {
  // Put some code in order to be sure to build correctly.
  inlib::sout("");
  Slash::set_color(aUI.session(),"0 0 0");
  Slash::Printer printer;
  Lib::Session session(printer);
}

}
