// not finished.

#include <inlib/args>
#include <inlib/system>
#include <exlib/gzip>

#include <Lib/Debug.h>
#include <Lib/ItemML.h>
#include <Lib/BaseML.h>

#include <fstream>
#include <cstdio> //printf

class AIDA_ML : public Lib::BaseML {
public:
  AIDA_ML(Lib::ItemMLFactory& aFactory,
          std::ostream& a_out,
          int aVerbose)
  :Lib::BaseML(aFactory,a_out,aVerbose)
  ,fCompress(false)
  {}
public:
  virtual bool visitEndElement(Lib::ItemML& aItemML,bool& aKeep) {
    const std::string& tagName = aItemML.tagName();
    if(tagName=="aida") {
      aKeep = false;
    } else if(isAIDA_Object(tagName)) {
      std::string path;
      if(aItemML.attributeValue("","path",path)) {
        std::string name;
        if(aItemML.attributeValue("","name",name)) write(path,name);
      }  
      aKeep = false;
    } else {
      aKeep = true;
    }
    return true;
  }
  bool write(const std::string& aPath,const std::string& aName) const { 
    std::string fileName = "./"+aPath+"/"+aName+".aida";
    ::printf("debug : write %s\n",fileName.c_str());
    std::string fullName;
    inlib::file_name(fileName,fullName);
    if(fCompress) {
      exlib::ogzip fout(fullName,f_out);
      if(!fout.opened()) return false;
      file_write(fout);
      fout.close();
    } else {
      std::ofstream fout(fullName.c_str());
      if(fout.fail()) return false;
      file_write(fout);
      fout.close();
    }

    return true;
  }  
  bool isAIDA_Object(const std::string& aTagName) const {
    if(aTagName=="histogram1d") return true;
    if(aTagName=="histogram2d") return true;
    if(aTagName=="histogram3d") return true;
    if(aTagName=="profile1d") return true;
    if(aTagName=="profile2d") return true;
    if(aTagName=="cloud1d") return true;
    if(aTagName=="cloud2d") return true;
    if(aTagName=="cloud3d") return true;
    if(aTagName=="tuple") return true;
    if(aTagName=="dataPointSet") return true;
    if(aTagName=="function") return true;
    return false;
  }
private:
  void file_write(std::ostream& a_fout) const {
    // Header :
    a_fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    a_fout << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">" << std::endl;
    //a_fout << "<aida version=" << Lib::sout(AIDA_VERSION) << ">" << std::endl;
    //a_fout << "  <implementation package=" << Lib::sout("BatchLab") 
    //    << " version=" << Lib::sout(BATCHLAB_VERSION) << "/>" << std::endl;

    //XML_StoreVisitor visitor(fManager,*this,a_fout,printer());
    //rootFolder().visit(visitor);

    // Footer :
    a_fout << "</aida>" << std::endl;
  }
private:
  bool fCompress;
};

#include <iostream>

int main(int a_argc,char** a_argv) {
 {inlib::args args(a_argc,a_argv);

  if(args.is_arg("-h")) {
    std::cout << "OS> Lib_xml.exe [options] <file>" << std::endl;
    std::cout << " -h : help" << std::endl;
    return 0;
  }

  int verboseLevel = 0;
  std::string sverbose;
  args.find("-verbose",verboseLevel);

  std::string file;
  if(!args.file(file)) {
    std::cout << "no XML aida file given." << std::endl;
    return 0;
  }

  Lib::ItemMLFactory factory;
  AIDA_ML aidaML(factory,std::cout,verboseLevel);
  std::vector<std::string> tags;
  tags.push_back("aida");
  tags.push_back("histogram1d");
  tags.push_back("histogram2d");
  tags.push_back("histogram3d");
  tags.push_back("profile1d");
  tags.push_back("profile2d");
  tags.push_back("axis");
  tags.push_back("statistics");
  tags.push_back("data1d");
  tags.push_back("data2d");
  tags.push_back("data3d");
  tags.push_back("tuple");
  tags.push_back("columns");
  tags.push_back("rows");
  tags.push_back("row");
  tags.push_back("entryITuple"); //aida.dtd spec.
  tags.push_back("entryTuple");  //not in aida.dtd ! Back comp.
  tags.push_back("cloud1d");
  tags.push_back("cloud2d");
  tags.push_back("cloud3d");
  tags.push_back("entries1d");
  tags.push_back("entries2d");
  tags.push_back("entries3d");
  tags.push_back("dataPointSet");
  tags.push_back("dataPoint");
  tags.push_back("function");
  tags.push_back("arguments");
  tags.push_back("argument");
  tags.push_back("parameters");
  tags.push_back("annotation");
  aidaML.setTags(tags);
  if(!aidaML.loadFile(file,false)) {
    std::cout << "Lib_xml : " 
        << " problem when loading file " << inlib::sout(file) << std::endl;
  }


  }Lib::Debug::balance(std::cout);

  return 0;
}
