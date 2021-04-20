%module OnXLab

%include std_string.i
%include std_vector.i

%template() std::vector<std::string>;
%template() std::vector<double>;
%template() std::vector<int>;

#ifdef SWIGJAVA
%include java_typemaps.i
%apply ARG_STD_VECTOR_STD_STRING {std::vector<std::string>}
%apply ARG_CONST_STD_VECTOR_STD_STRING {const std::vector<std::string>&}
%apply RET_STD_VECTOR_STD_STRING {std::vector<std::string>}
%apply RET_CONST_STD_VECTOR_STD_STRING {const std::vector<std::string>&}

%apply ARG_STD_VECTOR_DOUBLE {std::vector<double>}
%apply ARG_CONST_STD_VECTOR_DOUBLE {const std::vector<double>&}
%apply RET_STD_VECTOR_DOUBLE {std::vector<double>}
%apply RET_CONST_STD_VECTOR_DOUBLE {const std::vector<double>&}

%apply ARG_STD_VECTOR_INT {std::vector<int>}
%apply ARG_CONST_STD_VECTOR_INT {const std::vector<int>&}
%apply RET_STD_VECTOR_INT {std::vector<int>}
%apply RET_CONST_STD_VECTOR_INT {const std::vector<int>&}
#endif

#ifdef SWIGJAVA

%typemap(javacode) AIDA::ITupleFactory %{
  //FIXME : have a typemap for Class.
  public ITuple create(String aPath,
                             String aTitle,
                             String[] aColumnNames,
                             Class[] aColumnTypes,
                             String aOptions) {
    String[] columnTypes = new String[aColumnTypes.length];
    for(int index=0;index<columnTypes.length;index++) {
             if(aColumnTypes[index]==Integer.TYPE) {
        columnTypes[index] = "int";
      } else if(aColumnTypes[index]==Float.TYPE) {
        columnTypes[index] = "float";
      } else if(aColumnTypes[index]==Double.TYPE) {
        columnTypes[index] = "double";
    //} else if(aColumnTypes[index]==String.TYPE) {
    //  columnTypes[index] = "string";
      } else {
        System.out.println
          ("OnXLab : %extend AIDA::ITupleFactory : type not treated.");
        return null;
      }
    }
    return create(aPath,aTitle,aColumnNames,columnTypes,aOptions);
  }
%}

/*
%typemap(javacode) AIDA::ITree %{
  public IBaseHistogram find(String aPath) {
    IManagedObject mobject = find(aPath);
    return mobject.cast_IBaseHistogram();
  }
%}
*/

#endif

%newobject OnXLab::Main::createTreeFactory;
%newobject OnXLab::Main::createHistogramFactory;
%newobject OnXLab::Main::createTupleFactory;
%newobject OnXLab::Main::createFunctionFactory;
%newobject OnXLab::Main::createPlotterFactory;
%newobject OnXLab::Main::createDataPointSetFactory;
%newobject OnXLab::Main::createFitFactory;

//////////////////////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

%include AIDA.i

//FIXME : %include <OnXLab/Core/Main.h>
namespace OnXLab {
class Main : public virtual AIDA::IAnalysisFactory  //so that %extend works.
{
public: //Slash::Core::ISession
  virtual void* cast(const std::string&) const;
public: //AIDA::IAnalysisFactory
  virtual AIDA::ITreeFactory* createTreeFactory();
  virtual AIDA::IHistogramFactory* createHistogramFactory(AIDA::ITree&);
  virtual AIDA::ITupleFactory* createTupleFactory(AIDA::ITree&);
  virtual AIDA::IFunctionFactory* createFunctionFactory(AIDA::ITree&);
  virtual AIDA::IPlotterFactory* createPlotterFactory(int = 0,char** = 0,
                                                      const std::string& = "",
                                                      const std::string& = "");
  virtual AIDA::IDataPointSetFactory* createDataPointSetFactory(AIDA::ITree&);
  virtual AIDA::IFitFactory* createFitFactory();
public:
  Main(const std::vector<std::string>& args = std::vector<std::string>());
  virtual ~Main();
public:
  Slash::UI::IUI* createUI(const std::string& driver,
                           const std::string& file,
                           bool);
  Slash::UI::IUI* ui() const;
  bool loadInterpreter(const std::string&,bool aQuiet = false) const;
};
}

%inline {
AIDA::IAnalysisFactory* find_aida(Slash::Core::ISession& aSession){
  return Slash::find_AIDA(aSession);
}
}

%extend OnXLab::Main {
  Slash::Core::ISession* cast_Slash_Core_ISession() const {
    return (Slash::Core::ISession*)self->cast("Slash::Core::ISession");
  }
}

//////////////////////////////////////////////////////////////////////////////
/// Post SWIG ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/* managed :
%newobject AIDA::IAnalysisFactory::create_tree;
%newobject AIDA::IAnalysisFactory::create_histogram1D;
%newobject AIDA::IAnalysisFactory::create_histogram2D;
%newobject AIDA::IAnalysisFactory::create_histogram3D;
%newobject AIDA::IAnalysisFactory::copy_histogram1D;
%newobject AIDA::IAnalysisFactory::copy_histogram2D;
%newobject AIDA::IAnalysisFactory::copy_baseHistogram;
%newobject AIDA::IAnalysisFactory::create_profile1D;
%newobject AIDA::IAnalysisFactory::create_profile2D;
%newobject AIDA::IAnalysisFactory::create_cloud2D;
%newobject AIDA::IAnalysisFactory::create_dataPointSet;
%newobject AIDA::IAnalysisFactory::create_function;
%newobject AIDA::IAnalysisFactory::create_scripted_function;
%newobject AIDA::IAnalysisFactory::create_tuple;
*/
%newobject AIDA::IAnalysisFactory::create_fitter;
%newobject AIDA::IAnalysisFactory::create_filter;
%newobject AIDA::IAnalysisFactory::create_evaluator;
%newobject AIDA::IAnalysisFactory::create_plotter;

%extend AIDA::IAnalysisFactory {
  AIDA::ITree* find_memoryTree() {
    return amanip::find_mem(*self);
  }
  AIDA::ITree* create_tree(const std::string& aStoreName,
                           const std::string& aStoreType,
                           bool aReadOnly,bool aCreateNew,
                           const std::string& aOptions = "") {
    return amanip::create_tree(*self,aStoreName,aStoreType,
	  	       aReadOnly,aCreateNew,aOptions);
  }
  AIDA::IHistogram1D* create_histogram1D(AIDA::ITree& aTree,
                                         const std::string& aName,
                                         const std::string& aTitle,
				 int aXnumber,double aXmin,double aXmax){
    return amanip::create_histogram_1D(*self,aTree,aName,aTitle,
                              aXnumber,aXmin,aXmax);
  }
  AIDA::IHistogram1D* copy_histogram1D(AIDA::ITree& aTree,
                                       const std::string& aPath,
                                       AIDA::IHistogram1D& aHisto) {
    return amanip::copy_histogram_1D(*self,aTree,aPath,aHisto);
  }
  AIDA::IHistogram2D* copy_histogram2D(AIDA::ITree& aTree,
                                       const std::string& aPath,
                                       AIDA::IHistogram2D& aHisto) {
    return amanip::copy_histogram_2D(*self,aTree,aPath,aHisto);
  }
  AIDA::IBaseHistogram* copy_baseHistogram(AIDA::ITree& aTree,
                                       const std::string& aPath,
                                       AIDA::IBaseHistogram& aHisto) {
    return amanip::copy_base_histogram(*self,aTree,aPath,aHisto);
  }

  AIDA::IHistogram2D* create_histogram2D(AIDA::ITree& aTree,
                                         const std::string& aName,
                                         const std::string& aTitle,
			 int aXnumber,double aXmin,double aXmax,
			 int aYnumber,double aYmin,double aYmax){
    return amanip::create_histogram_2D(*self,aTree,aName,aTitle,
                              aXnumber,aXmin,aXmax,
                              aYnumber,aYmin,aYmax);
  }

  AIDA::IHistogram3D* create_histogram3D(AIDA::ITree& aTree,
                                         const std::string& aName,
                                         const std::string& aTitle,
			 int aXnumber,double aXmin,double aXmax,
			 int aYnumber,double aYmin,double aYmax,
			 int aZnumber,double aZmin,double aZmax){
    return amanip::create_histogram_3D(*self,aTree,aName,aTitle,
                              aXnumber,aXmin,aXmax,
                              aYnumber,aYmin,aYmax,
                              aZnumber,aZmin,aZmax);
  }

  AIDA::IProfile1D* create_profile1D(AIDA::ITree& aTree,
                                     const std::string& aName,
                                     const std::string& aTitle,
				     int aXnumber,double aXmin,double aXmax){
    return amanip::create_profile_1D(*self,aTree,aName,aTitle,
                              aXnumber,aXmin,aXmax);
  }
  AIDA::IProfile1D* create_profile1D(AIDA::ITree& aTree,
                                     const std::string& aName,
                                     const std::string& aTitle,
				     int aXnumber,double aXmin,double aXmax,
                                     double aVmin,double aVmax){
    return amanip::create_profile_1D(*self,aTree,aName,aTitle,
                                     aXnumber,aXmin,aXmax,
                                     aVmin,aVmax);
  }

  AIDA::IProfile2D* create_profile2D(AIDA::ITree& aTree,
                                     const std::string& aName,
                                     const std::string& aTitle,
				     int aXnumber,double aXmin,double aXmax,
				     int aYnumber,double aYmin,double aYmax){
    return amanip::create_profile_2D(*self,aTree,aName,aTitle,
                                     aXnumber,aXmin,aXmax,
                                     aYnumber,aYmin,aYmax);
  }
  AIDA::IProfile2D* create_profile2D(AIDA::ITree& aTree,
                                     const std::string& aName,
                                     const std::string& aTitle,
				     int aXnumber,double aXmin,double aXmax,
				     int aYnumber,double aYmin,double aYmax,
                                     double aVmin,double aVmax){
    return amanip::create_profile_2D(*self,aTree,aName,aTitle,
                                     aXnumber,aXmin,aXmax,
                                     aYnumber,aYmin,aYmax,
                                     aVmin,aVmax);
  }

  AIDA::ICloud2D* create_cloud2D(AIDA::ITree& aTree, 
                                 const std::string& aName,
                                 const std::string& aTitle,
                                 int aLimit = -1,
                                 const std::string& aOptions = "") {
    return amanip::create_cloud_2D
             (*self,aTree,aName,aTitle,aLimit,aOptions);
  }

  AIDA::IDataPointSet* create_dataPointSet(AIDA::ITree& aTree,
                                           const std::string& aName,
                                           const std::string& aTitle,
                                           int aDimension) {
    return amanip::create_data_point_set
             (*self,aTree,aName,aTitle,aDimension);
  }

  AIDA::IFitter* create_fitter(const std::string& aEngine = "") {
    return amanip::create_fitter(*self,aEngine);
  }

  AIDA::IFunction* create_function(AIDA::ITree& aTree,
                                   const std::string& aPath,
                                   const std::string& aTitle,
                                   const std::string& aModel){
    return amanip::create_function(*self,aTree,aPath,aTitle,aModel);
  }
  AIDA::IFunction* create_scripted_function(AIDA::ITree& aTree,
                                            const std::string& aName,
                                            int aDimension,
                                            const std::string& aScript) {
    return amanip::create_scripted_function
             (*self,aTree,aName,aDimension,aScript);
  }
  AIDA::ITuple* create_tuple(AIDA::ITree& aTree,
                             const std::string& aPath,
                             const std::string& aTitle,
                             const std::string& aColumnNames,
                             const std::string& aOptions = "") {
    return amanip::create_tuple
             (*self,aTree,aPath,aTitle,aColumnNames,aOptions);
  }
  AIDA::IFilter* create_filter(AIDA::ITree& aTree,
                               const std::string& aExpression) {
    return amanip::create_filter(*self,aTree,aExpression);
  }
  AIDA::IEvaluator* create_evaluator(AIDA::ITree& aTree,
                                     const std::string& aExpression) {
    return amanip::create_evaluator(*self,aTree,aExpression);
  }
  AIDA::IPlotter* create_plotter(const std::string& aWhere = "") {
    return amanip::create_plotter(*self,aWhere);
  }
}

%extend AIDA::ITree {
  AIDA::IBaseHistogram* find_baseHistogram(const std::string& aName)  {
    return amanip::find_base_histogram(*self,aName);
  }
  AIDA::IHistogram1D* find_histogram1D(const std::string& aName) {
    return amanip::find_histogram_1D(*self,aName);
  }
  AIDA::IHistogram2D* find_histogram2D(const std::string& aName) {
    return amanip::find_histogram_2D(*self,aName);
  }
  AIDA::IFunction* find_function(const std::string& aName) {
    return amanip::find_function(*self,aName);
  }
  AIDA::ITuple* find_tuple(const std::string& aName) {
    return amanip::find_tuple(*self,aName);
  }
#ifdef SWIGPYTHON
  // From Marcus Mendenhall :
  %pythoncode %{
    def smartFind(self, *args, **kwargs):
      return self.find(*args, **kwargs).smartCast()
  %}
#endif
}

/*
  std::string tuple_name(const AIDA::ITuple& aTuple) {
  }
  std::string histogram_name(const AIDA::IBaseHistogram& aHistogram) {
  }
*/

#ifdef SWIGPYTHON
%pythoncode %{

#  print 'debug : OnXLab_SWIG.i'

#
# Script execution context :
# ------------------------
#  An AIDA .py is of the form :
#      import AIDA
#      aida = AIDA.createAnalysisFactory()
#      ...
#      del aida
# This kind of script can be executed in various contexts.
# For example it can be executed "from scratch"
# from the python shell with :
#    OS> <setup AIDA implementation>
#    OS> <start python shell>
#    >>> import my_aida
# or it can be executed in a "Python callback" of
# an OnX interactive program which had not been started
# from Python. For example, someone can execute
# the script from an OnX Python callback, with OnX
# having been started with :
#    OS> <setup AIDA implementation>
#    OS> osc-gui-viewer my_gui.onx
#    GUI> <click in a button executing my_aida.py>
#  It is clear that the AIDA.createAnalysisFactory
# procedure must take into account all these
# possible contexts.
# 

def createAnalysisFactory():
  import OnX
  comment = 0 # FIXME : OnX could have been started, but the
  comment = 0 # OnX Python plugin may be not yet activated, so
  comment = 0 # the below OnX.session is still None...
  if OnX.session() == None:  # Start from scratch :
    OnX.set_env(OnX.Slash_cout())
    comment = 0 # FIXME : case of OnX started without OnXPython loaded ?
    import OnXLab
    import sys
    args = sys.argv
    comment = 0 # args.append('-verbose')  
    comment = 0 # 0 = do not create UI now. 
    comment = 0 # The UI creation,if needed, is delegated to the plotter factory.
    comment = 0 # The plotter factory will use the OnXLab/scripts/OnX/Plotter.onx GUI.
    onxlab_main = OnXLab.Main(args) 
    onxlab_main.loadInterpreter('Python');
    return onxlab_main
  else: # OnX started :
    if OnX.session().findManager('AnalysisFactory') != None:
      import OnXLab
      return OnXLab.find_aida(OnX.session())
    else:
      print('Found OnX, but there is no AnalysisFactory manager !')
      return None
    
%}
#endif

#ifdef SWIGJAVA
%pragma(java) jniclasscode=%{
  static {
    try {
      System.loadLibrary("OnXLab_SWIG_Java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Can't load OnXLab_SWIG_Java jni library.");
    }
  }
%}

%extend AIDA::IAnalysisFactory {
  // Reemulate SLAC java startup :
  static AIDA::IAnalysisFactory* create() {
    return new OnXLab::Main(); 
  }
}

#endif

//////////////////////////////////////////////////////////
/// various cast /////////////////////////////////////////
//////////////////////////////////////////////////////////

// From Marcus Mendenhall :
%define AIDA_CAST(aClass)
  AIDA::aClass* cast_##aClass () {
    return INLIB_CAST(*self,AIDA::aClass);
  }
%enddef

%extend AIDA::IManagedObject {
  AIDA_CAST(IBaseHistogram);
  AIDA_CAST(IHistogram1D);
  AIDA_CAST(IHistogram2D);
  AIDA_CAST(IHistogram3D);
  AIDA_CAST(IProfile1D);
  AIDA_CAST(IProfile2D);
  AIDA_CAST(ICloud1D);
  AIDA_CAST(ICloud2D);
  AIDA_CAST(ICloud3D);
  AIDA_CAST(IDataPointSet);
  AIDA_CAST(ITuple);
  AIDA_CAST(IFunction);
#ifdef SWIGPYTHON
  // From Marcus Mendenhall :
  %pythoncode %{
    def smartCast(self):
      for convertor in (
        self.cast_IHistogram1D,
        self.cast_IHistogram2D,
        self.cast_IHistogram3D,
        self.cast_IProfile1D,
        self.cast_IProfile2D,
        self.cast_ICloud1D,
        self.cast_ICloud2D,
        self.cast_ICloud3D,
        self.cast_IDataPointSet,
        self.cast_ITuple,
        self.cast_IFunction):
        a = convertor()
        if a: 
          return a #a non-null value means it worked
  %}
#endif
}

%extend AIDA::IBaseHistogram {
  AIDA_CAST(IHistogram1D);
  AIDA_CAST(IHistogram2D);
  AIDA_CAST(IHistogram3D);
  AIDA_CAST(IProfile1D);
  AIDA_CAST(IProfile2D);
  AIDA_CAST(ICloud1D);
  AIDA_CAST(ICloud2D);
  AIDA_CAST(ICloud3D);
}

%extend AIDA::ITupleEntry {
  //AIDA_CAST(ITuple); //no ITupleEntry::cast method in AIDA 3.2.1.
  AIDA::ITuple* cast_ITuple() {
    return dynamic_cast<AIDA::ITuple*>(self);
  }
}

// Linux : so that the below dynamic_cast works,
//         before doing an "import AIDA", take care to do :
//
//   import sys
//   if sys.platform == 'linux2':
//     import dl
//     sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)
//

%extend AIDA::IPlotter {
  //no AIDA::IPlotter::cast method in AIDA 3.2.1.
  Slash::UI::IPlotter* cast_Slash_UI_IPlotter() {
    return dynamic_cast<Slash::UI::IPlotter*>(self);
  }
}

%include <Slash/Core/IManager.h>

/* SWIG failed on the below include.
%include <Slash/AIDA/IAnalysisFactory.h>
*/

// the below forward cast + INLIB_SAFE_CAST seems ok. At least we keep that.
namespace Slash {
namespace AIDA {
class IAnalysisFactory;
}}

%extend Slash::Core::IManager {

  Slash::AIDA::IAnalysisFactory* cast_Slash_AIDA_IAnalysisFactory() {
    //return INLIB_CAST(*self,Slash::AIDA::IAnalysisFactory);
    return INLIB_SAFE_CAST(*self,Slash::AIDA::IAnalysisFactory);
  }

  AIDA::ITree* cast_AIDA_ITree () {
    return INLIB_CAST(*self,AIDA::ITree);
  }
}

%define EXTEND_CAST(aName,aClass)
  aClass* cast_##aName () {return INLIB_CAST(*self,aClass);}
%enddef

%extend AIDA::ITree {
  EXTEND_CAST(Slash_Store_ITree,Slash::Store::ITree);
  EXTEND_CAST(Slash_Core_IManager,Slash::Core::IManager);
  EXTEND_CAST(Slash_Core_IHierarchy,Slash::Core::IHierarchy);
}

%extend AIDA::IHistogram1D {
  const inlib::histo::h1d& inlib_object() const {
    const BatchLab::Histogram1D* bh = 
      INLIB_CONST_CAST(*self,BatchLab::Histogram1D);
    return bh->hclHistogram();
  }
}
%extend AIDA::IHistogram2D {
  const inlib::histo::h2d& inlib_object() const {
    const BatchLab::Histogram2D* bh = 
      INLIB_CONST_CAST(*self,BatchLab::Histogram2D);
    return bh->hclHistogram();
  }
}
%extend AIDA::IHistogram3D {
  const inlib::histo::h3d& inlib_object() const {
    const BatchLab::Histogram3D* bh = 
      INLIB_CONST_CAST(*self,BatchLab::Histogram3D);
    return bh->hclHistogram();
  }
}
%extend AIDA::IProfile1D {
  const inlib::histo::p1d& inlib_object() const {
    const BatchLab::Profile1D* bh = 
      INLIB_CONST_CAST(*self,BatchLab::Profile1D);
    return bh->hclHistogram();
  }
}
%extend AIDA::IProfile2D {
  const inlib::histo::p2d& inlib_object() const {
    const BatchLab::Profile2D* bh = 
      INLIB_CONST_CAST(*self,BatchLab::Profile2D);
    return bh->hclHistogram();
  }
}


//%include <amanip/all.h>
