//////////////////////////////////////////////////////////////////////////////
/// OpenGL contour example ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <cmath>

inline double data_function(double x,double y,void*) {
  return ::fabs(::sin(x)/x)*(::cos(y)*y);
}

#include <inlib/CListContour>
#include <exlib/OpenGL>

static void draw_contour(inlib::CListContour& aListContour) {
  glLineWidth(1);

  double limits[4];
  limits[0] = 0;
  limits[1] = 0;
  limits[2] = 0;
  limits[3] = 0;
  //aListContour.GetLimits(limits);
  for (unsigned int i=0;i<aListContour.GetNPlanes();i++)  {
    inlib::CLineStripList* pStripList = aListContour.GetLines(i);
    if(!pStripList) {
      //FIXME : should produce a warning.
      return;
    }
    for (inlib::CLineStripList::iterator pos=pStripList->begin(); 
         pos != pStripList->end() ; pos++) {
      inlib::CLineStrip* pStrip = (*pos);
      if(!pStrip) {
        //FIXME : should produce a warning.
        return;
      }
      if (pStrip->empty()) continue;

      // putting point at start and end of strip
      glBegin(GL_POINTS);
      glColor4f(1,0,0,.8f);
      // retreiving index
      unsigned int index=pStrip->front();
      // drawing
      double xb=aListContour.GetXi(index);
      double yb=aListContour.GetYi(index);
      glVertex2f((GLfloat)(limits[0]+xb),(GLfloat)(limits[2]+yb));
      // retreiving index
      //glColor4f(0,0,1,.8f);
      index=pStrip->back();
      // drawing
      double xe=aListContour.GetXi(index);
      double ye=aListContour.GetYi(index);
      //printf("debug : %g %g : %g %g\n",xb,yb,xe,ye);
      glVertex2f((GLfloat)(limits[0]+xe),(GLfloat)(limits[2]+ye));
      glEnd();

      glColor3f(i/(float)aListContour.GetNPlanes(),
                i/(float)aListContour.GetNPlanes(),
                1.f-i/(float)aListContour.GetNPlanes());

      // filling strip if closed...
      glBegin(GL_LINE_STRIP);
      for (inlib::CLineStrip::iterator pos2=pStrip->begin(); 
           pos2 != pStrip->end() ; pos2++) {
        // retreiving index
        index=(*pos2);
        // drawing
        double x=aListContour.GetXi(index);
        double y=aListContour.GetYi(index);
        glVertex2f((GLfloat)(limits[0]+x),(GLfloat)(limits[2]+y));
      }
      glEnd();
    }
  }
}

#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

namespace OnXExas {
class ContourManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& aClass) const {
    if(aClass=="OnXExas::ContourManager") {
      return (void*)static_cast<const OnXExas::ContourManager*>(this);
    } else if(aClass=="Slash::Core::IManager") {
      return (void*)static_cast<const Slash::Core::IManager*>(this);
    } else {return 0;}
  }
public:
  ContourManager(Slash::Core::ISession& aSession) 
  :fSession(aSession),fName("OnXExas_ContourManager")
  ,fListContour(0){
    fListContour = new inlib::CListContour;
    //fListContour->Generate();

    // WARNING : data_function not defined at x = 0 !!!

    double limits[4];
    double epsilon = 0.01; //To avoid the singularity.
    limits[0] = -6+epsilon;
    limits[1] =  6+epsilon;    
    limits[2] = -6+epsilon;    
    limits[3] =  6+epsilon;    
    fListContour->SetLimits(limits);
    int zn = 100;
    std::vector<double> zs(zn+1);
    double zmn = -6;
    double zmx = 6;
    double zd = (zmx-zmn)/zn;
    for(int zi=0;zi<=zn;zi++) zs[zi] = zmn + zi * zd;
    fListContour->SetPlanes(zs);

    //fListContour->SetFirstGrid(32,32);
    //fListContour->SetSecondaryGrid(256,256);

    fListContour->SetFieldFcn(data_function,0);
    fListContour->Generate();
  }
  virtual ~ContourManager(){delete fListContour;}
private:
  ContourManager(const ContourManager& aFrom):fSession(aFrom.fSession){}
  ContourManager& operator=(const ContourManager&){return *this;}
public:
  inlib::CListContour* listContour() const { return fListContour;}
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  inlib::CListContour* fListContour;
};
}


#ifdef OnXExas_include
//FIXME : arrange so that the below be local.

// Slash :
#include <Slash/Tools/Manager.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ICallbackData.h>

extern "C" {

void opengl_contour_paint(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();

  OnXExas::ContourManager* contourManager = 
    Slash_findManager(session,"OnXExas_ContourManager",
                      OnXExas::ContourManager);
  if(!contourManager) {
    contourManager = new OnXExas::ContourManager(session);
    session.addManager(contourManager);
  }

  inlib::CListContour* listContour = contourManager->listContour();

  Slash::UI::IWidget* widget = aUI.callbackData().widget();
  if(!widget) return;
  unsigned int width,height;
  if(!widget->size(width,height)) return;
  //printf("debug : %s : %d %d\n",widget->name().c_str(),width,height);

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_SCISSOR_TEST);

  glEnable(GL_LINE_STIPPLE);
  glPolygonMode(GL_FRONT,GL_FILL);
  //  So that lightning is correctly computed 
  // when some scaling is applied.
  glEnable(GL_NORMALIZE);

  glShadeModel(GL_FLAT);
  //glEnable(GL_LIGHTING);
  //glDisable(GL_LIGHT0);

  glViewport(0,0,width,height);
  //glScissor(0,0,width,height);

  // In OpenGL, by default,
  // camera is placed at origin, looking at -z, up along y.
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  double aspect = ((double)viewport[2])/((double)viewport[3]);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();

  double limits[4];
  listContour->GetLimits(limits);
  double xb = limits[0];
  double xe = limits[1];
  double yb = limits[2];
  double ye = limits[3];

  double half_height = (ye-yb)/2;
  half_height *= 1.2;

  glOrtho(-aspect * half_height,aspect*half_height,
          -half_height,half_height,-100,100);

  // Better to clear after glViewport and glScissor (else problems with Mesa).
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslated(-(xb+xe)/2,-(yb+ye)/2,0);

  draw_contour(*listContour);

  glFinish();
}

}

#endif //OnXExas_include

#include <Slash/Tools/UI.h>

// this method is reused in examples/Callbacks/Included.cxx.

static void opengl_contour(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  if(!ui->setCallback
        ("opengl_area","paint","DLD","OnXExas opengl_contour_paint")) {
    ui->session().cout() << "opengl_contour :" 
                         << " Can't set paint callback "
                         << " on widget opengl_area."
                         << std::endl;
  }
  if(!ui->setCallback
        ("opengl_area","event","DLD","OnXExas opengl_event")) {
    ui->session().cout() << "opengl_contour :" 
                         << " Can't set event callback "
                         << " on widget opengl_area."
                         << std::endl;
  }

  if(a_ui) ui->steer();
}


#include <Slash/Core/ISession.h>

#include <iostream>

int main(int,char*[]) {

  int argc = 1;
  static char* argv[] = {(char*)"-onx_file=opengl_area.onx"};  

  Slash::Core::ISession* session = Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "opengl_contour :"
              << " session not found." << std::endl;
    return 1;
  }

  opengl_contour(*session,std::cout,"@current@",true);

  delete session;

  return 0;
}
