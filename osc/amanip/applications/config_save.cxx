// Dummy file to help building the slash-config system.

// In fact we use it to do some testing...

#include <amanip/all.h>

#include <cmath>

void cheeck_templates_compilation() {

 {AIDA::IBaseHistogram* o;
  amanip::cast<AIDA::IHistogram1D,AIDA::IBaseHistogram>
         (*o,"AIDA::IHistogram1D");
  amanip::cast_const<AIDA::IHistogram1D,AIDA::IBaseHistogram>
         (*o,"AIDA::IHistogram1D");}

 {AIDA::ITree* o;
  amanip::find<AIDA::IBaseHistogram>
    (*o,"","AIDA::IBaseHistogram",true,std::cout);}

 {AIDA::IHistogram1D* o;
  amanip::name<AIDA::IHistogram1D>(*o);}

 {AIDA::ITuple* t;
  AIDA::IHistogram1D* h1;
  AIDA::IFilter* f;
  amanip::project<AIDA::IHistogram1D>(std::cout,*t,*h1,0,*f);
  AIDA::IEvaluator* e;
  amanip::project<AIDA::IHistogram1D>(std::cout,*t,*h1,*e,*f);
  AIDA::IHistogram2D* h2;
  amanip::project<AIDA::IHistogram2D>(std::cout,*t,*h2,0,0,*f);
  amanip::project<AIDA::IHistogram2D>(std::cout,*t,*h2,*e,*e,*f);
  amanip::project<AIDA::IHistogram2D>(std::cout,*t,*h2,*e,*e,*f,*f,*t);
  AIDA::IHistogram3D* h3;
  amanip::project<AIDA::IHistogram3D>(std::cout,*t,*h3,0,0,0,*f);
  amanip::project<AIDA::IHistogram3D>(std::cout,*t,*h3,*e,*e,*e,*f);}

}


int main() {
  //std::cout << "debug : 007" << std::endl;   

  cheeck_templates_compilation();

  return 0;
}
