//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(IUI&);
//     void callback_with_arguments(IUI&,const std::vector<std::string>&);
//   }
//


#include <exlib/xml/parser>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>

// Emulate Gaudi LHCb begin of transient store :
static std::vector<std::string> uiSvc_dataChildren(
 Slash::Core::ISession& aSession
,const std::string& aPath
) {
  std::vector<std::string> v;

  // /dd :
  if(aPath=="/dd") {
    v.push_back(aPath+"/"+"Structure");
    v.push_back(aPath+"/"+"Geometry");
  } else if(aPath=="/dd/Structure") {
    v.push_back(aPath+"/"+"LHCb");
  } else if(aPath=="/dd/Structure/LHCb") {
    v.push_back(aPath+"/"+"BeforeMagnet");
    v.push_back(aPath+"/"+"AfterMagnet");
  } else if(aPath=="/dd/Structure/LHCb/BeforeMagnet") { //Leaf
  } else if(aPath=="/dd/Structure/LHCb/AfterMagnet") { //Leaf

  } else if(aPath=="/dd/Geometry") {
    v.push_back(aPath+"/"+"LHCb");
  } else if(aPath=="/dd/Geometry/LHCb") { //Leaf

  } else if(aPath=="/Event") {
    v.push_back(aPath+"/"+"MC");
    v.push_back(aPath+"/"+"Rec");
  } else if(aPath=="/Event/MC") {
    v.push_back(aPath+"/"+"Particles");
    v.push_back(aPath+"/"+"Hits");
  } else if(aPath=="/Event/MC/Particles") { //Leaf
  } else if(aPath=="/Event/MC/Hits") { //Leaf
  } else if(aPath=="/Event/Rec") {
    v.push_back(aPath+"/"+"Particles");
    v.push_back(aPath+"/"+"Hits");
  } else if(aPath=="/Event/Rec/Particles") { //Leaf
  } else if(aPath=="/Event/Rec/Hits") { //Leaf

  } else { 
    std::ostream& out = aSession.out();
    out << "uiSvc_dataChildren :" 
        << " unknown path " << inlib::sout(aPath)
        << std::endl;
  }

  return v;
}

//////////////////////////////////////////////////////////////////////////////
static inlib::xml::tree* Lib_ItemML_find_itemFromPath(
 inlib::xml::tree& aItemML
,const std::vector<std::string>& aPath
,unsigned int& aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aIndex>=aPath.size()) return 0;
  std::string s;
  if(aItemML.element_value("label",s)) { //if ! it is a <tree>, continue.
    if(aPath[aIndex]!=s) return 0; //Bad path, do not jump in children.
    // Matching item.
    if(aIndex==(aPath.size()-1)) return &(aItemML); //End of aPath.
    aIndex++;
  }
  // Look children :
  inlib::xml::looper _for(aItemML);
  while(inlib::xml::tree* _tree = _for.next_tree()) {
    inlib::xml::tree* item = Lib_ItemML_find_itemFromPath(*_tree,aPath,aIndex);
    if(item) return item;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
static bool Lib_ItemML_visitToXML(
 inlib::xml::tree& aItemML
,std::string& aOut
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s;
  bool hasLabel = aItemML.element_value("label",s);
  // !hasLabel if head <tree>.
  if(hasLabel) {
    aOut +=  "<treeItem>";
    aOut +=  "<label>" + s + "</label>";
  }
  inlib::xml::looper _for(aItemML);
  while(inlib::xml::tree* _tree = _for.next_tree()) {
    if(!Lib_ItemML_visitToXML(*_tree,aOut)) return false;
  }
  if(hasLabel) aOut += "</treeItem>";
  return true;
}

extern "C" {

void OnX_TestTree_initialize(Slash::UI::IUI& aUI) {
  aUI.setCallback("mainTree","open","DLD",
		  "OnXTests OnX_TestTree_tree_open_branch");
  aUI.setCallback("mainTree","close","DLD",
		  "OnXTests OnX_TestTree_tree_close_branch");

  std::string out;
  out += "<tree>";
  out += "  <treeItem>";
  out += "    <label>dd</label>";
  out += "    <treeItem><label></label></treeItem>";
  out += "  </treeItem>";
  out += "  <treeItem>";
  out += "    <label>Event</label>";
  out += "    <treeItem><label></label></treeItem>";
  out += "  </treeItem>";
  out += "</tree>";
  aUI.setParameter("mainTree.items",out);

}

void OnX_TestTree_tree_open_branch(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();

  std::string branch = aUI.callbackValue();
  //printf("debug : OnX_TestTree_tree_open_branch : branch \"%s\"\n",
  //branch.c_str());

  std::vector<std::string> items;
  inlib::words(branch,"\n",true,items);
  if(!items.size()) return;

  inlib::replace(branch,"\n","/");
  std::string s = "/" + branch;
  //aUI.echo("OnX_TestTree_tree_open_branch : " + s);
  std::vector<std::string> text = uiSvc_dataChildren(session,s);
  if(!text.size()) return; //no children.

  std::string svalue;
  aUI.parameterValue("mainTree.items",svalue);
  //printf("debug : items : \"%s\"\n",svalue.c_str());

  inlib::xml::default_factory factory;
  exlib::xml::parser treeML(factory,session.out());
  std::vector<std::string> tags;
  tags.push_back("tree");
  tags.push_back("treeItem");
  treeML.set_tags(tags);
  if(!treeML.load_string(svalue)) return;
  inlib::xml::tree* top = treeML.top_item();
  if(!top) return;
  unsigned int pathi = 0;
  inlib::xml::tree* branchML = Lib_ItemML_find_itemFromPath(*top,items,pathi);
  if(!branchML) return;
  branchML->delete_sub_trees();
  for(unsigned int index=0;index<text.size();index++) {
    std::vector<inlib::xml::tree::atb> atbs;
    inlib::xml::tree* childML = factory.create("",atbs,branchML);
    if(!childML) return;
    std::string p,n,s;
    inlib::path_name_suffix(text[index],p,n,s);
    childML->add_element("label",atbs,n);
    branchML->add_child(childML);

    if(uiSvc_dataChildren(session,text[index]).size()) { // Is it a leaf ?
      // Add an empty item for the moment :
      std::vector<inlib::xml::tree::atb> atbs;
      inlib::xml::tree* subChildML = factory.create("",atbs,childML);
      if(!subChildML) return;
      subChildML->add_element("label",atbs,"");
      childML->add_child(subChildML);
    }
  }

  std::string out = "<tree>";
  if(!Lib_ItemML_visitToXML(*top,out)) return;
  out += "</tree>";

  aUI.setParameter("mainTree.items",out);
}

void OnX_TestTree_tree_close_branch(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();

  std::string branch = aUI.callbackValue();
  //printf("debug : OnX_TestTree_tree_close_branch : branch\"%s\"\n",
  //branch.c_str());

  std::vector<std::string> items;
  inlib::words(branch,"\n",true,items);
  if(!items.size()) return;

  std::string svalue;
  aUI.parameterValue("mainTree.items",svalue);
  //printf("debug : items : \"%s\"\n",svalue.c_str());

  inlib::xml::default_factory factory;
  exlib::xml::parser treeML(factory,session.out());
  std::vector<std::string> tags;
  tags.push_back("tree");
  tags.push_back("treeItem");
  treeML.set_tags(tags);
  if(!treeML.load_string(svalue)) return;
  inlib::xml::tree* top = treeML.top_item();
  if(!top) return;
  unsigned int pathi = 0;
  inlib::xml::tree* branchML = Lib_ItemML_find_itemFromPath(*top,items,pathi);
  if(!branchML) return;
  branchML->delete_sub_trees();

 {// Add an empty item for the moment :
  //inlib::xml::tree* subChildML = factory.create("",childML);
  std::vector<inlib::xml::tree::atb> atbs;
  inlib::xml::tree* subChildML = factory.create("",atbs,branchML);
  if(!subChildML) return;
  //subChildML->add_element("label",std::vector<std::string>(),"");
  subChildML->add_element("label",atbs,"");
  branchML->add_child(subChildML);}

  std::string out = "<tree>";
  if(!Lib_ItemML_visitToXML(*top,out)) return;
  out += "</tree>";

  aUI.setParameter("mainTree.items",out);
}

} // extern "C"

