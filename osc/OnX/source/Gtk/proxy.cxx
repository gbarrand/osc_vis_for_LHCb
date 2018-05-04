
// proxy should be on pure gtk (and not on OnX itself).

#include <inlib/args>

#include <OnX/Gtk/GtkTools.h>

#include "../source/Core/Proxy.h"

bool gtk_proxy(
 void* aTag
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;
  GtkWidget* widget = (GtkWidget*)aTag;
  if(aWhat==PX_WRITE) {
    if(aArgs.size()!=3) return false;
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];
#ifdef ONX_HAS_GTK_GL_AREA
    if(GTK_IS_GL_AREA(widget)) {
      if(format=="gl2ps") {
        return OnX::gtk_gl_area_write_gl2ps(GTK_GL_AREA(widget),file,opts);
      } else {
        return false;
      }
    } else 
#endif
    {
      return false;
    }
  } else if(aWhat==PX_SIZE) {
    typedef unsigned int uint_t;
    uint_t* sz = new uint_t[2];
    sz[0] = widget->allocation.width;
    sz[1] = widget->allocation.height;
    aReturn = sz;
    return true;
  } else if(aWhat==PX_POSITION) {
    int* pos = new int[2];
    pos[0] = widget->allocation.x;
    pos[1] = widget->allocation.y;
    aReturn = pos;
    return true;
  } else if(aWhat==PX_MANAGE) {
    if(aArgs.size()!=1) return false;
    bool value;
    if(!inlib::to(aArgs[0],value)) return false;
    //FIXME
    //if(value) 
    //  XtManageChild(widget);
    //else 
    //  XtUnmanageChild(widget);
    return false;
  } else if(aWhat==PX_REFRESH) {
#ifdef ONX_HAS_GTK_GL_AREA
    if(GTK_IS_GL_AREA(widget)) {
      // Send an expose event :
      GdkEventExpose event;
      event.type = GDK_EXPOSE;
      event.window = widget->window;
      event.count = 0;
      event.area.x = 0;
      event.area.y = 0;
      event.area.width = widget->allocation.width;
      event.area.height = widget->allocation.height;
      gtk_widget_event(widget,(GdkEvent*)&event);
      return true;
    } else 
#endif
    {
      return false;
    }
  } else if(aWhat==PX_SHOW) {
    GtkWidget* shell = gtk_widget_get_toplevel(widget);
    if(shell) gtk_widget_show(shell);
  } else if(aWhat==PX_HIDE) {
    GtkWidget* shell = gtk_widget_get_toplevel(widget);
    if(shell) gtk_widget_hide(shell);
  }
  return false; //bad protocol.
}
