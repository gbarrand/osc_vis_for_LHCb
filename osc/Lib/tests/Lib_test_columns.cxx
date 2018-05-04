
//#include <Lib/Columns.h>
#include <Lib/Columns_no_yacc.h>

#include <Lib/Debug.h>

#include <inlib/args>

int main(int a_argc,char** a_argv){

  Lib::Debug::checkByClass(true);

 {inlib::args args(a_argc,a_argv);

  int verbose = 0;
  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verbose = 1;
  }


 {Lib::Columns proc
    ("int i,int j = 2, float f = -3.2,double d,string s = \"xxx\"",
     std::cout,verbose);
   std::cout << "Lib_columns : " 
      << " parse : " << proc.script() << std::endl;
  if(!proc.findVariables()) {
    std::cout << "Lib_columns : " 
        << " parse failed."<< std::endl;
  }
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);
  Lib::Columns::deleteColumns(vars);}

 {Lib::Columns proc
    ("int[] i, float[] f,double[] d,string[] s",
     std::cout,verbose);
   std::cout << "Lib_columns : " 
      << " parse : " << proc.script() << std::endl;
  if(!proc.findVariables()) {
    std::cout << "Lib_columns : " 
        << " parse failed."<< std::endl;
  }
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);
  Lib::Columns::deleteColumns(vars);}

  // AIDA/TestTuple.cpp :
 {Lib::Columns proc(
    //    "ITuple col11_ITuple={ double px = 2,double py = 3., double pz = 0., ITuple xyz = {int x,int y,int z}, float ipx, float ipy, float ipz, int track=-1}",

    "\
int col1_Int= 1\
,short col2_Short=2\
,float col4_Float=0.3\
,double col5_Double=-0.5\
,boolean col6_Boolean=true\
,byte col7_Byte=23\
,char col8_Character=f\
,java.lang.String col9_string=de\
,java.lang.String col10_Object\
,ITuple col11_ITuple={ \
  double px = .2, py = 3., pz = 0., \
  ITuple xyz = {int x,y,z}, \
  float ipx, ipy,ipz, int track=-1\
}\
",
    std::cout,verbose);
   std::cout << "Lib_columns : " 
      << " parse : " << proc.script() << std::endl;
  if(!proc.findVariables()) {
    std::cout << "Lib_columns : " 
        << " parse failed."<< std::endl;
  }
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);
  Lib::Columns::deleteColumns(vars);}

 {Lib::Columns proc
    ("boolean bool_col=false,byte byte_col=0,short short_col=0,int int_col=0,long long_col=0,float float_col=0,double double_col=0",
     std::cout,verbose);
  std::cout << "Lib_columns : " << " parse : " << proc.script() << std::endl;
  if(!proc.findVariables()) {
    std::cout << "Lib_columns : " 
        << " parse failed."<< std::endl;
  }
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);
  Lib::Columns::deleteColumns(vars);}

 {std::string booking;
  booking += "int eventId, inputEvtId, interMode, vtxVol, "; 
  booking += "ITuple vtxPos = { double x, y, z }, ";
  booking += "int nPart, leptonIndex, protonIndex, ";                
  booking += "ITuple track = { int pId, parent, float timeStart, ";        
  booking +=                  "ITuple direction = { double dx, dy, dz }, ";
  booking +=                  "double mass, pTot, ETot, ";
  booking +=                  "ITuple momentum  = { double px, py, pz }, ";
  booking +=                  "ITuple startPos  = { double  x,  y,  z }, ";
  booking +=                  "ITuple stopPos   = { double  x,  y,  z }, ";
  booking +=                  "int startVol, stopVol ";
  booking +=                "}, ";
  booking += "int nHits,";
  booking += "ITuple hit = { int tubeId, float x, y, z, int totalPE, ITuple pe = { float time } }, ";      
  booking += "int nDigits, double sumPE, ";
  booking += "ITuple digit = { int tubeId, double pe, time }";
  Lib::Columns proc(booking,std::cout,verbose);
  std::cout << "Lib_columns : " << " parse : " << proc.script() << std::endl;
  if(!proc.findVariables()) {
    std::cout << "Lib_columns : " 
        << " parse failed."<< std::endl;
  }
  std::list<Lib::Value> vars = proc.result();
  Lib::Columns::dumpColumns(vars,std::cout);
  Lib::Columns::deleteColumns(vars);}

}

  Lib::Debug::balance(std::cout);
 
  return 0;
}
