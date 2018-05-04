//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Try to do some graphics in a thread. 
//
// No chance to do that in a safe way as long
// as someone can play with the viewer from
// the GUI thread.
//
// And this because the viewer can then attempt to 
// traverse the scene graph whilst... it is partly
// deleted in the below runit !
//

#include <Slash/Tools/Inventor.h>

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>

// HEPVis :
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

static void* iv_main(void* aTag) {
  Slash::UI::IUI& aUI = *((Slash::UI::IUI*)aTag);
  float width = 2, dw = 0.01;
  unsigned int count = 0;
  while(true) {
    //printf("debug : %d\n",count);count++;

   {std::string s;
    aUI.session().parameterValue("monitor.iv_main",s);
    if(s=="stop") {
      aUI.session().setParameter("monitor.iv_main_exit","true");
      return 0;
    }}

    aUI.lock();    

    SoPage* soPage = Slash::find_SoPage(aUI,"@current@");
    if(soPage) {
    SoRegion* soRegion = soPage->currentRegion();
    if(soRegion) {
    if(soRegion->isOfType(SoDisplayRegion::getClassTypeId())) {
    SoDisplayRegion* soDisplayRegion = static_cast<SoDisplayRegion*>(soRegion);
    if(soDisplayRegion) {
    SoSeparator* soSeparator = soDisplayRegion->getStaticScene();

    //SbBool flag = soSeparator->enableNotify(FALSE);

    soSeparator->removeAllChildren();
    SoCube* soCube = new SoCube;
    soCube->width.setValue(width);
    soSeparator->addChild(soCube);

/*
    if(soSeparator->getNumChildren()==0) {
      SoCube* soCube = new SoCube;
      soCube->width.setValue(width);     
      soSeparator->addChild(soCube);
    } else {
      SoCube* soCube = (SoCube*)soSeparator->getChild(0);
      soCube->width.setValue(width);     
    }
*/

    //soSeparator->enableNotify(flag);

    width += dw;
    if (width>3) dw=-dw;
    if (width<2) dw=-dw;

    }}}}

    //usleep(1000);
    //aUI.synchronize();    
    aUI.unlock();    
  }
}

#ifdef WIN32
#else

#include <pthread.h>

extern "C" {

void monitor_iv_start(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  aUI.setCallback
    ("OnXLab_File_exit","activate","DLD","OnXLabExas monitor_exit");

  aUI.session().setParameter("monitor.iv_main","start");
  aUI.session().setParameter("monitor.iv_main_exit","false");
  pthread_t thread;
  pthread_create(&thread,0,iv_main,&aUI);
  //iv_main(&aUI);
}

void monitor_iv_stop(Slash::UI::IUI& aUI){
  aUI.session().setParameter("monitor.iv_main","stop");
}

} // extern "C"

#endif
