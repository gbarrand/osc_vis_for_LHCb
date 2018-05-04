// Dummy file to help building the onx-config system for HEPVis and Inventor.

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/Printer.h>

#include <inlib/sout>
#include <exlib/OpenGL>

#include <Lib/Session.h>

// Inventor :
#include <Inventor/nodes/SoSeparator.h>

// HEPVis :
#include <HEPVis/nodes/SoStyle.h>

extern "C" {
  void HEPVisUtils_initClasses();
  void HEPVisGeometry_initClasses();
  void HEPVisDetector_initClasses();
  void HEPVisPlotter_initClasses();
  void HEPVisGUI_initClasses();
}

extern "C" {

void OnX_config_opengl(Slash::UI::IUI& aUI) {
  // Put some code in order to be sure to build correctly.
  inlib::sout("");

  Slash::Printer printer;
  Lib::Session session(printer);

  glBegin(GL_POLYGON);
  glEnd();

  // Inventor :
  new SoSeparator();

  // HEPVis :
  HEPVisUtils_initClasses();
  HEPVisGeometry_initClasses();
  HEPVisDetector_initClasses();
  HEPVisPlotter_initClasses();
  HEPVisGUI_initClasses();
}

}
