// this :
#include <OnX/Inventor/HEPVis.h>

extern "C" {
  void HEPVisUtils_initClasses();
  void HEPVisGeometry_initClasses();
  void HEPVisDetector_initClasses();
  void HEPVisPlotter_initClasses();
  void HEPVisGUI_initClasses();
}

void OnX::HEPVis::initClasses() {
  HEPVisUtils_initClasses();
  HEPVisGeometry_initClasses();
  HEPVisDetector_initClasses();
  HEPVisPlotter_initClasses();
  HEPVisGUI_initClasses();
}

