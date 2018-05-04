// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/sys/dir>
#include <inlib/args>
#include <inlib/file>
#include <inlib/mnmx>
#include <inlib/smatch>

#include <cstdlib>

class finder_visitor : public virtual inlib::dir::visitor {
public:
  virtual bool directory(const std::string& /*aDirectory*/,bool& a_process) {
    a_process = true;
    return true;
  }
  virtual bool file(const std::string& a_file) {
    if(m_print) m_out << a_file  << std::endl;
    if(m_search.size()) {
      std::vector<std::string> text;
      if(!inlib::file::read(a_file,text)) {
        m_out << "Can't read " << inlib::sout(a_file) << std::endl;
        return false;
      }
      for(unsigned int index=0;index<text.size();index++) {
        std::string s = text[index];
        if(s.find(m_search)!=std::string::npos) {
          m_out << "Found at line " << (index+1) 
                << " in " << inlib::sout(a_file) << " :" << std::endl;
          for(unsigned int ivis=0;ivis<m_nvis;ivis++) {
            int i = index+m_offset+ivis;
            if((i>=0)&&(i<int(text.size()))) {
              m_out << text[i] << std::endl;
            }
          }
        }
      }
    }
    return true;
  }
public:
  finder_visitor(std::ostream& a_out,
                 const std::string& a_search,
                 bool a_print,
                 int a_offset,
                 unsigned int a_nvis)
  :m_out(a_out)
  ,m_search(a_search)
  ,m_print(a_print)
  ,m_offset(a_offset)
  ,m_nvis(a_nvis)
  {}
  virtual ~finder_visitor() {}
private:
  std::ostream& m_out;
  std::string m_search;
  bool m_print;
  int m_offset;
  unsigned int m_nvis;
};

#include <iostream>

int main(int a_argc,char** a_argv){
  inlib::args args(a_argc,a_argv);

  //args.print(std::cout);

  std::string path;
  if(!args.find("-path",path)) path = ".";

  std::string filter;
  if(!args.find("-file",filter)) filter = "*";

  int offset = 0;
  if(!args.find("-offset",offset)) offset = 0;
  unsigned int nvis = 1;
  if(!args.find("-show",nvis)) nvis = 1;
  
  bool print = args.is_arg("-print");

  args.remove_first();
  args.remove("-path");
  args.remove("-file");
  args.remove("-print");

  std::string k,v;
  if(!args.last(k,v) && !print) {
    std::string help = 
      "  OS> inlib_find -path=<starting path> -file=<file pattern> -print=<string to search in files>";
    std::cout << help << std::endl;
    return EXIT_FAILURE;
  }
  std::string what = v.size()?(k+"="+v):k;

  inlib::dir::tree tree(std::cout);
  tree.set_path(path);
  tree.set_match_func(inlib::match2);
  if(!tree.build(filter)) {
    std::cout << "Can't get files of " << inlib::sout(path) << std::endl;
    return EXIT_FAILURE;
  }

  finder_visitor visitor(std::cout,what,print,offset,nvis);
  tree.visit(visitor);

  return EXIT_SUCCESS;
}
