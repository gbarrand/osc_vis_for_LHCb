//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(IUI&);
//     void callback_with_arguments(IUI&,const std::vector<std::string>&);
//   }
//

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>

// HEPVis :
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

// Slash :
#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Manager.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/Inventor.h>
#include <Slash/Tools/DummySceneHandler.h>

#include <inlib/args>

// this :
#include <OnXTemplateSix/FrameworkManager.h>
#include <OnXTemplateSix/Track_Accessor.h>

// User :
#include <User/Framework.h>
#include <User/PrintProcessor.h>

OnXTemplateSix::FrameworkManager* findManager(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();
  Slash::Core::IManager* manager = 
    session.findManager("OnXTemplateSix_FrameworkManager");
  if(!manager) return 0;
  OnXTemplateSix::FrameworkManager* frameworkManager = 
    (OnXTemplateSix::FrameworkManager*)manager->cast
                                        ("OnXTemplateSix::FrameworkManager");
//    dynamic_cast<OnXTemplateSix::FrameworkManager*>(manager);
  if(!frameworkManager) {
    std::ostream& out = session.cout();
    out << " manager not a OnXTemplateSix_FrameworkManager."
        << std::endl;
    return 0;
  }
  return frameworkManager;
}

extern "C" {

void OnXTemplateSix_init(Slash::UI::IUI& aUI) {
  // OnX callback to :
  // - load the User::Framework.
  // - declare it to the ISession by using a FrameworkManager instance.
  // - declare a Track_Accessor to the OnX accessor (type) manager.
  //
  // If using onx.exe, it should be executed on a create callback
  // of a <GUI>.onx file (see scripts/OnX/OnXTemplateSix.onx file).

  Slash::Core::ISession& session = aUI.session();

  OnXTemplateSix::FrameworkManager* frameworkManager = findManager(aUI);
  if(frameworkManager) {
    aUI.echo("user framework startup.");
    // If having a pointer here, it means that the visualization
    // had not been started from the onx.exe program, but
    // from a user (data framework) program that has initialized
    // OnX from one of its service and then had declared
    // the user framework to OnX by using a 
    // OnXTemplateSix::FrameworkManager.
  } else {
    aUI.echo("onx.exe startup.");
    // If having no pointer here, it means that the visualization
    // had been started from the onx.exe program.
 
    // In a concrete situation, here we should dynamic load
    // the data framework. In this example we simply 
    // instentiate our simple User::Framework here.
    User::Framework* userFramework = new User::Framework();
    userFramework->addProcessor(new User::PrintProcessor(*userFramework));

    // then we declare the user framework to OnX by using
    // a OnXTemplateSix::FrameworkManager.
    // The OnXTemplateSix::FrameworkManager will delete the
    // passed userFramework at end (at delete of the OnX::Session).
    frameworkManager = 
      new OnXTemplateSix::FrameworkManager(userFramework,true);
    session.addManager(frameworkManager);
  }

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(session);
  if(!sh) {
    session.addManager(new Slash::DummySceneHandler(Slash_SceneHandler));
  }

  // Declare the accessor for each class of the user event model :
  Slash::Data::IProcessor* accessorManager = 
    Slash_findManager(session,"AccessorManager",Slash::Data::IProcessor);
  if(accessorManager && !accessorManager->findAccessor("User_Track")) {
    accessorManager->addAccessor
      (new OnXTemplateSix::Track_Accessor
       (session,*sh,*(frameworkManager->framework())));
  }
}

void OnXTemplateSix_exit(Slash::UI::IUI& aUI) {
  OnXTemplateSix::FrameworkManager* mgr = findManager(aUI);
  if(!mgr) return;

  if(mgr->startedFromOnX_exe()) {

    aUI.exit();

  } else {

    // Tell the user framework that we want to exit :
    User::Framework* fmk = mgr->framework();
    fmk->wantToExit();
    aUI.exit();

  }
}

void OnXTemplateSix_vis_tracks(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();

  // Visualize tracks in blue :

  aUI.session().setParameter("modeling.color","blue");
  Slash::Data::collect(session,"User_Track","");
  Slash::Data::visualize(session,"Viewer");
  
}

void OnXTemplateSix_nextEvent(Slash::UI::IUI& aUI) {
  OnXTemplateSix::FrameworkManager* mgr = findManager(aUI);
  if(!mgr) return;

  if(mgr->startedFromOnX_exe()) {

    User::Framework* fmk = mgr->framework();
    const User::Event& event = fmk->nextEvent();
    fmk->dispatchEvent(event);

    aUI.executeScript("DLD","OnX viewer_clear Viewer dynamicScene");
    OnXTemplateSix_vis_tracks(aUI);

  } else {

    // Have to return to the user framework...
    aUI.exit();
   
  }
}

void OnXTemplateSix_vis_cube(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // Example of a direct Inventor visualization without passing
  // by the OnX DataAccessor system.
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  // Get the current SoPage :
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  // Get the SoDisplayRegion :
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  SoDisplayRegion* soDisplayRegion = static_cast<SoDisplayRegion*>(soRegion);
  if(!soDisplayRegion) return;
  
  //SoSeparator* getDynamicScene() const;

  // Get the display region "static scene" entry separator.
  // In general the stataic scene is for landscape things
  // (for example a detector in HEP) :
  SoSeparator* soSeparator = soDisplayRegion->getStaticScene();

  // Have a material (for picking) :
  SoMaterial* soMaterial = new SoMaterial;
  soMaterial->diffuseColor.setValue(SbVec3f(1.0F,1.0F,0));
  soSeparator->addChild(soMaterial);

  // Some user scene :
  SoCube* soCube = new SoCube;
  soSeparator->addChild(soCube);

}

} // extern "C"

