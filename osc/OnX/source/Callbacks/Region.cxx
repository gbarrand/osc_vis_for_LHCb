//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
// HEPVis and Inventor callbacks dealing with a region.
//

#include <Slash/Tools/Inventor.h>

#include <inlib/args>
#include <inlib/sprintf>
#include <inlib/sjust>

#include "inlib"

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoSearchAction.h>

// HEPVis :
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>
#include <HEPVis/nodes/SoCoordinateAxis.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_region_set_background_color(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//  args[0] where : <strign> or <string>@<int>
//  args[1] color name.
// or :
//  args[0] widget
//  args[1] red
//  args[2] green
//  args[3] blue
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_or_args(aArgs,2,4,out)) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  double r,g,b;
  if(aArgs.size()==2) {
    if(!ui_check_rgb(aUI,aArgs[1],r,g,b)) return;
  } else {
    if(!inlib::to<double>(out,aArgs[1],r)) return;
    if(!inlib::to<double>(out,aArgs[2],g)) return;
    if(!inlib::to<double>(out,aArgs[3],b)) return;
  }
  soRegion->color.setValue((float)r,(float)g,(float)b);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_view_scene_of(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] widget
// args[1] wanted region to connect the current region to
//  When done the current region will display the scene of the args[1] region.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,2,out)) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion(); //FIXME : can't choose a region
  if(!soRegion) return;
  int index;
  if(!inlib::to<int>(out,aArgs[1],index)) return;
  SoRegion* region = soPage->getRootRegion(index);
  if(!region) {
    out << "region_view_scene_of :"
        << " region " << index << " not found."
        << std::endl;
    return;
  }
  soRegion->connectFrom(region);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_node_set(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] widget
// args[1] [region,scene,staticScene,dynamicScene]
// args[2] [name,type]
// args[3] name or type to search
// args[4] [all,first,last]
// args[5] string to pass to nodes set() method.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();
  //out_args(aUI,aArgs);
  if(!inlib::check_args(aArgs,6,out)) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoNode* head = soRegion->getNodeOfInterest(aArgs[1].c_str());
  if(!head) {
    out << "region_node_set :" 
        << " nothing to search for " << inlib::sout(aArgs[1])
        << std::endl;
    return;
  }

  SbBool oldsearch = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  SoSearchAction searchAction;
  if(aArgs[2]=="name") {
    //searchAction.setFind(SoSearchAction::NAME);
    searchAction.setName(aArgs[3].c_str());
  } else if(aArgs[2]=="type") {
    SoType soType = SoType::fromName(aArgs[3].c_str());
    if(soType==SoType::badType()) {
      SoBaseKit::setSearchingChildren(oldsearch);
      out << "region_node_set :" 
          << " unknown So class " << inlib::sout(aArgs[3])
          << std::endl;
      return;
    }
    //searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(soType);
  } else {
    SoBaseKit::setSearchingChildren(oldsearch);
    out << "region_node_set :" 
        << " unknown find keyword " << inlib::sout(aArgs[3])
        << " Should be [name,type]."
        << std::endl;
    return;
  }

  if(aArgs[4]=="first") {
    searchAction.setInterest(SoSearchAction::FIRST);
  } else if(aArgs[4]=="last") {
    searchAction.setInterest(SoSearchAction::LAST);
  } else if(aArgs[4]=="all") {
    searchAction.setInterest(SoSearchAction::ALL);
  } else {
    SoBaseKit::setSearchingChildren(oldsearch);
    out << "region_node_set :" 
        << " unknown interest keyword " << inlib::sout(aArgs[4])
        << " Should be [name,type]."
        << std::endl;
    return;
  }

  searchAction.apply(head);
  SoBaseKit::setSearchingChildren(oldsearch);

  SoPathList pathList = searchAction.getPaths();
  int number = pathList.getLength();
  if(!number) {
    out << "region_node_set :"
        << " nothing found."
        << std::endl;
    return;
  }  

  //::printf("debug : \"%s\"\n",aArgs[5].c_str());

  //::printf("debug : found %d\n",number);

  for(int index=0;index<number;index++) {
    SoFullPath* path = (SoFullPath*)pathList[index];
    SoNode* node = path->getTail();
    //::printf("debug :  %d \"%s\"\n",index,node->getName().getString());
    if(node) node->set(aArgs[5].c_str());
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_set_parameter(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] where : <strign> or <string>@<int>
// args[1] what
// args[2,n] value
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string last;
  if(!inlib::check_min(aArgs,3,last,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoTools::setKitPart(soRegion,aArgs[1].c_str(),last.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_to_page(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoPage* soPage;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0],&soPage);
  if(!soRegion) return;
  soPage->mapRegionToPage(soRegion);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_on_top(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoPage* soPage;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0],&soPage);
  if(!soRegion) return;
  soPage->setRegionOnTop(soRegion);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_no_highlighted(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoTools::resetHighlight(soRegion);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_set_camera(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] widget
// args[1] field
// args[2,n] value
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_min_args(aArgs,3,out)) return;
  std::string last;
  if(!inlib::check_min(aArgs,2,last,out)) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoCamera* camera = soRegion->getCamera();
  if(!camera) {
    out << "region_set_camera :"
        << " region without camera."
        << std::endl;
    return;
  }
  if(camera->set(last.c_str())==FALSE) {
    out << "region_set_camera :"
        << " unable to set " << inlib::sout(last)
        << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_dump_camera(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoCamera* camera = soRegion->getCamera();
  if(!camera) {
    aUI.echo("SoCamera not found.");
    return;
  }
  /*FIXME : not reliable. Sometime it works, sometime not !
  SbString ss;
  camera->get(ss);
  aUI.echo(ss.getString());*/

  std::string vpm;
  switch(camera->viewportMapping.getValue()){
    case SoCamera::CROP_VIEWPORT_FILL_FRAME:
      vpm = "CROP_VIEWPORT_FILL_FRAME";
      break;
    case SoCamera::CROP_VIEWPORT_LINE_FRAME:
      vpm = "CROP_VIEWPORT_LINE_FRAME";
      break;
    case SoCamera::CROP_VIEWPORT_NO_FRAME:
      vpm = "CROP_VIEWPORT_NO_FRAME";
      break;
    case SoCamera::ADJUST_CAMERA: 
      vpm = "ADJUST_CAMERA";
      break;
    case SoCamera::LEAVE_ALONE:
      vpm = "LEAVE_ALONE";
      break;
  }
  const SbVec3f&  position = camera->position.getValue();
  SbVec3f axis;
  float angle;
  camera->orientation.getValue().getValue(axis,angle);
  std::string sb;
  inlib::sprintf(sb,256,"\
viewportMapping %s\n\
position %g %g %g\n\
orientation %g %g %g %g\n\
aspectRatio %g\n\
nearDistance %g\n\
farDistance %g\n\
focalDistance %g\n"
,vpm.c_str()
,position[0],position[1],position[2]
,axis[0],axis[1],axis[2],angle
,camera->aspectRatio.getValue()
,camera->nearDistance.getValue()
,camera->farDistance.getValue()
,camera->focalDistance.getValue());
  std::string sh;
  if(camera->isOfType(SoOrthographicCamera::getClassTypeId())==TRUE) {
    inlib::sprintf(sh,64,"height %g",
      ((SoOrthographicCamera*)camera)->height.getValue());
  } else if(camera->isOfType(SoPerspectiveCamera::getClassTypeId())==TRUE) {
    inlib::sprintf(sh,64,"heightAngle %g",
      ((SoPerspectiveCamera*)camera)->heightAngle.getValue());
  }
  std::string sc = sb + sh;  
  aUI.echo(sc.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_reset_camera(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  soRegion->resetCamera();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_count(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,1,out)) return;
  SoNode* node = Slash::find_SoNode(aUI,"@current@",aArgs[0]);
  if(!node) return;

  unsigned int triangles;
  unsigned int lineSegments;
  unsigned int points;
  unsigned int nodes;
  unsigned int shapes;
  SoTools::count(*node,triangles,lineSegments,points,nodes,shapes);

#define OUT(a_string,a_value)\
 {std::string s(a_string);\
  inlib::justify(s,23);\
  out << s << " = " << a_value << std::endl;}

  OUT("Number of triangles",triangles);
  OUT("Number of line segments",lineSegments);
  OUT("Number of points",points);
  OUT("Number of nodes",nodes);
  OUT("Number of shapes",shapes);

#undef OUT
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_read_scene(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//  args[0] where : <strign> or <string>@<int>
//  args[1] file
// optional :
//  args[2] placement
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_min_args(aArgs,2,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  const std::string& file = aArgs[1];
  std::string placement = (aArgs.size()>=3?aArgs[2]:"staticScene");
  std::string format = (aArgs.size()>=4?aArgs[3]:"");
  if(soRegion->readScene
       (file.c_str(),format.c_str(),placement.c_str())==FALSE) {
    aUI.echo("Can't read file "+inlib::sout(file)+".");
  }
}

void OnX_region_clear(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  SoRegion* soRegion = 0;
  if(!aArgs.size()) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_region_clear :"
        << " no argument is deprecated."
        << " You have to pass the viewer name."
        << std::endl;
    soRegion = Slash::find_SoRegion(aUI,Slash::s_current());
  } else {
    soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  }
  if(!soRegion) return;
  soRegion->clear();
}

//////////////////////////////////////////////////////////////////////////////
/// FIXME : revisit the belows ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnX_region_setNode(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] what [part,name,type]
// aArgs[1] searched
// args[2,n-1] : partial value
//  value is built with : args[2] + " " + args[2] + ..... + args[n-1]
// Example : 
//   OnX region_setNode type SoCamera height 500 position 0 0 500
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  SoNode* node = soRegion->findNode(aArgs[0].c_str(),aArgs[1].c_str());
  if(!node) {
    aUI.echo("node not found.");
    return;
  }
  std::string value = aArgs[2];
  for(unsigned int index=3;index<aArgs.size();index++) 
    value += " " + aArgs[index];
  node->set(value.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_writeScene(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] : string : file name
// aArgs[1] : boolean : binary or not
// aArgs[2] : string : what in the scene to write. For exa all or scene.
//            all put the scene but may also put light, lightModel and camera. 
// aArgs[3] : boolean : generate alternate representations.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aArgs.size()!=4) return;
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;

  const std::string& file = aArgs[0];
  std::string format = inlib::suffix(file);

  bool binary = false;
  if(!inlib::to(aArgs[1],binary)) binary = false;
  const std::string& what = aArgs[2];
  bool genAlternateRep = true;
  if(!inlib::to(aArgs[3],genAlternateRep)) genAlternateRep = true;

  SoNode* node = soRegion->getNodeOfInterest(what.c_str());
  if(!node) return;
  if(SoTools::write(*node,
                    file.c_str(),
                    format.c_str(),
                    binary?TRUE:FALSE,
                    genAlternateRep?TRUE:FALSE)==FALSE) {
    aUI.echo("Can't write file "+inlib::sout(file)+".");
  }
}

//DEPRECATED
/////////////////////////////////////////////////////////////////////////////
void OnX_region_addFrame(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] : size
// aArgs[1] : color
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return;
  SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;

  if(aArgs.size()!=2) return;

  double size;
  if(!inlib::to<double>(aArgs[0],size)) return;

  SoSeparator* stat = displayRegion->getStaticScene();
  if(!stat) return;

  double r,g,b;
  inlib::to_rgb(aArgs[1],r,g,b);

  SoSeparator* separator = new SoSeparator();
  separator->setName("sceneGraph");

  SoHighlightMaterial* highlightMaterial = new SoHighlightMaterial;
  highlightMaterial->diffuseColor.setValue
    (SbColor((float)r,(float)g,(float)b));
  //highlightMaterial->transparency.setValue((float)transparency);
  highlightMaterial->highlightColor.setValue(SbColor(1,1,1));
  separator->addChild(highlightMaterial);

  SoCoordinateAxis* frame = new SoCoordinateAxis();
  frame->setName("frame");
  frame->fNDivision.setValue(10);
  frame->fDivisionLength.setValue((float)size);
  separator->addChild(frame);

  stat->addChild(separator);

}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_setParameter(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] what
// args[1,n-1] : partial value
//  value is built with : args[1] + " " + args[2] + ..... + args[n-1]
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();

  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!aArgs.size()) return;
  const std::string& what = aArgs[0];
  if(aArgs.size()==1) {
    SoTools::setKitPart(soRegion,what.c_str(),"");
  } else {
    std::string value = aArgs[1];
    for(unsigned int index=2;index<aArgs.size();index++) 
      value += " " + aArgs[index];
    if(what=="camera") {
      SoCamera* camera = soRegion->getCamera();
      if(camera) {
        if(camera->set(value.c_str())==FALSE) {
          out << "region_setParameter :"
              << " On camera, unable to set " << inlib::sout(value)
              << std::endl;
        }
      }
    } else {
      SoTools::setKitPart(soRegion,what.c_str(),value.c_str());
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_toggleParameter(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] what
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aArgs.size()) return;
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  SoTools::toggleKitPart(soRegion,aArgs[0].c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_resetUndo(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  soRegion->resetUndo();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_undo(
 Slash::UI::IUI& aUI
,const std::vector<std::string>&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  soRegion->undo();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_reset_highlight( //FIXME : inconsistent casing
 Slash::UI::IUI& aUI
,const std::vector<std::string>&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  //FIXME page->resetCallbackValue();
  SoTools::resetHighlight(soRegion);
  //FIXME page->executeCollectCallback();
  //FIXME page->resetCallbackValue();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_setSize(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] : A4H, A4V
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aArgs.size()) return;
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  SoViewportRegion* vpr = soRegion->getViewportRegion();
  unsigned int w,h;
  vpr->getSizePixels(w,h);
  if(!w || !h) return;
  if(aArgs[0]=="A4V") {
    if(w>=h) {
      w = (unsigned int)(float(h)*21.0F/29.7F);
      vpr->setSizePixels(w,h);
    } else {
      h = (unsigned int)(float(w)*29.7F/21.0F);
      vpr->setSizePixels(w,h);
    }
  } else if(aArgs[0]=="A4H") {
    if(w>=h) {
      h = (unsigned int)(float(w)*21.0F/29.7F);
      vpr->setSizePixels(w,h);
    } else {
      w = (unsigned int)(float(h)*29.7F/21.0F);
      vpr->setSizePixels(w,h);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_dump_highlighted(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();

  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;

  SoNode* node = soRegion->getTopSeparator();
  if(!node) return;
 
  SoPathList* list = SoTools::getSceneGraphs(node);
  if(!list) return;

  unsigned int number = list->getLength();

  for(unsigned int index=0;index<number;index++) {
    SoFullPath* path = (SoFullPath*)(*list)[index];
    SoNode* sg = path->getTail();

    bool highlighted = false;
   {SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoHighlightMaterial::getClassTypeId());
    searchAction.apply(sg);
    SoPath* path = searchAction.getPath();
    if(path) {
      SoHighlightMaterial* material = (SoHighlightMaterial*)path->getTail();
      if(material->getSavedMaterial()) highlighted = true;
    }}

    if(highlighted) {
      SoSearchAction searchAction;
      searchAction.setFind(SoSearchAction::TYPE);
      searchAction.setType(SoShape::getClassTypeId());
      searchAction.apply(sg);
      SoPath* path = searchAction.getPath();
      if(path) {
        SoNode* nd = (SoNode*)path->getTail();
        //std::string spath = 
        //  path ? path->getTail()->getName().getString():"nil";
        //out << std::string(spath) << std::endl;
        out << std::string(nd->getTypeId().getName().getString()) << std::endl;
      }
    }
  }

  delete list;
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_addSoNode(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] node class (def is SoCube)
// aArgs[1] staticScene or dynamicScene (def is staticScene)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aUI.session().cout();

  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) {
    out << "region_addSoNode :" 
        << " region not an SoDisplayRegion." 
        << std::endl;
    return;
  }
  SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;

  std::string sclass = "SoCube";
  if(aArgs.size()) sclass = aArgs[0];

  SoSeparator* sep = displayRegion->getStaticScene();
  if((aArgs.size()==2) && (aArgs[0]=="dynamicScene")) {
    sep = displayRegion->getDynamicScene();
  }

  SoType soType = SoType::fromName(sclass.c_str());
  if(soType==SoType::badType()) {
    out << "region_addSoNode :" 
        << " unknown class " << inlib::sout(sclass)
        << std::endl;
    return;
  }
  if(soType.canCreateInstance()==FALSE) {
    out << "region_addSoNode :" 
        << " can't create instance for class " << inlib::sout(sclass)
        << std::endl;
    return;
  }

  SoNode* node = (SoNode*)soType.createInstance();
  if(!node) {
    out << "region_addSoNode :" 
        << " create instance failed for class " << inlib::sout(sclass)
        << std::endl;
    return;
  }

  sep->addChild(node);
}

} // extern "C"

extern "C" {

#define ALIAS(a_alias,a_new) \
void a_alias(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {\
 a_new(aUI,aArgs);\
}

ALIAS(OnX_region_set_color,OnX_region_set_background_color)

}
