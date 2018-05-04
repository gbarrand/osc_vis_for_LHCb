#ifndef OnX_PageViewerP_h
#define OnX_PageViewerP_h 

// this :
#include <OnX/Xt/PageViewer.h>

#include <X11/IntrinsicP.h>
#include <Xm/FormP.h>

class SoXtFullViewer;

extern "C"{

#ifndef XtInheritGeoMatrixCreate  // For HP-UX MOTIF.
#define XtInheritGeoMatrixCreate ((XmGeoCreateProc)_XtInherit)
#endif

typedef struct {
  void* extension;
} PageViewerClassPart;

typedef struct _PageViewerClassRec {
  CoreClassPart core_class;
  CompositeClassPart composite_class;
  ConstraintClassPart constraint_class;
  XmManagerClassPart manager_class;
  XmBulletinBoardClassPart bulletin_board_class;
  XmFormClassPart form_class;
  PageViewerClassPart page_viewer_class;
} PageViewerClassRec;

extern PageViewerClassRec pageViewerClassRec;

typedef struct {
  XtCallbackList rectangularCollectCallback;
  XtCallbackList collectCallback;
  XtCallbackList popupCallback;
  char* viewerClass;
  SoXtFullViewer* viewer;
  Widget popup;
  GC rubberGC;
  bool rubberMove;
  XPoint beginMove,endMove;
  unsigned int wMove,hMove;
  int popx,popy;
} PageViewerPart;

typedef struct _PageViewerRec {
  CorePart core;
  CompositePart        composite;
  ConstraintPart constraint;
  XmManagerPart manager;
  XmBulletinBoardPart bulletin_board;
  XmFormPart form;
  PageViewerPart pageViewer;
} PageViewerRec;

}

#endif

