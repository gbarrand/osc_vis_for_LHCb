
inline void to_lines(const std::string& a_string,std::vector<std::string>& a_lines) {
  inlib::words(a_string,"\n",false,a_lines);  
}

//////////////////////////////////////////////////////////////////////////////
void OnX::BaseUI::addFactory(
 Slash::UI::IWidgetFactory* aFactory
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fFactories.push_back(aFactory);
}
//////////////////////////////////////////////////////////////////////////////
inlib::xml::tree* OnX::BaseUI::topItemML(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTopItemML;
}
//////////////////////////////////////////////////////////////////////////////
inlib::xml::factory& OnX::BaseUI::itemMLFactory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fWidgetFactory;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::BaseUI::addWidgetClass(
 Slash::UI::IWidgetClass* aClass
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fWidgetClasses.push_back(aClass);
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::BaseUI::createWidgetName(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string name;
  inlib::sprintf(name,256,"OnX_Component_%d",fWidgetNameCounter);
  fWidgetNameCounter++;
  return name;
}
//////////////////////////////////////////////////////////////////////////////
static bool isGeneratedName(
 const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //OnX_Component_
  //012345678901234
  if(aString.size()<=14) return false;
  int i;
  return inlib::to<int>(aString.substr(14,aString.size()-14),i);
}
//////////////////////////////////////////////////////////////////////////////
void OnX::BaseUI::createWidgetTable(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> sChilds;
  std::vector<std::string> sAttrs;
  std::vector<std::string> sProps;
  std::vector<std::string> sCbs;

  //tooltip, pixmap : only at the creation
  std::string containers = "\
VContainer\n\
HContainer\n\
TabStack\n\
VBox\n\
HBox\n\
VPaned\n\
HPaned\n\
Reference";
  std::string leaves = "\
ScrolledText\n\
ScrolledList\n\
ScrolledTree\n\
VScrollBar\n\
HScrollBar\n\
VScale\n\
HScale\n\
ComboBox\n\
Label\n\
PushButton\n\
CommandLine\n\
OptionMenu\n\
ProgressBar\n\
OpenGLArea\n\
PageViewer\n\
PlanePageViewer\n\
Entry\n\
Password\n\
ToggleButton";

  // CallbackList "valueChanged activate create select complete collect drag increment decrement popup update" paint event"


  // Containers

  to_lines("Window\nDialog\nReference",sChilds);
  to_lines("name\nclass\nversion",sAttrs);
  to_lines("",sProps);
  to_lines("create",sCbs);
  //FIXME fWidgetClasses.push_back(new WidgetClass("OnX",sChilds,sAttrs,sProps,sCbs));

  // no more attributes except "name" and "class"
  to_lines("name\nclass",sAttrs);

  to_lines(std::string("Window\nMenuBar\nDialog\nToolBar\nWarningDialog")+"\n"+containers +"\n"+leaves,sChilds);
  to_lines("geometry\ndecoration\ntitle\nshow\nbackground\nforeground",sProps);
  to_lines("create",sCbs);
  to_lines("close",sCbs);
  fWidgetClasses.push_back(new WidgetClass("Window",sChilds,sAttrs,sProps,sCbs));

  to_lines("CascadeButton\nReference",sChilds);
  to_lines("background\nforeground",sProps);
  to_lines("",sCbs);
  fWidgetClasses.push_back(new WidgetClass("MenuBar",sChilds,sAttrs,sProps,sCbs));

  to_lines("MenuItem\nMenuSeparator\nToggleMenuItem\nCascadeButton\nReference",sChilds);
  to_lines("background\nforeground",sProps);
  to_lines("",sCbs);
  fWidgetClasses.push_back(new WidgetClass("Menu",sChilds,sAttrs,sProps,sCbs));

  to_lines("Menu\nReference",sChilds);
  to_lines("label\npixmap\ntooltip\nbackground\nforeground",sProps);
  to_lines("activate",sCbs);
  to_lines("cascading",sCbs);
  fWidgetClasses.push_back(new WidgetClass("CascadeButton",sChilds,sAttrs,sProps,sCbs));

  to_lines("ToolBarItem\nComboBox\nLabel\nPushButton\nCommandLine\nOptionMenu\nProgressBar\nPassword\nEntry",sChilds);
  to_lines("background\nforeground",sProps);
  to_lines("",sCbs);
  fWidgetClasses.push_back(new WidgetClass("ToolBar",sChilds,sAttrs,sProps,sCbs));


  // sChild = container & leaves
  to_lines(containers+"\n"+leaves,sChilds);
  //  to_lines("name",sAttrs);

  to_lines("expand\nbackground",sProps);
  to_lines("",sCbs);
  fWidgetClasses.push_back(new WidgetClass("VBox",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("HBox",sChilds,sAttrs,sProps,sCbs));

  to_lines("background",sProps);
  fWidgetClasses.push_back(new WidgetClass("VContainer",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("HContainer",sChilds,sAttrs,sProps,sCbs));

  to_lines("splitPosition\nbackground",sProps);
  fWidgetClasses.push_back(new WidgetClass("VPaned",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("HPaned",sChilds,sAttrs,sProps,sCbs));

  to_lines("tabLabel\nbackground\nforeground",sProps);
  to_lines("select",sCbs);

  fWidgetClasses.push_back(new WidgetClass("TabStack",sChilds,sAttrs,sProps,sCbs));
  to_lines("geometry\ndecoration\ntitle\nshow\nbackground\nforeground",sProps);
  to_lines("create",sCbs);
  fWidgetClasses.push_back(new WidgetClass("Dialog",sChilds,sAttrs,sProps,sCbs));


  // leaves : no more sChilds
  sChilds.clear();
  sChilds.resize(0);
  //  to_lines("name",sAttrs);

  to_lines("selection\nitems\nbackground\nforeground",sProps);
  to_lines("select",sCbs);
  fWidgetClasses.push_back(new WidgetClass("ScrolledList",sChilds,sAttrs,sProps,sCbs));

  to_lines("title\ndirMask\ndirectory\nbackground\nforeground",sProps);
  to_lines("ok\ncancel",sCbs);
  fWidgetClasses.push_back(new WidgetClass("FileSelection",sChilds,sAttrs,sProps,sCbs));

  to_lines("background\nforeground",sProps);
  to_lines("ok",sCbs);
  fWidgetClasses.push_back(new WidgetClass("ColorSelection",sChilds,sAttrs,sProps,sCbs));

  to_lines("background\nforeground",sProps);
  to_lines("select\nselectBranch\nopen\nclose",sCbs);
  fWidgetClasses.push_back(new WidgetClass("ScrolledTree",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\nbackground\nforeground",sProps);
  to_lines("drag\nvalueChanged\nincrement\ndecrement",sCbs);
  fWidgetClasses.push_back(new WidgetClass("VScrollBar",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("HScrollBar",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\nbackground\nforeground\nminimum\nmaximum",sProps);
  to_lines("drag",sCbs);
  fWidgetClasses.push_back(new WidgetClass("VScale",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("HScale",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\ndefault\nitems\nbackground\nforeground",sProps);
  to_lines("valueChanged",sCbs);
  fWidgetClasses.push_back(new WidgetClass("ComboBox",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\nbackground\nforeground",sProps);
  to_lines("activate\ncomplete",sCbs);
  fWidgetClasses.push_back(new WidgetClass("CommandLine",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\nlabel\nitems\ndefault\nbackground\nforeground",sProps);
  to_lines("valueChanged",sCbs);
  fWidgetClasses.push_back(new WidgetClass("OptionMenu",sChilds,sAttrs,sProps,sCbs));

  to_lines("popupItems\nbackground",sProps);
  to_lines("collect\npopup\nupdate",sCbs);
  fWidgetClasses.push_back(new WidgetClass("PageViewer",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("PlanePageViewer",sChilds,sAttrs,sProps,sCbs));

  to_lines("",sProps);
  to_lines("",sCbs);
  fWidgetClasses.push_back(new WidgetClass("SoExaminerViewer",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("SoPlaneViewer",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("SoWalkViewer",sChilds,sAttrs,sProps,sCbs));
  fWidgetClasses.push_back(new WidgetClass("SoFlyViewer",sChilds,sAttrs,sProps,sCbs));

  to_lines("background",sProps);
  to_lines("paint\nevent",sCbs);
  fWidgetClasses.push_back(new WidgetClass("OpenGLArea",sChilds,sAttrs,sProps,sCbs));


  // activate callbacks
  to_lines("activate",sCbs);
  //  to_lines("name",sAttrs);
  //  to_lines("",sChilds);

  to_lines("background\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("Password",sChilds,sAttrs,sProps,sCbs));

  to_lines("value\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("Entry",sChilds,sAttrs,sProps,sCbs));

  to_lines("label\npixmap\ntooltip\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("ToolBarItem",sChilds,sAttrs,sProps,sCbs));

  to_lines("label\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("PushButton",sChilds,sAttrs,sProps,sCbs));

  to_lines("set\nlabel\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("ToggleButton",sChilds,sAttrs,sProps,sCbs));

  to_lines("background\nforeground\nlabel",sProps);
  fWidgetClasses.push_back(new WidgetClass("MenuItem",sChilds,sAttrs,sProps,sCbs));

  to_lines("label\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("ToggleMenuItem",sChilds,sAttrs,sProps,sCbs));

  // no more callbacks
  to_lines("",sCbs);
  //  to_lines("name",sAttrs);
  //  to_lines("",sChilds);

  to_lines("value\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("ScrolledText",sChilds,sAttrs,sProps,sCbs));

  to_lines("background\nforeground\ntitle\ngeometry\ndecoration",sProps);
  fWidgetClasses.push_back(new WidgetClass("WarningDialog",sChilds,sAttrs,sProps,sCbs));

  to_lines("label\nlabelString\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("Label",sChilds,sAttrs,sProps,sCbs));

  to_lines("background\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("MenuSeparator",sChilds,sAttrs,sProps,sCbs));
 
  to_lines("value\nbackground\nforeground",sProps);
  fWidgetClasses.push_back(new WidgetClass("ProgressBar",sChilds,sAttrs,sProps,sCbs));

  //no name
  //  to_lines("",sChilds);
  to_lines("class\nfile",sAttrs);
  to_lines("",sProps);
  //FIXME fWidgetClasses.push_back(new WidgetClass("Reference",sChilds,sAttrs,sProps,sCbs));

  return;
}

//////////////////////////////////////////////////////////////////////////////
void OnX::BaseUI::createLanguageList(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // FIXME : get this list from the System manager
 
  fCallbackLanguageList.push_back("DLD");
  fCallbackLanguageList.push_back("tcl");
  fCallbackLanguageList.push_back("cint");
  fCallbackLanguageList.push_back("kuip");
  fCallbackLanguageList.push_back("system");
  fCallbackLanguageList.push_back("sh");
  fCallbackLanguageList.push_back("DOS");
  fCallbackLanguageList.push_back("java");
}
//////////////////////////////////////////////////////////////////////////////
void OnX::BaseUI::createPropertyList(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fPropertyList.push_back("foreground background");
  fPropertyList.push_back("color");

  fPropertyList.push_back("title label labelString tooltip dirMask tabLabel");
  fPropertyList.push_back("string");

  fPropertyList.push_back("directory pixmap file");
  fPropertyList.push_back("file");

  fPropertyList.push_back("value");
  fPropertyList.push_back("value");

  fPropertyList.push_back("set show opened");
  fPropertyList.push_back("bool");

  fPropertyList.push_back("selection default");
  fPropertyList.push_back("selection");

  fPropertyList.push_back("geometry");
  fPropertyList.push_back("geometry");

  fPropertyList.push_back("decoration");
  fPropertyList.push_back("decoration");

  fPropertyList.push_back("expand");
  fPropertyList.push_back("expand");

  fPropertyList.push_back("splitPosition sizeOfFixed value minimum maximum");
  fPropertyList.push_back("int");

  fPropertyList.push_back("items popupItems");
  fPropertyList.push_back("items");
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::propertyType(
 const std::string& aName
,std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  for (unsigned int index=0;index<fPropertyList.size();index += 2) {
    if((" "+fPropertyList[index]+" ").find(" "+aName+" ") != std::string::npos) {
      aValue = fPropertyList[index+1];
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<std::string>& OnX::BaseUI::callbackLanguageList(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCallbackLanguageList;
}
//////////////////////////////////////////////////////////////////////////////
Slash::UI::IWidgetClass* OnX::BaseUI::findWidgetClass(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = fWidgetClasses.size();
  for (unsigned int index=0;index<number;index++) {
    if (fWidgetClasses[index]->name() == aName) {
      return fWidgetClasses[index];
    } 
  }
  return 0;
}

bool OnX::BaseUI::loadReferenceFile(inlib::xml::tree& aItemML,const std::string& aFile){
  // Load a file and add its items in the aItemML tree
  // insert a <reference> tag before the insertion of item
  // this is useful for inserting file from other files.onx 
  if(!fTopItemML) return false;
  
  // build a new component
  // build a label/title propertie if necessairy
  std::vector<inlib::xml::tree::atb> atts;
  inlib::xml::tree* itemML = fWidgetFactory.create("reference",atts,&aItemML);
  if(!itemML) return false;
  
  std::string name = buildName(*itemML);
  if(!itemML->set_attribute_value("file",aFile))  {
    delete itemML;
    return false;
  }
  inlib::xml::tree* top;
  if(!load(itemML,aFile,true,true,true,top)) {
    delete itemML;
    return false;
  }

  aItemML.add_child(itemML);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::BaseUI::tree(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTopItemML) return "";
  return "<tree><treeItem>"+subTree(*fTopItemML,2)+"</treeItem></tree>";
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::BaseUI::subTree(
 inlib::xml::tree& aItemML
,int depth
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string line;
  std::string value;
  std::string spaceItem = "";
  std::string spaceRoot = "";
  //spaceItem.insert(0,depth*2,' ');
  //spaceRoot.insert(0,(depth-1)*2,' ');

  aItemML.attribute_value("class",value);
  line += spaceItem + "<label>"+value;
  aItemML.attribute_value("name",value);
  value = inlib::to_xml(value);

  line += " name=\""+value+"\"";
  
  const std::vector<inlib::xml::tree::atb>& atbs = aItemML.attributes();
  unsigned int atbn = atbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    const std::string& atb = atbs[index].first;
    if ((atb != "name") && (atb !="class")){
      line += " "+atb+"=\""+inlib::to_xml(atbs[index].second)+"\"";
    }
  }
  line += "</label>\n";

  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    line += spaceRoot + "<treeItem>";
    value = inlib::to_xml((*it)->value());
    
    const std::vector<inlib::xml::tree::atb>& atbs = 
      (*it)->attributes();
    if (atbs.size() == 0){
      line += spaceItem+"<label>"+(*it)->name()+"=\""+value+"\"</label>";      

    } else {
      line += spaceItem+"<label>"+(*it)->name()+" ";      
      for (unsigned int b=0;b<atbs.size();b++) {
        line += atbs[b].first+"=\""+atbs[b].second+"\" ";
      }
      line += "value=\""+value+"\"</label>";
    }
    line += spaceRoot + "</treeItem>";
  }

  //line += spaceItem + "<opened>false</opened>";
 {std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    line += spaceRoot + "<treeItem>";
    line += subTree(*(*it),depth+1);
    line += spaceRoot + "</treeItem>";
  }}
  return line;
}
