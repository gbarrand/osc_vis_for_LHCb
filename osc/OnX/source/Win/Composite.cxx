// this :
#include <OnX/Win/Composite.h>

#include <inlib/smanip>
#include <inlib/sprintf>

//#include <windows.h>
//#include <windowsx.h>
//#include <wingdi.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ColorChooser::ColorChooser(
 Component& aParent
)
:Component("ColorChooser",aParent)
,fShell(0)
,fRed(0)
,fGreen(0)
,fBlue(0)
,fPaintArea(0)
,fScaleRed(0)
,fScaleGreen(0)
,fScaleBlue(0)
,fEntryRed(0)
,fEntryGreen(0)
,fEntryBlue(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fShell = new Shell(WS_OVERLAPPEDWINDOW);
  fWidgets.push_back(fShell);
  fShell->setSize(300,4*40+30);

  VBox* vBox = new VBox(*fShell);
  fWidgets.push_back(vBox);
  vBox->setSizeOfFixed(30);
  vBox->setExpand(VBox::FIRST);

  // R, G, B sliders :
  VContainer* vCont = new VContainer(*vBox);
  fWidgets.push_back(vCont);

  fPaintArea = new PaintArea(*vCont);
  fWidgets.push_back(fPaintArea);
  fPaintArea->addCallback("paint",paintCallback,this);

 {HBox* hbox = new HBox(*vCont);
  fWidgets.push_back(hbox);
  fScaleRed = new HScale(*hbox);
  fWidgets.push_back(fScaleRed);
  fScaleRed->setMinimum(0);
  fScaleRed->setMaximum(255);
  fScaleRed->addCallback("drag",rScaleCallback,this);
  fEntryRed = new Entry(*hbox);
  fWidgets.push_back(fEntryRed);
  fEntryRed->addCallback("activate",rEntryCallback,this);
  fEntryRed->setValue("0");
  hbox->setSizeOfFixed(50);
  hbox->setExpand(HBox::FIRST);}

 {HBox* hbox = new HBox(*vCont);
  fWidgets.push_back(hbox);
  fScaleGreen = new HScale(*hbox);
  fWidgets.push_back(fScaleGreen);
  fScaleGreen->setMinimum(0);
  fScaleGreen->setMaximum(255);
  fScaleGreen->addCallback("drag",gScaleCallback,this);
  fEntryGreen = new Entry(*hbox);
  fWidgets.push_back(fEntryGreen);
  fEntryGreen->addCallback("activate",gEntryCallback,this);
  fEntryGreen->setValue("0");
  hbox->setSizeOfFixed(50);
  hbox->setExpand(HBox::FIRST);}

 {HBox* hbox = new HBox(*vCont);
  fWidgets.push_back(hbox);
  fScaleBlue = new HScale(*hbox);
  fWidgets.push_back(fScaleBlue);
  fScaleBlue->setMinimum(0);
  fScaleBlue->setMaximum(255);
  fScaleBlue->addCallback("drag",bScaleCallback,this);
  fEntryBlue = new Entry(*hbox);
  fWidgets.push_back(fEntryBlue);
  fEntryBlue->addCallback("activate",bEntryCallback,this);
  fEntryBlue->setValue("0");
  hbox->setSizeOfFixed(50);
  hbox->setExpand(HBox::FIRST);}

  // Ok, Apply, Cancel :
  HContainer* hCont = new HContainer(*vBox);
  fWidgets.push_back(hCont);

  Button* ok = new Button(*hCont);
  fWidgets.push_back(ok);
  ok->setLabel("Ok");
  ok->addCallback("activate",okCallback,this);

  Button* apply = new Button(*hCont);
  fWidgets.push_back(apply);
  apply->setLabel("Apply");
  apply->addCallback("activate",applyCallback,this);

  Button* cancel = new Button(*hCont);
  fWidgets.push_back(cancel);
  cancel->addCallback("activate",cancelCallback,this);
  cancel->setLabel("Cancel");
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ColorChooser::~ColorChooser(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  std::list<Component*>::iterator it;
  for(it=fWidgets.begin();it!=fWidgets.end();it=fWidgets.erase(it)) 
    delete *it;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fShell) return;
  fShell->show();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::hide(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fShell) return;
  fShell->hide();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::rScaleCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  This->fRed  = ((WinTk::HScale*)&aWidget)->value();
  std::string s;
  inlib::sprintf(s,32,"%g",double(This->fRed)/255.0);
  This->fEntryRed->setValue(s);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::gScaleCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  This->fGreen  = ((WinTk::HScale*)&aWidget)->value();
  std::string s;
  inlib::sprintf(s,32,"%g",double(This->fGreen)/255.0);
  This->fEntryGreen->setValue(s);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::bScaleCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  This->fBlue  = ((WinTk::HScale*)&aWidget)->value();
  std::string s;
  inlib::sprintf(s,32,"%g",double(This->fBlue)/255.0);
  This->fEntryBlue->setValue(s);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::rEntryCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  std::string s = ((WinTk::Entry*)&aWidget)->value();
  double d;
  if(!inlib::to<double>(s,d)) return;
  int i = int(d*255.0);
  if((i<0)||(i>255)) return;
  This->fRed = i;
  This->fScaleRed->setValue(i);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::gEntryCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  std::string s = ((WinTk::Entry*)&aWidget)->value();
  double d;
  if(!inlib::to<double>(s,d)) return;
  int i = int(d*255.0);
  if((i<0)||(i>255)) return;
  This->fGreen = i;
  This->fScaleGreen->setValue(i);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::bEntryCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  std::string s = ((WinTk::Entry*)&aWidget)->value();
  double d;
  if(!inlib::to<double>(s,d)) return;
  int i = int(d*255.0);
  if((i<0)||(i>255)) return;
  This->fBlue = i;
  This->fScaleBlue->setValue(i);
  This->fPaintArea->refresh();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::okCallback(
 Component&
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  This->hide();
  double r = double(This->fRed)/255.0;
  double g = double(This->fGreen)/255.0;
  double b = double(This->fBlue)/255.0;
  CallbackData data;
  inlib::sprintf(data.value,3*32,"%g %g %g",r,g,b);
  //printf("debug : ok : \"%s\"\n",data.value.c_str());
  This->executeCallbacks("ok",data);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::applyCallback(
 Component&
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  double r = double(This->fRed)/255.0;
  double g = double(This->fGreen)/255.0;
  double b = double(This->fBlue)/255.0;
  CallbackData data;
  inlib::sprintf(data.value,3*32,"%g %g %g",r,g,b);
  //printf("debug : apply : \"%s\"\n",data.value.c_str());
  This->executeCallbacks("ok",data);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::cancelCallback(
 Component&
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  This->hide();
  CallbackData data;
  This->executeCallbacks("cancel",data);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorChooser::paintCallback(
 Component& aWidget
,CallbackData&
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ColorChooser* This = (ColorChooser*)aTag;
  HWND hwnd = aWidget.nativeWindow();
  PAINTSTRUCT ps;
  HDC hDC = BeginPaint(hwnd,&ps);
  RECT rect;
  ::GetClientRect(hwnd,&rect);
  HBRUSH hbrush = ::CreateSolidBrush(RGB(This->fRed,This->fGreen,This->fBlue));
  ::FillRect(hDC,&rect,hbrush);
  EndPaint(hwnd,&ps);
}
