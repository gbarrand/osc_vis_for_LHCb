#ifndef OnX_QCommandLine_h
#define OnX_QCommandLine_h

#include <vector>
#include <string>

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qlineedit.h>
#endif

#include <QtGui/qevent.h> //Qt4 : to include QKeyEvent.

namespace OnX {

class QCommandLine : public QLineEdit  {
  Q_OBJECT
public:
  QCommandLine(QWidget* aParent):QLineEdit(aParent),fHistoryPos(-1) {
    QObject::connect(((QLineEdit*)this),SIGNAL(returnPressed()),
               this,SLOT(execute()));
  }
  virtual ~QCommandLine(){}
public:
  const std::string& getValue() const {return fValue;}
signals:
  void complete();
public slots:
  virtual void execute(){
    fValue = text().toStdString();
    setText("");
    fHistory.push_back(fValue);
    fHistoryPos = -1;
  }
private:
  void keyPressEvent(QKeyEvent* aKeyEvent){
    if(aKeyEvent->key()==Qt::Key_Up) {
      int n = fHistory.size(); 
      int pos = fHistoryPos == -1 ? n-1 : fHistoryPos-1;
      if((pos>=0)&&(pos<n)) {
        setText(fHistory[pos].c_str()); // Put cursor at eol.
        fHistoryPos = pos;
      }
    } else if(aKeyEvent->key()==Qt::Key_Down) {
      int n = fHistory.size(); 
      int pos = fHistoryPos == -1 ? n : fHistoryPos + 1;
      if((pos>=0)&&(pos<n)) {
        setText(fHistory[pos].c_str()); // Put cursor at eol.
        fHistoryPos = pos;
      } else if(pos>=n) {
        QLineEdit::setText("");
        fHistoryPos = -1;
      }
    } else if(aKeyEvent->key()==Qt::Key_Escape) {
      fValue = text().toStdString();
      emit complete();
    } else {
      QLineEdit::keyPressEvent(aKeyEvent);
    }
  }
private:
  std::vector<std::string> fHistory;
  int fHistoryPos;
  std::string fValue;
};

}

#endif
