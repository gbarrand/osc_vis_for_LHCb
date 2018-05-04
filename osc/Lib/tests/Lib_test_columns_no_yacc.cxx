// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <Lib/Columns_no_yacc.h>

void Lib::Debug::increment(const char*){}
void Lib::Debug::decrement(const char*){}

int main() {

 {std::string in = "double x = 1,ITuple y={bool z,ITuple z = {string u = \"aaa\"},float uuu},int z";

  //parser pr;
  //pr.parse(in);
  //pr.dump();

  Lib::Columns proc(in,std::cout);
  proc.findVariables();
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);}

  return 0;
}
