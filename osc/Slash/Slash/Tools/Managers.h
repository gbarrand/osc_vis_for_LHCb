#ifndef Slash_Tools_Managers_h
#define Slash_Tools_Managers_h

#include "Manager.h"

#include <Slash/Meta/IDictionary.h>
#include <Slash/Core/ILibraryManager.h>
#include <Slash/Core/IRelationManager.h>
#include <Slash/UI/IStyleManager.h>
#include <Slash/UI/IScriptManager.h>
#include <Slash/UI/IUI_Manager.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Net/INetManager.h>
#include <Slash/Store/ITree.h>
#include <Slash/Data/IFunctionManager.h>
#include <Slash/UI/ISceneHandler.h>

#include "names.h"

namespace Slash {

inline Slash::Core::ILibraryManager* libraryManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_LibraryManager,Slash::Core::ILibraryManager);}

inline Slash::Meta::IDictionary* dictionary(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,"Lib::Dictionary",Slash::Meta::IDictionary);}

inline Slash::Core::IRelationManager* relationManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_RelationManager,Slash::Core::IRelationManager);}

inline Slash::UI::IStyleManager* styleManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_StyleManager,Slash::UI::IStyleManager);}

inline Slash::UI::IScriptManager* scriptManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_ScriptManager,Slash::UI::IScriptManager);}

inline Slash::UI::IUI_Manager* uiManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_UI_Manager,Slash::UI::IUI_Manager);}

inline Slash::Data::IProcessor* accessorManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_AccessorManager,Slash::Data::IProcessor);}

inline Slash::Net::INetManager* netManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_NetManager,Slash::Net::INetManager);}

//BatchLab
inline Slash::Store::ITree* memoryTree(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,"MemoryTree",Slash::Store::ITree);}

inline Slash::Data::IFunctionManager* functionManager(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,"FunctionManager",Slash::Data::IFunctionManager);}

//vis :
inline Slash::UI::ISceneHandler* sceneHandler(Slash::Core::ISession& aSession) {return Slash_findManager(aSession,Slash_SceneHandler,Slash::UI::ISceneHandler);}

//NOTE : if adding some, think to add in OnX/source/SWIG/Slash.i

}

#endif
