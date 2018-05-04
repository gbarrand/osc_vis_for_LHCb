#include <OnX/Qt/QPageViewer.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qmenu.h>
#endif

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <OnX/Inventor/GuiViewer.h>

#include <OnX/Qt/QCallbackData.h>

#include <inlib/sprintf>

#define Qt_gray Qt::gray

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

namespace OnX {
class QtPageViewerCollectCallback : public SbCollectCallback {
public: //SbCollectCallback
  virtual void execute(const SoCanvas&,const SoNodeList& aValue) {
    std::string value;
    int number = aValue.getLength();
    for(int index=0;index<number;index++) {
      if(index) value += "\n";
      value += std::string(aValue[index]->getName().getString());      
    }
    fPageViewer.emitCollect(value);
  }
public:
  QtPageViewerCollectCallback(QPageViewer& aPageViewer)
    :fPageViewer(aPageViewer){}
private:
  QPageViewer& fPageViewer;
};
}

//////////////////////////////////////////////////////////////////////////////
OnX::QPageViewer::QPageViewer(
 QWidget* aParent
,const char* aViewerClass
)
:QWidget(aParent)
,fViewer(0)
,fRubberMove(false)
,fBeginMove(0,0)
,fEndMove(0,0)
,fPopX(0),fPopY(0)
,fHasCollectConnections(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aViewerClass && !strcmp(aViewerClass,"Plane")) {
    fViewer = new SoQtPlaneViewer(aParent,"PAGEVIEWER_VIEWER",TRUE);
  } else {
    fViewer = new SoQtExaminerViewer(aParent,"PAGEVIEWER_VIEWER",TRUE);
  }
  GuiViewer_initialize<SoQtViewer>(*fViewer);

  SoPage* soPage = GuiViewer_SoPage<SoQtViewer>(*fViewer);
  if(soPage) {
    soPage->setCollectCallback(new QtPageViewerCollectCallback(*this));
  }

  // Create a "pick mode" popup menu :
  //QWidget parent = getNormalWidget();
  fPopupMenu = new QMenu(aParent);
  fViewer->setEventCallback(eventCB,this);
}
//////////////////////////////////////////////////////////////////////////////
OnX::QPageViewer::~QPageViewer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete fPopupMenu;
  if(fViewer) {
    GuiViewer_finalize<SoQtViewer>(*fViewer);
    if(fViewer->isOfType(SoQtExaminerViewer::getClassTypeId())) {
      delete (SoQtExaminerViewer*)fViewer;
    } else if(fViewer->isOfType(SoQtPlaneViewer::getClassTypeId())) {
      delete (SoQtPlaneViewer*)fViewer;
    } else {
      SoDebugError::postInfo
        ("OnX::QPageViewer::~QPageViewer","unknown viewer class");
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
SoNode* OnX::QPageViewer::sceneGraph(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fViewer) return 0;
  return fViewer->getSceneGraph();
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::emitRectangularCollect(
  const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QCallbackData data;
  data.value = aValue;
  emit rectangularCollect(data);
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::emitCollect(
  const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QCallbackData data;
  data.value = aValue;
  emit collect(data);
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::emitPopup(
  const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QCallbackData data;
  data.value = aValue;
  data.x = fPopX;
  data.y = fPopY;
  emit popup(data);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnX::QPageViewerPopupCallback::QPageViewerPopupCallback(
 QPageViewer& aPageViewer
,const std::string& aLabel
)
:fPageViewer(aPageViewer),fLabel(aLabel)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewerPopupCallback::execute(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fPageViewer.emitPopup(fLabel);
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::addPopupEntry(
 const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : who delete the QPageViewerPopupCallback ?
  if(!fPopupMenu) return;
  //QAction* act = //have to delete.
  fPopupMenu->addAction(aLabel.c_str(),
                        new QPageViewerPopupCallback(*this,aLabel),
                        SLOT(execute()));
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::drawRect(
 QPainter& aPainter
,const QPoint& aBegin
,const QPoint& aEnd
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPainter.drawRect(aBegin.x(),aBegin.y(),aEnd.x(),aEnd.y());
}
//////////////////////////////////////////////////////////////////////////////
SbBool OnX::QPageViewer::eventCB(
 void* aTag
,QEvent* aEvent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QPageViewer* This = (QPageViewer*)aTag;

  //if(aEvent->type()==QEvent::ContextMenu)

  if(aEvent->type()==QEvent::MouseButtonPress) {
    QMouseEvent* event = (QMouseEvent*)aEvent;
    if(event->button()==Qt::RightButton) {
      if(This->fViewer->isViewing()==TRUE) return FALSE;

      This->fPopupMenu->popup
        (This->mapToGlobal(QPoint(event->x(),event->y())));

      This->fPopX = event->x();
      int hwin = This->fViewer->getNormalWidget()->height();
      This->fPopY = hwin - event->y(); //GL coordinates.

      return FALSE; //So that SoPage::mouseEventCB is treated.
    } else if(event->button()==Qt::LeftButton) {
      //NOTE : on my Mac, for Qt, Ctrl and "apple" keys are inverted
      //       whilst they are ok for Xt and gtk.
      if( ((event->modifiers() & Qt::ShiftModifier)==Qt::ShiftModifier) 
      &&  ((event->modifiers() & Qt::ControlModifier)==Qt::ControlModifier) 
      ){
        This->fRubberMove = true;
        This->fBeginMove.setX(event->x());
        This->fBeginMove.setY(event->y());
        This->fEndMove = This->fBeginMove;

        //FIXME : how to do a rubber band with Qt4 ?
        //painter.setCompositionMode(QPainter::CompositionMode_Xor);

        return TRUE;
      } else {
        // Simple pick :
        if(This->fHasCollectConnections) {
        } else {
          SoPage* soPage = GuiViewer_SoPage<SoQtViewer>(*(This->fViewer));
          if(soPage) soPage->setHandlePickedPoint(FALSE);
        }
        return FALSE; //And then let the viewer treats also the event.
      }
    }
  } else if(aEvent->type()==QEvent::MouseButtonRelease) {
    QMouseEvent* event = (QMouseEvent*)aEvent;
    if(event->button()==Qt::LeftButton) {
      if(This->fRubberMove) {
        This->fRubberMove = false;

        //FIXME : how to do a rubber band with Qt4 ?
        //painter.setCompositionMode(QPainter::CompositionMode_Xor);

        int x,y;
        unsigned int w,h;
        This->getRectangle(This->fBeginMove.x(),This->fBeginMove.y(),
                           This->fEndMove.x(),This->fEndMove.y(),
                           x,y,w,h);
        
        int wwin = This->fViewer->getNormalWidget()->width();
        int hwin = This->fViewer->getNormalWidget()->height();

        // We pass in GL coordinate (origin is bottom-left, y toup) :
        int xgl = x;
        int ygl = hwin - (y+h);

        //printf("debug : %d %d %d %d : %d %d\n",xgl,ygl,w,h,wwin,hwin);

        if(This->fViewer->isViewing()==TRUE) {
          GuiViewer_map<SoQtViewer>(*(This->fViewer),
                            SbVec2s((short)xgl,(short)ygl),
                            SbVec2s((short)w,(short)h),
                            SbVec2s((short)wwin,(short)hwin));
        } else {

          std::string value;
          inlib::sprintf(value,132,"%d %d %d %d",xgl,ygl,w,h);
          This->emitRectangularCollect(value);
        }

        return TRUE;
      }
    }
  } else if(aEvent->type()==QEvent::MouseMove) {
    QMouseEvent* event = (QMouseEvent*)aEvent;
    if((event->buttons() & Qt::LeftButton)!=Qt::LeftButton) return FALSE;

    if( ((event->modifiers() & Qt::ShiftModifier)==Qt::ShiftModifier) 
    &&  ((event->modifiers() & Qt::ControlModifier)==Qt::ControlModifier) 
    ){
      if(This->fRubberMove) {

        QWidget* w = This->fViewer->getNormalWidget();

        if( (event->x()<0) || (event->y()<0) 
            || (event->x()>=w->width()) || (event->y()>=w->height()) ){
          // Out of the window :
          return TRUE;
        }

        This->fEndMove.setX(event->x());
        This->fEndMove.setY(event->y());

        //FIXME : how to do a rubber band with Qt4 ?
        //painter.setCompositionMode(QPainter::CompositionMode_Xor);

        return TRUE;
      }
    } else {
      //Picking mode : not discarded. We can be in resize/move region.
      if(This->fViewer->isViewing()==FALSE) return FALSE; //Picking mode.
      //GL coordinates.
      int hwin = This->fViewer->getNormalWidget()->height();
      int px = event->x();
      int py = hwin - event->y();
      // Discard mouse move event not in current region in viewing mode.
      if(!GuiViewer_isInCurrentRegion<SoQtViewer>
            (*(This->fViewer),SbVec2s(px,py))) return TRUE;
    }
  } else if(aEvent->type()==QEvent::KeyPress) {
    if(This->fViewer->isViewing()!=TRUE) return FALSE;
    QKeyEvent* event = (QKeyEvent*)aEvent;
    if(event->key()==Qt::Key_Backspace) {
      GuiViewer_popCamera<SoQtViewer>(*(This->fViewer));
      return TRUE;
    }
  }

  return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QPageViewer::getRectangle(
 int aBeginX
,int aBeginY
,int aEndX
,int aEndY
,int& aX
,int& aY
,unsigned int& aWidth
,unsigned int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aX = MINIMUM(aBeginX,aEndX);
  aY = MINIMUM(aBeginY,aEndY);
  aWidth = MAXIMUM(aBeginX,aEndX) - aX;
  aHeight = MAXIMUM(aBeginY,aEndY) - aY;
}
void OnX::QPageViewer::connectNotify(const char* a_signal) {
  if(QLatin1String(a_signal) == SIGNAL(collect(QCallbackData&)) ){
    fHasCollectConnections = true;
  }
}
SoQtFullViewer* OnX::QPageViewer::soViewer() const {return fViewer;}

