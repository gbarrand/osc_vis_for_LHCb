#ifndef OnX_QPageViewer_h
#define OnX_QPageViewer_h 

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include <Inventor/SbBasic.h>
class SoQtFullViewer;
class SoNode;

#include <string>

class QPainter;
class QPoint;
class QCallbackData;
class QMenu;

namespace OnX {

class QPageViewer : public QWidget {
  Q_OBJECT
public:
  QPageViewer(QWidget* = 0,const char* = 0);
  virtual ~QPageViewer();
  SoNode* sceneGraph() const;
  SoQtFullViewer* soViewer() const;
  void emitRectangularCollect(const std::string&);
  void emitCollect(const std::string&);
  void emitPopup(const std::string&);
  void addPopupEntry(const std::string&);
signals:
  void rectangularCollect(QCallbackData&);
  void collect(QCallbackData&);
  void popup(QCallbackData&);
private:
  static SbBool eventCB(void*,QEvent*);
  void drawRect(QPainter&,const QPoint&,const QPoint&);
  void getRectangle(int,int,int,int,int&,int&,unsigned int&,unsigned int&);
  virtual void connectNotify(const char*);
private:
  SoQtFullViewer* fViewer;
  QMenu* fPopupMenu;
  bool fRubberMove;
  QPoint fBeginMove,fEndMove;
  int fPopX,fPopY;
  bool fHasCollectConnections;
};

class QPageViewerPopupCallback : public QObject {
  Q_OBJECT
public:
  QPageViewerPopupCallback(QPageViewer&,const std::string&);
public slots:
  void execute();
private:
  QPageViewer& fPageViewer;
  std::string fLabel;
};

}

#endif

