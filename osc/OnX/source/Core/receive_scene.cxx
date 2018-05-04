
#include <Slash/Core/ISession.h>
#include <Slash/Tools/Inventor.h>

inline bool receive_scene(
 Slash::UI::IUI& aUI
,const std::string& aWhere
,const std::string& aPlacement
,const std::string& aFile
)
{
  //used in the *UI.cxx receiveScene method.
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aWhere);
  if(!soRegion) return false;
  if(soRegion->readScene(aFile.c_str(),"hiv",aPlacement.c_str())==FALSE) {
    std::ostream& out = aUI.session().cout();
    out << "ui_read_scene :"
        << " can't read file " << inlib::sout(aFile)
        << std::endl;
    return false;
  }
  return true;
}

