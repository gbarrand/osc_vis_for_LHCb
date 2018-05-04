#ifndef OnX_PageViewer_h
#define OnX_PageViewer_h

#include <X11/Intrinsic.h>

#include <string>

#ifndef XtNrectangularCollectCallback
#define XtNrectangularCollectCallback "rectangularCollectCallback"
#endif

#ifndef XtNcollectCallback
#define XtNcollectCallback "collectCallback"
#endif

#ifndef XtNpopupCallback
#define XtNpopupCallback "popupCallback"
#endif

#ifndef XtNviewerClass
#define XtNviewerClass "viewerClass"
#endif

#ifndef XtCR_COLLECT
#define XtCR_COLLECT 1
#endif

#ifndef XtCR_RECTANGULAR_COLLECT
#define XtCR_RECTANGULAR_COLLECT 1
#endif

#ifndef XtCR_POPUP
#define XtCR_POPUP 2
#endif

extern "C"{

typedef struct {
  int reason;
  XEvent* event;
  char* value;
} PageViewerCallbackStruct;

typedef struct _PageViewerClassRec* PageViewerWidgetClass;
typedef struct _PageViewerRec* PageViewerWidget;

extern WidgetClass pageViewerWidgetClass;
}

class SoNode;
class SoPage;
class SoXtFullViewer;

SoPage* PageViewerGetSoPage(Widget);
SoNode* PageViewerGetSceneGraph(Widget);
SoXtFullViewer* PageViewerGetSoViewer(Widget);

void PageViewerDisable(Widget);
void PageViewerSetVerboseLevel(Widget,int);
void PageViewerAddPopupEntry(Widget,const std::string&);

#endif
