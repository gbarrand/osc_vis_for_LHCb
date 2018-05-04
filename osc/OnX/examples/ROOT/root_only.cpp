
#include <TApplication.h>

#include <TRandom.h>
#include <TH1.h>
#include <TCanvas.h>

int main(int argc,char** argv) {

  TApplication app("A2R_read",&argc,argv);

  TH1* histogram = new TH1D("hGauss","Random gauss",100,-5,5);
  for(unsigned int count=0;count<10000;count++) 
    histogram->Fill(gRandom->Gaus(0,1));
  histogram->Draw();

  app.Run();

  return 0;
}
