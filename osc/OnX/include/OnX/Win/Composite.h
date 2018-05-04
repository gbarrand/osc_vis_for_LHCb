#ifndef OnX_Composite_h
#define OnX_Composite_h 

#include <OnX/Win/WinTk.h>

#include <list>

namespace WinTk {

class ColorChooser : public Component {
public:
  ColorChooser(Component&);
  virtual ~ColorChooser();
  virtual void show();
  virtual void hide();
private:
  static void paintCallback(Component&,CallbackData&,void*);

  static void rScaleCallback(Component&,CallbackData&,void*);
  static void gScaleCallback(Component&,CallbackData&,void*);
  static void bScaleCallback(Component&,CallbackData&,void*);

  static void rEntryCallback(Component&,CallbackData&,void*);
  static void gEntryCallback(Component&,CallbackData&,void*);
  static void bEntryCallback(Component&,CallbackData&,void*);

  static void okCallback(Component&,CallbackData&,void*);
  static void applyCallback(Component&,CallbackData&,void*);
  static void cancelCallback(Component&,CallbackData&,void*);
private:
  Shell* fShell;
  int fRed;
  int fGreen;
  int fBlue;
  PaintArea* fPaintArea;
  HScale* fScaleRed;
  HScale* fScaleGreen;
  HScale* fScaleBlue;
  Entry* fEntryRed;
  Entry* fEntryGreen;
  Entry* fEntryBlue;
  std::list<Component*> fWidgets;
};

}

#endif

