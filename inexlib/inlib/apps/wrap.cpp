// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/srep>
#include <inlib/sto>
#include <inlib/file>

#include <cstdlib> //EXIT_SUCCESS

#include <iostream>

int main(int a_argc,char** a_argv) {
  std::vector<std::string> args;
 {for(int index=0;index<a_argc;index++) args.push_back(a_argv[index]);}
  
  if(args.size()!=3) {
    std::cout << "main :" 
              << " two arguments expected."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string in_file = args[1];
  std::string out_file = args[2];

  if(in_file==out_file) {
    std::cout << "main :" 
              << " out file is the same than in file."
              << std::endl;
    return EXIT_FAILURE;
  }
  
  std::vector<std::string> in_txt;
  if(!inlib::file::read(in_file,in_txt)) {
    std::cout << "main :" 
              << " can't read input file \"" << in_file << "\"."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string s_in_file = inlib::base_name(in_file);

  std::vector<std::string> out_txt;
  out_txt.push_back("//");
  out_txt.push_back("//  This file had been produced automatically");
  out_txt.push_back("// by the inlib/wrap program from the file :");
  out_txt.push_back("//    "+s_in_file);
  out_txt.push_back("//");

  inlib::replace(s_in_file,".","_");

  out_txt.push_back("");
  out_txt.push_back("inline const char** "+s_in_file+"(unsigned int& a_linen){");

 {unsigned int number = in_txt.size();
  for(unsigned int index=0;index<number;index++) {
    std::string s = "  static const char ";
    s += "_"+inlib::to(index)+"[] = \"";
    std::string ss = in_txt[index];
    inlib::replace(ss,"\"","@@double_quote@@");
    inlib::replace(ss,"\\","@@back_slash@@");
    s += ss;
    s += "\";";
    out_txt.push_back(s);
  }}

 {out_txt.push_back("  static const char* _lines[] = {");
  unsigned int number = in_txt.size();
  for(unsigned int index=0;index<number;index++) {
    std::string s;
    if(index) s += ",";
    s += "_"+inlib::to(index);
    out_txt.push_back(s);
  }
  out_txt.push_back("};");}

  out_txt.push_back(" a_linen = "+inlib::to(in_txt.size())+";");
  out_txt.push_back(" return _lines;");
  out_txt.push_back("}");

  if(!inlib::file::write(out_file,out_txt)) {
    std::cout << "main :" 
              << " can't write output file \"" << out_file << "\"."
              << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
