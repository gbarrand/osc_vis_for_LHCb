
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>

//////////////////////////////////////////////////////////////////////////////
int main (
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Fl_Window window(200, 55);
  Fl_Return_Button b(20, 10, 160, 35, "Hello"); 
  //b.callback(rename_me);
  window.add(b);
  window.resizable(&b);
  window.show(aArgc, aArgv);

  // Also we test to see if the exit callback works:
  //window.callback(window_callback);

  return Fl::run();

}
