#/////////////////////////////////////////////////////////////////////////////
# OnX module :
#/////////////////////////////////////////////////////////////////////////////

proc OnX_session {} {
  # Load OnX_SWIG_Tcl DLL and get the OnX session :

  if { [info commands OnX_sessionPointer] == "" } {
    puts "OnXTcl not loaded."
    return ""
  }

  if { [info commands OnX_cast_ISession] == "" } {
    if { [info exists ::env(ONX_SWIG_TCL_LIB) ] } {
      load $::env(ONX_SWIG_TCL_LIB) OnX
    } else {
      puts "ONX_SWIG_TCL_LIB environment variable not defined."
      puts "OnX setup probably not executed."
      return ""
    }    
  }

  if { [info commands OnX_cast_ISession] == "" } {
    puts "OnX_SWIG_Tcl not loaded."
    return ""
  }

  # Get the raw session pointer through the 
  # Tcl.cxx/OnX_tcl_session_pointer native Tcl command :
  set raw_session_pointer [OnX_sessionPointer]
  #puts $raw_session_pointer
  if { $raw_session_pointer == "NULL" } {
    puts "init.tcl : can't get the OnX session pointer."
    return ""
  }

  # Convert the raw session pointer to a SWIG ISession pointer :
  return [OnX_cast_ISession $raw_session_pointer]
}

