
#include <OnX/Qt/QtTools.h>

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qdialog.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qtreewidget.h>
#include <QtGui/qabstractbutton.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qabstractbutton.h>
#endif

#include "q3_vbox.h"
#include "q3_hbox.h"

#include <inlib/words>
#include <inlib/srep>

#define NotFound (-1)

namespace OnX {

Qt::WindowFlags
decorationFlags(const std::vector< std::pair<bool,std::string> >& aItems){
  Qt::WindowFlags Qt_decor = 0;

  // default values for decorations are in BaseUI.cxx
  bool close_button = true;
  bool iconify_button = true;
  bool min_max_button = true;
  bool resize_handle = true;
  bool border = true;
  bool caption = true;
  bool system_menu = true;
    
  std::vector< std::pair<bool,std::string> >::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    if ((*it).second == "close_button") close_button = (*it).first; 
    if ((*it).second == "iconify_button") iconify_button = (*it).first; 
    if ((*it).second == "min_max_button") min_max_button = (*it).first; 
    if ((*it).second == "resize_handle") resize_handle = (*it).first; 
    if ((*it).second == "border") border = (*it).first;
    if ((*it).second == "caption") caption = (*it).first; 
    if ((*it).second == "system_menu") system_menu = (*it).first;
  }

  if (!close_button) {}
  if (!resize_handle) {}

  if (iconify_button) Qt_decor |= Qt::WindowMinimizeButtonHint;
  if (min_max_button) Qt_decor |= Qt::WindowMaximizeButtonHint;
  if (border) Qt_decor |= 0;
  else        Qt_decor |= Qt::FramelessWindowHint;
  if (caption) Qt_decor |= Qt::WindowTitleHint;
  if (system_menu) Qt_decor |=  Qt::WindowSystemMenuHint;
  Qt_decor |= Qt::Window;
  //if(aMeta.inherits("QDialog")) Qt_decor |= Qt::Dialog;

  return Qt_decor;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void QButtonSetBackground(
 QAbstractButton& aWidget
,const std::string& aValue){
  //FIXME : does not work
  QPalette p = aWidget.palette();
  p.setColor(QPalette::Background, QColor(aValue.c_str()));
  p.setColor(QPalette::Button, QColor(aValue.c_str()));
  p.setColor(QPalette::Base, QColor(aValue.c_str()));
  aWidget.setPalette(p);
  //QPalette palette;
  //palette.setColor(aWidget.backgroundRole(),QColor(aValue.c_str()));
  //aWidget.setPalette(palette);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void QComboBoxSetBackground(QComboBox& aWidget,const std::string& aValue){
  //FIXME : does not work
  QPalette p = aWidget.palette();
  p.setColor(QPalette::Background, QColor(aValue.c_str()));
  p.setColor(QPalette::Button, QColor(aValue.c_str()));
  p.setColor(QPalette::Base, QColor(aValue.c_str()));
  aWidget.setPalette(p);
  //QPalette palette;
  //palette.setColor(aWidget.backgroundRole(),QColor(aValue.c_str()));
  //aWidget.setPalette(palette);
}
void QComboBoxSetItems(QComboBox& aWidget,const std::vector<std::string>& aItems){
  aWidget.clear();
  std::vector<std::string>::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    aWidget.insertItem(aWidget.count(),(*it).c_str());
  }
}
std::vector<std::string> QComboBoxGetItems(QComboBox& aWidget){
  std::vector<std::string> items;
  int number = aWidget.count();
  for(int index=0;index<number;index++) 
    items.push_back(aWidget.itemText(index).toStdString());
  return items;
}
void QComboBoxSetValue(QComboBox& aWidget,const std::string& aValue){
  int i = aWidget.findText(aValue.c_str());
  if(i!=NotFound) {
    aWidget.setCurrentIndex(i);
  } else if(aWidget.isEditable()) {
    aWidget.setEditText(aValue.c_str());
  } else {
    aWidget.setItemText(aWidget.currentIndex(),aValue.c_str());
  }
}
std::string QComboBoxGetValue(QComboBox& aWidget){return aWidget.currentText().toStdString();}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void QOptionMenuInitialize(QComboBox& aWidget,const std::vector<std::string>& aItems,const std::string& aDefault){
  aWidget.clear();
  int history = NotFound;
  std::vector<std::string>::const_iterator it;
  unsigned int index = 0;
  for(it=aItems.begin();it!=aItems.end();++it,index++) {
    aWidget.insertItem(aWidget.count(),(*it).c_str());
    if(history==NotFound) history = index;//first item.
    if(aDefault.size()&&((*it)==aDefault)) history = index;
  }
  if(history!=NotFound) {
    aWidget.setCurrentIndex(history);
  }
}

std::vector<std::string> QOptionMenuGetItems(QComboBox& aWidget){return QComboBoxGetItems(aWidget);}

bool QOptionMenuSetDefault(QComboBox& aWidget,const std::string& aValue){
  int number = aWidget.count();
  for(int index=0;index<number;index++) {
    if(aWidget.itemText(index).toStdString()==aValue) {
      aWidget.setCurrentIndex(index);
      return true;
    }
  }
  return false;
}
std::string QOptionMenuGetValue(QComboBox& aWidget){return aWidget.currentText().toStdString();}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool QHBoxSetExpand(OnX_Q3HBox& aWidget,const std::string& aExpand){
  const QObjectList& list = aWidget.children();
  QObjectList* children = (QObjectList*)&list;
#define setHorData setHorizontalPolicy
  QWidget* first = 0;
  QWidget* second = 0;
  // WARNING : QHBox have one extra QHBoxLayout child.
  if (children->count() == 2) {
    if ((((QObject*)(children->at(0)))->inherits("QBoxLayout")) &&
        (((QObject*)(children->at(1)))->inherits("QWidget"))) {
      first = ((QWidget*)(children->at(1)));
      // no use of second in this case
    } else {
      return false;
    }
  } else if (children->count() == 3) {
    if ((((QObject*)(children->at(0)))->inherits("QBoxLayout")) &&
        (((QObject*)(children->at(1)))->inherits("QWidget")) &&
        (((QObject*)(children->at(2)))->inherits("QWidget"))) { 
      first = ((QWidget*)(children->at(1)));
      second = ((QWidget*)(children->at(2)));
    } else {
      return false;
    }
  } else {
    return false;
  }

  if(aExpand=="second") {
    if(children->count()==2) {
      aWidget.setStretchFactor(first,0);
    } else if(children->count()==3) {
      QSizePolicy        sp = second->sizePolicy();
      sp.setHorData(QSizePolicy::Expanding);
      second->setSizePolicy(sp);
      aWidget.setStretchFactor(second,10);
      
      sp = first->sizePolicy();
      sp.setHorData(QSizePolicy::Preferred);
      first->setSizePolicy(sp);
      aWidget.setStretchFactor(first,0);
    }
  } else if(aExpand=="first") {
    if(children->count()==2) {
      QSizePolicy        sp = first->sizePolicy();
      sp.setHorData(QSizePolicy::Expanding);
      first->setSizePolicy(sp);
      aWidget.setStretchFactor(first,10);
    } else if(children->count()==3) {
      QSizePolicy        sp = first->sizePolicy();
      sp.setHorData(QSizePolicy::Expanding);
      first->setSizePolicy(sp);
      aWidget.setStretchFactor(first,10);
        
      sp = second->sizePolicy();
      sp.setHorData(QSizePolicy::Preferred);
      second->setSizePolicy(sp);
      aWidget.setStretchFactor(second,0);
    }
  } else { // expand both :
    if(children->count()>=2) {
      QSizePolicy sp = first->sizePolicy();
      sp.setHorData(QSizePolicy::Preferred);
      first->setSizePolicy(sp);
      aWidget.setStretchFactor(first,0);

      if(children->count()>=3) {
        sp = second->sizePolicy();
        sp.setHorData(QSizePolicy::Preferred);
        second->setSizePolicy(sp);
        aWidget.setStretchFactor(second,0);
      }
    }
  }
  return true;
}

bool QVBoxSetExpand(OnX_Q3VBox& aWidget,const std::string& aExpand){
  const QObjectList& list = aWidget.children();
  QObjectList* children = (QObjectList*)&list;
#define setVerData setVerticalPolicy
  QWidget* first = 0;
  QWidget* second = 0;
  // WARNING : QVBox has one extra QVBoxLayout child.
   if (children->count() == 2) {
    if ((((QObject*)(children->at(0)))->inherits("QBoxLayout")) &&
        (((QObject*)(children->at(1)))->inherits("QWidget"))) {
      first = ((QWidget*)(children->at(1)));

      QSizePolicy sp = first->sizePolicy();
        
      sp.setVerData(QSizePolicy::Expanding);
      first->setSizePolicy(sp);
      aWidget.setStretchFactor(first,10);
      return true;
    } else {
      return false;
    }
  } else if (children->count() == 3) {
    if ((((QObject*)(children->at(0)))->inherits("QBoxLayout")) &&
        (((QObject*)(children->at(1)))->inherits("QWidget")) &&
        (((QObject*)(children->at(2)))->inherits("QWidget"))) { 
      first = ((QWidget*)(children->at(1)));
      second = ((QWidget*)(children->at(2)));
    } else {
      return false;
    }
  } else {
    return false;
  }

  if(children->count()!=3) return true;

  if(aExpand=="second") { 
    QSizePolicy sp = second->sizePolicy();

    sp.setVerData(QSizePolicy::Expanding);
    second->setSizePolicy(sp);
    aWidget.setStretchFactor(second,10);
    
    sp = first->sizePolicy();
    sp.setVerData(QSizePolicy::Preferred);
    first->setSizePolicy(sp);
    aWidget.setStretchFactor(first,0);
  }
  else if(aExpand=="first") {
    QSizePolicy sp = first->sizePolicy();

    sp.setVerData(QSizePolicy::Expanding);
    first->setSizePolicy(sp);
    aWidget.setStretchFactor(first,10);
    
    sp = second->sizePolicy();
    sp.setVerData(QSizePolicy::Preferred);
    second->setSizePolicy(sp);
    aWidget.setStretchFactor(second,0);
  }
  else { // expand both :    
    QSizePolicy sp = second->sizePolicy();
    sp = second->sizePolicy();
    sp.setVerData(QSizePolicy::Preferred);
    second->setSizePolicy(sp);
    aWidget.setStretchFactor(second,0);

    sp = first->sizePolicy();
    sp.setVerData(QSizePolicy::Preferred);
    first->setSizePolicy(sp);
    aWidget.setStretchFactor(first,0);
  }
  return true;
}

std::string QTreeWidgetItemGetPath(QTreeWidgetItem& aItem){
  std::string path;
  QTreeWidgetItem* item = &aItem;
  while(item) {
    std::string opath = path;
    path = "\n";
    path += item->text(0).toStdString();
    path += opath;
    item = item->parent();
  }
  // Remove the leading \n
  if(path.size()) path = path.substr(1,path.size()-1);
  return path;
}

std::string QTreeWidgetGetSelection(QTreeWidget& aTree){
  QList<QTreeWidgetItem*> itms = aTree.selectedItems();
  if(!itms.size()) return "";
  return QTreeWidgetItemGetPath(*(itms[0]));
}

void QTreeWidgetClear(QTreeWidget& aTree){
  aTree.clear();
  if(!aTree.columnCount()) {
    aTree.setColumnCount(1);
    aTree.setRootIsDecorated(true);
  }
}

static bool do_child(QTreeWidget& a_tree,QTreeWidgetItem& a_item,const std::vector<std::string>& a_sel,unsigned int a_level){
  const std::string& slevel = a_sel[a_level];
  int number = a_item.childCount();
  for(int index=0;index<number;index++) {
    QTreeWidgetItem* item = a_item.child(index);
    if(item->text(0).toStdString()!=slevel) continue;
    if(a_level==(a_sel.size()-1)) { //leaf
      a_tree.setItemSelected(item,true);
      return true;
    }
    if(!item->childCount()) return false;
    a_tree.blockSignals(true);
    item->setExpanded(true);
    a_tree.blockSignals(false);
    return do_child(a_tree,*item,a_sel,a_level+1);
  }
  a_tree.clearSelection();
  return false;
}

bool QTreeWidgetSetSelection(QTreeWidget& aTree,const std::string& aSelection){
  aTree.clearSelection();
  QTreeWidgetItem* header = aTree.invisibleRootItem();
  if(!header) return false;
  if(!header->childCount()) return false;
  std::vector<std::string> items;
  inlib::words(aSelection,"\n",true,items);
  if(items.empty()) return false;
  return do_child(aTree,*header,items,0);
}

void QDialogDisableEnter(QDialog& aDialog){
  QList<QPushButton*> list = aDialog.findChildren<QPushButton*>();
  int number = list.size();
  for (int index=0;index<number;index++) {
    list.at(index)->setAutoDefault(false);
  }
}

std::string QTreeWidgetItemGetXML(QTreeWidgetItem& aItem){
  //return a XML string representing this tree
  std::string spaceItem;
  std::string spaceRoot;
  std::string line;
  int number = aItem.childCount();
  for(int index=0;index<number;index++) {
    QTreeWidgetItem* item = aItem.child(index);
    line += spaceRoot + "<treeItem>";
    line += spaceItem + "<label>";
    std::string s = inlib::to_xml(item->text(0).toStdString());
    line.append(s);
    line += "</label>";
    line += spaceItem + "<opened>";
    if(item->isExpanded()) line.append("true");
    else line.append("false");
    line += "</opened>";
    line += QTreeWidgetItemGetXML(*item);
    line += spaceRoot + "</treeItem>";
  }
  return line;
}

bool QTabWidgetSetSelection(QTabWidget& aWidget,const std::string& aValue){
  int number = aWidget.count();
  for(int index=0;index<number;index++) {
    if(aWidget.tabText(index).toStdString()==aValue) {    
      QWidget* page = aWidget.widget(index);
      if(page) {
        aWidget.setCurrentWidget(page);
        return true;
      }
    }
  }
  return false;
}

} //namespace OnX
