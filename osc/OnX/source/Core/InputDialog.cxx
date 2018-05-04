//////////////////////////////////////////////////////////////////////////////
bool OnX::BaseUI::createInputDialog(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
// Assume that class of the component element is "Form".
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string inputName = buildName(aItemML);

  //out() << "InputDialog \"" << inputName << "\"." << std::endl;
  
  inlib::xml::tree* dialogItemML = 
    createItemML(inputName+"_dialog","Dialog",&aItemML);
  std::string value;
  aItemML.element_value("label",value);
  std::vector<inlib::xml::tree::atb> atts;
  dialogItemML->add_element("title",atts,value);

  std::string vbox = inputName + "_vbox";
  inlib::xml::tree* vBoxItemML = createItemML(vbox,"VBox",dialogItemML);
  vBoxItemML->add_element("expand",atts,"first");
  vBoxItemML->add_element("sizeOfFixed",atts,"30");

  std::string vcontainer = inputName + "_vcontainer";
  inlib::xml::tree* vContainerItemML = 
    createItemML(vcontainer,"VContainer",vBoxItemML);

  /*
  inlib::xml::tree* labelItemML = 
    createItemML(inputName + "_label","Label",vContainerItemML);
  labelItemML->add_element("label",atts,value);
  */


  // Loop on input properties :
  std::vector<std::string> inputNames;
  std::list<inlib::xml::element*> properties;
  aItemML.sub_elems(properties);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=properties.begin();it!=properties.end();++it) {
    if((*it)->name()=="input") {
      std::string name;
      if(!(*it)->attribute_value("name",name)) {
        out() << "OnX::BaseUI::createInputDialog :" 
              << " An input property must have a name." << std::endl;
        return false;
      }
      std::string type;
      (*it)->attribute_value("type",type);
      if((type.empty()) || (type=="text") ) {
        inlib::xml::tree* hBoxItemML = 
          createItemML(inputName + "_hbox_" + name,"HBox",vContainerItemML);
        inlib::xml::tree* labelItemML = 
          createItemML(inputName + "_label_" + name,"Label",hBoxItemML);
        std::vector<inlib::xml::tree::atb> atts;
        int length = 0;
        if((*it)->attribute_value("label",value)) {
          labelItemML->add_element("label",atts,value);
          length = value.size();
        } else {
          labelItemML->add_element("label",atts,name);
          length = name.size();
        }

        inlib::sprintf(value,64,"%d",length * 10);
        hBoxItemML->add_element("expand",atts,"second");
        hBoxItemML->add_element("sizeOfFixed",atts,value);

        inlib::xml::tree* entryItemML = 
          createItemML(inputName + "_input_" + name,"Entry",hBoxItemML);
        entryItemML->add_element("background",atts,"white");
        (*it)->attribute_value("default",value);
        entryItemML->add_element("value",atts,value);

      } else if( (type=="option")|| (type=="combo") ) {

        inlib::xml::tree* hBoxItemML = 
          createItemML(inputName+"_hbox_"+name,"HBox",vContainerItemML);

        inlib::xml::tree* labelItemML = 
          createItemML(inputName + "_label_" + name,"Label",hBoxItemML);
        std::vector<inlib::xml::tree::atb> atts;
        int length = 0;
        if((*it)->attribute_value("label",value)) {
          labelItemML->add_element("label",atts,value);
          length = value.size();
        } else {
          labelItemML->add_element("label",atts,name);
          length = name.size();
        }

        inlib::sprintf(value,64,"%d",length * 10);
        hBoxItemML->add_element("expand",atts,"second");
        hBoxItemML->add_element("sizeOfFixed",atts,value);

        inlib::xml::tree* optionItemML = createItemML
          (inputName + "_input_" + name,
           (type=="combo"?"ComboBox":"OptionMenu"),hBoxItemML);
        (*it)->attribute_value("items",value);
        optionItemML->add_element("items",atts,value);
        (*it)->attribute_value("default",value);
        optionItemML->add_element("default",atts,value);

      } else if( (type=="fileChooser")||(type=="fileSaver")) {
        // Variant of type "text" with the label being
        // now a button mapping a file chooser.

        inlib::xml::tree* hBoxItemML = 
          createItemML(inputName + "_hbox_" + name,"HBox",vContainerItemML);

        std::string buttonName = inputName+"_pushbutton_"+name;
        inlib::xml::tree* buttonItemML = 
          createItemML(buttonName,"PushButton",hBoxItemML);

        std::vector<inlib::xml::tree::atb> atts;
        int length = 0;
        if((*it)->attribute_value("label",value)) {
          buttonItemML->add_element("label",atts,value);
          length = value.size();
        } else {
          buttonItemML->add_element("label",atts,name);
          length = name.size();
        }

        std::string dirMask;
        if(!(*it)->attribute_value("dirMask",dirMask)) dirMask = "*.*";

        inlib::sprintf(value,64,"%d",length * 10);
        hBoxItemML->add_element("expand",atts,"second");
        hBoxItemML->add_element("sizeOfFixed",atts,value);

        std::string entryName = inputName + "_input_" + name;
        inlib::xml::tree* entryItemML = createItemML(entryName,"Entry",hBoxItemML);
        entryItemML->add_element("background",atts,"white");
        (*it)->attribute_value("default",value);
        entryItemML->add_element("value",atts,value);

        // Set the push button script. 
        // It maps a file chooser by passing the name of the entry.
        // The "ok" of the chooser deposits the choosed file 
        // on the entry widget.

       {std::string param = "ui."+buttonName+".script";
        std::string script = 
          "OnX ui_set_parameter "+entryName+".value @this.value@";
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("name",param));
        buttonItemML->add_element("session_parameter",atts,script);
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));
        std::string s = (type=="fileChooser"?"open":"save");
        buttonItemML->add_element
          ("activate",atts,"OnX ui_set_file_chooser . "+dirMask+" "+s+" DLD @session@"+param+"@");}

        buttonItemML->add_element
          ("activate",atts,"OnX ui_show fileChooser");
        atts.clear();

      } else if(type=="colorChooser") {
        // Variant of type "combo" with the label being
        // now a button mapping a color chooser.

        inlib::xml::tree* hBoxItemML = 
          createItemML(inputName+"_hbox_"+name,"HBox",vContainerItemML);

        std::string buttonName = inputName+"_pushbutton_"+name;
        inlib::xml::tree* buttonItemML = 
          createItemML(buttonName,"PushButton",hBoxItemML);

        std::vector<inlib::xml::tree::atb> atts;
        int length = 0;
        if((*it)->attribute_value("label",value)) {
          buttonItemML->add_element("label",atts,value);
          length = value.size();
        } else {
          buttonItemML->add_element("label",atts,name);
          length = name.size();
        }

        inlib::sprintf(value,64,"%d",length * 10);
        hBoxItemML->add_element("expand",atts,"second");
        hBoxItemML->add_element("sizeOfFixed",atts,value);

        std::string comboName = inputName + "_input_" + name;
        inlib::xml::tree* optionItemML = 
          createItemML(comboName,"ComboBox",hBoxItemML);
        (*it)->attribute_value("items",value);
        optionItemML->add_element("items",atts,value);
        (*it)->attribute_value("default",value);
        optionItemML->add_element("default",atts,value);

        // Set the push button script. 
        // It maps a color chooser by passing the name of the combo. 
        // The "ok" of the chooser deposits the choosed color on the 
        // combo.
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));

       {std::string param = "ui."+buttonName+".script";
        std::string script = 
          "OnX ui_set_parameter "+comboName+".default @this.value@";
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("name",param));
        buttonItemML->add_element("session_parameter",atts,script);
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));
        std::string s = (type=="fileChooser"?"open":"save");
        buttonItemML->add_element("activate",atts,
          "OnX ui_set_callback colorChooser ok DLD @session@"+param+"@");}

        buttonItemML->add_element("activate",atts,"OnX ui_show colorChooser");
        atts.clear();

      } else if( (type=="scale")||(type=="scrollBar")) {

        // Box(Box2(Label,Entry),Scale)
        inlib::xml::tree* hBoxItemML = 
          createItemML(inputName + "_hbox_" + name,"HBox",vContainerItemML);
        hBoxItemML->add_element("expand",atts,"second");
        hBoxItemML->add_element("sizeOfFixed",atts,"150");

        // Box2(Label,Entry)
        inlib::xml::tree* hBox2ItemML = 
          createItemML(inputName + "_hbox2_" + name,"HBox",hBoxItemML);

        inlib::xml::tree* labelItemML = 
          createItemML(inputName + "_label_" + name,"Label",hBox2ItemML);
        std::vector<inlib::xml::tree::atb> atts;
        int length = 0;
        if((*it)->attribute_value("label",value)) {
          labelItemML->add_element("label",atts,value);
          length = value.size();
        } else {
          labelItemML->add_element("label",atts,name);
          length = name.size();
        }

        inlib::sprintf(value,64,"%d",length * 10);
        hBox2ItemML->add_element("expand",atts,"second");
        hBox2ItemML->add_element("sizeOfFixed",atts,value);

        std::string entryName = inputName + "_input_" + name;
        inlib::xml::tree* entryItemML = createItemML(entryName,"Entry",hBox2ItemML);
        entryItemML->add_element("background",atts,"white");
        (*it)->attribute_value("default",value);
        entryItemML->add_element("value",atts,value);

        std::string sliderName = inputName + "_slider_" + name;

        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));
        entryItemML->add_element("activate",atts,
          "OnX ui_set_parameter "+sliderName+".value @this.value@");
        atts.clear();

        inlib::xml::tree* sliderItemML = 
          createItemML(sliderName,
            (type=="scale"?"HScale":"HScrollBar"),hBoxItemML);
        (*it)->attribute_value("default",value);
        sliderItemML->add_element("value",atts,value);
        (*it)->attribute_value("minimum",value);
        sliderItemML->add_element("minimum",atts,value);
        (*it)->attribute_value("maximum",value);
        sliderItemML->add_element("maximum",atts,value);

        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));
        sliderItemML->add_element("valueChanged",atts,
          "OnX ui_set_parameter "+entryName+".value @this.value@");
        sliderItemML->add_element("drag",atts,
          "OnX ui_set_parameter "+entryName+".value @this.value@");
        sliderItemML->add_element("increment",atts,
          "OnX ui_set_parameter "+entryName+".value @this.value@");
        sliderItemML->add_element("decrement",atts,
          "OnX ui_set_parameter "+entryName+".value @this.value@");
        atts.clear();

      } else {
        out() << "OnX::BaseUI::createInputDialog :" 
              << " unknown type " << inlib::sout(type)
              << std::endl;
        return false;
      }

      inputNames.push_back(name);
    }
  }

  int margin = 5;
  int hchild = 35;
  if(cast("OnX::NS_UI")) { //FIXME. And remove the Slash::UI::IUI::cast ?
    margin = 0;
    hchild = 30;
  }
  
  // WIN32 : set geometry (mainly height) of the dialog :
  std::string sgeom;
  // WinTk::[V,H}Container has a margin of 5 :
  int childn = inputNames.size()+1;
  inlib::sprintf(sgeom,256,"%dx%d",300,margin + childn * (hchild + margin));
  dialogItemML->add_element("geometry",atts,sgeom);

  inlib::xml::tree* hContainerItemML = 
    createItemML(inputName + "_hcontainer","HContainer",vBoxItemML);

  // Ok button :
  inlib::xml::tree* okItemML = 
    createItemML(inputName + "_ok","PushButton",hContainerItemML);
  okItemML->add_element("label",atts,"Ok");
  // Apply button :
  inlib::xml::tree* applyItemML = 
    createItemML(inputName + "_apply","PushButton",hContainerItemML);
  applyItemML->add_element("label",atts,"Apply");
  atts.clear();
  atts.push_back(inlib::xml::tree::atb("exec","DLD"));
  okItemML->add_element("activate",atts,"OnX ui_hide "+inputName+"_dialog");
  // Get the scripts :
 {std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if((*it)->name()!="ok") continue;
    std::string sexec;
    if((*it)->attribute_value("exec",sexec)) {
      const std::string& value = (*it)->value();
      if(value.size()) {
        if(sexec=="Python") {
          std::string script;
          for(unsigned int index=0;index<inputNames.size();index++) {
            script += inputNames[index];
            script += " = OnX.ui_parameterValue(\"";
            script += inputName;
            script += "_input_";
            script += inputNames[index];
            script += ".value\")\n";
          }
          script += value;
          atts = (*it)->attributes();
          okItemML->add_element("activate",atts,"import OnX;"+script);
          applyItemML->add_element("activate",atts,"import OnX;"+script);
        } else {
          atts = (*it)->attributes();
          okItemML->add_element("activate",atts,value);
          applyItemML->add_element("activate",atts,value);
        }
      }
    }
  }}

  // Cancel button :
 {inlib::xml::tree* cancelItemML = 
    createItemML(inputName + "_cancel","PushButton",hContainerItemML);
  atts.clear();
  atts.push_back(inlib::xml::tree::atb("exec","DLD"));
  cancelItemML->add_element
    ("activate",atts,"OnX ui_hide "+inputName+"_dialog");
  atts.clear();
  cancelItemML->add_element("label",atts,"Cancel");}

  // Reset button :
 {inlib::xml::tree* resetItemML = 0;
  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if((*it)->name()!="reset") continue;
    std::string sexec;
    if((*it)->attribute_value("exec",sexec)) {
      const std::string& value = (*it)->value();
      if(value.size()) {
        if(!resetItemML) {
          resetItemML = 
            createItemML(inputName + "_reset","PushButton",hContainerItemML);
          atts.clear();
          resetItemML->add_element("label",atts,"Reset");
        }
        atts = (*it)->attributes();
        resetItemML->add_element("activate",atts,value);
      }
    }
  }}

  // Help button :
 {inlib::xml::tree* helpItemML = 0;
  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if((*it)->name()!="help") continue;    
    std::string sfile;
    if((*it)->attribute_value("file",sfile)) {
      if(sfile.size()) {
        if(!helpItemML) {
          helpItemML = 
            createItemML(inputName + "_help","PushButton",hContainerItemML);
          atts.clear();
          helpItemML->add_element("label",atts,"Help");
        }
        atts.clear();
        atts.push_back(inlib::xml::tree::atb("exec","DLD"));
        helpItemML->add_element("activate",atts,"OnX ui_help_file "+sfile);
      }
    }
  }}

  return true;
}
