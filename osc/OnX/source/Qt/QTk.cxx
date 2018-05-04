// this :
#include <OnX/Qt/QTk.h>

#include <inlib/sprintf>

//#define DEBUG_DESTROY

//FIXME : OnX_ -> OnX::

OnX_QColorDialog_Base::OnX_QColorDialog_Base()
#if QT_VERSION < 0x040600
:QWidget(0){}
#else
:QColorDialog(0){}
#endif
OnX_QColorDialog_Base::~OnX_QColorDialog_Base(){}

#if QT_VERSION < 0x040600
OnX_QColorDialog::OnX_QColorDialog() {
  Lib::Debug::increment(s_class());
}
void OnX_QColorDialog::map(){
  fValue = "";
  QColor color = QColorDialog::getColor();
  if(!color.isValid()) return;
  inlib::sprintf(fValue,3*32,"%g %g %g",
  ((double)color.red())/255,
  ((double)color.green())/255,
  ((double)color.blue())/255);
  emit ok();
  fValue = "";
}
void OnX_QColorDialog::setOkHide(bool) {}
void OnX_QColorDialog::color_selected(){}
#else
OnX_QColorDialog::OnX_QColorDialog()
:f_ok_hide(true) //Qt default
,f_do_done(true)
{
  Lib::Debug::increment(s_class());
  //setWindowTitle(title);
  //setOptions(options);
  setCurrentColor(Qt::white);
  setModal(false);
  QObject::connect(this,SIGNAL(colorSelected(const QColor&)),
                   this,SLOT(color_selected()));
}
void OnX_QColorDialog::color_selected(){
  QColor color = selectedColor();
  if(!color.isValid()) return;
  //printf("debug : colorSelected\n");
  inlib::sprintf(fValue,3*32,"%g %g %g",
  ((double)color.red())/255,
  ((double)color.green())/255,
  ((double)color.blue())/255);
  emit ok();
  fValue = "";
  if(f_ok_hide) return;
  //To have an "Apply" instead of a "Ok" :
  show();
}
void OnX_QColorDialog::setOkHide(bool aValue) {f_ok_hide = aValue;}

//NOTE : with modeless/show, an "Ok" induces a
//       recursive done()/setVisible().
//       We overload done() and setVisible() to avoid
//       that.
void OnX_QColorDialog::done(int r){
  if(!f_do_done) return;
  QColorDialog::done(r);
}
void OnX_QColorDialog::setVisible(bool aVisible){
  f_do_done = false;
  QColorDialog::setVisible(aVisible);
  f_do_done = true;
}
#endif

#include "../source/Core/OnX_gl2ps.h"

bool OnX_QOpenGLArea::write_gl2ps(const std::string& aFileName,const std::string&) { 
  FILE* file = ::fopen(aFileName.c_str(),"w");
  if(!file) return false;

  int options = GL2PS_OCCLUSION_CULL 
    | GL2PS_BEST_ROOT 
    | GL2PS_SILENT
    | GL2PS_DRAW_BACKGROUND;
  int sort = GL2PS_BSP_SORT;
  //sort = GL2PS_SIMPLE_SORT;
    
  GLint vp[4];
  vp[0] = 0;
  vp[1] = 0;
  vp[2] = width();
  vp[3] = height();

  int bufsize = 0;
  gl2psBeginPage("title","OnX_gl2ps", 
                 vp,GL2PS_EPS,sort,options, 
                 GL_RGBA,0, NULL,0,0,0,bufsize, 
                 file,aFileName.c_str());
    
  paintGL();

  gl2psEndPage();
  ::fclose(file);

  return true;
}
