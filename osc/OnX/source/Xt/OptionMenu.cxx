
// this :
#include "OptionMenuP.h"

// Private :
#include <X11/StringDefs.h>

#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

#include <inlib/sprintf>

#include <OnX/Xt/XtTools.h>

static void InitializeClass(void);
static void InitializeWidget(Widget, Widget,ArgList,Cardinal*);
static void ValueChangedCallback(Widget,XtPointer,XtPointer);

static XtResource resources [] ={
 {(char*)XmNvalueChangedCallback,(char*)XtCCallback,(char*)XtRCallback,
  sizeof(XtCallbackList),
  XtOffset(OptionMenuWidget,optionMenu.valueChangedCallback),
  XtRImmediate,(XtPointer)NULL},
};

#define athis ((OptionMenuWidget)This)->optionMenu

OptionMenuClassRec optionMenuClassRec = {
/* Core Class Part */
{
    (WidgetClass)&xmFormClassRec, /* pointer to superclass ClassRec   */
    (char*)"OptionMenu",             /* widget resource class name       */
    sizeof(OptionMenuRec),             /* size in bytes of widget record   */
    InitializeClass,            /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                       /* has class been initialized?      */
    InitializeWidget,           /* initialize                       */
    NULL,                        /* notify that initialize called    */
    XtInheritRealize,           /* XCreateWindow for widget         */
    NULL,                             /* widget semantics name to proc map*/
    0,                                 /* number of entries in actions     */
    resources,                  /* resources for subclass fields    */
    XtNumber(resources),        /* number of entries in resources   */
    NULLQUARK,                        /* resource class quarkified        */
    TRUE,                        /* compress MotionNotify for widget */
    TRUE,                        /* compress Expose events for widget*/
    TRUE,                        /* compress enter and leave events  */
    TRUE,                        /* select for VisibilityNotify      */
    NULL,                        /* free data for subclass pointers  */
    XtInheritResize,            /* geom manager changed widget size */
    XtInheritExpose,            /* expose                           */
    NULL,                        /* set subclass resource values     */
    NULL,                        /* notify that SetValues called     */
    XtInheritSetValuesAlmost,   /* SetValues got "Almost" geo reply */
    NULL,                        /* notify that get_values called    */
    XtInheritAcceptFocus,       /* assign input focus to widget     */
    XtVersion,                        /* version of intrinsics used       */
    NULL,                        /* list of callback offsets         */
    XtInheritTranslations,      /* translations                     */        
    XtInheritQueryGeometry,     /* return preferred geometry        */
    XtInheritDisplayAccelerator,/* display your accelerator         */
    NULL                         /* pointer to extension record      */
},
/* Composite Class Part */
{
    XtInheritGeometryManager,   /* geometry manager for children   */
    XtInheritChangeManaged,     /* change managed state of child   */
    XtInheritInsertChild,       /* physically add child to parent  */
    XtInheritDeleteChild,       /* physically remove child         */
    NULL                         /* pointer to extension record     */
},
/* constraint_class fields */
{
    NULL,                         /* constraint resource     */
    0,                            /* number of constraints   */
    sizeof(XmFormConstraintRec),  /* constraint size         */
    NULL,                         /* initialization          */
    NULL,                         /* destroy proc            */
    NULL,                         /* set_values proc         */
    NULL,                         /* extension               */
},
/* manager_class fields   */
{                        
    XtInheritTranslations,                /* translations           */
    NULL,                                 /* syn_resources          */
    0,                                    /* num_syn_resources      */
    NULL,                                 /* syn_cont_resources     */
    0,                                    /* num_syn_cont_resources */
    XmInheritParentProcess,               /* parent_process         */
    NULL,                                 /* extension              */
},
/* bulletin_board_class fields */
{                        
    FALSE,                                /* always_install_accelerators */
    NULL,                                 /* geo_matrix_create  */
    XmInheritFocusMovedProc,              /* focus_moved_proc   */
    NULL,                                 /* extension          */
},

/* form_class fields  */
{   
    NULL,                     /* extension          */
},


/* OptionMenu */
{
    NULL
}
   
};

WidgetClass optionMenuWidgetClass = (WidgetClass) &optionMenuClassRec;

//////////////////////////////////////////////////////////////////////////////
static void InitializeClass(
 void
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
static void InitializeWidget(
 Widget
,Widget This
,ArgList
,Cardinal*
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  This->core.width = 0;
  This->core.height = 0;

  athis.pdm = XmCreatePulldownMenu(This,
                                   (char*)"OPTIONMENU_PULLDOWNMENU",0,0);
  Arg args[5];
  XtSetArg(args[0],XmNsubMenuId,athis.pdm);
  XtSetArg(args[1],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[3],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[4],XmNrightAttachment,XmATTACH_FORM);
  athis.rowColumn = XmCreateOptionMenu(This,
                                       (char*)"OPTIONMENU_ROWCOLUMN",args,5);
  XtManageChild(athis.rowColumn);

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OptionMenuInitialize(
 Widget This
,const std::vector<std::string>& aItems
,const std::string& aDefault 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Unmanage/manage the rowColumn to compell a recomputation of geometry.
  XtUnmanageChild(athis.rowColumn);

  Arg args[1];
  // Set history to null :
  XtSetArg(args[0],XmNmenuHistory,0);
  XtSetValues(athis.rowColumn,args,1);
  // kill existing pull children :
  std::vector<Widget> items;
  XWidgetGetChildren(athis.pdm,items);
  for(unsigned int index=0;index<items.size();index++) {
    XtDestroyWidget(items[index]);
  }
  // If no option given, create a "blank" push button :
  if(aItems.size()==0) { 
    XmString cps = XmStringLtoRCreate((char*)"",
                                      XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[0],XmNlabelString,cps);
    std::string name(XtName(This));
    name += "_1";
    Widget button = 
      XtCreateManagedWidget(name.c_str(),
                            xmPushButtonWidgetClass,athis.pdm,args,1);
    XmStringFree(cps);
    XtSetArg(args[0],XmNmenuHistory,button);
    XtSetValues(athis.rowColumn,args,1);
    XtAddCallback(button,XmNactivateCallback,
                  ValueChangedCallback,(XtPointer)This);
  } else {
    // Creaet new pull children :
    Widget whistory  = NULL;
    std::vector<std::string>::const_iterator it;
    unsigned int index = 0;
    for(it=aItems.begin();it!=aItems.end();++it,index++) {
      XmString cps = XmStringLtoRCreate((char*)(*it).c_str(),
                                        XmSTRING_DEFAULT_CHARSET);
      XtSetArg(args[0],XmNlabelString,cps);
      std::string name;
      inlib::sprintf(name,::strlen(XtName(This))+32,"%s_%d",
                    XtName(This),index+1);
      Widget button = 
        XtCreateManagedWidget(name.c_str(),
                              xmPushButtonWidgetClass,athis.pdm,args,1);
      XmStringFree(cps);
      XtAddCallback(button,XmNactivateCallback,
                    ValueChangedCallback,(XtPointer)This);
      if(whistory==NULL) whistory = button;  // history = first option.
      if(aDefault.size()&&((*it)==aDefault)) whistory = button;
    }
    if(whistory) {
      XtSetArg(args[0],XmNmenuHistory,whistory);
      XtSetValues(athis.rowColumn,args,1);
    }
  }

  XtManageChild(athis.rowColumn);
}
//////////////////////////////////////////////////////////////////////////////
void OptionMenuSetDefault(
 Widget This
,const std::string& aDefault 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Unmanage/manage the rowColumn to compell a recomputation of geometry.
  XtUnmanageChild(athis.rowColumn);

  Widget whistory  = NULL;
  std::vector<Widget> items;
  XWidgetGetChildren(athis.pdm,items);
  for(unsigned int index=0;index<items.size();index++) {
    std::string label;
    XmGetString(items[index],XmNlabelString,label);
    if( (aDefault.size()) && (aDefault==label)) whistory = items[index];
  }
  if(whistory) {
    Arg args[1];
    XtSetArg(args[0],XmNmenuHistory,whistory);
    XtSetValues(athis.rowColumn,args,1);
  }

  XtManageChild(athis.rowColumn);
}
//////////////////////////////////////////////////////////////////////////////
bool OptionMenuGetOption(
 Widget This
,std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  Arg args[1];
  Widget whistory;
  XtSetArg(args[0],XmNmenuHistory,&whistory);
  XtGetValues(athis.rowColumn,args,1);
  if(!whistory) return false;
  return XmGetString(whistory,XmNlabelString,aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool OptionMenuSetOption(
 Widget This
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Widget> items;
  XWidgetGetChildren(athis.pdm,items);
  for(unsigned int index=0;index<items.size();index++) {
    std::string label;
    if(!XmGetString(items[index],XmNlabelString,label)) return false;
    if(label==aValue) {
      Arg args[1];
      XtSetArg(args[0],XmNmenuHistory,items[index]);
      XtSetValues(athis.rowColumn,args,1);
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool OptionMenuGetItems(
 Widget This
,std::vector<std::string>& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue.clear();
  std::vector<Widget> items;
  XWidgetGetChildren(athis.pdm,items);
  for(unsigned int index=0;index<items.size();index++) {
    std::string label;
    if(!XmGetString(items[index],XmNlabelString,label)) {
      aValue.clear();
      return false;
    }
    aValue.push_back(label);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Widget OptionMenuGetLabel(
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return athis.rowColumn;
}
//////////////////////////////////////////////////////////////////////////////
void ValueChangedCallback(
 Widget
,XtPointer aTag
,XtPointer aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget This = (Widget)aTag;

  std::string s;
  OptionMenuGetOption(This,s);

  OptionMenuCallbackStruct data;
  data.reason = XmCR_VALUE_CHANGED;
  data.event = ((XmAnyCallbackStruct*)aData)->event;
  data.value = XtNewString(s.c_str());
  XtCallCallbacks(This,XmNvalueChangedCallback,(XtPointer)&data);
  XtFree(data.value);
}
