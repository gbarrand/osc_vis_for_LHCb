/* mycint.c,  must be compiled with -I$CINTSYSDIR */

#include "G__ci.h"

extern "C" {
  //extern void G__setothermain(int othermain);
  //extern int G__main(int argc,char **argv);
  //extern void G__set_p2fsetup(void (*p2f)());
  //extern void G__free_p2fsetup();
  //extern void G__cpp_setupTestCINT();
}

int main() {
   //G__set_p2fsetup(G__cpp_setupTestCINT);
   G__init_cint("cint");

   // test.C is in examples/CINT and contains the definition of class A.
   G__loadfile("../examples/CINT/test.C");
   //G__free_p2fsetup();

   //G__calc("t->dump()");

   G__exec_text("\
printf(\"debug : hello\\n\");\n\
A a;\n\
a.dump();\n\
");

   //bad G__calc("A* b = new A");

   G__scratch_all();
   return 0;
}

