//////////////////////////////////////////////////////////////////////////////
/// Pre SWIG /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

%rename(pck) Slash::UI::IInterpreter::package; //package is a java keyword.

%ignore Slash::Data::IArray::vector();
// Avoid a clash with a finalize() produced by SWIG :
%ignore Slash::UI::IUI::finalize();

%rename(file_remove) Slash::file::remove(const std::string&);

#ifdef SWIGPYTHON
// FIXME : lxplus 64 bits. The code generated has a call to :
//   set(std::vector<unsigned int,std::allocator<unsigned int > > const &,
//    std::vector<unsigned long long,std::allocator<Slash::uint64 > > const &);
// and NOT : 
//   set(std::vector<unsigned int,std::allocator<unsigned int > > const &,
//    std::vector<Slash::uint64,std::allocator<Slash::uint64 > > const &);
// and then does not compile.

%ignore Slash::Core::IValue::set(const std::vector<unsigned int>&,
                                 const std::vector<Slash::uint64>&);
%ignore Slash::Core::IValue::set(const std::vector<unsigned int>&,
                                 const std::vector<Slash::int64>&);
#endif
//////////////////////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//FIXME : see the #ifdef SWIG in Slash/IAll.h
//        there are problems due to the fact that
//        IObject, IArray exists in two different places
%include <Slash/IAll.h> //wrap interfaces only.

// but have ... 
%include <Slash/Tools/Style.h>

#if defined(SWIGJAVA) || defined(SWIGTCL)
//FIXME : despite the upper IAll.h, SWIG java and Tcl needs that !
%include <Slash/Core/IManager.h>
%include <Slash/Core/ISession.h>
%include <Slash/UI/IUI.h>
%include <Slash/UI/IWidget.h>
%include <Slash/Data/IProcessor.h>
#endif

//////////////////////////////////////////////////////////////
/// Post SWIG ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

// Put helpers as extensions :

%extend Slash::Core::ISession {
  std::string parameterValue(const std::string& aKey) {
    std::string s;
    self->parameterValue(aKey,s);
    return s;
  }
}

%extend Slash::Data::IProcessor {
/*void types() {
    data_collect(*self,"Lib_Type");
    data_dump(*self);
  }*/
  void collect(const std::string& aWhat,const std::string& aCuts = "") {
    std::vector<std::string> ss;
    ss.push_back(aWhat);
    ss.push_back(aCuts);
    self->execute("collect",ss);
  }
  void visualize(const std::string& where = "@current@",const std::string& style = "") {
    std::vector<std::string> ss;
    ss.push_back("where="+where);
    if(style.size()) ss.push_back("style="+style); //used in InventorAccessor.h
    self->execute("visualize",ss);
  }

  void visualize(const Slash::UI::IStyle& aStyle,const std::string& where = "@current@"){
    Slash::Core::IAction* action = self->findAction("visualize");
    if(!action) {
      self->out() << "Slash::Data::IProcessor::visualize (SWIG extend) :"
                  << " \"visualize\" Action not found."
                  << std::endl;
      return;
    }
    Slash::Data::IVisualizeAction* vaction = 
      INLIB_CAST(*action,Slash::Data::IVisualizeAction);
    if(!vaction) {
      self->out() << "Slash::Data::IProcessor::visualize (SWIG extend) :"
                  << " IAction is not a IVisualizeAction."
                  << std::endl;
      return;
    }
    std::vector<std::string> ss;
    ss.push_back("where="+where);
    vaction->execute(ss,aStyle);
  }

  int number() {
    std::vector<std::string> ss;
    std::string s = self->execute("number",ss);
    int number;
    if(!inlib::to<int>(s,number)) return 0;
    return number;  
  }
  std::vector<std::string> values(const std::string& aWhat) {
    std::vector<std::string> ss;
    ss.push_back(aWhat);
    return inlib::words(self->execute("values",ss),"\n");
  }
  void dump(const std::string& aFormat = "table") {
    std::vector<std::string> ss;
    ss.push_back(aFormat);
    self->execute("dump",ss);
  }
  void filter(const std::string& aWhat,
                          const std::string& aFilter = "") {
    std::vector<std::string> ss;
    ss.push_back(aWhat);
    ss.push_back(aFilter);
    self->execute("filter",ss);
  }
  void set(const std::string& aWhat,
                       const std::string& aValue) {
    std::vector<std::string> ss;
    ss.push_back(aWhat);
    ss.push_back(aValue);
    self->execute("set",ss);
  }
  void destroy() {
    std::vector<std::string> ss;
    self->execute("destroy",ss);
  }
}

%extend Slash::Core::ISession {
  void setColor(double aR,double aG,double aB) {
    char s[512];
    ::sprintf(s,"%g %g %g",aR,aG,aB);
    self->setParameter("modeling.color",s);
  }
  void setColor(const std::string& aColor) {
    self->setParameter("modeling.color",aColor);
  }
  Slash::UI::IUI* ui() {
    Slash::UI::IUI_Manager* mgr = Slash::uiManager(*self);
    if(!mgr) return 0;
    return mgr->find("default");
  }
  Slash::UI::IUI* createUI(const std::string& a_file = "@empty@",bool a_show = true,const std::string& a_tk = "") {
    Slash::UI::IUI_Manager* mgr = Slash::uiManager(*self);
    if(!mgr) return 0;
    std::vector<std::string> args;
    return mgr->create(a_tk,a_file,args,a_show);
  }
  bool loadInterpreter(const std::string& aInterp,bool aQuiet = false) {
    Slash::UI::IScriptManager* sm = Slash::scriptManager(*self);
    if(!sm) return false;
    Slash::UI::IInterpreter* interp = sm->findInterpreter(aInterp);
    if(!interp) return false;
    return interp->load(aQuiet);
  }

  // get managers :
  Slash::Core::ILibraryManager* libraryManager() {
    return Slash::libraryManager(*self);
  }
  Slash::Meta::IDictionary* dictionary() {return Slash::dictionary(*self);}
  Slash::Core::IRelationManager* relationManager() {
    return Slash::relationManager(*self);
  }
  Slash::UI::IStyleManager* styleManager() {return Slash::styleManager(*self);}
  Slash::UI::IScriptManager* scriptManager() {
    return Slash::scriptManager(*self);
  }
  Slash::UI::IUI_Manager* uiManager() {return Slash::uiManager(*self);}
  Slash::Data::IProcessor* accessorManager() {
    return Slash::accessorManager(*self);
  }
  Slash::Net::INetManager* netManager() {return Slash::netManager(*self);}
  Slash::Store::ITree* memoryTree() {return Slash::memoryTree(*self);}
  Slash::Data::IFunctionManager* functionManager(){
    return Slash::functionManager(*self);
  }
  Slash::UI::ISceneHandler* sceneHandler() {return Slash::sceneHandler(*self);}

}

%extend Slash::UI::IUI {
  std::string parameterValue(const std::string& aKey) {
    std::string s;
    self->parameterValue(aKey,s);
    return s;
  }
  bool setTreeItems(const std::string& aTreeWidget,const std::string& aFile) {
    std::vector<std::string> lines;
    std::string file;
    if(!inlib::file_name(aFile,file)) return false;
    if(!inlib::file::read(file,lines)) return false;
    self->setParameter(aTreeWidget+".items",inlib::tos<std::string>(lines,"\n"));
    return true;
  }
  void showHelp(const std::string& aFile) {
    self->executeScript("DLD","OnX ui_help_file "+aFile);
  }
  void showDialog(const std::string& aFile,
                  const std::string& aDialog) {
    self->executeScript("DLD","OnX ui_show_dialog "+aFile+" "+aDialog);
  } 
}

#ifdef SWIGPYTHON
%pythoncode {
def create_session(a_args = []):
  set_env(Slash_cout())
  session = Slash_create_session(a_args)
  session.loadInterpreter('Python');
  return session
}
#endif

%inline {
std::ostream& Slash_cout() {return std::cout;}
std::ostream& Slash_cerr() {return std::cerr;}
}

//////////////////////////////////////////////////////////
/// various cast /////////////////////////////////////////
//////////////////////////////////////////////////////////

%define EXTEND_CAST(aName,aClass)
  aClass* cast_##aName () {return INLIB_CAST(*self,aClass);}
%enddef

%extend Slash::Core::IAction {
  EXTEND_CAST(Slash_Core_IVisualizeAction,Slash::Core::IAction);
}

%extend Slash::Core::IManager {
  EXTEND_CAST(Slash_Core_IHierarchy,Slash::Core::IHierarchy);
  EXTEND_CAST(Slash_Core_ILibraryManager,Slash::Core::ILibraryManager);
  EXTEND_CAST(Slash_Core_IOpaqueManager,Slash::Core::IOpaqueManager);
  EXTEND_CAST(Slash_Core_IParameterManager,Slash::Core::IParameterManager);
  EXTEND_CAST(Slash_Core_IRelationManager,Slash::Core::IRelationManager);

  EXTEND_CAST(Slash_Data_IProcessor,Slash::Data::IProcessor);
  EXTEND_CAST(Slash_Data_IFunctionManager,Slash::Data::IFunctionManager);

  EXTEND_CAST(Slash_Meta_IDictionary,Slash::Meta::IDictionary);

  EXTEND_CAST(Slash_Net_INetManager,Slash::Net::INetManager);

  EXTEND_CAST(Slash_UI_IPlotterManager,Slash::UI::IPlotterManager);
  EXTEND_CAST(Slash_UI_IScriptManager,Slash::UI::IScriptManager);
  EXTEND_CAST(Slash_UI_IStyleManager,Slash::UI::IStyleManager);
  EXTEND_CAST(Slash_UI_IUI_Manager,Slash::UI::IUI_Manager);

  EXTEND_CAST(Slash_Store_IStoreManager,Slash::Store::IStoreManager);
  EXTEND_CAST(Slash_Store_ITree,Slash::Store::ITree);

  // DEPRECATED. Keep for backward compatibilty.
  // Use cast_Slash_Data_IProcessor() instead.
  Slash::Data::IProcessor* cast_Lib_IProcessor() {
    return INLIB_CAST(*self,Slash::Data::IProcessor);
  }
}

%extend Slash::UI::IWidget {
  EXTEND_CAST(Slash_UI_ISoViewer,Slash::UI::ISoViewer);

  // DEPRECATED. Keep for backward compatibilty.
  // Use cast_Slash_UI_ISoViewer() instead.
  Slash::UI::ISoViewer* cast_ISoViewer() {
    return INLIB_CAST(*self,Slash::UI::ISoViewer);
  }
}

//%extend Slash::Meta::IObject {
//  template <T>
//  T* safe_cast(Slash::Meta::IObject& a_obj,const std::string& a_to) {
//    std::vector<std::string> v = a_obj.castables();
//    if(a_to not in v) return 0;
//    return a_obj.cast(a_to);
//  }
//}

