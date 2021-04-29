//////////////////////////////////////////////////////////////////////////////
/// Pre SWIG /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
/// Post SWIG ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

%inline {

// Tcl :
// In OnXTcl/Tcl.cxx we define the OnX_sessionPointer
// Tcl command that returns on a string the 
// session address. With the below, we can
// reconvert it to a SWIG wrapped pointer in the
// OnX/scripts/Tcl/init.tcl script.
Slash::Core::ISession* OnX_cast_ISession(const std::string& aString) {
  void* p;
  // Read with %lx and, if failure, with %lu.
  if(!inlib::to_pointer(aString,p)) return 0;
  return (Slash::Core::ISession*)p;
}

static std::string OnX_p2sx(void* a_value){
  char s[512];
  ::sprintf(s,"0x%lx",(unsigned long)a_value);
  return s;
}

static bool set_env(std::ostream& a_out){
  return inlib::osc::set_env(a_out);
}

}

#ifdef SWIGPYTHON

%inline {

void set_PyOS_InputHook(Slash::UI::IUI& aUI) {
  if(PyOS_InputHook==NULL) {
    //see OnX_SWIG_Python.cxx.
    save_PyOS_InputHook = NULL;
    PyOS_InputHook = OnX_PyOS_InputHook;
    sUI = &aUI;
  } else if(PyOS_InputHook==OnX_PyOS_InputHook) {
    sUI = &aUI;
  } else {
    aUI.session().printer().write
       ("OnX::set_PyOS_InputHook : PyOS_InputHook already used. Take control anyway.\n");
    save_PyOS_InputHook = PyOS_InputHook;
    PyOS_InputHook = OnX_PyOS_InputHook;
    sUI = &aUI;
  }
}
void reset_PyOS_InputHook() {
  PyOS_InputHook = save_PyOS_InputHook;
  sUI = 0;
}

}


%pythoncode %{
# print('debug : OnX_SWIG_OnX.i')

def sys_import(aModule):
  import sys
  if aModule in sys.modules:
    reload(sys.modules[aModule])
  else:
    exec('import %s' % aModule)

# WARNING : take care of having defined no "x" variable before the below ; else
#           a OnX.x('<module>') will not call the below !
def x(aModule): # very short cut.
  sys_import(aModule)

def xn(aModule,aCount):
  import OnX
  for i in range(0,aCount):
    OnX.x(aModule)

def import_py(a_dir,a_name):
  comment = 0 # Example : OnX.import_py('$ONXROOT/examples/Python','OnX_exa_detector')
  if fmanip.exists(a_name+'.pyc') == 0 : 
    comment = 0 # Have a local copy of the .py file. 
    comment = 0 # Else the .pyc may be attempted to be created on a 
    comment = 0 # non-writable area (for example the one of the release area).
    if fmanip.exists(a_name+'.py') == 0 : 
      if fmanip.exists(a_dir+'/'+a_name+'.py') == 0 : 
        print('File '+a_dir+'/'+a_name+'.py does not exist.')
        return
      else :
        import compiler
        if fmanip.copy(fmanip.name(a_dir+'/'+a_name+'.py'),'.') == 0 :
          print('Copy of file '+a_dir+'/'+a_name+'.py failed.')
          return
        compiler.compileFile(a_name+'.py')
        fmanip.remove(a_name+'.py')
    else :
      import compiler
      compiler.compileFile(a_name+'.py')
  sys.path.append('') # THE trick to load from current directory.
  sys_import(a_name)
  del sys.path[-1]

def x_exa(a_name):
  import_py('$ONXROOT/examples/Python',a_name)

def dumpMain():
  import sys
  print(dir(sys.modules['__main__']))

def exists(name):
  try:
    eval(name)
    return 1
  except NameError:
    return 0

def sys2onx():
  comment = 0 # Use this function to redirect properly the output to the "UI console".  
  comment = 0 # On some platform (Darwin,Windows), if the OnX::Session redirect 
  comment = 0 # std[out,err] to session.printer (by using the OnX::Session::stdoutToOnX()
  comment = 0 # method), the Python output does not follow. Doing a :
  comment = 0 #   import OnX;OnX.sys2onx() 
  comment = 0 # on a callback, redirect the Python output properly.
  import sys
  sys.stdout = session().printer() 
  sys.stderr = session().printer()

def save2sys():
  import sys
  sys.stdout = onx_stdout_save
  sys.stderr = onx_stderr_save

# Lonely actions done in this module.
import sys
onx_stderr_save = sys.stderr
onx_stdout_save = sys.stdout

def session():
  import sys
  try:
    import OnXPython # could fail if the OnX Python driver not loaded.
    comment = 0 # The below could fail even if the OnXPython module exists.
    comment = 0 # This happens when OnX is deleted. This induces the
    comment = 0 # destruction of the OnX::PythonManager
    comment = 0 # that will remove the "sessionPointer" function from
    comment = 0 # the OnXPython module dictionary.
    string_session = OnXPython.sessionPointer()
    comment = 0 # print(string_session)
    return OnX_cast_ISession(string_session)
  except: # case of OnX having not yet loaded the Python interpreter.
    return None
    
def ui_parameterValue(aWhat):
  comment = 0 # Used by palettes that uses Python on their ok callback.
  comment = 0 # See : OnX::BaseUI::createInputDialog(
  return session().ui().parameterValue(aWhat)

%}
#endif
