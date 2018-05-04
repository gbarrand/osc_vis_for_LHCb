#include "SceneGraphAccessor.h"

#include <Slash/Data/IIterator.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/names.h>
#include <Slash/Tools/Value.h>

#include <inlib/words>
#include <inlib/sto>
#include <inlib/sout>

#include <Lib/Debug.h>

#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/manips/SoTransformManip.h>

#include <HEPVis/SbColors.h>
#include <HEPVis/misc/SoStyleCache.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoSceneGraph.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>

OnX::SceneGraphAccessor::SceneGraphAccessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.out())
,fSession(aSession)
,fName("SceneGraph")
,fSceneGraphs(0)
,fNumber(0)
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("highlight",Slash::Data::IProperty::BOOLEAN);
  addProperty("with_manip",Slash::Data::IProperty::BOOLEAN);
  addProperty("address",Slash::Data::IProperty::POINTER);
}

OnX::SceneGraphAccessor::~SceneGraphAccessor() {clear();}

std::string OnX::SceneGraphAccessor::name() const {return fName;}

namespace OnX {
class SceneGraphAccessorIterator : public virtual Slash::Data::IIterator {
public: //Slash::Data::IIterator
  virtual Slash::Data::IAccessor::Data object() {
    if(fIndex>=fNumber) return 0;
    return &(fList[fIndex]);
  }
  virtual void next() {fIndex++;}
  virtual void* tag() { return 0;}
public:
  SceneGraphAccessorIterator(OnX::SceneGraph* aList,unsigned int aNumber)
    :fIndex(0),fList(aList),fNumber(aNumber) {
    Lib::Debug::increment("OnX::SceneGraphAccessorIterator");
  }
  virtual ~SceneGraphAccessorIterator() {
    Lib::Debug::decrement("OnX::SceneGraphAccessorIterator");
  }
private:
  unsigned int fIndex;
  OnX::SceneGraph* fList;
  unsigned int fNumber;
};
}

Slash::Data::IIterator* OnX::SceneGraphAccessor::iterator() {
  // Returned object should be deleted by the receiver.
  getSceneGraphs(iteratorArguments());
  return new SceneGraphAccessorIterator(fSceneGraphs,fNumber);
}

Slash::Core::IValue* OnX::SceneGraphAccessor::findValue(
 Slash::Data::IAccessor::Data aData
,const std::string& aName
,void*
){
  SceneGraph* obj = (SceneGraph*)aData;
  if(aName=="address") {
    return new Slash::Value((void*)obj);
  } else if(aName=="name") {
    if(!obj->fSep) {return new Slash::Value("nil");}
    if(obj->fSep->isOfType(SoSceneGraph::getClassTypeId())==TRUE) {
      SoSceneGraph* soSG = (SoSceneGraph*)obj->fSep;
      return new Slash::Value(soSG->getString());
    } else if(obj->fInfo) {
      SoInfo* soInfo = (SoInfo*)obj->fInfo;
      const SbString& s = soInfo->string.getValue();
      if((s.getLength()>=5)&&(s.getSubString(0,4)=="name:")) { 
        printf("debug : info sg : \"%s\"\n",s.getSubString(5).getString());
        return new Slash::Value(s.getSubString(5).getString());
      } else {
        return new Slash::Value("nil");
      }
    } else {
      //old logic. A SoSeparator named "sceneGraph" with, under it,
      //           a SoShape bearing the data infos on its name.
      SoSearchAction searchAction;
      searchAction.setFind(SoSearchAction::TYPE);
      searchAction.setType(SoShape::getClassTypeId());
      searchAction.apply(obj->fSep);
      SoPath* path = searchAction.getPath();
      std::string spath = path ? path->getTail()->getName().getString():"nil";
      return new Slash::Value(spath);
    }

  } else if(aName=="highlight") {
    if(!obj->fSep) {return new Slash::Value(false);}
    SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoHighlightMaterial::getClassTypeId());
    searchAction.apply(obj->fSep);
    SoPath* path = searchAction.getPath();
    bool value = false;
    if(path) {
      SoHighlightMaterial* material = (SoHighlightMaterial*)path->getTail();
      if(material->getSavedMaterial()) {
        value = true;
      }
    }
    return new Slash::Value(value);

  } else if(aName=="with_manip") {
    if(!obj->fSep) {return new Slash::Value(false);}
    SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoTransformManip::getClassTypeId());
    searchAction.apply(obj->fSep);
    SoPath* path = searchAction.getPath();
    bool value = path ? true : false;
    return new Slash::Value(value);

  } else {
    return new Slash::Value();
  }
}

void OnX::SceneGraphAccessor::destroy(Slash::Data::IAccessor::Data aData,void*){
  SceneGraph* obj = (SceneGraph*)aData;
  //printf("debug : destroy %lx\n",aData);
  if(!obj->fParent) return;
  if(obj->fParent->isOfType(SoGroup::getClassTypeId())==FALSE) return;
  ((SoGroup*)obj->fParent)->removeChild(obj->fSep);
}

void OnX::SceneGraphAccessor::set(
 Slash::Data::IAccessor::Data aData
,const std::string& aWhat
,const std::string& aValue
,void*
){
  //::printf("debug : OnX::SceneGraphAccessor::set : %lx %s %s\n",
  //    aData,aWhat.c_str(),aValue.c_str());

  SceneGraph* obj = (SceneGraph*)aData;
  if(!obj->fSep) return;

  // Look first for a name :
  SoSearchAction searchAction;
  searchAction.setFind(SoSearchAction::NAME);
  searchAction.setName(aWhat.c_str());
  searchAction.setInterest(SoSearchAction::ALL);
  searchAction.apply(obj->fSep);
  SoPathList pathList = searchAction.getPaths();
  int number = pathList.getLength();
  if(!number) {
    // Not found ; look for a type :
    SoType type = SoType::fromName(aWhat.c_str());
    if(type==SoType::badType()) {
      std::ostream& out = fSession.cout();
      out << "Unknown type " << aWhat << std::endl;
      return;
    }
    SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(type);
    searchAction.setInterest(SoSearchAction::ALL);
    searchAction.apply(obj->fSep);
    pathList = searchAction.getPaths();
    number = pathList.getLength();
  }

  std::vector<std::string> words;
  inlib::words(aValue," ",false,words);

  if( (words.size()==2) && (words[0]=="manip") && (words[1]=="off") ) { 
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      if(node->isOfType(SoTransformManip::getClassTypeId())) { 
        ((SoTransformManip*)node)->replaceManip(path,NULL);
        ((SoTransformManip*)node)->unref();
      }
    }
  } else if( (words.size()==2) && (words[0]=="manip") ) { 
    SoType type = SoType::fromName(words[1].c_str());
    if(type==SoType::badType()) {
      std::ostream& out = fSession.cout();
      out << "Unknown type \"" << words[1] << "\"" << std::endl;
      return;
    }
    if(!type.isDerivedFrom(SoTransformManip::getClassTypeId())) {
      std::ostream& out = fSession.cout();
      out << "Accessor \"" << words[1] 
          << "\" not a SoTransformManip." << std::endl;
      return;
    }
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      if(node->isOfType(SoTransform::getClassTypeId())) { 
        SoTransformManip* manip = (SoTransformManip*)type.createInstance();
        if(manip) {
          manip->ref();
          manip->replaceNode(path);
        }
      }
    }

  } else if( (words.size()==4) && (words[0]=="diffuseColor") ) {

    //FIXME : do not do it if color is the same !

    // Have to look for an SoHighlightMaterial 
    // (that may come from a SoStyleCache) :
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      //::printf("debug : data_set : name : %lu \"%s\" \"%s\"\n",node,
      //  node->getName().getString(),node->getTypeId().getName().getString());
      if(node->getName()==SoTools::highlightName()) {
        SoHighlightMaterial* material = (SoHighlightMaterial*)node;
        SoMaterial* savedMaterial = material->getSavedMaterial();
        if(savedMaterial) {
          if(savedMaterial->getName()==SoStyleCache::cachedName()) {
            SoNode* cpy = savedMaterial->copy(); //then not cached
            if(cpy) {
              cpy->ref();
              cpy->setName("");
              cpy->set(aValue.c_str());
              material->setSavedMaterial((SoMaterial*)cpy);
              savedMaterial->unref();
            }      
          } else {
            savedMaterial->set(aValue.c_str());
          }
        } else {
          // Anomaly.
          std::ostream& out = fSession.cout();
          out << " \"highlight\" node without a savedMaterial !" << std::endl;
        }
      } else {
        if(node->getName()==SoStyleCache::cachedName()) {
          if(path->getLength()>=2) {
            SoNode* parent = path->getNodeFromTail(1);
            if(parent->isOfType(SoGroup::getClassTypeId())) { 
              SoGroup* grp = (SoGroup*)parent;
              SoNode* cpy = node->copy(); //then not cached.
              if(cpy) {
                grp->replaceChild(node,cpy);     
                cpy->setName("");
                cpy->set(aValue.c_str());
              }
            }        
          }      
        } else {
          node->set(aValue.c_str());
        }
      }
    }

  } else if( (words.size()==2) && (words[0]=="solid") ) {

    //FIXME : do not do it if solid is the same !

    bool value = false;
    if(!inlib::to(words[1],value)) { 
      std::ostream& out = fSession.cout();
      out << "\"" << words[1] << "\" not a boolean." << std::endl;
    }
    
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(node) node->set(aValue.c_str());
    }

    // Have to set the light model :
   {SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoLightModel::getClassTypeId());
    searchAction.setInterest(SoSearchAction::ALL);
    searchAction.apply(obj->fSep);
    pathList = searchAction.getPaths();
    number = pathList.getLength();
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      if(node->getName()==SoStyleCache::cachedName()) {
        if(path->getLength()>=2) {
          SoNode* parent = path->getNodeFromTail(1);
          if(parent->isOfType(SoGroup::getClassTypeId())) { 
            SoGroup* grp = (SoGroup*)parent;
            SoNode* cpy = node->copy(); //then not cached.
            if(cpy) {
              grp->replaceChild(node,cpy);     
              cpy->setName("");
              ((SoLightModel*)cpy)->model.setValue(value ?
                                              SoLightModel::PHONG:
                                              SoLightModel::BASE_COLOR);
            }
          }        
        }      
      } else {
        ((SoLightModel*)node)->model.setValue(value ?
                                              SoLightModel::PHONG:
                                              SoLightModel::BASE_COLOR);
      }
    }}

    // Have to set the draw style :
   {SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoDrawStyle::getClassTypeId());
    searchAction.setInterest(SoSearchAction::ALL);
    searchAction.apply(obj->fSep);
    pathList = searchAction.getPaths();
    number = pathList.getLength();
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      if(node->getName()==SoStyleCache::cachedName()) {
        if(path->getLength()>=2) {
          SoNode* parent = path->getNodeFromTail(1);
          if(parent->isOfType(SoGroup::getClassTypeId())) { 
            SoGroup* grp = (SoGroup*)parent;
            SoNode* cpy = node->copy(); //then not cached.
            if(cpy) {
              grp->replaceChild(node,cpy);     
              cpy->setName("");
              ((SoDrawStyle*)cpy)->style.setValue(value ?
                                             SoDrawStyle::FILLED:
                                             SoDrawStyle::LINES);
            }
          }        
        }      
      } else {
        ((SoDrawStyle*)node)->style.setValue(value ?
                                             SoDrawStyle::FILLED:
                                             SoDrawStyle::LINES);
      }
    }}

    // Have to set the material emissiveColor (because of VRML) :
   {SoSearchAction searchAction;
    searchAction.setFind(SoSearchAction::TYPE);
    searchAction.setType(SoMaterial::getClassTypeId());
    searchAction.setInterest(SoSearchAction::ALL);
    searchAction.apply(obj->fSep);
    pathList = searchAction.getPaths();
    number = pathList.getLength();
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(!node) continue;
      //::printf("debug : data_set : name : %lu \"%s\" \"%s\"\n",node,
      //  node->getName().getString(),node->getTypeId().getName().getString());
      if(node->getName()==SoTools::highlightName()) {
        SoHighlightMaterial* material = (SoHighlightMaterial*)node;
        if(value) { //want solid 
          ((SoMaterial*)material)->emissiveColor.setValue(SbColor_black);
        } else {
          ((SoMaterial*)material)->emissiveColor = 
             ((SoMaterial*)material)->diffuseColor;
        }
        SoMaterial* savedMaterial = material->getSavedMaterial();
        if(savedMaterial) {
          if(savedMaterial->getName()==SoStyleCache::cachedName()) {
            SoNode* cpy = savedMaterial->copy(); //then not cached
            if(cpy) {
              cpy->ref();
              cpy->setName("");
              if(value) { //want solid 
                ((SoMaterial*)cpy)->emissiveColor.setValue(SbColor_black);
              } else {
                ((SoMaterial*)cpy)->emissiveColor = 
                  ((SoMaterial*)cpy)->diffuseColor;
              }
              material->setSavedMaterial((SoMaterial*)cpy);
              savedMaterial->unref();
            }      
          } else {
            if(value) { //want solid 
              ((SoMaterial*)savedMaterial)->emissiveColor.setValue
                  (SbColor_black);
            } else {
              ((SoMaterial*)savedMaterial)->emissiveColor = 
                ((SoMaterial*)savedMaterial)->diffuseColor;
            }
          }
        } else {
          // Anomaly.
          std::ostream& out = fSession.cout();
          out << " \"highlight\" node without a savedMaterial !" << std::endl;
        }
      } else {
        if(node->getName()==SoStyleCache::cachedName()) {
          if(path->getLength()>=2) {
            SoNode* parent = path->getNodeFromTail(1);
            if(parent->isOfType(SoGroup::getClassTypeId())) { 
              SoGroup* grp = (SoGroup*)parent;
              SoNode* cpy = node->copy(); //then not cached.
              if(cpy) {
                grp->replaceChild(node,cpy);     
                cpy->setName("");

                if(value) { //want solid 
                  ((SoMaterial*)cpy)->emissiveColor.setValue(SbColor_black);
                } else {
                  ((SoMaterial*)cpy)->emissiveColor = 
                    ((SoMaterial*)cpy)->diffuseColor;
                }
              }
            }        
          }      
        } else {
          if(value) { //want solid 
            ((SoMaterial*)node)->emissiveColor.setValue(SbColor_black);
          } else {
            ((SoMaterial*)node)->emissiveColor = 
              ((SoMaterial*)node)->diffuseColor;
          }
        }
      }
    }}

  } else {
    for(int index=0;index<number;index++) {
      SoFullPath* path = (SoFullPath*)pathList[index];
      SoNode* node = path->getTail();
      if(node) node->set(aValue.c_str());
    }
  }
}

void OnX::SceneGraphAccessor::getSceneGraphs(const std::vector<std::string>& aArgs) {
  clear();

  if(!aArgs.size()) {
    std::ostream& out = fSession.cout();
    out << "OnX::SceneGraphAccessor::getSceneGraphs :" 
        << " no viewer specified." 
        << std::endl;
    return;
  }

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(fSession);
  if(!sh) {
    std::ostream& out = fSession.cout();
    out << "OnX::SceneGraphAccessor::getSceneGraphs :" 
        << " scene handler not found." 
        << std::endl;
    return;
  }

  SoNode* node = sh->find(aArgs[0],Slash_topSeparator);
  if(!node) {
    std::ostream& out = fSession.cout();
    out << "OnX::SceneGraphAccessor::getSceneGraphs :" 
        << " " << Slash_topSeparator 
        << " not found in viewer " << inlib::sout(aArgs[0]) << "."
        << std::endl;
    return;
  }
 
  SoPathList* list = SoTools::getSceneGraphs(node);
  if(!list) return;

  unsigned int number = list->getLength();
  if(number) {
    fSceneGraphs = new SceneGraph[number];
    if(fSceneGraphs) {
      for(unsigned int index=0;index<number;index++) {
        SceneGraph* sg = &(fSceneGraphs[index]);

        SoFullPath* path = (SoFullPath*)(*list)[index];
        SoNode* pnode = path->getTail();
        if(pnode->isOfType(SoInfo::getClassTypeId())==TRUE) {
          int l = path->getLength();
          if(l>=3) {
            sg->fSep = path->getNodeFromTail(1);
            sg->fParent = path->getNodeFromTail(2);
            sg->fInfo = path->getNodeFromTail(0);
          } else {
            std::ostream& out = fSession.cout();
            out << "OnX::SceneGraphAccessor::getSceneGraphs :" 
                << " strange picked SoInfo path."
                << std::endl;
          }
        } else { 
          //pnode is a SoSceneGraph or a SoSeparator named "sceneGraph" .
          sg->fInfo = 0;
          sg->fSep = path->getTail();
          int l = path->getLength();
          sg->fParent = l>=2 ? path->getNodeFromTail(1) : 0;
        }
      }
      fNumber = number;
    }
  }

  delete list;
}

void OnX::SceneGraphAccessor::clear() {
  delete [] fSceneGraphs;
  fSceneGraphs = 0;
  fNumber = 0;
}
