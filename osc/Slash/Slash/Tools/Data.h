#ifndef Slash_Tools_Data_h
#define Slash_Tools_Data_h

#include <Slash/Core/ISession.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/names.h>
#include <Slash/Data/IDumpAction.h>

#include <inlib/sto>
#include <inlib/words>

namespace Slash {
namespace Data {

inline void collect(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aCuts = "") {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aCuts);
  proc->execute("collect",ss);
}

inline void visualize(Slash::Core::ISession& aSession,const std::string& aWhere,const std::string& aStyle = "") {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back("where="+aWhere);
  if(aStyle.size()) ss.push_back("style="+aStyle); //used in InventorAccessor.
  proc->execute("visualize",ss);
}

inline int number(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return 0;
  std::vector<std::string> ss;
  std::string s = proc->execute(Slash_action_number,ss);
  int number;
  if(!inlib::to<int>(s,number)) return 0;
  return number;  
}

inline std::string values(Slash::Core::ISession& aSession,const std::string& aWhat) {
  // returned string is a \n separated list.
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return std::string();
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  return proc->execute("values",ss);
}

inline std::vector<std::string> values_vector(Slash::Core::ISession& aSession,const std::string& aWhat) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return std::vector<std::string>();
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  std::vector<std::string> ws;
  inlib::words(proc->execute("values",ss),"\n",false,ws);
  return ws;
}

inline void dump(Slash::Core::ISession& aSession,const std::string& aFormat = "table") {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back(aFormat);
  proc->execute(Slash_action_dump,ss);
}

inline void filter(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aFilter = "") {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aFilter);
  proc->execute("filter",ss);
}

inline void set(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aValue) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aValue);
  proc->execute("set",ss);
}

inline void init(Slash::Core::ISession& aSession,const std::string& aValues) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back(aValues);
  proc->execute("init",ss);
}

inline void destroy(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  proc->execute("destroy",ss);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inline void fill_histogram_1D(
 Slash::Core::ISession& aSession
,const std::string& aHistogram
,const std::string& aExpressionX
) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back("tree=MemoryTree");
  ss.push_back("path="+aHistogram);
  ss.push_back("x="+aExpressionX);
  ss.push_back("create=no");
  proc->execute("histogram",ss);
}

inline void fill_histogram_2D(
 Slash::Core::ISession& aSession
,const std::string& aHistogram
,const std::string& aExpressionX
,const std::string& aExpressionY
) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back("tree=MemoryTree");
  ss.push_back("path="+aHistogram);
  ss.push_back("dimension=2");
  ss.push_back("x="+aExpressionX);
  ss.push_back("y="+aExpressionY);
  ss.push_back("create=no");
  proc->execute("histogram",ss);
}

inline void create_tuple(
 Slash::Core::ISession& aSession
,const std::string& aPath
,const std::string& aTitle
) {
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  std::vector<std::string> ss;
  ss.push_back("tree=MemoryTree");
  ss.push_back("path="+aPath);
  ss.push_back("title="+aTitle);
  proc->execute("tuple",ss);
}

}}

#include <Slash/Core/IAction.h>
#include <Slash/Data/IVisualizeAction.h>
#include <Slash/UI/IStyle.h>

#include <inlib/cast>

namespace Slash {
namespace Data {

inline void visualize(Slash::Core::ISession& aSession,const Slash::UI::IStyle& aStyle,const std::string& where = "@current@"){
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  Slash::Core::IAction* action = proc->findAction("visualize");
  if(!action) {   
    aSession.out() << "Slash::Data::visualize :"
                   << " \"visualize\" Action not found."
                   << std::endl;
    return;
  }
  Slash::Data::IVisualizeAction* vaction = 
    INLIB_CAST(*action,Slash::Data::IVisualizeAction);
  if(!vaction) {
    aSession.out() << "Slash::Data::IProcessor :"
                   << " IAction is not a IVisualizeAction."
                   << std::endl;
    return;
  }
  std::vector<std::string> ss;
  ss.push_back("where="+where);
  vaction->execute(ss,aStyle);
}

inline void dump(Slash::Core::ISession& aSession,std::ostream& aOut,const std::string& aFormat = "table"){
  Slash::Data::IProcessor* proc = Slash::accessorManager(aSession);
  if(!proc) return;
  Slash::Core::IAction* action = proc->findAction(Slash_action_dump);
  if(!action) {   
    aSession.out() << "Slash::Data::dump :"
               << " Action " << inlib::sout(Slash_action_dump) << " not found."
                   << std::endl;
    return;
  }
  Slash::Data::IDumpAction* daction = 
    INLIB_CAST(*action,Slash::Data::IDumpAction);
  if(!daction) {
    aSession.out() << "Slash::Data::IProcessor :"
                   << " IAction is not a IDumpAction."
                   << std::endl;
    return;
  }
  std::vector<std::string> ss;
  ss.push_back(aFormat);
  daction->execute(ss,aOut);
}

}}

#endif
