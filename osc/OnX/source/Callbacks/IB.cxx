//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments
//       (Slash::UI::IUI&,const std::vector<std::string>&);
//   }
//

#include "inlib"

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IIB.h> //it has forward cast to inlib::xml::tree
#include <Slash/UI/IWidgetClass.h>

#include <inlib/srep>
#include <inlib/args>
#include <inlib/cast>
#include <inlib/xml/tree>

// Callbacks.cxx :
extern "C" {
 void OnX_ui_set_fileChooser(Slash::UI::IUI&,const std::vector<std::string>&);
 void OnX_ui_show_dialog(Slash::UI::IUI&,const std::vector<std::string>&);
}

static inlib::xml::tree* getCurrentComponent(Slash::UI::IUI&);
static void refreshTree(Slash::UI::IUI&);
static inlib::xml::element* getCallbackSelected(Slash::UI::IUI&);
static void openDecorationEditor(Slash::UI::IUI&,const std::string&);
static void openFileEditor(Slash::UI::IUI&,const std::string&);

static Slash::UI::IIB& toIB(Slash::UI::IUI& aUI) {
  return *INLIB_CAST(aUI,Slash::UI::IIB);
}

extern "C" {

void OnX_IB_initialize(Slash::UI::IUI& aUI) {
  // Check that we have a IIB :
  if(!INLIB_CAST(aUI,Slash::UI::IIB)) {
    aUI.echo("OnX_IB_initialize : Slash::UI::IUI not a IIB.");
    return;
  }

  aUI.setParameter("OnX_IB_mainTree.items",toIB(aUI).tree());
}

void OnX_IB_treeSelect(Slash::UI::IUI& aUI) {
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;

  // get the selected property if their is one
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) return;
  if (path.empty()) return;
  std::vector<std::string> lines;
  smanip_lines(path,lines);
  if (!lines.size()) return;
  std::string itemClass;
  if(currentItem->attribute_value("class",itemClass)) {
    aUI.setParameter("OnX_IB_EditComponent.label",itemClass+" : ");
  }
  Slash::UI::IWidgetClass* widgetClass = aUI.findWidgetClass(itemClass);

  std::string allProperties ="";
  std::string allChilds ="";
  std::string allCallbacks ="";
  if(widgetClass) {
    allProperties = inlib::tos<std::string>(widgetClass->attributes(),"\n");
    if (allProperties != "") allProperties +="\n";
    allProperties += inlib::tos<std::string>(widgetClass->properties(),"\n");
    allChilds = inlib::tos<std::string>(widgetClass->children(),"\n");
    allCallbacks = inlib::tos<std::string>(widgetClass->callbacks(),"\n");
  }

  // get the selected name and value for property or callback

  std::string selectedLine = lines[lines.size()-1];
  std::string selectedTreeItemPropertyName = "";
  std::string selectedTreeItemPropertyValue = "";
  std::string selectedTreeItemCallbackName = "";
  std::string selectedTreeItemCallbackValue = "";

  bool selectedTreeItemIsCallback = false;
  bool selectedTreeItemIsProperty = false;

  if (selectedLine.find(" name=") ==std::string::npos) {
    if (selectedLine.find("\"") == selectedLine.rfind("\"")) {
      // we have only one ", we have to find the other
      selectedTreeItemPropertyValue = selectedLine.substr(0,selectedLine.find("\""));
      int a = lines.size();
      while ((a>1) && (lines[a-2].find("\"") == std::string::npos)) {
	selectedTreeItemPropertyValue = lines[a-2]+"\\n"+selectedTreeItemPropertyValue; 
        a--;
      }
      if (lines[a-2].find("\"") != std::string::npos) {
        selectedTreeItemIsProperty =true;
	selectedTreeItemPropertyValue = lines[a-2].substr(lines[a-2].find("\"")+1)+"\\n"+selectedTreeItemPropertyValue; 
	selectedTreeItemPropertyName = lines[a-2].substr(0,lines[a-2].find("="));
      }
    }
    else if (selectedLine.find("\"") !=std::string::npos){
      selectedTreeItemPropertyName = selectedLine.substr(0,selectedLine.find("="));
      selectedTreeItemPropertyValue = selectedLine.substr(selectedLine.find("=")+2,selectedLine.size()-selectedLine.find("=")-3);
      if (allProperties.find(selectedTreeItemPropertyName) != std::string::npos) {
        selectedTreeItemIsProperty =true;
      }
    }
  }
  if (!selectedTreeItemIsProperty) {
    selectedTreeItemCallbackName = selectedLine.substr(0,selectedLine.find(" "));
    if (allCallbacks.find(selectedTreeItemCallbackName) != std::string::npos) {
      selectedTreeItemIsCallback =true;
      // value is always the last item
      std::string::size_type pos = selectedLine.find(" value=");
      if (pos != std::string::npos) {
        pos = pos+7;
        selectedTreeItemCallbackValue = selectedLine.substr(
                                      pos+1,selectedLine.rfind("\"")-pos-1);
      } else // this callback has no value
        selectedTreeItemCallbackValue = "";
    }
  }
  //set child component possible
  //except for HBox and VBox when two components already given
  //except menuBar and ToolBar if it have already one
  if (((itemClass == "VBox") || (itemClass == "HBox") || (itemClass == "VPaned") || (itemClass == "HPaned")) && (currentItem->number_of_trees() == 2)) {
    allChilds ="";
  } else if (itemClass == "Window"){
    inlib::xml::tree* i;
    unsigned int a = 0;
    i = currentItem->find_by_attribute("class","MenuBar");
    if (i) {
      a = allChilds.find("MenuBar");
      allChilds.erase(a,8);
    }
    i = currentItem->find_by_attribute("class","ToolBar");
    if (i) {
      a = allChilds.find("ToolBar");
      allChilds.erase(a,8);
    }
  }
  aUI.setParameter("OnX_IB_AddComponentName.items",allChilds);

  // set properties
  // check for \n

  allProperties = "\n"+allProperties;
  if (!((allProperties.rfind("\n") == allProperties.length()-1) && (allProperties.length() >0)))
    allProperties +="\n";
  
  std::string::size_type apos = allProperties.find("\nclass\n");
  if (apos != std::string::npos) {
    allProperties.erase(apos,6);
  }
  apos = allProperties.find("\nname\n");
  if (apos != std::string::npos) {
    allProperties.erase(apos,6);
  }
  if (allProperties.find("\n") == 0) {
    allProperties = allProperties.substr(1,allProperties.length()-1);
  }
  if ((allProperties.rfind("\n") == allProperties.length()-1) && (allProperties.length() >0))
    allProperties = allProperties.substr(0,allProperties.length()-1);
  
  aUI.setParameter("OnX_IB_PropertyName.items",allProperties);
  if (selectedTreeItemIsProperty) {
    if (selectedTreeItemPropertyName.size()) {
      aUI.setParameter("OnX_IB_PropertyName.default",selectedTreeItemPropertyName);
    }
    aUI.setParameter("OnX_IB_PropertyValue.value",selectedTreeItemPropertyValue);
  } else {
    std::string defaultValue;
    if(allProperties.size()) {
      std::string::size_type a = allProperties.find("\n");
      if (a != std::string::npos) {
        defaultValue = allProperties.substr(0,a);
      } else {
        defaultValue = allProperties;
      }
      if (defaultValue.size()) {
        aUI.setParameter("OnX_IB_PropertyName.default",defaultValue);
      }
      std::string value;
      if (currentItem->element_value(defaultValue,value)) {
        aUI.setParameter("OnX_IB_PropertyValue.value",value);
      } else {
        aUI.setParameter("OnX_IB_PropertyValue.value","");
      }
    } else {
      aUI.setParameter("OnX_IB_PropertyValue.value","");  
    }
  }

  // look for callbacks
  
  if ((allCallbacks.rfind("\n") == allCallbacks.length()-1) && (allCallbacks.length() >0))
    allCallbacks = allCallbacks.substr(0,allCallbacks.length()-1);

  aUI.setParameter("OnX_IB_CallbackName.items",allCallbacks);

  if ((selectedTreeItemIsCallback) && (selectedTreeItemCallbackName != "")){
    aUI.setParameter("OnX_IB_CallbackName.default",selectedTreeItemCallbackName);
  } else {
    if(allCallbacks.size()) {
      std::string::size_type a = allCallbacks.find("\n");
      if (a != std::string::npos) {
        selectedTreeItemCallbackName = allCallbacks.substr(0,a);
      } else {
        selectedTreeItemCallbackName = allCallbacks;
      }
      if (selectedTreeItemCallbackName != "") {
        aUI.setParameter("OnX_IB_CallbackName.default",selectedTreeItemCallbackName);
      }
    }
  }

  // look for callback Id ("new","create 1,"create 2"...)

  std::string callbackItems = "";
  std::string defaultValue= "";
  if (allCallbacks != "") {
    callbackItems = "new";
    defaultValue= "new";
    std::string newVal;
    unsigned int count =0;
    std::list<inlib::xml::element*> props;
    currentItem->sub_elems(props);
    std::list<inlib::xml::element*>::const_iterator itp;
    for(itp=props.begin();itp!=props.end();++itp) {
      if ((*itp)->name() == selectedTreeItemCallbackName) {
        count ++;
        newVal = selectedTreeItemCallbackName+" "+ inlib::to<int>((int)(count));
        callbackItems += "\n"+newVal;
        if ((*itp)->value() == selectedTreeItemCallbackValue) {
          defaultValue = newVal;
        }
      }
    }
  }
  aUI.setParameter("OnX_IB_CallbackId.items",callbackItems);

  if (defaultValue != "") {
    aUI.setParameter("OnX_IB_CallbackId.default",defaultValue);
  }

  // get label and title
  std::string slabel;
  if (currentItem->attribute_value("name",slabel))
    aUI.setParameter("OnX_IB_EditName.value",slabel);
  return;
}

void OnX_IB_ChangeName(Slash::UI::IUI& aUI){
  // Callback activated when user changes the actual name of the component
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  std::string value;
  if(!aUI.parameterValue("OnX_IB_EditName.value",value)) return;
  if (value.empty()) return;
  if (value.find("OnX_IB_") == 0) {
    aUI.echo("OnX_IB_ChangeName :\
 Can't change the name of this component of the Interface Builder.");
    return;
  }
  currentItem->set_element_value("name",value);
  
  refreshTree(aUI);
}

void OnX_IB_AttrRemove(Slash::UI::IUI& aUI) {
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;

  //remove this one
  std::string selected ;
  if(!aUI.parameterValue("OnX_IB_AttrName.value",selected)) return;
  if (selected.empty()) return;
  if (selected == "name") return; //cannot remove a "name" !

  //FIXME : GB : to verify.
  currentItem->remove_attribute(selected);

  refreshTree(aUI);
}

void OnX_IB_SelectPropertyName(Slash::UI::IUI& aUI) {
  // Callback activated when user selects a actual property optionMenu
  //we have to put the right value
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  std::string prop= "";
  std::string selected ;
  if(!aUI.parameterValue("OnX_IB_PropertyName.value",selected)) return;
  if (selected.empty()) return;

  // test if it is a property or a attribut
  std::string itemClass;
  currentItem->attribute_value("class",itemClass);
  Slash::UI::IWidgetClass* widgetClass = aUI.findWidgetClass(itemClass);
  if(widgetClass) {
    std::string newProp = 
      "\n"+inlib::tos<std::string>(widgetClass->properties(),"\n")+"\n";
    if (newProp.find("\n"+selected+"\n",0) !=std::string::npos) {
      if(!currentItem->element_value(selected,prop))
        prop = "";
    } else {
      if(!currentItem->attribute_value(selected,prop))
        prop = "";
    }
  }
  aUI.setParameter("OnX_IB_PropertyValue.value",prop);
  return;
}

void OnX_IB_PropertyApply(Slash::UI::IUI& aUI){
  // Callback activated when user applies an actual property
  // we have to put the right value
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  std::string value;
  std::string selected ;
  std::string name;
  if(!aUI.parameterValue("OnX_IB_PropertyName.value",selected)) return;
  if(!aUI.parameterValue("OnX_IB_PropertyValue.value",value)) return;
  if (selected.empty()) return;
  if (value.empty()) return;

  bool oldValuePresent = currentItem->element_value(selected,name);

  // test if it is a property or a attribut
  std::string itemClass;
  currentItem->attribute_value("class",itemClass);
  Slash::UI::IWidgetClass* widgetClass = aUI.findWidgetClass(itemClass);
  
  if(widgetClass) {
    std::string newProp = 
      "\n"+inlib::tos<std::string>(widgetClass->properties(),"\n")+"\n";
    if (newProp.find("\n"+selected+"\n",0) !=std::string::npos) {
      currentItem->attribute_value("name",name);
      if (!aUI.setParameter(name+"."+selected,value)) {
        //except if property is splitPosition or expand for first time
        if (! ((!oldValuePresent) &&
            ((selected == "splitPosition") || (selected== "expand")))) {
          return;
        }
      }
      if ((selected == "expand") && (value!="first") && (value!="second")) {
        value = "both";
      }
      if (!currentItem->set_element_value(selected,value)) return;
    } else {
      if ((selected.empty()) || (value.empty())) return;      
      if (!currentItem->set_attribute_value(selected,value)) return;
    }
  }
  refreshTree(aUI);
  return;
}

void OnX_IB_PropertyRemove(Slash::UI::IUI& aUI){
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;

  //remove this one
  std::string selected ;
  if(!aUI.parameterValue("OnX_IB_PropertyName.value",selected)) return;
  if (selected.empty()) return;

  // test if it is a property or a attribut
  std::string itemClass;
  currentItem->attribute_value("class",itemClass);
  Slash::UI::IWidgetClass* widgetClass = aUI.findWidgetClass(itemClass);
  if(widgetClass) {
    std::string newProp = 
      "\n"+inlib::tos<std::string>(widgetClass->properties(),"\n")+"\n";
    if (newProp.find("\n"+selected+"\n",0) !=std::string::npos) {
      currentItem->delete_element(selected);
      std::string name;
      currentItem->attribute_value("name",name);

      // FIXME : do not try to set a default value, just let the old value on
      // display and don't save the deleted property

      //      aUI.setParameter(name+"."+selected,"");
    } else {
      //remove this one
      if (selected == "name") return; //cannot remove a "name" !
      //FIXME : GB : to verify.
      currentItem->remove_attribute(selected);
    }
  }
  refreshTree(aUI);
  return;
}

void OnX_IB_PropertyChoose(Slash::UI::IUI& aUI) {
  std::string sValue;
  std::string sName ;
  // get the current values
  if(!aUI.parameterValue("OnX_IB_PropertyName.value",sName)) return;
  if(!aUI.parameterValue("OnX_IB_PropertyValue.value",sValue)) return;

  std::string currentType;
  if (!toIB(aUI).propertyType(sName,currentType)) return;

  // all theses Dialog box set the current property value !
  if (currentType == "color") {
  } else if (currentType == "string") { // nothing to do
  } else if (currentType == "file") {
    openFileEditor(aUI,sValue);
  } else if (currentType == "value") { // string or int
  } else if (currentType == "bool") {
  } else if (currentType == "selection") {
  } else if (currentType == "geometry") {
  } else if (currentType == "decoration") {
    openDecorationEditor(aUI,sValue);
  } else if (currentType == "expand") { // true false both
  } else if (currentType == "int") {
  } else if (currentType == "items") { // \n...
  }
  return;
}

void OnX_IB_AddComponentChange(Slash::UI::IUI& aUI) {
  // Callback activated when user changes the value of the new component.
  // If he selects the ReferenceFile, we display a fileselector.
  std::string newName;
  if(!aUI.parameterValue("OnX_IB_AddComponentName.value",newName)) return;
  if (newName == "") return;
  if (newName =="Reference") {
    std::vector <std::string> vec;
    std::string name;
    inlib::file_name("$ONXROOT/scripts/OnX",name);
    vec.push_back(name);
    vec.push_back("*.onx");
    vec.push_back("open");
    vec.push_back("DLD");
    vec.push_back("OnX OnX_IB_LoadReference");
    OnX_ui_set_fileChooser(aUI,vec);
    Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
    if(w) w->show();
  }
}


void OnX_IB_AddComponentApply(Slash::UI::IUI& aUI) {
  // Callback activated when user creates a new component
  // we have to set this new component, refresh the tree and 
  // select it in the tree
  //get old path
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) return;
  if (path.empty()) return;

  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  std::string parentName;
  currentItem->attribute_value("name",parentName);
  std::string newChildClass ;
  if(!aUI.parameterValue("OnX_IB_AddComponentName.value",newChildClass)) return;
  if (newChildClass .empty()) return;

  // build a new component
  std::string newOnXName = aUI.createWidgetName();

  aUI.createComponent(newOnXName,newChildClass,parentName);
  Slash::UI::IWidget* w = aUI.findWidget(newOnXName);
  if(w) w->show();
  
  Slash::UI::IWidgetClass* widgetClass = aUI.findWidgetClass(newChildClass);
  std::string wcprops;
  if (widgetClass) {
    wcprops = inlib::tos<std::string>(widgetClass->properties(),"\n");
  } else {
    wcprops = "";
  }

  aUI.setParameter("OnX_IB_mainTree.items",toIB(aUI).tree());

  //select the new item
  std::string newPath;
  std::string treeLine = 
    newChildClass+std::string(" name=\"")+newOnXName+std::string("\"");
  std::vector<std::string> lines;
  smanip_lines(path,lines);
  lines.push_back(treeLine);
  newPath = inlib::tos<std::string>(lines,"\n");

  aUI.setParameter("OnX_IB_mainTree.selection",newPath);
  //set the new parameters in panel
  OnX_IB_treeSelect(aUI);
}

void OnX_IB_DeleteComponent(Slash::UI::IUI& aUI) {
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;

  std::string sname;
  currentItem->attribute_value("name",sname);
  if (sname == "") return;

  std::string sclass;
  currentItem->attribute_value("class",sclass);
  if (sclass == "") return;

  inlib::xml::tree *parent = currentItem;
  while(parent) {
    std::string pname;
    parent->attribute_value("name",pname);
    if (pname.find("OnX_IB_") == 0) {
      aUI.echo("OnX_IB_DeleteComponent :\
 You can't delete a Component of the Interface Builder.");
      return;
    }
    parent = parent->parent();
  }
  if (sclass == "Reference") {
    std::list<inlib::xml::tree*> list;
    currentItem->sub_trees(list);
    if(list.size()) {
      inlib::xml::tree* childItem = list.front();
      std::string schildName;
      childItem->attribute_value("name",schildName); 
      aUI.destroyComponent(schildName);
    }
    // Remove in parent :
    inlib::xml::tree* parentItemML = currentItem->parent();
    if(parentItemML) parentItemML->remove_child(currentItem);
  } else {
    aUI.destroyComponent(sname);
  }
  refreshTree(aUI);
}

void OnX_IB_CallbackName(Slash::UI::IUI& aUI) {
  // Callback activated when user changes the
  // callback name (activate, create...)
  // we have to set new callbackID
  std::string selectedCallback;
  if(!aUI.parameterValue("OnX_IB_CallbackName.value",selectedCallback))
    return;
  if (selectedCallback == "") return;  
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;

  std::string callbackItems = "new";
  std::string newVal;
  unsigned int count =0;

  std::list<inlib::xml::element*> props;
  currentItem->sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator itp;
  for(itp=props.begin();itp!=props.end();++itp) {
    if ((*itp)->name() == selectedCallback) {
      count ++;
      callbackItems += 
        "\n"+selectedCallback+" "+ inlib::to<int>((int)(count));
    }
  }
  aUI.setParameter("OnX_IB_CallbackId.items",callbackItems);
  aUI.setParameter("OnX_IB_CallbackId.default","new");
}

void OnX_IB_CallbackEdit(Slash::UI::IUI& aUI) {
  std::string value;
  if(!aUI.parameterValue("OnX_IB_CallbackId.value",value)) return;
  if(value == "") return;
  
  std::string list = 
    inlib::tos<std::string>(toIB(aUI).callbackLanguageList(),"\n");
  std::vector<std::string> sarg;
  std::string name;
  inlib::file_name("$ONXROOT/scripts/OnX/IB/Callback.onx",name);
  sarg.push_back(name);
  sarg.push_back("OnX_Callback_editor");
  
  OnX_ui_show_dialog(aUI,sarg);
  
  aUI.setParameter("OnX_IB_CallbackEditorLanguage.items",list);
  
  if (value == "new") {
    std::string defaultValue;
    if(list.size()) {
      std::string::size_type a = list.find("\n");
      if (a != std::string::npos)  defaultValue = list.substr(0,a);
      else defaultValue = list;
    } else defaultValue = "";
    aUI.setParameter("OnX_IB_CallbackEditorAction.label","Edit new callback");
    if (defaultValue != "") {
      aUI.setParameter("OnX_IB_CallbackEditorLanguage.default",defaultValue);
    }
    aUI.setParameter("OnX_IB_CallbackEditorArgs.value","");
  } else {
    inlib::xml::element* elem = getCallbackSelected(aUI);
    if (!elem) return;
    value = elem->value();
    std::string defLang;
    if(! elem->attribute_value("exec",defLang)) return;
    aUI.setParameter("OnX_IB_CallbackEditorAction.label","Edit callback");
    if (defLang != "") {
      aUI.setParameter("OnX_IB_CallbackEditorLanguage.default",defLang);
    }
   aUI.setParameter("OnX_IB_CallbackEditorArgs.value",value);
  }
}

void OnX_IB_CallbackRemove(Slash::UI::IUI& aUI) {
  // Remove callback, only if it has been added
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  inlib::xml::element * elem = getCallbackSelected(aUI);
  if (!elem) return;

  std::string name;
  if (!currentItem->attribute_value("name",name)) return;
  
  std::string callbackName;
  if(!aUI.parameterValue("OnX_IB_CallbackName.value",callbackName)) return;
  if (callbackName == "") return;

  int index = 0;
  std::string selected;
  if(!aUI.parameterValue("OnX_IB_CallbackId.value",selected)) return;
  if (selected == "") return;
  if (selected == "new") return;
  std::vector<std::string> words;
  inlib::words(selected," ",false,words);
  int l;
  inlib::to<int>(words[words.size()-1],l);
  index = l-1;   //-1 because first is number 0

  if (!aUI.removeCallback(name,callbackName,index)) return;
  currentItem->delete_element(elem);
  refreshTree(aUI);
}


void OnX_IB_CallbackEditorApply(Slash::UI::IUI& aUI) {
  // Callback activated when user applies the callback Editor
  // test if it is an Add or Edit CallbackEditor
  // Add the callback if this is not a create callback.
  // If this is activate, just add property to itemML field
  // Has to check if this callback hadn't already define

  std::string value = "";
  if(!aUI.parameterValue("OnX_IB_CallbackEditorArgs.value",value)) return;
  if (value.empty()) return;

  std::string sexec ="";
  if(!aUI.parameterValue("OnX_IB_CallbackEditorLanguage.value",sexec)) return;
  if (sexec == "") return;

  std::string callbackName;
  if(!aUI.parameterValue("OnX_IB_CallbackName.value",callbackName)) return;
  if (callbackName == "") return;

  std::string name;
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  if (!currentItem->attribute_value("name",name)) return;

  // special cases :
  // -if someone create a new callback : CallbackId="new" and index = 0
  // -if someone create a new callback, then click "apply" and "Cancel"
  //  we have to remove this callback
  // -if someone create a new callback,then click "apply" and "Ok"
  //  we have nothing to do  CallbackId!="new" and index !=0
  // -if someone modify a Callback : CallbackId!="new" and index!= 0
  int index = 0;
  //get index
  std::string selected ;
  if(!aUI.parameterValue("OnX_IB_CallbackId.value",selected)) return;
  if (selected == "") return;
  if (selected != "new") {
    std::vector<std::string> words;
    inlib::words(selected," ",false,words);
    int l;
    inlib::to<int>(words[words.size()-1],l);
    index = l -1;   //-1 because first is number 0
  }
  else {
    index = -1;
  }

  if (callbackName == "create") {

    inlib::xml::element * elem = getCallbackSelected(aUI);
    std::vector<inlib::xml::tree::atb> att;
    att.push_back(inlib::xml::tree::atb("exec",sexec));
    if (elem) { // if this is not a new one
      currentItem->delete_element(elem);
    }
    currentItem->add_element(callbackName,att,value);
    refreshTree(aUI);
  }
  else if (aUI.setCallback(name,callbackName,sexec,value,index)) {
    // set the new selection on the main tree, we can now say if a callback
    // has been realize or not
    std::string path;
    if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) path ="";
    // remove the last treeItem and set ower new
    
    if(path.size()>3) {
      if (path.find(" name=",path.rfind("\n",path.size()-2)) == std::string::npos) {
        path = path.substr(0,path.rfind("\n",path.size()-2));
      }
    }

    // build the last item.be careful with theses lines because
    // if we change the tree lines constrution, this won't work!
    path += "\n"+callbackName+ " exec=\""+sexec+"\" value=\""+value+"\"";

    aUI.setParameter("OnX_IB_mainTree.items",toIB(aUI).tree());
    aUI.setParameter("OnX_IB_mainTree.selection",path);
    //set the new parameters in panel
    OnX_IB_treeSelect(aUI);
  }
  return;
}

void OnX_IB_Save(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // Callback activated when user wants to save this new UI
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  inlib::xml::tree* topItem = toIB(aUI).topItemML();
  if (!topItem) return;
  topItem->set_file(aArgs[0]);
  aUI.save();
}

void OnX_IB_LoadReference(Slash::UI::IUI& aUI) {
  // Callback activated when user wants to import a reference file

  Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
  if(w) w->hide();
  //get old path
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) return;
  if (path.empty()) return;
  
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return;
  
  // load new reference file
  std::string name;
  inlib::file_name(aUI.callbackValue(),name);
  if(!toIB(aUI).loadReferenceFile(*currentItem,name)) return;
  
  refreshTree(aUI);
  return;
}


void OnX_IB_FileEditorApply(Slash::UI::IUI& aUI) {
  // Callback activated when user applies the decorations editor
  Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
  if(w) w->hide();
  aUI.setParameter("OnX_IB_PropertyValue.value",aUI.callbackValue());
  return;
}

void OnX_IB_DecorationEditorApply(Slash::UI::IUI& aUI) {
  // Callback activated when user applies the decorations editor
  std::string sclose;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",sclose)) return;
  std::string siconify;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",siconify)) return;
  std::string sminmax;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",sminmax)) return;
  std::string sresize;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",sresize)) return;
  std::string sborder;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",sborder)) return;
  std::string scaption;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",scaption)) return;
  std::string ssystem;
  if(!aUI.parameterValue("OnX_IB_ToggleCloseButton.set",ssystem)) return;

  std::string sValue="";
  bool bclose;
  bool biconify;
  bool bminmax;
  bool bresize;
  bool bborder;
  bool bcaption;
  bool bsystem;
  inlib::to(sclose,bclose);
  inlib::to(siconify,biconify);
  inlib::to(sminmax,bminmax);
  inlib::to(sresize,bresize);
  inlib::to(sborder,bborder);
  inlib::to(scaption,bcaption);
  inlib::to(ssystem,bsystem);

  if (bclose) sValue += "close_button\\n"; 
  if (!bclose) sValue += "no_close_button\\n"; 
  if (biconify) sValue += "iconify_button\\n"; 
  if (!biconify) sValue += "no_iconify_button\\n"; 
  if (bminmax) sValue += "min_max_button\\n"; 
  if (!bminmax) sValue += "no_min_max_button\\n"; 
  if (bresize) sValue += "resize_handle\\n"; 
  if (!bresize) sValue += "no_resize_handle\\n"; 
  if (bborder) sValue += "border\\n"; 
  if (!bborder) sValue += "no_border\\n"; 
  if (bcaption) sValue += "caption\\n"; 
  if (!bcaption) sValue += "no_caption\\n"; 
  if (bsystem) sValue += "system_menu"; 
  if (!bsystem) sValue += "no_system_menu";

  aUI.setParameter("OnX_IB_PropertyValue.value",sValue);
  return;
}

}  // end of extern C

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inlib::xml::tree* getCurrentComponent(Slash::UI::IUI& aUI) {
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) return 0;
  if (path.empty()) return 0;
  std::vector<std::string> lines;
  smanip_lines(path,lines);

  //search for the itemML corresponding to what is selected 
  if (!lines.size()) return 0;
  std::string sname;
  std::string::size_type spos = std::string::npos;//FIXME : npos instead of -1
  int currentLine = lines.size()-1;
  std::string search = " name=";
  while ((currentLine>-1) && (spos == std::string::npos)) {
    spos = lines[currentLine].find(search,0);
    if (spos != std::string::npos)
      sname = lines[currentLine].substr(spos+search.size()+1,lines[currentLine].find("\"",spos+search.size()+1)-spos-search.size()-1);
    else
      currentLine --;
  }
  inlib::xml::tree* topItem = toIB(aUI).topItemML();
  if (!topItem) return 0;
  inlib::xml::tree* currentItem = topItem->find_item(sname);
  return currentItem;
}

Slash::UI::IWidget* getCurrentWidget(Slash::UI::IUI& aUI){
  // G.Barrand : try to use IWidget to not expose inlib::xml::tree.
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) return 0;
  if (path.empty()) return 0;
  std::vector<std::string> lines;
  smanip_lines(path,lines);

  //search for the itemML corresponding to what is selected 
  if (!lines.size()) return 0;
  std::string sname;
  std::string::size_type spos = std::string::npos;//FIXME : npos instead of -1
  int currentLine = lines.size()-1;
  std::string search = " name=";
  while ((currentLine>-1) && (spos == std::string::npos)) {
    spos = lines[currentLine].find(search,0);
    if (spos != std::string::npos)
      sname = lines[currentLine].substr(spos+search.size()+1,lines[currentLine].find("\"",spos+search.size()+1)-spos-search.size()-1);
    else
      currentLine --;
  }
  return aUI.findWidget(sname);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void refreshTree(Slash::UI::IUI& aUI){
  std::string path;
  if(!aUI.parameterValue("OnX_IB_mainTree.selection",path)) path ="";
  // remove the last treeItem, because if we want to set a new selection, we
  // can't set it on a path witch has change, for example :
  // current is SplitPosition=200 and user is changing this value to 250, 
  // in this case, we set the selection to the top widget item. 

 if(path.size()>3) {
   if (path.find(" name=",path.rfind("\n",path.size()-2)) == std::string::npos) {
     path = path.substr(0,path.rfind("\n",path.size()-2));
   }
 }
 aUI.setParameter("OnX_IB_mainTree.items",toIB(aUI).tree());
 aUI.setParameter("OnX_IB_mainTree.selection",path);
 //set the new parameters in panel
 OnX_IB_treeSelect(aUI);
}


inlib::xml::element* getCallbackSelected(Slash::UI::IUI& aUI){
  // This function parse the current callback selected and return the element
  // corresponding.
  // Their is some case when we the callback looks like "create 3" or "init"
  // and we have to return the good create value.
  inlib::xml::tree* currentItem = getCurrentComponent(aUI);
  if (!currentItem) return 0;

  std::string selected ;
  if(!aUI.parameterValue("OnX_IB_CallbackId.value",selected)) return 0;
  if (selected == "") return 0;
  if (selected=="new") return 0;
  std::vector<std::string> words;
  inlib::words(selected," ",false,words);
  std::string callbackName = words[0];
  int search =0;
  if (words.size() ==2) {
    inlib::to<int>(words[1],search);
  }

  //get this callback
  std::list<inlib::xml::element*> itemProps;
  currentItem->sub_elems(itemProps);
  
  std::list<inlib::xml::element*>::const_iterator it;
  int indice = 1;
  for(it=itemProps.begin();it!=itemProps.end();++it) {
    if(callbackName==(*it)->name()) {
      if (search == indice) {
        return  (*it);
      }
      indice ++;
    }
  }
  return 0;
}


void openFileEditor(Slash::UI::IUI& aUI,const std::string& /*aFile*/){
  aUI.setCallback("fileChooser","ok","DLD","OnX OnX_IB_FileEditorApply ");
  aUI.setCallback("fileChooser","cancel",
                  "DLD","OnX ui_hide fileChooser");
  aUI.setParameter("fileChooser.dirMask","*");
  aUI.setParameter("fileChooser.directory","$ONXROOT");
  aUI.setParameter("fileChooser.mode","open");
  Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
  if(w) w->show();
  return;
}

void openDecorationEditor(Slash::UI::IUI& aUI,const std::string& aDecoration){
  std::vector< std::pair<bool,std::string> > sDecorations = 
    toIB(aUI).parseDecorations(aDecoration);
  std::vector< std::pair<bool,std::string> >::iterator it;
  
  std::vector<std::string> sarg;
  std::string name;
  inlib::file_name("$ONXROOT/scripts/OnX/IB/SelectDecorations.onx",name);
  sarg.push_back(name);
  sarg.push_back("OnX_IB_selectDecorations");
  OnX_ui_show_dialog(aUI,sarg);

  aUI.setParameter("OnX_IB_ToggleCloseButton.set","false");
  aUI.setParameter("OnX_IB_ToggleIconifyButton.set","false");
  aUI.setParameter("OnX_IB_ToggleMinMaxButton.set","false");
  aUI.setParameter("OnX_IB_ToggleResizeButton.set","false");
  aUI.setParameter("OnX_IB_ToggleBorderButton.set","false");
  aUI.setParameter("OnX_IB_ToggleCaptionButton.set","false");
  aUI.setParameter("OnX_IB_ToggleSystemMenuButton.set","false");
 
  for(it=sDecorations.begin();it!=sDecorations.end();++it) {
    if((*it).first) {
      if ((*it).second == "close_button")
        aUI.setParameter("OnX_IB_ToggleCloseButton.set","true");
      if ((*it).second == "iconify_button")
        aUI.setParameter("OnX_IB_ToggleIconifyButton.set","true");
      if ((*it).second == "min_max_button")
        aUI.setParameter("OnX_IB_ToggleMinMaxButton.set","true");
      if ((*it).second == "resize_handle")
        aUI.setParameter("OnX_IB_ToggleResizeButton.set","true");
      if ((*it).second == "border")
        aUI.setParameter("OnX_IB_ToggleBorderButton.set","true");
      if ((*it).second == "caption")
        aUI.setParameter("OnX_IB_ToggleCaptionButton.set","true");
      if ((*it).second == "system_menu")
        aUI.setParameter("OnX_IB_ToggleSystemMenuButton.set","true");
    }
  }
}
