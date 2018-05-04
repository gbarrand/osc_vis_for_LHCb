#ifndef Slash_IAll_h
#define Slash_IAll_h

// interfaces only.
// It is used in OnX/source/SWIG/Slash.i.

// Meta :
#include <Slash/Meta/IClass.h>
#include <Slash/Meta/IDictionary.h>

#ifdef SWIG
//IObject exists also in Slash/Store
#else
#include <Slash/Meta/IObject.h>
#endif

// Core :
#include <Slash/Core/IReader.h>
#include <Slash/Core/IWriter.h>

#include <Slash/Core/IManager.h>
#include <Slash/Core/ISession.h>

#include <Slash/Core/ILibrary.h>
#include <Slash/Core/ILibraryManager.h>

#include <Slash/Core/IRelation.h>
#include <Slash/Core/IRelationVisitor.h>
#include <Slash/Core/IRelationManager.h>

#include <Slash/Core/IDirectoryVisitor.h>
#include <Slash/Core/IAction.h>
#include <Slash/Core/IMessage.h>

#include <Slash/Core/IFileGetter.h>
#include <Slash/Core/IValue.h>
#include <Slash/Core/IParameterManager.h>
#include <Slash/Core/IHierarchy.h>
#include <Slash/Core/IOpaqueManager.h>

// Thread :
#include <Slash/Thread/IMutex.h>

// UI :
#include <Slash/UI/IInterpreter.h>
#include <Slash/UI/IScriptManager.h>

#include <Slash/UI/IStyleManager.h>

#include <Slash/UI/ICallbackData.h>
#include <Slash/UI/IWidgetFactory.h>
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IUI_Manager.h>
#include <Slash/UI/ISceneHandler.h>

#include <Slash/UI/ICyclic.h>

#include <Slash/UI/IPlotter.h>
#include <Slash/UI/IPlotterManager.h>

#include <Slash/UI/ISoViewer.h> //it had forward cast to SoNode.
#include <Slash/UI/IIB.h> //it has forward cast to Lib::ItemML

#include <Slash/UI/IConsole.h>

#include <Slash/UI/IStyle.h>

// Data :
#include <Slash/Data/IAccessor.h>
#include <Slash/Data/IHandler.h>
#include <Slash/Data/IHandlersIterator.h>
#include <Slash/Data/IIterator.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Data/IProperty.h>
#include <Slash/Data/IVisualizer.h>
#include <Slash/Data/IFunction.h>
#include <Slash/Data/IFunctionManager.h>
#include <Slash/Data/IArray.h>
#include <Slash/Data/IArrayFactory.h>

#include <Slash/Data/IVisualizeAction.h>
#include <Slash/Data/IDumpAction.h>

// Store :
#include <Slash/Store/IObject.h>
#include <Slash/Store/ITree.h>
#include <Slash/Store/IFolder.h>
#include <Slash/Store/IFolderIterator.h>
#include <Slash/Store/IFolderVisitor.h>
#include <Slash/Store/IObjectIterator.h>
#include <Slash/Store/IStoreManager.h>

#ifdef SWIG
//IArray exists also in Slash/Data
#else
#include <Slash/Store/IArray.h>
#endif
#include <Slash/Store/IStorable.h>
#include <Slash/Store/IVisitor.h>
#include <Slash/Store/IConstVisitor.h>

// Net :
#include <Slash/Net/INetManager.h>

// AIDA :
//#include <Slash/AIDA/IAnalysisFactory.h>

#endif
