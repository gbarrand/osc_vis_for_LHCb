
#include <Slash/Core/ISession.h>

#include <Slash/Tools/Inventor.h>
#include <Slash/Tools/Managers.h>

#include <inlib/colord>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>

// this method is reused in examples/Callbacks/Included.cxx.
static void mire(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  unsigned int number = inlib::colord::named_color_number();
  const inlib::colord::rgb* rgbs = inlib::colord::named_color_rgbs();

  SoPage* soPage = Slash::find_SoPage(*ui,a_viewer);
  if(!soPage) return;
  soPage->title.setValue("OnX/Mire");

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(a_session);
  if(!sh) {
    a_out << "mire :"
          << " scene handler not found." 
          << std::endl;
  } else {

    SoSeparator* sep = new SoSeparator;
    sep->ref();
  
    unsigned int index = 0;
  
    float y = 0;    
    for(unsigned int irow=0;irow<10;irow++) {
      float x = 0;
      for(unsigned int icol=0;icol<10;icol++) {
        if(index>=number) break;  
  
        SoSeparator* sep2 = new SoSeparator;
        sep->addChild(sep2);
  
        SoTransform* soTrans = new SoTransform;
        soTrans->translation.setValue(SbVec3f(x,y,0));
        sep2->addChild(soTrans);
  
        double r = rgbs[index][0];
        double g = rgbs[index][1];
        double b = rgbs[index][2];
        //inlib::to_rgb(scolors[index],r,g,b);
        
        SoMaterial* soMat = new SoMaterial;
        soMat->diffuseColor.setValue(SbVec3f(float(r),float(g),float(b)));
        sep2->addChild(soMat);
  
        SoCube* soCube = new SoCube;
        sep2->addChild(soCube);
  
        index++;
  
        x += 5;
      }
      y -= 5;
    }

    sh->setViewer(a_viewer);
    sh->addScene("staticScene",sep);

    sep->unref();
  }

  if(a_ui) ui->steer();

}

#include <Slash/Core/ISession.h>
#include <inlib/osc_env> //For TTFPATH

#include <iostream>

int main(int,char*[]) {

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "create_ui :"
              << " unable to set OSC environment." << std::endl;
    return 1;
  }

  int argc = 1;
  static char* argv[] = {(char*)"-onx_file=mire.onx"};  

  Slash::Core::ISession* session = Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "mire :"
              << " session not found." << std::endl;
    return 1;
  }

  mire(*session,std::cout,"@current@",true);
  delete session;

  return 0;
}
