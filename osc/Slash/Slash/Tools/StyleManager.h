#ifndef Slash_Tools_StyleManager_h
#define Slash_Tools_StyleManager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/IStyleManager.h>

#include <Slash/Core/ISession.h>

#include <vector>

#include <inlib/cast>
#include <inlib/smatch>

#include <exlib/xml/parser>

namespace Slash {

class StyleManager : public virtual Slash::Core::IManager, public virtual Slash::UI::IStyleManager {
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::StyleManager)
    else INLIB_IF_CAST(Slash::UI::IStyleManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public: //Slash::UI::IStyleManager
  virtual bool isStyle(const std::string& aString) const {
    std::vector< std::pair<std::string,std::string> >::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      //if(aString==(*it).first) return true;
      if(inlib::match(aString,(*it).first)) return true;
    }
    return false;
  }
  virtual std::vector<std::string> styles() const {
    std::vector<std::string> v;
    std::vector< std::pair<std::string,std::string> >::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) v.push_back((*it).first);
    return v;
  }
  virtual void removeStyle(const std::string& a_name) {
    std::vector< std::pair<std::string,std::string> >::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(a_name==(*it).first) {
        fList.erase(it);
        return;
      }
    } 
  }

  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////

  virtual bool loadFile(const std::string& aFile) {
    inlib::xml::default_factory factory;
    exlib::xml::parser styleML
      (factory,fSession.cout(),fSession.verboseLevel()?true:false);
    std::vector<std::string> tags;
    tags.push_back("styles");
    tags.push_back("style");
    styleML.set_tags(tags);
    if(!styleML.load_file(aFile,false)) return false;
    inlib::xml::tree* top = styleML.top_item();
    if(!top) return true; //File could be empty.
    load_tree(*top);
    return true;
  }

  virtual bool loadString(const std::string& aString) {
    inlib::xml::default_factory factory;
    exlib::xml::parser styleML
      (factory,fSession.cout(),fSession.verboseLevel()?true:false);
    std::vector<std::string> tags;
    tags.push_back("styles");
    tags.push_back("style");
    styleML.set_tags(tags);
    if(!styleML.load_string(aString)) return false;
    inlib::xml::tree* top = styleML.top_item();
    if(!top) return true;
    load_tree(*top);
    return true;
  }

  virtual void addStyle(const std::string& a_name,const std::string& a_value) {
    std::vector< std::pair<std::string,std::string> >::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(a_name==(*it).first) {
        //Existing style, override :
        (*it).second = a_value;
        return;
      }
    }  
    fList.push_back( std::pair<std::string,std::string>(a_name,a_value));
  }

  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
  virtual bool findStyle(const std::string& aString,std::string& aValue) const {
    std::vector< std::pair<std::string,std::string> >::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      //if(aString==(*it).first) {
      if(inlib::match(aString,(*it).first)) {
        aValue = (*it).second;
        return true;
      }
    }
    aValue = "";
    return false;
  }
public:
  inline StyleManager(Slash::Core::ISession& aSession,const std::string& aName)
  :fSession(aSession)
  ,fName(aName)
  {}
  virtual ~StyleManager(){}
protected:
  inline StyleManager(const StyleManager& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName){}
private:
  inline StyleManager& operator=(const StyleManager& aFrom) {return *this;}
private:
  inline void load_tree(inlib::xml::tree& a_tree) {
    inlib::xml::looper _for(a_tree);
    while(inlib::xml::tree* _tree = _for.next_tree()) {
      // Find the field with name "name" :
      std::string sname;
      _tree->attribute_value("name",sname);
      if(sname.empty()) break;
  
      /* with :
      <style name="OnX_Shape_Cube">
        <modeling>solid</modeling>
        <color>red</color>
        <highlightColor>green</highlightColor>
        <transparency>0.4</transparency>
        <cut>box 10 10 10 translation 10 10 0</cut>
      </style>

      a style "OnX_Shape_Cube" is created with value :
        modeling solid\ncolor red\nhighlightColor green\ntransparency 0.4\ncutbox 10 10 10 translation 10 10 0
      */
       
      std::string s;
      inlib::xml::looper _for2(*_tree);
      while(inlib::xml::element* _elem2 = _for2.next_element()) {
        s += _elem2->name();
        s += " ";
        s += _elem2->value();
        s += "\n";
      }
      //FIXME : rm last \n.
  
      if(fSession.verboseLevel()) {
        std::ostream& out = fSession.cout();
        out << "Slash::StyleManager::loadFile :"
            << " style " << inlib::sout(sname) << " : " 
            << s
            << std::endl;
      }
  
      bool found = false;
     {std::vector< std::pair<std::string,std::string> >::iterator it2;
      for(it2=fList.begin();it2!=fList.end();++it2) {
        if(sname==(*it2).first) {
          //Existing style, override :
          (*it2).second = s;
          found = true;
          break;
        }
      }}
  
      if(!found)
        fList.push_back( std::pair<std::string,std::string>(sname,s));
    }
  }
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::vector< std::pair<std::string,std::string> > fList;
private:
  static void check_instantiation(){
    Slash::Core::ISession* s;
    StyleManager o(*s,"");
  }  
};

}

#endif
