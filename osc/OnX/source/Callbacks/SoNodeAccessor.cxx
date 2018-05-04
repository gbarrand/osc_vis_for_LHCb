// this :
#include "SoNodeAccessor.h"

// Slash :
#include <Slash/Data/IIterator.h>
#include <Slash/UI/IStyle.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Value.h>
#include <Slash/Tools/UI.h>

#include <inlib/args>

#include <Lib/Debug.h>

// Inventor :
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoSearchAction.h>

OnX::SoNodeAccessor::SoNodeAccessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.out())
,fSession(aSession)
,fName("SoNode")
,fSoNodes(0)
,fNumber(0)
,fSH(0)
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("sotype",Slash::Data::IProperty::STRING);

  fSH = Slash::sceneHandler(fSession);
  if(!fSH) {
    std::ostream& out = fSession.cout();
    out << "OnX::SoNodeAccessor::SoNodeAccessor :" 
        << " scene handler not found." 
        << std::endl;
  }

}

OnX::SoNodeAccessor::~SoNodeAccessor() {clear();}

std::string OnX::SoNodeAccessor::name() const {return fName;}

namespace OnX {
class SoNodeAccessorIterator : public virtual Slash::Data::IIterator {
public: //Slash::Data::IIterator
  virtual Slash::Data::IAccessor::Data object() {
    if(fIndex>=fNumber) return 0;
    return fList[fIndex];
  }
  virtual void next() {fIndex++;}
  virtual void* tag() { return 0;}
public:
  SoNodeAccessorIterator(SoNode** aList,unsigned int aNumber)
    :fIndex(0),fList(aList),fNumber(aNumber) {
    Lib::Debug::increment("OnX::SoNodeAccessorIterator");
  }
  virtual ~SoNodeAccessorIterator() {
    Lib::Debug::decrement("OnX::SoNodeAccessorIterator");
  }
private:
  unsigned int fIndex;
  SoNode** fList;
  unsigned int fNumber;
};
}

Slash::Data::IIterator* OnX::SoNodeAccessor::iterator() {
  // Returned object should be deleted by the receiver.
  getSoNodes(iteratorArguments());
  return new SoNodeAccessorIterator(fSoNodes,fNumber);
}

Slash::Core::IValue* OnX::SoNodeAccessor::findValue(
 Slash::Data::IAccessor::Data aData
,const std::string& aName
,void*
){
  SoNode* obj = (SoNode*)aData;

  if(aName=="name") {
    return new Slash::Value(obj->getName().getString());
  } else if(aName=="sotype") {
    return new Slash::Value(obj->getTypeId().getName().getString());

  } else {
    return new Slash::Value();
  }
}

void OnX::SoNodeAccessor::getSoNodes(const std::vector<std::string>& aArgs) {
  clear();

  if(!aArgs.size()) {
    std::ostream& out = fSession.cout();
    out << "OnX::SoNodeAccessor::getSoNodes :" 
        << " no viewer specified." 
        << std::endl;
    return;
  }

  Slash::UI::IUI* ui = Slash::find_UI(fSession);
  if(!ui) {
    std::ostream& out = fSession.cout();
    out << "OnX::SoNodeAccessor::getSoNodes :" 
        << " UI not found." 
        << std::endl;
    return;
  }

  Slash::UI::ISoViewer* viewer = Slash::find_SoViewer(*ui,aArgs[0]);
  if(!viewer) {
    std::ostream& out = fSession.cout();
    out << "OnX::SoNodeAccessor::getSoNodes :" 
        << " viewer " << inlib::sout(aArgs[0]) 
        << " not found." 
        << std::endl;
    return;
  }

  SoNode* node = viewer->sceneGraph();
  if(!node) {
    std::ostream& out = fSession.cout();
    out << "OnX::SoNodeAccessor::getSoNodes :" 
        << " viewer " << inlib::sout(aArgs[0]) 
        << " has no scene graph."
        << std::endl;
    return;
  }
 
  SbBool oldsearch = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  SoSearchAction sa;
  sa.setFind(SoSearchAction::TYPE);
  sa.setType(SoNode::getClassTypeId());
  //saT.setSearchingAll(TRUE); //to search all under an SoSwitch.
  sa.setInterest(SoSearchAction::ALL);
  sa.apply(node);
  SoBaseKit::setSearchingChildren(oldsearch);

  SoPathList& list = sa.getPaths();
  unsigned int number = list.getLength();
  if(!number) return;
  typedef SoNode* SoNodePointer;
  fSoNodes = new SoNodePointer[number];
  if(!fSoNodes) return;
  for(unsigned int index=0;index<number;index++) {
    SoFullPath* path = (SoFullPath*)list[index];
    fSoNodes[index] = path->getTail();
  }
  fNumber = number;
}

void OnX::SoNodeAccessor::clear() {
  delete [] fSoNodes;
  fSoNodes = 0;
  fNumber = 0;
}

void OnX::SoNodeAccessor::beginVisualize() {
  fPlacement = "";

  if(!fSH) return;
  const std::vector<std::string>& eargs = fExecuteArgs;
  inlib::args args(eargs);
  std::string where;
  args.find("where",where);

  // keep consitent with Slash::find_SoRegion '@' logic 
  std::string viewer = where;
  if(where!=Slash::s_current()) {
    std::string::size_type pos = where.rfind('@');  
    if(pos!=std::string::npos) { 
      //where = "<string>@<string>" (but not @current@)
      //where = @current@@<string> works too.
      viewer = where.substr(0,pos);
      fPlacement = where.substr(pos+1,where.size()-(pos+1));
    }
  }

  fSH->setViewer(viewer);

  //std::ostream& out = fSession.cout();
  //out << "OnX::SoNodeAccessor::beginVisualize :" 
  //    << " viewer " << inlib::sout(viewer)
  //    << " placement " << inlib::sout(fPlacement)
  //    << std::endl;

}

void OnX::SoNodeAccessor::beginVisualize(const Slash::UI::IStyle& aStyle) {
  OnX::SoNodeAccessor::beginVisualize();
}

void OnX::SoNodeAccessor::visualize(Slash::Data::IAccessor::Data aData,void*) {
  SoNode* obj = (SoNode*)aData;
  if(!fSH) return;
  fSH->addScene(fPlacement,obj);
}
