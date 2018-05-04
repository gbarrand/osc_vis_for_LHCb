
#include <stdlib.h> //KCC : to define NULL before glib.h.

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/engines/SoTimeCounter.h>

#define ITEM_ANIMATE 0
#define ITEM_EXIT    1

static void menuitem_response(GtkWidget*,gpointer);
static SoTimeCounter* timeCounter = 0;
static SoCube* cube = 0;
static SbBool first = TRUE;
//////////////////////////////////////////////////////////////////////////////
int main(
 int
,char** aArgv
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GtkWidget* top = SoGtk::init(aArgv[0]);
  if (top==NULL) exit(1);

  GtkWidget* pdm = gtk_menu_new ();

  GtkWidget* item = gtk_menu_item_new_with_label("Animate");
  gtk_menu_append (GTK_MENU (pdm), item);
  gtk_signal_connect(GTK_OBJECT (item), 
		     "activate",
		     GTK_SIGNAL_FUNC(menuitem_response), 
		     (gpointer)ITEM_ANIMATE);
  item = gtk_menu_item_new_with_label("Exit");
  gtk_menu_append (GTK_MENU (pdm), item);
  gtk_signal_connect(GTK_OBJECT (item), 
		     "activate",
		     GTK_SIGNAL_FUNC(menuitem_response), 
		     (gpointer)ITEM_EXIT);

  GtkWidget* casc = gtk_menu_item_new_with_label ("File");

  gtk_menu_item_set_submenu (GTK_MENU_ITEM (casc), pdm);

  GtkWidget* vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (top), vbox);

  GtkWidget* menu_bar = gtk_menu_bar_new ();
  gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 2);

  gtk_menu_bar_append (GTK_MENU_BAR (menu_bar), casc);


  GtkWidget* frame = gtk_frame_new ("ExaminerViewer");
  gtk_box_pack_end (GTK_BOX (vbox), frame, TRUE, TRUE, 2);
  SoGtkExaminerViewer* viewer = new SoGtkExaminerViewer(frame);

  // Create a scene :

  SoSeparator* root=new SoSeparator;
  root->ref();

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1.,1.,0.));
  root->addChild(material);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1,0.1,0.1);
  //scale->scaleFactor.setValue(0.5,0.5,0.5);
  root->addChild(scale);

  cube = new SoCube;
  root->addChild(cube);

  // Animate things :
  /*
  */
  timeCounter = new SoTimeCounter;
  timeCounter->min = 1; 
  timeCounter->max = 5; 
  timeCounter->step = 1; 
  timeCounter->frequency=5;
  timeCounter->on = FALSE;
  timeCounter->ref();

  viewer->setBackgroundColor(SbColor(0.4,0.4,0.4));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  //viewer->setSize(SbVec2s(700,500));
  viewer->show();
  viewer->setTitle("SoFreeT");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);

  gtk_widget_set_usize(GTK_WIDGET(frame),700,500);

  gtk_widget_show_all(top);
  SoGtk::show(top);

  SoGtk::mainLoop();

  //Cleanup
  timeCounter->on = FALSE;
  cube->width.disconnect();
  timeCounter->unref();
  root->unref();

  delete viewer;

  //SoGtk::clear();  

  return 0;
}
void menuitem_response(GtkWidget*,gpointer aTag)
{
  if(((unsigned long)aTag)==ITEM_ANIMATE) {
    if(first) { 
      cube->width.connectFrom(&timeCounter->output);
      timeCounter->on = TRUE;
      first = FALSE;
    } else {
      cube->width.disconnect();
      timeCounter->on = FALSE;
      first = TRUE;
    }
  } else if(((unsigned long)aTag)==ITEM_EXIT) {
    gtk_main_quit();
  }
}
