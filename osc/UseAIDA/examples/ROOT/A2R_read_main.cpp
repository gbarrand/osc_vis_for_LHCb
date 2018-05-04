#include <AIDA/AIDA.h>

#include <TCanvas.h>
#include <TApplication.h>

#include "A2R.h"
#include "A2R_read.C"

int main(int argc,char** argv) {
  TApplication app("A2R_read",&argc,argv);
  A2R_read();
  app.Run();
  return 0;
}
