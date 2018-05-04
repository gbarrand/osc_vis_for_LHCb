//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI& ui);
//     void callback_with_arguments(Slash::UI::IUI& ui,
//                                  const std::vector<std::string>& args);
//   }
//

#include "inlib"

// AIDA :
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

// Slash :
#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/UI.h>

// amanip :
#include <amanip/find.h>
#include <amanip/create.h>
#include <amanip/name.h>
#include <amanip/hprint.h>

#include <Slash/Tools/Manager.h>
#include <inlib/cast>

#include <OnXLab/Core/AnalysisFactory.h>

#include "HistogramAction.h"
#include "TupleAction.h"
#include "Accessors.h"

#include <inlib/args>
#include <inlib/math>
#include <inlib/random>

extern "C" {

void OnXLab_open_file(Slash::UI::IUI& aUI,const std::vector<std::string>&);

void OnXLab_command_line_args(Slash::UI::IUI& aUI) {
  // Look if some data file had been passed in argument :
  //   OS> <application> -data_file=<file> <file>
  // Example :
  //   OS> onxlab -data_file=my.data
  inlib::args args(aUI.session().arguments());

 {std::vector<std::string> files = args.find("-data_file");
  for(std::vector<std::string>::const_iterator it = files.begin();
      it!=files.end();++it) {
    if((*it).size()) {
      std::vector<std::string> vs;
      vs.push_back(*it);
      vs.push_back("guessed");
      OnXLab_open_file(aUI,vs);
    }
  }}

}

void OnXLab_createAIDA(Slash::Core::ISession&); //in SessionCallbacks.cxx
void OnXLab_initialize_AIDA(Slash::UI::IUI& aUI) {
  OnXLab_createAIDA(aUI.session());
}

void OnXLab_declare_accessors_sess(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* da = Slash::accessorManager(aSession);
  if(da) {
    if(!da->findAccessor("ManagedObject")) {
      da->addAction(new OnXLab::HistogramAction(*da,aSession));
      da->addAction(new OnXLab::TupleAction(*da,aSession));

      da->addAccessor(new OnXLab::SbObject_Accessor(aSession));
      da->addAccessor(new OnXLab::SbFunction1D_Accessor(aSession));
      da->addAccessor(new OnXLab::SbFunction2D_Accessor(aSession));
      da->addAccessor(new OnXLab::SbBin1D_Accessor(aSession));
      da->addAccessor(new OnXLab::SbBin2D_Accessor(aSession));

      //not used !
      da->addAccessor(new OnXLab::ManagedObjectAccessor(aSession));
      da->addAccessor(new OnXLab::FunctionAccessor(aSession));
    }
  }
}

void OnXLab_declare_accessors(Slash::UI::IUI& aUI) {
  OnXLab_declare_accessors_sess(aUI.session());
}

//////////////////////////////////////////////////////////////////////////////
/// AIDA /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnXLab_aida_tree_clear(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] name of the AIDA::ITree.
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  AIDA::ITree* tree = Slash_findManager(aUI.session(),aArgs[0],AIDA::ITree);
  if(!tree) return;
  tree->rmdir("/");
}

void OnXLab_aida_tree_delete_object(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] path (coming from a tree selection).
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  AIDA::ITree* tree;
  std::string mgr,path,basename;
  AIDA::IManagedObject* object = 
    find_aida_object(aArgs[0],aUI.session(),tree,mgr,path,basename);
  if(!object) return;
  delete object;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnXLab_tree_selection(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] path (coming from a tree selection).
  // args[1] where to put the graphics.
  // optional :
  // args[2] append to the plotter.
  // args[3] automatic show palette.
  // args[4] is_active
  if(!inlib::check_min_args(aArgs,2,aUI.session().cout())) return;

  // do not pass directly aArgs[1] to pf->create(<wiget>);
  // in order to treat @current@.
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[1]);
  if(!widget) return;

  //NOTE : the below find_aida_object may trigger an update message
  //       in case the object is found in file and then managed
  //       in the tree. (The tree.addObject adds the object in a
  //       Folder that then does emitUpdate).

  AIDA::ITree* tree;
  std::string mgr,path,basename;
  AIDA::IManagedObject* object = 
    find_aida_object(aArgs[0],aUI.session(),tree,mgr,path,basename);
  if(!object) return;

  Slash::Core::ISession& session = aUI.session();
  std::ostream& out = aUI.session().cout();

  bool append = false;
  if((aArgs.size()>=3) && !inlib::to(out,aArgs[2],append)) return;
  //bool show_palette = true;
  //if((aArgs.size()>=4) && !inlib::to(out,aArgs[3],show_palette)) return;
  bool is_active = true;
  if((aArgs.size()>=5) && !inlib::to(out,aArgs[4],is_active)) return;

  if(!is_active) return;

  // Find AIDA :
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,widget->name());
  if(!plotter) return;
  AIDA::IPlotterRegion& region = plotter->currentRegion();

  AIDA::ICloud* cl = INLIB_CAST(*object,AIDA::ICloud);
  if(cl) {
    region.clear();
    delete plotter;
    amanip::hprint(*object,session.cout());
    return;
  } 

  AIDA::IBaseHistogram* h = INLIB_CAST(*object,AIDA::IBaseHistogram);
  if(h) {
    if(!append) region.clear();
    region.plot(*h);
    delete plotter;

    amanip::hprint(*object,session.cout());
    return;
  } 

  AIDA::IFunction* f = INLIB_CAST(*object,AIDA::IFunction);
  if(f) {
    if(!append) region.clear();
    region.plot(*f);
    delete plotter;
    amanip::hprint(*object,session.cout());
    return;
  } 

  AIDA::IDataPointSet* dps = INLIB_CAST(*object,AIDA::IDataPointSet);
  if(dps) {
    if(!append) region.clear();
    region.plot(*dps);
    delete plotter;

    amanip::hprint(*object,session.cout());
    return;
  }

  AIDA::ITuple* tuple = INLIB_CAST(*object,AIDA::ITuple);
  if(tuple) {
    region.clear();
    delete plotter;
    std::ostream& out = aUI.session().cout();
    out << "/////////////////" << std::endl;
    out << "name    : " << object->name() << std::endl;
    out << "title   : " << tuple->title() << std::endl;
    out << "columns : " << tuple->columns() << std::endl;
    out << "rows    : " << tuple->rows() << std::endl;
    return;
  }

  delete plotter;

  out << "OnXLab_tree_selection :"
      << " don't know how to represent " << inlib::sout(basename)
      << std::endl;
}

//////////////////////////////////////////////////////////////////////////////
/// OnXLab/scripts/OnX/File.onx //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnXLab_open_file(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs
) {
  // args[0] file name
  // args[1] format = [guessed, aida, hdf5, ascii, tnt, root, hbook]
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  const std::string& file = aArgs[0];
  if(file.empty()) return;
  std::string format = aArgs[1];
  if((format=="guessed")||(format=="*")) {
    std::string sfx = inlib::suffix(file);
    if(sfx=="gz") sfx = inlib::suffix(file.substr(0,file.size()-3));
    //FIXME : zip, fs
    if( (sfx!="aida") &&
        (sfx!="hdf5") &&
        (sfx!="ascii") &&
        (sfx!="tnt") &&
        (sfx!="root") &&
        (sfx!="hbook") ){
      std::ostream& out = aUI.session().cout();
      out << "OnXLab_open_file :"
          << " suffix of file " << inlib::sout(file) << " unknown."
          << " You have to specify explicitly the file format."
          << std::endl;
      return;
    }
    format = sfx;
  }
  Slash::Core::ISession& session = aUI.session();
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;
  amanip::create_tree(*aida,file,format,true,false);
}

//////////////////////////////////////////////////////////////////////////////
/// OnXLab/scripts/OnX/InputHistogram.onx ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLab_InputHistogram_ok(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // Input fields : 
  //   string sname
  //   string stitle
  //   int xnumber
  //   real xmin 
  //   real xmax
  //   bool fill
  //   int entries
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;

  /////////////////////////////
  // Get input fields values :
  /////////////////////////////
  std::string svalue;

  std::string name;
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_name.value",name)) return;

  std::string title;
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_title.value",title)) return;
  
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_xnumber.value",svalue)) return;
  int xnumber;
  if(!inlib::to<int>(svalue,xnumber)) return;

  double xmin;
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_xmin.value",svalue)) return;
  if(!inlib::to<double>(svalue,xmin)) return;

  double xmax;
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_xmax.value",svalue)) return;
  if(!inlib::to<double>(svalue,xmax)) return;

  bool fill;
  if(!aUI.parameterValue("OnXLab_InputHistogram_input_fill.value",svalue)) return;
  if(!inlib::to(svalue,fill)) return;

  if(!aUI.parameterValue("OnXLab_InputHistogram_input_entries.value",svalue)) return;
  int entries;
  if(!inlib::to<int>(svalue,entries)) return;

  /////////////////////////////
  /////////////////////////////
  /////////////////////////////
  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return;

  AIDA::IManagedObject* object = memoryTree->find(name);
  if(object) {
    aUI.echo("Histogram "+name+" already exists.");
  } else {
    if(xnumber<=0) {
      aUI.echo("Bad number of bins.");
    } else if(xmax<=xmin) {
      aUI.echo("Max <= min !");
    } else {
      
      AIDA::IHistogram1D* histogram = 
        amanip::create_histogram_1D
          (*aida,*memoryTree,name,title,xnumber,xmin,xmax);
      if(!histogram) return;

      if(fill) {
        // Fill with random :;
        inlib::random::gauss rg(xmin,xmax);
        for(int index=0;index<entries;index++)
          histogram->fill(rg.shoot(),1);
      }
      // Plot it :
      AIDA::IPlotter* plotter = 
        amanip::create_plotter(*aida,widget->name());
      if(plotter) {
        AIDA::IPlotterRegion& region = plotter->currentRegion();
        region.clear();
        region.plot(*histogram);
        delete plotter;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
/// OnXLab/scripts/OnX/InputFunction.onx /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnXLab_InputFunction_ok(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // Input fields : 
  //   string name
  //   string title
  //   string kind
  //   string class_script
  //   string scripted_script
  //   string compiled_script
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;

  /////////////////////////////
  // Get input fields values :
  /////////////////////////////
  std::string svalue;

  std::string name;
  if(!aUI.parameterValue("OnXLab_InputFunction_input_name.value",name)) return;

  std::string title;
  if(!aUI.parameterValue("OnXLab_InputFunction_input_title.value",title)) return;
  
  std::string kind;
  if(!aUI.parameterValue("OnXLab_InputFunction_input_kind.value",kind)) return;

  std::string script;
  if(!aUI.parameterValue
    ("OnXLab_InputFunction_input_"+kind+"_script.value",script)) return;

  bool plot;
  if(!Slash::parameter(aUI,"OnXLab_InputFunction_input_plot.value",plot)) return;

  /////////////////////////////
  /////////////////////////////
  /////////////////////////////
  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return;

  AIDA::IManagedObject* object = memoryTree->find(name);
  if(object) {
    aUI.echo("Function \""+name+"\" already exists.");
    name = amanip::create_name(*memoryTree,name+"_");
    aUI.echo("Use \""+name+"\".");
  }

  std::string path = name;    
  std::string codelet = kind+":"+script;
   
  AIDA::IFunction* function = 
    amanip::create_function(*aida,*memoryTree,path,title,codelet);
  if(!function) {
    aUI.echo("Can't create function \""+name+"\".");
    return;
  }

  if(plot) {
    if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
    Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
    if(!widget) return;
    AIDA::IPlotter* plotter = 
      amanip::create_plotter(*aida,widget->name());
    if(plotter) {
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.plot(*function);
      delete plotter;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnXLab_LegendEditor_update(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] the tree widget selection.
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  if(aArgs[0].empty()) {
    std::ostream& out = aUI.session().cout();
    out << "OnXLab_LegendEditor_update :"
        << " no selection given." 
        << std::endl;
    return;
  }

  AIDA::ITree* tree;
  std::string mgr,path,basename;
  AIDA::IManagedObject* object = 
    find_aida_object(aArgs[0],aUI.session(),tree,mgr,path,basename);
  if(!tree) return;
  if(object) {
    aUI.setParameter("OnXLab_LegendEditor_tree.value",mgr);
    aUI.setParameter("OnXLab_LegendEditor_object.value",path);
  } else {
    aUI.setParameter("OnXLab_LegendEditor_tree.value","");
    aUI.setParameter("OnXLab_LegendEditor_object.value","");
  }
}

void OnXLab_LegendEditor_entry(Slash::UI::IUI& aUI) {
  std::string legend = aUI.callbackValue();

  std::string stree;
  std::string path;
  aUI.parameterValue("OnXLab_LegendEditor_tree.value",stree);
  aUI.parameterValue("OnXLab_LegendEditor_object.value",path);

  Slash::Core::IManager* manager = aUI.session().findManager(stree);
  if(!manager) {
    std::ostream& out = aUI.session().cout();
    out << "OnXLab_LegendEditor_entry :"
        << " manager " << inlib::sout(stree) << " not found."
        << std::endl;
    return;
  }
  AIDA::ITree* tree = INLIB_CAST(*manager,AIDA::ITree);
  if(!tree) {
    std::ostream& out = aUI.session().cout();
    out << "OnXLab_LegendEditor_entry :"
        << " manager " << inlib::sout(stree) << " is not an AIDA::ITree."
        << std::endl;
    return;
  }
  AIDA::IManagedObject* object = tree->find(path);
  if(!object) {
    std::ostream& out = aUI.session().cout();
    out << "OnXLab_LegendEditor_entry :"
        << " object " << inlib::sout(path) << " not found in AIDA::ITree "
        << inlib::sout(stree)
        << std::endl;
    return;
  }

  if(AIDA::IBaseHistogram* bh = INLIB_CAST(*object,AIDA::IBaseHistogram)) {
    bh->annotation().addItem("Legend",legend);
  } else if(AIDA::IDataPointSet* dps = INLIB_CAST(*object,AIDA::IDataPointSet)) {
    dps->annotation().addItem("Legend",legend);
  } else if(AIDA::IFunction* f = INLIB_CAST(*object,AIDA::IFunction)) {
    f->annotation().addItem("Legend",legend);
  }
}

//////////////////////////////////////////////////////////////////////////////
/// data /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void data_fill1D(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(aArgs.size()!=2) return;
  Slash::Data::fill_histogram_1D(aUI.session(),aArgs[0],aArgs[1]);
}

void data_fill2D(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(aArgs.size()!=3) return;
  Slash::Data::fill_histogram_2D(aUI.session(),aArgs[0],aArgs[1],aArgs[3]);
}

void data_tuple(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  if(aArgs.size()!=2) return;
  Slash::Data::create_tuple(aUI.session(),aArgs[0],aArgs[1]);
}

} // extern "C"


#include <inlib/math>
#include <cmath>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
/// OnXLab/scripts/OnX/InputInsidePolygon.onx ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLab_InputInsidePolygon_ok(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args :
  //   viewer
  // Input fields : 
  //   string name
  //   string title
  //   unsigned int pointn

  /////////////////////////////
  // Get input fields values :
  /////////////////////////////
  std::string svalue;

  std::string name;
  if(!aUI.parameterValue("OnXLab_InputInsidePolygon_input_name.value",name)) return;

  std::string title;
  if(!aUI.parameterValue("OnXLab_InputInsidePolygon_input_title.value",title)) return;
  
  unsigned int pointn;
  typedef unsigned int uint_t;
  if(!Slash::parameter<uint_t>(aUI,"OnXLab_InputInsidePolygon_input_pointn.value",pointn)) return;

  double radius;
  if(!Slash::parameter<double>(aUI,"OnXLab_InputInsidePolygon_input_radius.value",radius)) return;
  
  std::vector<double> trans;
  if(!Slash::parameter<double>(aUI,"OnXLab_InputInsidePolygon_input_translation.value",trans)) return;
  
  double angle; //in degree
  if(!Slash::parameter<double>(aUI,"OnXLab_InputInsidePolygon_input_angle.value",angle)) return;
  
  bool plot;
  if(!Slash::parameter(aUI,"OnXLab_InputInsidePolygon_input_plot.value",plot)) return;

  bool editable;
  if(!Slash::parameter(aUI,"OnXLab_InputInsidePolygon_input_editable.value",editable)) return;

  if(pointn<=2) {
    aUI.echo("At least 3 points expected.");
    return;
  }

  if(trans.size()!=2) {
    aUI.echo("At least 2 numbers expected for translation.");
    return;
  }

  /////////////////////////////
  /////////////////////////////
  /////////////////////////////
  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return;

  AIDA::IManagedObject* object = memoryTree->find(name);
  if(object) {
    aUI.echo("Function \""+name+"\" already exists.");
    name = amanip::create_name(*memoryTree,name+"_");
    aUI.echo("Use \""+name+"\".");
  }

  std::string path = name;   

  AIDA::IFunction* function = 
    amanip::create_function
      (*aida,*memoryTree,path,title,"class:InsidePolygon");
  if(!function) {
    aUI.echo("Can't create function \""+name+"\".");
    return;
  }

  std::vector<double> params(2*pointn);
 {double pi = inlib::pi();
  angle *= pi/180.0;
  double d = 2*pi/double(pointn);
  double a = 0;
  for(unsigned int index=0;index<pointn;index++) {
    params[2*index] = trans[0]+radius*::cos(angle+a);         
    params[2*index+1] = trans[1]+radius*::sin(angle+a);         
    a += d;
  }}
  function->setParameters(params);

  if(plot) {
    if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
    Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
    if(!widget) return;
    AIDA::IPlotter* plotter = 
      amanip::create_plotter(*aida,widget->name());
    if(plotter) {
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.setParameter("plotter.shapeAutomated","FALSE");
      region.setParameter("plotter.shape","XY");

      function->annotation().addItem("plotter.modeling","polygon");
      function->annotation().addItem("plotter.color","0.6 0.6 0.6");
      function->annotation().addItem("plotter.transparency","0.5");
      function->annotation().addItem("plotter.editable",editable?"TRUE":"FALSE");

      region.plot(*function);
      delete plotter;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
/// OnXLab/scripts/OnX/InputInsideEllipse.onx ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLab_InputInsideEllipse_ok(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  // args :
  //   viewer
  // Input fields : 
  //   string name
  //   string title
  //   a
  //   b

  /////////////////////////////
  // Get input fields values :
  /////////////////////////////
  std::string svalue;

  std::string name;
  if(!aUI.parameterValue("OnXLab_InputInsideEllipse_input_name.value",name)) return;

  std::string title;
  if(!aUI.parameterValue("OnXLab_InputInsideEllipse_input_title.value",title)) return;
  
  double a;
  if(!Slash::parameter<double>(aUI,"OnXLab_InputInsideEllipse_input_a.value",a)) return;

  double b;
  if(!Slash::parameter<double>(aUI,"OnXLab_InputInsideEllipse_input_b.value",b)) return;

  bool plot;
  if(!Slash::parameter(aUI,"OnXLab_InputInsideEllipse_input_plot.value",plot)) return;

  bool editable;
  if(!Slash::parameter(aUI,"OnXLab_InputInsideEllipse_input_editable.value",editable)) return;

  /////////////////////////////
  /////////////////////////////
  /////////////////////////////
  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  AIDA::ITree* memoryTree = amanip::find_mem(*aida);
  if(!memoryTree) return;

  AIDA::IManagedObject* object = memoryTree->find(name);
  if(object) {
    aUI.echo("Function \""+name+"\" already exists.");
    name = amanip::create_name(*memoryTree,name+"_");
    aUI.echo("Use \""+name+"\".");
  }

  std::string path = name;   

  AIDA::IFunction* function = 
    amanip::create_function
      (*aida,*memoryTree,path,title,"class:InsideEllipse");
  if(!function) {
    aUI.echo("Can't create function \""+name+"\".");
    return;
  }

  std::vector<double> params(2);
  params[0] = a;
  params[1] = b;
  function->setParameters(params);

  if(plot) {
    if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
    Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
    if(!widget) return;
    AIDA::IPlotter* plotter = 
      amanip::create_plotter(*aida,widget->name());
    if(plotter) {
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.setParameter("plotter.shapeAutomated","FALSE");
      region.setParameter("plotter.shape","XY");

      function->annotation().addItem("plotter.modeling","polygon");
      function->annotation().addItem("plotter.color","0.6 0.6 0.6");
      function->annotation().addItem("plotter.transparency","0.5");
      function->annotation().addItem("plotter.editable",editable?"TRUE":"FALSE");

      region.plot(*function);
      delete plotter;
    }
  }
}


} // extern "C"
