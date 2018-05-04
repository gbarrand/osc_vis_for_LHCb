// this :
#include <OnX/Core/WidgetFactory.h>

// do we want to put the below in WidgetFactory.h ?

#include <OnX/Core/Widgets.h>

inlib::xml::tree* OnX::WidgetFactory::create(const std::string& aTagName,const std::vector<inlib::xml::tree::atb>& aAtts,inlib::xml::tree* aParent){
    bool save = true;
   {unsigned int linen = aAtts.size();
    for(unsigned int index=0;index<linen;index++) {
      if(aAtts[index].first=="save") {
        if(!inlib::to(aAtts[index].second,save)) save = true;
        break;
      }
    }}
  
    if(aTagName=="widget") {
  
      std::string sclass;
  
     {unsigned int linen = aAtts.size();
      for(unsigned int index=0;index<linen;index++) {
        if(aAtts[index].first=="class") {
          sclass = aAtts[index].second;
          break;
        }
      }}
  
      if(sclass=="InputDialog") {
        inlib::xml::tree* itemML = 
          new inlib::xml::tree(aTagName,*this,aParent);
        itemML->set_attributes(aAtts);
        itemML->set_save_flag(save);
        return itemML;
      }
  
      if(sclass=="Copy") {
        inlib::xml::tree* itemML = 
          new inlib::xml::tree(aTagName,*this,aParent);
        itemML->set_attributes(aAtts);
        itemML->set_save_flag(save);
        return itemML;
      }
  
      Slash::UI::IWidgetClass* widgetClass = findWidgetClass(sclass);  
      if(widgetClass) {
        if( (widgetClass->name()=="PageViewer") ||
            (widgetClass->name()=="PlanePageViewer") ||
            (widgetClass->name()=="SoExaminerViewer") ||
            (widgetClass->name()=="SoPlaneViewer") ||
            (widgetClass->name()=="SoWalkViewer") ||
            (widgetClass->name()=="SoFlyViewer") ) {
          inlib::xml::tree* itemML = 
            new SoViewer(fUI,*this,aParent,*widgetClass);
          itemML->set_attributes(aAtts);
          itemML->set_save_flag(save);
          return itemML;
  	/*
        } else if(widgetClass->name()=="ScrolledTree") {
          inlib::xml::tree* itemML = 
            new OpenGLArea(*this,aParent,*widgetClass);
          itemML->set_attributes(aAtts);
          itemML->set_save_flag(save);
          return itemML;
        } else if(widgetClass->name()=="OpenGLArea") {
          inlib::xml::tree* itemML = 
            new OpenGLArea(*this,aParent,*widgetClass);
          itemML->set_attributes(aAtts);
          itemML->set_save_flag(save);
          return itemML;
          */
        } else {
          inlib::xml::tree* itemML = 
            new Widget(fUI,*this,aParent,*widgetClass);
          itemML->set_attributes(aAtts);
          itemML->set_save_flag(save);
          return itemML;
        }
      } else {
        f_out << "OnX::WidgetFactory::create :"
              << " Slash::UI::IWidgetClass not found for "
              << inlib::sout(sclass) << "."
              << std::endl;
        return 0;
      }
    } else { //OnX, reference, comment are not widgets.
      //f_out << "OnX::WidgetFactory::create :"
      //      << " WARNING : Slash::UI::IWidgetClass not found for "
      //      << inlib::sout(aClass) << "."
      //      << std::endl;
      inlib::xml::tree* itemML = new inlib::xml::tree(aTagName,*this,aParent);
      itemML->set_attributes(aAtts);
      itemML->set_save_flag(save);
      return itemML;
    }
  }
