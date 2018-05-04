#ifndef OnX_QtTools_h
#define OnX_QtTools_h 

#include <string>
#include <vector>

#include <QtCore/qnamespace.h>

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QComboBox;
class QDialog;
class QTabWidget;

class QAbstractButton;
class OnX_Q3HBox;
class OnX_Q3VBox;
class QTreeWidget;
class QTreeWidgetItem;

namespace OnX {

Qt::WindowFlags 
decorationFlags(const std::vector< std::pair<bool,std::string> >&);

inline int QWidgetGetNumberOfChildren(QWidget& aWidget){
  const QObjectList& list = aWidget.children();
  return list.count();
}

inline void QWidgetSetBackground(QWidget& aWidget,const std::string& aValue){
  //FIXME : does not work
  QPalette p = aWidget.palette();
  p.setColor(QPalette::Background, QColor(aValue.c_str()));
  p.setColor(QPalette::Base, QColor(aValue.c_str()));
  aWidget.setPalette(p);
  //QPalette palette;
  //palette.setColor(aWidget.backgroundRole(),QColor(aValue.c_str()));
  //aWidget.setPalette(palette);
}

inline void QWidgetSetForeground(QWidget& aWidget,const std::string& aValue) {
  QPalette palette;
  palette.setColor(aWidget.foregroundRole(),QColor(aValue.c_str()));
  aWidget.setPalette(palette);
}


void QComboBoxSetBackground(QComboBox&,const std::string&);
void QComboBoxSetItems(QComboBox&,const std::vector<std::string>&);
std::vector<std::string> QComboBoxGetItems(QComboBox&);
void QComboBoxSetValue(QComboBox&,const std::string&);
std::string QComboBoxGetValue(QComboBox&);

void QOptionMenuInitialize(QComboBox&,const std::vector<std::string>&,const std::string&);
std::vector<std::string> QOptionMenuGetItems(QComboBox&);
bool QOptionMenuSetDefault(QComboBox&,const std::string&);
std::string QOptionMenuGetValue(QComboBox&);

void QButtonSetBackground(QAbstractButton&,const std::string&);
bool QHBoxSetExpand(OnX_Q3HBox&,const std::string&);
bool QVBoxSetExpand(OnX_Q3VBox&,const std::string&);
std::string QTreeWidgetItemGetPath(QTreeWidgetItem&);
std::string QTreeWidgetItemGetXML(QTreeWidgetItem&);

void QTreeWidgetClear(QTreeWidget&);
std::string QTreeWidgetGetSelection(QTreeWidget&);
bool QTreeWidgetSetSelection(QTreeWidget&,const std::string&);

void QDialogDisableEnter(QDialog&);

bool QTabWidgetSetSelection(QTabWidget&,const std::string&);

}

#endif

