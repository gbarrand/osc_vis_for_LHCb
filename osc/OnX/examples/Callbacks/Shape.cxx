//
// Example of declaration of some "user" data (here class Shape) 
// to the AccessorManager.
//


// "user" class to declare to the AccessorManager.
// All the code is in Shape.h.
#include "Shape.h"

//////////////////////////////////////////////////////////////////////////////
// Code to declare the user class to OnX.  ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// The OnX_ShapeManager manages data (here instances of class OnX::Shape)
// or permits to access the entity that manages the data.
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Slash/Core/IManager.h>

#include <vector>
#include <Slash/Core/ISession.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/DummySceneHandler.h>

#include <inlib/smatch>
#include <inlib/random>
#include <inlib/system>
#include <inlib/sprintf>
#include <inlib/cast>

namespace OnX {
class ShapeManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::ShapeManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  ShapeManager(Slash::Core::ISession& aSession)
  :fName("OnX_ShapeManager"){
    // Create some data.
    fShapes.push_back(new Shape("Cube",1, 10, 10,0));
    fShapes.push_back(new Shape("Cube",2,-10, 10,0));
    fShapes.push_back(new Shape("Cube",3,-10,-10,0));
    fShapes.push_back(new Shape("Cube",4, 10,-10,0));
    //Have an intersection with the previous one :
    fShapes.push_back(new Shape("Cube",4, 13,-10,0));
  
    fShapes.push_back(new Shape("Sphere",1,0, 10, 10));
    fShapes.push_back(new Shape("Sphere",2,0,-10, 10));
    fShapes.push_back(new Shape("Sphere",3,0,-10,-10));
    fShapes.push_back(new Shape("Sphere",4,0, 10,-10));
    //Have an intersection with the previous one :
    fShapes.push_back(new Shape("Sphere",4,0, 17.9,-10));
    fShapes.push_back(new Shape("Cylinder",1, 10,0, 10));
    fShapes.push_back(new Shape("Cylinder",2,-10,0, 10));
    fShapes.push_back(new Shape("Cylinder",3,-10,0,-10));
    fShapes.push_back(new Shape("Cylinder",5, 10,0,-10));
    fShapes.push_back(new Shape("Cone",1,0,0,0));
    fShapes.push_back(new Shape("Hole",1,10,10,10));
    fShapes.push_back(new Shape("Hole",1,-10,-10,-10));
    // Have one centered at origin to test shape cuts :
    fShapes.push_back(new Shape("Hole",2,0,0,0));
  
    // To test line drawing :
    inlib::random::flat r;
    double s = 100; //scale.
    fShapes.push_back
      (new Shape("Track1",0,s*r.shoot(),s*r.shoot(),s*r.shoot()));
    fShapes.push_back
      (new Shape("Track2",0,s*r.shoot(),s*r.shoot(),s*r.shoot()));
    fShapes.push_back
      (new Shape("Track3",0,s*r.shoot(),s*r.shoot(),s*r.shoot()));
    fShapes.push_back
      (new Shape("Track4",0,s*r.shoot(),s*r.shoot(),s*r.shoot()));
    fShapes.push_back
      (new Shape("Track5",0,s*r.shoot(),s*r.shoot(),s*r.shoot()));
  
    Slash::UI::IStyleManager* styleManager = Slash::styleManager(aSession);
    if(styleManager) {
      std::string file;
      std::string OSC_HOME_DIR;
      if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
        aSession.cout() << "OnX::ShapeManager::ShapeManager :"
                        << " environment variable OSC_HOME_DIR not defined."
                        << std::endl;
      } else {
        file = OSC_HOME_DIR + "/Resources/OnX/examples/OnX/shapes.style";
      }
      if(file.size()) {
        if(!styleManager->loadFile(file)) {
          aSession.cout() << "OnX::ShapeManager::ShapeManager :"
                          << " load of style file " << inlib::sout(file) 
                          << " failed."
                          << std::endl;
        }
      }
    }
  }

  virtual ~ShapeManager(){
    for(unsigned int index=0;index<fShapes.size();index++) 
      delete fShapes[index];
    fShapes.clear();
  }
private:
  ShapeManager(const ShapeManager&): Slash::Core::IManager() {}
  ShapeManager& operator=(const ShapeManager&){return *this;}
public:
  inline std::vector<OnX::Shape*>& shapes(){return fShapes;}
private:
  std::string fName;
  std::vector<Shape*> fShapes;
};
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// The Shape Accessor permits to filter the data with Slash::Data::collect)
// and order visualization (with Slash::Data::visualize).
//  See the below "OnX callback " OnXExas_shape_visualize.
// and file : OnX/examples/OnX/shapes.onx.
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Inheritance :
#include <Slash/Data/IIterator.h>
#include <Slash/Tools/InventorAccessor.h>
#include <Slash/Tools/Value.h>

namespace OnX {
class ShapeAccessorIterator : public virtual Slash::Data::IIterator {
public: //Slash::Data::IIterator
  virtual Slash::Data::IAccessor::Data object() {
    if(fIndex>=fNumber) return 0;
    return fList[fIndex];
  }
  virtual void next() { fIndex++;}
  virtual void* tag() { return 0;}
public:
  ShapeAccessorIterator(std::vector<OnX::Shape*>& aList)
  :fIndex(0),fList(aList),fNumber(aList.size()) {
    //Lib::Debug::increment("OnX::ShapeAccessorIterator");
  }
  virtual ~ShapeAccessorIterator() {
    //Lib::Debug::decrement("OnX::ShapeAccessorIterator");
  }
private:
  unsigned int fIndex;
  std::vector<OnX::Shape*>& fList;
  unsigned int fNumber;
};
}

namespace OnX {

class ShapeAccessor : public Slash::InventorAccessor {
public: //Slash::Data::IAccessor
  virtual std::string name() const {return fName;}

  virtual Slash::Data::IIterator* iterator() {
    // Returned object should be deleted by the receiver.
    // Emulated a HEP "Event" with tracks :
    inlib::random::flat r;
    double s = 100; //scale.
    for(unsigned int index=0;index<fShapes.size();index++) {
      if(inlib::match(fShapes[index]->type(),"Track*")) {
        fShapes[index]->setSize(s*r.shoot(),s*r.shoot(),s*r.shoot());
      }
    }
    return new ShapeAccessorIterator(fShapes);
  }

  virtual Slash::Core::IValue* findValue(
    Slash::Data::IAccessor::Data aData
   ,const std::string& aName
   ,void*
  ){
    Shape* obj = (Shape*)aData;
    if(aName=="address") {
      return new Slash::Value((void*)obj);
    } else if(aName=="type") {
      return new Slash::Value(obj->type());
    } else if(aName=="size") {
      return new Slash::Value(obj->size());
    } else if(aName=="x") {
      return new Slash::Value(obj->x());
    } else if(aName=="y") {
      return new Slash::Value(obj->y());
    } else if(aName=="z") {
      return new Slash::Value(obj->z());
    } else if(aName=="hits") {
      unsigned int order = 10;
      std::vector<double> vec(order);
      for(unsigned int index=0;index<order;index++) vec[index] = double(index);
      return new Slash::Value(vec);
    } else {
      return new Slash::Value();
    }
  }


public: //Slash::Data::IVisualizer
  virtual void visualize(Slash::Data::IAccessor::Data,void*); // done below.

public:
  ShapeAccessor(Slash::Core::ISession& aSession,
                Slash::UI::ISceneHandler& aSH,
                std::vector<Shape*>& aShapes)
  : Slash::InventorAccessor(aSession,aSH)
  ,fName("OnX_Shape")
  ,fShapes(aShapes){
    addProperty("type",Slash::Data::IProperty::STRING);
    addProperty("size",Slash::Data::IProperty::DOUBLE);
    addProperty("x",Slash::Data::IProperty::DOUBLE);
    addProperty("y",Slash::Data::IProperty::DOUBLE);
    addProperty("z",Slash::Data::IProperty::DOUBLE);
    addProperty("hits",Slash::Data::IProperty::VECTOR_DOUBLE);
    addProperty("address",Slash::Data::IProperty::POINTER);
  }
  virtual ~ShapeAccessor(){}
private:
  ShapeAccessor(const ShapeAccessor& aFrom)
  : Slash::InventorAccessor(aFrom)
  ,fShapes(aFrom.fShapes)
  {}
  ShapeAccessor& operator=(const ShapeAccessor&){return *this;}
private:
  std::string fName;
  std::vector<Shape*>& fShapes;
};
}

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>

// HEPVis :
#include <HEPVis/SbPolyhedron.h>
#include <HEPVis/nodes/SoSceneGraph.h>
#include <HEPVis/nodes/SoPolyhedron.h>
#include <HEPVis/nodes/SoTextTTF.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>

void OnX::ShapeAccessor::visualize(Slash::Data::IAccessor::Data aData,void*) {
  //if(!fSoRegion) return;
  Slash::UI::IUI* ui = Slash::find_UI(fSession);
  if(!ui) return;

  Shape* obj = (Shape*)aData;

  std::string style = "OnX_Shape_"+obj->type();
  fillSoGC(style); //Get a style corresponding to shape type.

  // Build scene graph :
  SoSceneGraph* separator = new SoSceneGraph;
  separator->ref(); //ref=1  

  separator->addChild(fSoGC.getHighlightMaterial());
  separator->addChild(fSoGC.getDrawStyle());
  separator->addChild(fSoGC.getLightModel());

  // Build name (for picking) :
  std::string s;
  inlib::sprintf(s,128,"OnX_Shape/0x%lx",(unsigned long)obj);
  separator->setString(s.c_str());

  SoTransform* transform = new SoTransform;
  transform->translation.setValue(SbVec3f((float)obj->x(),
                                          (float)obj->y(),
                                          (float)obj->z()));
  const SbString& modeling = fSoGC.getModeling();
  if(obj->type()=="Cube") {
    separator->addChild(transform);
    if(modeling==SbModeling_texts) {
      SoTextTTF* text = new SoTextTTF;
      //text->rescale.setValue(TRUE);
      text->size.setValue(5);
      text->string.set1Value(0,"Cube");
      separator->addChild(text);
    } else {
/*
      SoCube* cube = new SoCube;
      cube->width = (float)obj->size();
      cube->height = (float)obj->size();
      separator->addChild(cube);
      cube->depth = (float)obj->size();
*/
      //To have shape cut.
      float sz2 = (float)obj->size()/2.F;
      SbPolyhedronBox box(sz2,sz2,sz2);
      //FIXME : with SoSceneGraph we can try cache of polyhedron.
      //SoPolyhedron* soPolyhedron = fSoGC.getPolyhedron(box);
      SoPolyhedron* soPolyhedron = new SoPolyhedron(box);
      separator->addChild(soPolyhedron);
    }
  } else if(obj->type()=="Sphere") {
    separator->addChild(transform);
    if(modeling==SbModeling_texts) {
      SoTextTTF* text = new SoTextTTF;
      text->string.set1Value(0,"Sphere");
      text->size.setValue(20);
      // Ask to the node to rescale the text according the viewport size.
      // 200 is the viewport size for the wanted upper size.
      text->rescale.setValue(200);
      separator->addChild(text);
    } else {
/*
      SoSphere* sphere = new SoSphere;
      sphere->radius = (float)obj->size();
      separator->addChild(sphere);
*/
      //To have shape cut.
      float radius = (float)obj->size();
      SbPolyhedronSphere sphere(0,radius,0,2*M_PI,0,M_PI);
      //SoPolyhedron* soPolyhedron = fSoGC.getPolyhedron(sphere);
      SoPolyhedron* soPolyhedron = new SoPolyhedron(sphere);
      separator->addChild(soPolyhedron);
    }
  } else if(obj->type()=="Cone") {
    separator->addChild(transform);
    //WARNING : shape cut does not apply on SoCone.
    SoCone* cone = new SoCone;
    cone->bottomRadius = (float)obj->size();
    cone->height = (float)obj->size();
    separator->addChild(cone);
  } else if(obj->type()=="Cylinder") {
    separator->addChild(transform);
    //WARNING : shape cut does not apply on SoCone.
    SoCylinder* cylinder = new SoCylinder;
    cylinder->radius = (float)obj->size();
    cylinder->height = (float)obj->size();
    separator->addChild(cylinder);
  } else if(obj->type()=="Hole") {
    separator->addChild(transform);

    /* A flat box with a hole :
    SbPolyhedronBox box(1,1,0.1);
    SbPolyhedronTubs tubs(0,1,1,0,2*M_PI);
    SbPolyhedron op = box.subtract(tubs);
    */
    float scale = (float)obj->size();

    // A Tube with a transverse hole :
    SbPolyhedronTubs tubs_1(scale*0.7,scale*1.5,scale*2,0,2*M_PI);
    SbPolyhedronTubs tubs_2(scale*0,scale*0.5,scale*4,0,2*M_PI);
    tubs_2.Transform
      (HEPVis::SbRotation(SbVec3d(0,1,0),M_PI/2.),SbVec3d(0,0,0));
    SbPolyhedron op = tubs_1.subtract(tubs_2);

    //SoPolyhedron* soPolyhedron = fSoGC.getPolyhedron(op);
    SoPolyhedron* soPolyhedron = new SoPolyhedron(op);
    if(modeling==SbModeling_wire_frame) {
      soPolyhedron->solid.setValue(FALSE);
      soPolyhedron->reducedWireFrame.setValue(TRUE);
    } else {
      soPolyhedron->solid.setValue(TRUE);
    }

    separator->addChild(soPolyhedron);

  } else if(obj->type().substr(0,5)=="Track") {
    transform->unref();

    int32_t pointn = 2;
    SbVec3f points[2];
    points[0].setValue(0,0,0);
    points[1].setValue((float)obj->x(),(float)obj->y(),(float)obj->z());
 
    SoCoordinate3* coordinate3 = new SoCoordinate3;
    coordinate3->point.setValues(0,pointn,points);
    separator->addChild(coordinate3);

    SoLineSet* lineSet = new SoLineSet;
    lineSet->numVertices.setValues(0,1,&pointn);
    separator->addChild(lineSet);
  }
  
  addScene(separator,"dynamicScene");

  separator->unref();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// OnX callback to declare the ShapeManager to the Slash::Core::ISession 
// and declare ShapeAccessor to the accessor manager (access through 
// the Slash::Core::ISession).
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/Session.h>

#include <inlib/args>

extern "C" {

void OnXExas_shape_initialize_sess(Slash::Core::ISession& aSession) {
  // Should be executed before any use of Slash::Data::collect("OnX_Shape").

  OnX::ShapeManager* shapeManager = 
    Slash_findManager(aSession,"OnX_ShapeManager",OnX::ShapeManager);
  if(!shapeManager) {
    shapeManager = new OnX::ShapeManager(aSession);
    aSession.addManager(shapeManager);
  }

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(aSession);
  if(!sh) {
    //std::ostream& out = aSession.cout();
    //out << "OnXExas_shape_initialize_sess :"
    //    << " scene handler not found."
    //    << std::endl;
    //NOTE : this may happen.
    //       See AIDA/examples/cpp/osc/slash_shape_collect_tuple.
    aSession.addManager(new Slash::DummySceneHandler(Slash_SceneHandler));
  }

  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(accessorManager && !accessorManager->findAccessor("OnX_Shape")) {
    accessorManager->addAccessor
      (new OnX::ShapeAccessor(aSession,*sh,shapeManager->shapes()));
  }
}

void OnXExas_shape_initialize(Slash::UI::IUI& aUI) {
  OnXExas_shape_initialize_sess(aUI.session());
}

#include <Slash/Tools/Style.h>

void OnXExas_shape_visualize(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] where
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& where = aArgs[0];

  Slash::Core::ISession& session = aUI.session();

  Slash::Data::collect(session,"OnX_Shape","type==\"Cone\"");
  // There is no style for Cone in shapes.style, then it is
  // the session modeling style which is taken.
  Slash::set_color(session,"cyan");
  session.setParameter("modeling.modeling","solid");
  session.setParameter("modeling.transparency","0");
  Slash::Data::visualize(session,where);

  Slash::Data::collect(session,"OnX_Shape","type==\"Cube\"");
  // If OnX/examples/OnX/shapes.style loaded,
  // according to the fillSoGC of OnX::ShapeAccessor::visualize,
  // it is OnX_Shape_Cube style which is used (for color, etc...).
  // If so color of cubes should be red.
  Slash::Data::visualize(session,where);

  Slash::Data::collect(session,"OnX_Shape","(type==\"Hole\")&&(size<=1)");
  // Style taken from shapes.style file. Note that the
  // Hole style name in the file has a wildard.
  // Color of Hole shape should be blue
  Slash::Data::visualize(session,where);

  Slash::Data::collect(session,"OnX_Shape","(type==\"Hole\")&&(size>1)");
  // have a new style "big_hole" deposited on the style manager :
  std::string style("big_hole");
  Slash::UI::IStyleManager* styleManager = Slash::styleManager(session);
  if(styleManager && !styleManager->isStyle(style)) {
    styleManager->addStyle(style,"color yellow\nmodeling solid");
  }
  // solid hole shapes should be yellow.
  Slash::Data::visualize(session,where,style); //visualize with big_hole.
   
  Slash::Data::collect(session,"OnX_Shape","type==\"Track*\"");
  // Style taken from shapes.style.
  // Note that we had created Track<int> shape types.
  Slash::Data::visualize(session,where);

  Slash::Data::collect(session,"OnX_Shape","(type==\"Sphere\")&&(size<=2)");
  // Style taken from shapes.style.
  Slash::Data::visualize(session,where);

 {Slash::Data::collect(session,"OnX_Shape","(type==\"Sphere\")&&(size>2)");
  // local style.
  Slash::Style style;
  style.set("color","green");
  style.set("modeling","solid");
  // then big spheres should be in solid red.
  Slash::Data::visualize(session,style,where);}

}

}
