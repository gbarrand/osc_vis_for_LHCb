// Dummy file to help building the onx-config system for OpenGL.

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/Printer.h>

#include <inlib/sout>
#include <exlib/OpenGL>

#include <Lib/Session.h>

extern "C" {

void OnX_config_opengl(Slash::UI::IUI& aUI) {
  // Put some code in order to be sure to build correctly.
  inlib::sout("");

  Slash::Printer printer;
  Lib::Session session(printer);

  glBegin(GL_POLYGON);
  glEnd();
}

}
