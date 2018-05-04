// this :
#include <OnXTemplateSix/Track_Accessor.h>

// Slash :
#include <Slash/Data/IIterator.h>

#include <HEPVis/nodes/SoSceneGraph.h>

#include <Slash/Tools/Value.h>

// User :
#include <User/Framework.h>
#include <User/Event.h>
#include <User/Track.h>

OnXTemplateSix::Track_Accessor::Track_Accessor(
 Slash::Core::ISession& aSession
,Slash::UI::ISceneHandler& aSH
,const User::Framework& aUserFramework
)
: Slash::InventorAccessor(aSession,aSH)
,fName("User_Track")
,fUserFramework(aUserFramework)
{
  addProperty("px",Slash::Data::IProperty::DOUBLE);
  addProperty("py",Slash::Data::IProperty::DOUBLE);
  addProperty("pz",Slash::Data::IProperty::DOUBLE);
}

OnXTemplateSix::Track_Accessor::~Track_Accessor() {}
std::string OnXTemplateSix::Track_Accessor::name() const {return fName;}

namespace OnXTemplateSix {
class Track_AccessorIterator : 
public virtual Slash::Data::IIterator {
public: //Slash::Data::IIterator
  virtual Slash::Data::IAccessor::Data object() {
    if(fIndex>=fNumber) return 0;
    return (void*)&(fList[fIndex]);
  }
  virtual void next() { fIndex++;}
  virtual void* tag() { return 0;}
public:
  Track_AccessorIterator(const std::vector<User::Track>& aList)
  :fIndex(0),fList(aList),fNumber(aList.size()) {}
  virtual ~Track_AccessorIterator(){}
private:
  unsigned int fIndex;
  const std::vector<User::Track>& fList;
  unsigned int fNumber;
};
}

Slash::Data::IIterator* OnXTemplateSix::Track_Accessor::iterator() {
  // Returned object should be deleted by the receiver.
  const User::Event& event = fUserFramework.currentEvent();
  return new Track_AccessorIterator(event.tracks());
}

Slash::Core::IValue* OnXTemplateSix::Track_Accessor::findValue(
 Slash::Data::IAccessor::Data aIdentifier
,const std::string& aName
,void*
) {
  User::Track* obj = (User::Track*)aIdentifier;
  if(aName=="px") {
    return new Slash::Value(obj->px());
  } else if(aName=="py") {
    return new Slash::Value(obj->py());
  } else if(aName=="pz") {
    return new Slash::Value(obj->pz());
  } else {
    return new Slash::Value();
  }
}

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>

// HEPVis :
#include <HEPVis/nodes/SoHighlightMaterial.h>

void OnXTemplateSix::Track_Accessor::visualize(Slash::Data::IAccessor::Data aData,void*) {
  User::Track* obj = (User::Track*)aData;

  //std::string style = "User_Track";
  //fillSoGC(style); //Get a style corresponding to shape type.

  // Build name (for picking) :
  char s[128];
  ::sprintf(s,"User_Track/0x%lx",(unsigned long)obj);

  // Build scene graph :
  SoSceneGraph* separator = new SoSceneGraph;
  separator->ref();

  separator->setString(s);

  SoHighlightMaterial* material = new SoHighlightMaterial;
  material->diffuseColor.setValue(0,0,1);
  material->highlightColor.setValue(1,0,0);
  separator->addChild(material);

  SoDrawStyle* drawStyle = new SoDrawStyle;
  drawStyle->style.setValue(SoDrawStyle::LINES);
  separator->addChild(drawStyle);

  SoLightModel* lightModel = new SoLightModel;
  lightModel->model.setValue(SoLightModel::BASE_COLOR);
  separator->addChild(lightModel);

  int32_t pointn = 2;
  SbVec3f points[2];
  points[0].setValue(0,0,0);
  points[1].setValue((float)obj->px(),(float)obj->py(),(float)obj->pz());
 
  SoCoordinate3* coordinate3 = new SoCoordinate3;
  coordinate3->point.setValues(0,pointn,points);
  separator->addChild(coordinate3);

  SoLineSet* lineSet = new SoLineSet;
  lineSet->numVertices.setValues(0,1,&pointn);
  separator->addChild(lineSet);
  
  addScene(separator,"dynamicScene");
 
  separator->unref();
}

