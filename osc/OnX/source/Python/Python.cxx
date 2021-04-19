#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <Slash/UI/IScriptManager.h>
#include <Slash/UI/IInterpreter.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Manager.h>

#include <inlib/pointer>
#include <inlib/words>
#include <inlib/sto>
#include <inlib/tos>
#include <inlib/file>
#include <inlib/cstr>
#include <inlib/mnmx>
#include <inlib/cast>

extern "C" {
void OnXPythonInitialize(Slash::Core::ISession&);
void OnXPythonFinalize(Slash::Core::ISession&);
}

#if defined(__linux)
// Clash between os_defines.h (coming from <string>) and pyconfig.h
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif
#endif

#include <Python.h>

// WARNING : take care that the below functions be no more
//  used when the DLL will be closed.
static bool python_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
extern "C" {
static PyObject* sessionPointer(PyObject*,PyObject*);
}

namespace OnX {

class PythonManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const {
         INLIB_IF_CAST(OnX::PythonManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  PythonManager(Slash::Core::ISession& aSession)
  :fName("PythonManager")
  ,fSession(aSession)
  ,fInitedByOnX(false)
  ,fMethods(0)
  ,fModuleTag(0)
  ,fModule(0)
  //,fThreadState(0)
  {
    // We have to declare the OnX session to Python.
    if(!Py_IsInitialized()) {
      //FIXME : should pass a copied string. 
      // But in fact, do we want to complicate life in passing something ?
      // If passing something, have to restore something at end.
      //Py_SetProgramName((char*)"OnX::PythonManager");
      Py_Initialize();
      fInitedByOnX =  true;
    }
    PyEval_InitThreads();

    fModuleTag = PyCObject_FromVoidPtr(&fSession,NULL); 
    //fModuleTag->ob_refcnt = 1

    // See Include/modsupport.h Python/modsupport.c :
    //FIXME : module should be suffixed by some Slash::Core::ISession::name().
    std::string moduleName("OnX_PythonManager");     
    // WARNING :
    //  The below return a singleton for the whole Python session.
    // Then passing here twice with the same moduleName will return 
    // the same object without increasing its ob_refcnt.
    //  The second time, the new fModuleTag replaces the previous one
    // which is then dereferenced (the new one being referenced).
    fMethods = new PyMethodDef[2];
    fMethods[0].ml_name = inlib::str_dup("sessionPointer");
    fMethods[0].ml_meth = sessionPointer;
    fMethods[0].ml_flags = 0;
    fMethods[0].ml_doc = NULL; 

    fMethods[1].ml_name = NULL;
    fMethods[1].ml_meth = NULL;
    fMethods[1].ml_flags = 0;
    fMethods[1].ml_doc = NULL; 
    fModule = Py_InitModule4((char*)moduleName.c_str(),
                             fMethods, 
			     (char*)NULL,fModuleTag,
                             PYTHON_API_VERSION);
    //We are not the creator of fModule, we don't have to DECREF at end.
    if(fModule==NULL) {
      fSession.out() << "OnX::PythonManager::PythonManager :"
                     << " Py_InitModule4 failed for " 
                     << inlib::sout(moduleName) << "."
                     << std::endl;
      return;
    }
    //fModule->ob_refcnt = 1 // the first time. 
                             // But may be more if having done 
                             // a "import <moduleName>"
                             // and passed here again.
    //fModuleTag->ob_refcnt = 2

    //enableThread();
  }
  virtual ~PythonManager() { 
    //NOTE : an exit of Python shell may induce that
    //       Python is Finalized at this point.
    if(Py_IsInitialized()) {
      //disableThread();
      if(fModuleTag) {
        Py_DECREF(fModuleTag); //Since we are the creator.
        fModuleTag = 0;
      }
      if(fModule) {
        PyObject* d = PyModule_GetDict(fModule);
        if(d) {
          PyDict_DelItemString(d,fMethods[0].ml_name);
        }
        //Py_DECREF(fModule); //We are not the creator.
        fModule = 0;
      }
      if(fInitedByOnX) {
        Py_Finalize();
      }
    }
    char* c_str = (char*)fMethods[0].ml_name;
    inlib::str_del(c_str);
    delete [] fMethods;
  }
  bool isValid() const {
    //return fPyLoaded?true:false;
    return fModule?true:false;
  }
  //void disableThread() {
  //  if(!fThreadState) return; //done
  //  PyEval_RestoreThread(fThreadState);
  //  fThreadState = 0;
  //}
  //void enableThread() {
  //  if(fThreadState) return; //done
  //  fThreadState = PyEval_SaveThread();
  //}
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  bool fInitedByOnX;
  PyMethodDef* fMethods;
  PyObject* fModuleTag;
  PyObject* fModule;
  //PyThreadState* fThreadState;
};

}

//////////////////////////////////////////////////////////////////////////////
void OnXPythonInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
// Executed, by Interpreters, at first Python>... command
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnX::PythonManager* pythonManager =
    Slash_findManager(aSession,"OnX::PythonManager",OnX::PythonManager);
  if(!pythonManager) {
    pythonManager = new OnX::PythonManager(aSession);
    aSession.addManager(pythonManager);
  }

  Slash::UI::IScriptManager* scriptManager = Slash::scriptManager(aSession);
  if(scriptManager) {
    //Interpreter should have been declared in Session.cxx.
    Slash::UI::IInterpreter* interp = scriptManager->findInterpreter("Python");
    if(interp) {
      interp->setFunction(python_Function,pythonManager);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnXPythonFinalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("PythonManager");
}

inline void justify_lines(const std::vector<std::string>& a_text,std::vector<std::string>& a_lines){
  unsigned int number = a_text.size();
  int NotFound = -1;
  int begin = NotFound;
  unsigned int index;
  for(index=0;index<number;index++) {
    const std::string& line = a_text[index];
    std::string::size_type not_space = line.find_first_not_of(' ');
    if(not_space==std::string::npos) {
      // Empty or blank line :
      // skip.
    } else {
      // Some text is in the line :
      if(begin==NotFound) {
        begin = not_space;
      } else {
        begin = inlib::mn<int>(begin,(int)not_space);
      }
    }
  }
  if(begin==NotFound) return;
  a_lines.clear();
  for(index=0;index<number;index++) {
    const std::string& line = a_text[index];
    std::string::size_type not_space = line.find_first_not_of(' ');
    if(not_space==std::string::npos) {
      // Empty or blank line :
      a_lines.push_back("");
    } else {
      a_lines.push_back(line.substr(begin,line.length()-begin));
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
bool python_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void* aPythonManager
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnX::PythonManager* pythonManager = (OnX::PythonManager*)aPythonManager;
  if(!pythonManager->isValid()) return false;

  //FIXME : script may contain usage of '\n' !
  std::vector<std::string> text = inlib::words(aString,"\\n\\");
  text.push_back(""); //Text should finish with an empty ligne.
  // Justify text on the left :
  std::vector<std::string> lines;
  justify_lines(text,lines);
  std::string s = inlib::tos<std::string>(lines,"\n");

#if (PY_MAJOR_VERSION <= 2) && (PY_MINOR_VERSION <= 2)
#else 
  PyGILState_STATE state = PyGILState_Ensure();
#endif

  PyRun_SimpleString((char*)s.c_str());

#if (PY_MAJOR_VERSION <= 2) && (PY_MINOR_VERSION <= 2)
#else 
  PyGILState_Release(state);
#endif

  return true;
}
//////////////////////////////////////////////////////////////////////////////
PyObject* sessionPointer(
 PyObject* aTag
,PyObject* /*aArgs*/ 
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //aTag is the PythonManager::fModuleTag object.

  Slash::Core::ISession* session = 
    (Slash::Core::ISession*)(void*)PyCObject_AsVoidPtr(aTag);

  // The format must be the same than the 
  // OnX.i/OnX_cast_ISession function.
  std::string s = inlib::p2sx(session);

#if PY_VERSION_HEX >= 0x03000000
  return PyUnicode_FromString(s.c_str());
#else  
  return PyString_FromString(s.c_str());
#endif
}
