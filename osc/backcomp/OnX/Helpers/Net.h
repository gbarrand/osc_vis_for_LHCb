#ifndef OnX_Helpers_Net_h
#define OnX_Helpers_Net_h

/* Use Slash/Tools/IvNet.h

// to send a scene in client/server.

#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodekits/SoRegion.h>

#include <Lib/fmanip.h>
#include <Lib/Out.h>
#include <Lib/sout.h>

// For exa, used in OnX::ShapeAccessor::visualize.
//          and OnX_Examples_Detector.

inline bool ui_region_add_scene(
 Slash::UI::IUI& aUI,SoRegion& aRegion,SoNode* aNode
,const std::string& aWhere,const std::string& aPlacement
){
  Slash::Net::INetManager* netManager = Slash::netManager(aUI.session());
  if(netManager && netManager->isClient()) {
    std::string file;
    if(!Lib::fmanip::tmpname(file,".","OnX_",".hiv")) {
      Lib::Out out(aUI.session().printer());
      out << "ui_region_add_scene :"
          << " can't get tmp file name."
          << Lib::endl;
      return false;
    }
    //FIXME : should write in binary.
    //FIXME : should be able to avoid file and use direct mem copy.
    aNode->ref();
    SbBool binary = FALSE;
    if(SoTools::write
         (*aNode,file.c_str(),"hiv",FALSE,binary,"GZIP",0.5F)==FALSE) {
      aUI.echo("Can't write file "+Lib::sout(file)+".");
    } else {
      std::vector<std::string> args;
      args.push_back(aWhere);
      args.push_back(aPlacement);
      args.push_back(file);
      netManager->pushEnd("graphics",args);
    }
    aNode->unref();

  } else {  
    if(aRegion.doIt(SbAddNode(aNode,aPlacement.c_str()))==FALSE) return false;
  }
  return true;
}

// See also *UI::receiveScene
*/

#endif
