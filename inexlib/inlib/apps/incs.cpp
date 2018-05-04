// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#ifdef INLIB_MEM
#include "../inlib/mem"
#endif

#include <string>

inline const std::string& s_cpp_include() {
  static const std::string s_v("#include");
  return s_v;
}

inline const std::string& s_cpp_import() {
  static const std::string s_v("#import");
  return s_v;
}

#include <ostream>
#include "../inlib/strip"
#include "../inlib/sout"
#include "../inlib/file"

inline bool find_inc(std::ostream& a_out,
                     const std::string& a_file,
                     std::vector<std::string>& a_locs,
                     std::vector<std::string>& a_exts) {
  a_locs.clear();
  a_exts.clear();

  FILE* file = ::fopen(a_file.c_str(),"rb");
  if(!file) {
    a_out << "find_inc : can't read " << inlib::sout(a_file)
          << ". Continue anyway." << std::endl;
    return true; //continue
  }

  unsigned int BUFSIZE = 65536;
  char* buffer = new char[BUFSIZE+1];
  if(!buffer) {::fclose(file);return false;}

  while(true) {
    size_t l;
    if(!inlib::file::read_cstring(file,buffer,BUFSIZE,l)) break; //EOF

    std::string line = buffer;
    inlib::strip(line);

    std::string inc;

    if(line.substr(0,s_cpp_include().size())==s_cpp_include()) {
      inc = line.substr
              (s_cpp_include().size(),line.size()-s_cpp_include().size());
    } else if(line.substr(0,s_cpp_import().size())==s_cpp_import()) {
      inc = line.substr
              (s_cpp_import().size(),line.size()-s_cpp_import().size());
    }

    if(inc.size()) {
      //a_out << "debug : do |" << inc << "|" << std::endl;

     {std::string::size_type pos = inc.find('<');
      if(pos!=std::string::npos) {
        inc = inc.substr((pos+1),inc.size()-(pos+1)); 
       {std::string::size_type _pos = inc.find('>');
        if(_pos!=std::string::npos) {
          inc = inc.substr(0,_pos); 
          a_exts.push_back(inc);
          continue;
        }}
      }}

     {std::string::size_type pos = inc.find('"');
      if(pos!=std::string::npos) {
        inc = inc.substr((pos+1),inc.size()-(pos+1)); 
       {std::string::size_type _pos = inc.find('"');
        if(_pos!=std::string::npos) {
          inc = inc.substr(0,_pos); 
          a_locs.push_back(inc);
          continue;
        }}
      }}

      a_out << "find_inc :"
            << " unknown syntax for " << inlib::sout(line) << ". Skip it."
            << std::endl;
    }
  }

  delete [] buffer;
  ::fclose(file);

  return true;
}

#include "../inlib/path"
#include "../inlib/sep"

#include <inlib/file>
#include <inlib/vmanip>
#include <inlib/sys/dir>

inline bool find_incs(std::ostream& a_out,
                      const std::vector<std::string>& a_dirs,
                      const std::string& a_path,
                      std::vector<std::string>& a_locs,
                      std::vector<std::string>& a_exts,
                      bool a_recursive,
                      bool a_verbose) {
  if(a_verbose) {
    a_out << "do " << inlib::sout(a_path) << " ..." << std::endl;
  }

  std::vector<std::string> locs;
  std::vector<std::string> exts;
  if(!find_inc(a_out,a_path,locs,exts)) {
    a_out << "find_incs : failed." << std::endl;
    a_locs.clear();
    a_exts.clear();
    return false;
  }

  if(a_verbose) {
    a_out << "locs " << locs.size() << " :" << std::endl;
    inlib::dump(locs,a_out);
    a_out << "exts " << exts.size() << " :" << std::endl;
    inlib::dump(exts,a_out);
  }

 {
  std::vector<std::string>::iterator it;
  for(it=locs.begin();it!=locs.end();++it){
   {std::string path = inlib::dir_name(a_path);
    if(path.size()) {
      std::string fname = path + inlib::sep() + *it;        
      if(inlib::file::exists(fname)) {
        *it = fname;
        continue;
      }
    }}

    //path empty or fname does not exist. Try with a_dirs.

   {bool found = false;
    std::vector<std::string>::const_iterator it2;
    for(it2=a_dirs.begin();it2!=a_dirs.end();++it2){
      std::string fname = (*it2).size()?(*it2)+inlib::sep()+*it:*it;
      if(inlib::file::exists(fname)) {
        *it = fname;
        found = true;
        break;
      }
    }
    if(found) continue;}

    a_out << "find_incs : loc " << inlib::sout(*it)
          << " not found with dirs :" << std::endl;
   {std::vector<std::string>::const_iterator it2;
    for(it2=a_dirs.begin();it2!=a_dirs.end();++it2){
      a_out << inlib::sout(*it2) << std::endl;
    }}
   {std::string pwd;
    inlib::dir::pwd(pwd);
    a_out << "and current directory " << inlib::sout(pwd) << std::endl;}

  }}

 {std::vector<std::string> new_exts;
  std::vector<std::string>::const_iterator it;
  for(it=exts.begin();it!=exts.end();++it){

    bool found = false;
    std::vector<std::string>::const_iterator it2;
    for(it2=a_dirs.begin();it2!=a_dirs.end();++it2){
      std::string path = (*it2).size()?(*it2)+inlib::sep()+*it:*it;
      if(inlib::file::exists(path)) {
        locs.push_back(path);        
        found = true;
        break;
      }
    }
    if(!found) {
      //a_out << "find_incs : " << inlib::sout(*it)
      //      << " not found." << std::endl;
      new_exts.push_back(*it);
    }

  }
  exts = new_exts;}

  std::vector<std::string> old_locs = a_locs;

  inlib::append(a_locs,locs);
  inlib::append(a_exts,exts);

  inlib::unique(locs);
  inlib::unique(exts);

  if(a_recursive) {
    std::vector<std::string>::const_iterator it;
    for(it=locs.begin();it!=locs.end();++it){      
      unsigned int index;
      if(inlib::item_index(old_locs,*it,index)) continue; 
      if(!find_incs(a_out,a_dirs,*it,a_locs,a_exts,true,a_verbose)) {
        a_locs.clear();
        a_exts.clear();
        return false;
      }
    }
  }

  return true;
}

#include <inlib/args>
#include <inlib/words>
#include <inlib/dirmanip>

#include <cstdlib>
#include <iostream>

int main (int a_argc,char** a_argv){

#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif

  inlib::args args(a_argc,a_argv);

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::vector<std::string> dirs;
 {std::string sdirs;
  args.find("-dirs",sdirs);
  inlib::words(sdirs,":",false,dirs);}

  std::string file;
  if(!args.file(file)) {
    std::cout << "no file given." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> locs;
  std::vector<std::string> exts;
  if(!find_incs(std::cout,dirs,file,locs,exts,true,verbose)) {
    return EXIT_FAILURE;
  }

  inlib::unique(locs);
  inlib::unique(exts);

  if(args.is_arg("-pack")) {
    std::string target;
    if(!args.find("-to",target)) {
      std::cout << "-to needed." << std::endl;
      return EXIT_FAILURE;
    }

    locs.push_back(file);

    std::vector<std::string>::const_iterator it;
    for(it=locs.begin();it!=locs.end();++it){
      std::string path = target.size()?target+inlib::sep()+*it:*it;
      std::string dir = inlib::dir_name(path);
      //std::cout << "debug : " << inlib::sout(dir) << std::endl; 
      if(!inlib::mkdirs(std::cout,dir)) {
	std::cout << "pack failed for " << inlib::sout(*it) << std::endl;
        continue;
      }
      char* buffer;
      long l;
      if(inlib::file::read_bytes(*it,buffer,l)){
        if(!inlib::file::write_bytes(path,buffer,l)){}
      }
      delete [] buffer;
    }
  } else {
    std::cout << "locs :" << std::endl;
    inlib::dump(locs,std::cout);
    std::cout << "exts :" << std::endl;
    inlib::dump(exts,std::cout);
  }

#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif

  return EXIT_SUCCESS;
}
