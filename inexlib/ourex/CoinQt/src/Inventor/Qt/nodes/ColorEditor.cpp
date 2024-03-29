/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/SbPList.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/Qt/SoAny.h>

#include <Inventor/Qt/nodes/SoGuiViewportFix.h>
#include <Inventor/Qt/nodes/SoGuiPane.h>
#include <Inventor/Qt/nodes/SoGuiClickCounter.h>
#include <Inventor/Qt/nodes/SoGuiSlider1.h>
#include <Inventor/Qt/nodes/SoGuiSlider2.h>

#include <Inventor/Qt/nodes/SoGuiColorEditor.h>
#include <assert.h>

/*!
  \class SoGuiColorEditor Inventor/Qt/editors/SoGuiColorEditor.h
  \brief The SoGuiColorEditor class is a GUI component for interactively
  editing color fields.
*/

/*!
  \enum SoGuiColorEditor::Sliders
*/

/*!
  \val SoGuiColorEditor::NONE
*/

/*!
  \val SoGuiColorEditor::INTENSITY
*/

/*!
  \val SoGuiColorEditor::RGB
*/

/*!
  \val SoGuiColorEditor::HSV
*/

/*!
  \val SoGuiColorEditor::RGB_V
*/

/*!
  \val SoGuiColorEditor::RGB_HSV
*/

/*!
  \enum SoGuiColorEditor::Update
*/

/*!
  \val SoGuiColorEditor::CONTINUOUS
*/

/*!
  \val SoGuiColorEditor::AFTER_ACCEPT
*/

// *************************************************************************

class ColorEditor {
public:
  SoGuiColorEditor * api;

  void colorChange(void);

  SoFieldSensor * color_sensor;
  static void color_update_cb(void * closure, SoSensor * sensor);

  SoGuiPane * editor;
  SoGuiClickCounter * switcher;
  SoGuiSlider1 * slider_r; // red
  SoGuiSlider1 * slider_g; // green
  SoGuiSlider1 * slider_b; // blue
  SoGuiSlider1 * slider_h; // hue
  SoGuiSlider1 * slider_s; // saturation
  SoGuiSlider1 * slider_v; // value
  SoGuiSlider2 * slider_wheel;

  SoFieldSensor * sensor_r;
  SoFieldSensor * sensor_g;
  SoFieldSensor * sensor_b;
  SoFieldSensor * sensor_h;
  SoFieldSensor * sensor_s;
  SoFieldSensor * sensor_v;
  SoFieldSensor * sensor_wheel;

  static void update_r_cb(void * closure, SoSensor * sensor);
  static void update_g_cb(void * closure, SoSensor * sensor);
  static void update_b_cb(void * closure, SoSensor * sensor);
  static void update_h_cb(void * closure, SoSensor * sensor);
  static void update_s_cb(void * closure, SoSensor * sensor);
  static void update_v_cb(void * closure, SoSensor * sensor);
  static void update_wheel_cb(void * closure, SoSensor * sensor);

  static const char * editorscene[];

  static float calculateHue(float x, float y);
  static SbVec2f calculateFromHue(float h, float s, float v);

  void generateSliderTextureR(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureG(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureB(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureH(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureS(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureV(const SbColor & current, SbBool wysiwyg);
  void generateSliderTextureHSV(const SbColor & current, SbBool wysiwyg);
};

// *************************************************************************

#define PRIVATE(obj) ((ColorEditor *) ((SoGuiColorEditor *) obj)->internals)
#define PUBLIC(obj) (((ColorEditor *) obj)->api)

void
SoGuiColorEditor::initClass(void)
{
  SO_KIT_INIT_CLASS(SoGuiColorEditor, SoBaseKit, "BaseKit");
}

SO_KIT_SOURCE(SoGuiColorEditor);

SoGuiColorEditor::SoGuiColorEditor(void)
{
  this->internals = (void *) new ColorEditor;
  PRIVATE(this)->api = this;

  SO_KIT_CONSTRUCTOR(SoGuiColorEditor);

  SO_KIT_ADD_FIELD(wysiwyg, (FALSE));
  SO_KIT_ADD_FIELD(color, (SbColor(0.0f, 0.0f, 0.0f)));
  SO_KIT_ADD_FIELD(sliders, (SoGuiColorEditor::RGB_V));
  SO_KIT_ADD_FIELD(update, (SoGuiColorEditor::AFTER_ACCEPT));

  SO_KIT_DEFINE_ENUM_VALUE(Sliders, NONE);
  SO_KIT_DEFINE_ENUM_VALUE(Sliders, INTENSITY);
  SO_KIT_DEFINE_ENUM_VALUE(Sliders, RGB);
  SO_KIT_DEFINE_ENUM_VALUE(Sliders, HSV);
  SO_KIT_DEFINE_ENUM_VALUE(Sliders, RGB_V);
  SO_KIT_DEFINE_ENUM_VALUE(Sliders, RGB_HSV);

  SO_KIT_DEFINE_ENUM_VALUE(Update, CONTINUOUS);
  SO_KIT_DEFINE_ENUM_VALUE(Update, AFTER_ACCEPT);

  SO_KIT_SET_SF_ENUM_TYPE(sliders, Sliders);
  SO_KIT_SET_SF_ENUM_TYPE(update, Update);

  SO_KIT_ADD_CATALOG_ENTRY(root, SoGuiPane, TRUE, this, "", FALSE);

  SO_KIT_INIT_INSTANCE();

  PRIVATE(this)->switcher = NULL;
  PRIVATE(this)->sensor_r = NULL;
  PRIVATE(this)->sensor_g = NULL;
  PRIVATE(this)->sensor_b = NULL;
  PRIVATE(this)->sensor_h = NULL;
  PRIVATE(this)->sensor_s = NULL;
  PRIVATE(this)->sensor_v = NULL;
  PRIVATE(this)->sensor_wheel = NULL;

  PRIVATE(this)->editor = NULL;

  SoNode * scene = SoAny::loadSceneGraph(ColorEditor::editorscene);
  assert(scene != NULL);
  assert(scene->isOfType(SoGuiPane::getClassTypeId()));

  PRIVATE(this)->editor = (SoGuiPane *) scene;
  PRIVATE(this)->editor->ref();
  PRIVATE(this)->switcher = (SoGuiClickCounter *) SoAny::scanSceneForName(PRIVATE(this)->editor, "switcher");
  PRIVATE(this)->slider_r = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_r", TRUE);
  PRIVATE(this)->slider_g = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_g", TRUE);
  PRIVATE(this)->slider_b = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_b", TRUE);
  PRIVATE(this)->slider_h = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_h", TRUE);
  PRIVATE(this)->slider_s = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_s", TRUE);
  PRIVATE(this)->slider_v = (SoGuiSlider1 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "slider_v", TRUE);
  PRIVATE(this)->slider_wheel = (SoGuiSlider2 *) SoAny::scanSceneForName(PRIVATE(this)->editor, "colorwheel", TRUE);
  if ( PRIVATE(this)->slider_r ) {
    PRIVATE(this)->slider_r->ref();
    PRIVATE(this)->sensor_r = new SoFieldSensor(ColorEditor::update_r_cb, PRIVATE(this));
    PRIVATE(this)->sensor_r->attach(&(PRIVATE(this)->slider_r->value));
    PRIVATE(this)->generateSliderTextureR(this->color.getValue(), FALSE);
  }
  if ( PRIVATE(this)->slider_g ) {
    PRIVATE(this)->slider_g->ref();
    PRIVATE(this)->sensor_g = new SoFieldSensor(ColorEditor::update_g_cb, PRIVATE(this));
    PRIVATE(this)->sensor_g->attach(&(PRIVATE(this)->slider_g->value));
    PRIVATE(this)->generateSliderTextureG(this->color.getValue(), FALSE);
  }
  if ( PRIVATE(this)->slider_b ) {
    PRIVATE(this)->slider_b->ref();
    PRIVATE(this)->sensor_b = new SoFieldSensor(ColorEditor::update_b_cb, PRIVATE(this));
    PRIVATE(this)->sensor_b->attach(&(PRIVATE(this)->slider_b->value));
    PRIVATE(this)->generateSliderTextureB(this->color.getValue(), FALSE);
  }
  if ( PRIVATE(this)->slider_h ) {
    PRIVATE(this)->slider_h->ref();
    PRIVATE(this)->sensor_h = new SoFieldSensor(ColorEditor::update_h_cb, PRIVATE(this));
    PRIVATE(this)->sensor_h->attach(&(PRIVATE(this)->slider_h->value));
    PRIVATE(this)->generateSliderTextureH(this->color.getValue(), FALSE);
  }
  if ( PRIVATE(this)->slider_s ) {
    PRIVATE(this)->slider_s->ref();
    PRIVATE(this)->sensor_s = new SoFieldSensor(ColorEditor::update_s_cb, PRIVATE(this));
    PRIVATE(this)->sensor_s->attach(&(PRIVATE(this)->slider_s->value));
    PRIVATE(this)->generateSliderTextureS(this->color.getValue(), FALSE);

  }
  if ( PRIVATE(this)->slider_v ) {
    PRIVATE(this)->slider_v->ref();
    PRIVATE(this)->sensor_v = new SoFieldSensor(ColorEditor::update_v_cb, PRIVATE(this));
    PRIVATE(this)->sensor_v->attach(&(PRIVATE(this)->slider_v->value));
    PRIVATE(this)->generateSliderTextureV(this->color.getValue(), FALSE);
  }
  if ( PRIVATE(this)->slider_wheel ) {
    PRIVATE(this)->slider_wheel->ref();
    PRIVATE(this)->sensor_wheel = new SoFieldSensor(ColorEditor::update_wheel_cb, PRIVATE(this));
    PRIVATE(this)->sensor_wheel->attach(&(PRIVATE(this)->slider_wheel->value));
    PRIVATE(this)->generateSliderTextureHSV(this->color.getValue(), FALSE);
  }

  PRIVATE(this)->editor->unrefNoDelete();
  this->setAnyPart("root", scene);

  PRIVATE(this)->color_sensor = new SoFieldSensor(ColorEditor::color_update_cb, PRIVATE(this));
  PRIVATE(this)->color_sensor->attach(&(this->color));

  // SoGuiViewportFix * viewportfix = (SoGuiViewportFix *) SoAny::scanSceneForName(inherited::getSceneGraph(), "viewportfix", FALSE);
  // assert(viewportfix != NULL);
  // PRIVATE(this)->editor->objectSize.connectFrom(&(viewportfix->viewportSize));
}

SoGuiColorEditor::~SoGuiColorEditor(void)
{
  ColorEditor * instance = PRIVATE(this);
  delete instance;
}

// *************************************************************************

// CAT_MOD BEGIN
const char *
ColorEditor::editorscene[] = {
  "#Inventor V2.1 ascii",
  "DEF pane SoGuiPane {",
  "  worldSize 1 1 0",
  "  objectSize 400 340 0",
  //   Calculate positions
  "  SoGuiPosition {",
  "    position = DEF windowgeometry Calculator {",
  "      A = USE pane.objectSize",
  "      B 120 120 0",                               // colorwheel size
  "      C 140 30 0",                                // clickcounter size
  "      a 2",                                       // width of frame
  "      expression [",
  "        \"oA = vec3f(a, A[1] - C[1] - a, 0)\"",   // clickcounter pos
  "        \"oB = vec3f(A[0] - B[0] - a, A[1] - B[1] - a, 0)\"",  // colorwheel pos
  "      ]",
  "    }.oA",
  "  }",
  //   Create a click counter
  "  DEF switcher SoGuiClickCounter {",
  "    size = USE windowgeometry.C",
  "    first 2   # the INTENSITY sliders",
  "    last 6    # the RGB_HSV sliders",
  "    value 5   # start with the RGB_V group",
  "  }",
  //   Create a frame around the click counter area    
  "  SoGuiFrame {",
  "    size = USE windowgeometry.C",
  "    width 2",
  "    design EMBOSS",
  "  }",
  //   Create labels for the click counter
  "  Separator {",
  "    Scale {",
  "      scaleFactor 2 2 2",
  "    }",
  "    DEF sliderlabel SoGuiLabel {",
  "      which = USE switcher.value",
  "      text [",
  "        \"?\"",
  "        \"NONE\"",
  "        \"VALUE\"",
  "        \"RGB\"",
  "        \"HSV\"",
  "        \"RGBV\"",
  "        \"RGBHSV\"",
  "      ]",
  "    }",
  "  }",
  //   Create a frame around the color wheel
  "  SoGuiPosition { position = USE windowgeometry.oB }",
  "  SoGuiFrame {",
  "    size = USE windowgeometry.B",
  "    design EMBOSS",
  "    width 2",
  "    complement TRUE",
  "  }",
  //   Create the color wheel
  "  DEF colorwheel SoGuiSlider2 {",
  "    size = USE windowgeometry.B",
  "  }",
  //   Position the sliders
  "  SoGuiPosition {",
  "    position = DEF slidergeometry Calculator {",
  "      A = USE pane.objectSize",
  "      B = USE windowgeometry.B",                    // color wheel size
  "      a = USE windowgeometry.a",                    // width of frame
  "      b 10",                                        // space
  "      c 30",                                        // width of text
  "      expression [",
  "        \"ta = (A[1] - (2*a + B[1]) -  7*b) / 6\"", // slider height
  "        \"tb = A[0] - 2*b - c\"",                   // slider width
  "        \"oA = vec3f(tb, ta, 0)\"",                 // slider size
  "        \"oB = vec3f(c+b , b, 0)\"",                // slider pos
  "        \"oC = vec3f(0, ta + b, 0)\"",              // slider translation
  "        \"oD = vec3f(-c, 0, 0)\"",                  // slider label translation
  "      ]",
  "    }.oB",
  "  }",
  //   Create the sliders depending on the click count value
  "  Switch {",
  "    whichChild = USE switcher.value",
  "    Group {", // never to be rendered...
  "      DEF slider_r SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_g SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_b SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_h SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_s SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_v SoGuiSlider1 { size = USE slidergeometry.oA }",
  "      DEF slider_translation SoGuiTranslation { translation = USE slidergeometry.oC }",
  "      DEF slider_frame SoGuiFrame {",
  "        size = USE slidergeometry.oA ",
  "        design EMBOSS",
  "        width 2",
  "        complement TRUE",
  "      }",
  "      DEF label_translation SoGuiTranslation { translation = USE slidergeometry.oD }",
  "      DEF label_scale Scale { scaleFactor 2 2 2 }",
  "      DEF label_r Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"R\" }",
  "      }",
  "      DEF label_g Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"G\" }",
  "      }",
  "      DEF label_b Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"B\" }",
  "      }",
  "      DEF label_h Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"H\" }",
  "      }",
  "      DEF label_s Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"S\" }",
  "      }",
  "      DEF label_v Separator {",
  "        USE label_translation",
  "        USE label_scale",
  "        SoGuiLabel { text \"V\" }",
  "      }",
  "    }",
  "    DEF sliders_NONE Group {", // in case we search for names once, instead of indexing
  "    }",                        // children directly inside setCurrentSliders()
  "    DEF sliders_INTENSITY Group {",
  "      USE slider_frame",       // frames before sliders - the knobs must paint over frames
  "      USE slider_v",
  "      USE label_v",
  "    }",
  "    DEF sliders_RGB Group {",
  "      USE slider_frame",
  "      USE slider_b",
  "      USE label_b",
  "      USE slider_translation",
  "      USE slider_frame",
  "      USE slider_g",
  "      USE label_g",
  "      USE slider_translation",
  "      USE slider_frame",
  "      USE slider_r",
  "      USE label_r",
  "    }",
  "    DEF sliders_HSV Group {",
  "      USE slider_frame",
  "      USE slider_v",
  "      USE label_v",
  "      USE slider_translation",
  "      USE slider_frame",
  "      USE slider_s",
  "      USE label_s",
  "      USE slider_translation",
  "      USE slider_frame",
  "      USE slider_h",
  "      USE label_h",
  "    }",
  "    DEF sliders_RGB_V Group {",
  "      USE sliders_INTENSITY",
  "      USE slider_translation",
  "      USE sliders_RGB",
  "    }",
  "    DEF sliders_RGB_HSV Group {",
  "      USE sliders_HSV",
  "      USE slider_translation",
  "      USE sliders_RGB",
  "    }",
  "  } # Switch",
  "} # Pane pane",
  NULL
};
// CAT_MOD END

// *************************************************************************

void
ColorEditor::colorChange(void)
{
  float r = 0.0f, g = 0.0f, b = 0.0f, h = 0.0f, s = 0.0f, v = 0.0f;
  SbColor color(PUBLIC(this)->color.getValue());
  color.getValue(r, g, b);
  color.getHSVValue(h, s, v);

  SbVec2f wheel(this->calculateFromHue(h, s, v));

  this->sensor_r->detach();
  this->sensor_g->detach();
  this->sensor_b->detach();
  this->sensor_h->detach();
  this->sensor_s->detach();
  this->sensor_v->detach();
  this->sensor_wheel->detach();

  if ( r != this->slider_r->value.getValue() ) this->slider_r->value.setValue(r);
  if ( g != this->slider_g->value.getValue() ) this->slider_g->value.setValue(g);
  if ( b != this->slider_b->value.getValue() ) this->slider_b->value.setValue(b);
  if ( h != this->slider_h->value.getValue() ) this->slider_h->value.setValue(h);
  if ( s != this->slider_s->value.getValue() ) this->slider_s->value.setValue(s);
  if ( v != this->slider_v->value.getValue() ) this->slider_v->value.setValue(v);
  if ( wheel != this->slider_wheel->value.getValue() ) this->slider_wheel->value.setValue(wheel);

  assert(PUBLIC(this) != NULL);
  if ( PUBLIC(this)->wysiwyg.getValue() ) {
    this->generateSliderTextureR(color, TRUE);
    this->generateSliderTextureG(color, TRUE);
    this->generateSliderTextureB(color, TRUE);
    this->generateSliderTextureH(color, TRUE);
    this->generateSliderTextureS(color, TRUE);
    this->generateSliderTextureV(color, TRUE);
    this->generateSliderTextureHSV(color, TRUE);
  }

  this->sensor_r->attach(&(this->slider_r->value));
  this->sensor_g->attach(&(this->slider_g->value));
  this->sensor_b->attach(&(this->slider_b->value));
  this->sensor_h->attach(&(this->slider_h->value));
  this->sensor_s->attach(&(this->slider_s->value));
  this->sensor_v->attach(&(this->slider_v->value));
  this->sensor_wheel->attach(&(this->slider_wheel->value));
}

void
ColorEditor::color_update_cb(void * closure, SoSensor * sensor)
{
  assert(closure != NULL);
  ColorEditor * me = (ColorEditor *) closure;
  me->colorChange();
}

// *************************************************************************

void
ColorEditor::update_r_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float r, g, b;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getValue(r, g, b);
  r = me->slider_r->value.getValue();
  color.setValue(r, g, b);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_g_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float r, g, b;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getValue(r, g, b);
  g = me->slider_g->value.getValue();
  color.setValue(r, g, b);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_b_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float r, g, b;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getValue(r, g, b);
  b = me->slider_b->value.getValue();
  color.setValue(r, g, b);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_h_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float h, s, v;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getHSVValue(h, s, v);
  h = me->slider_h->value.getValue();
  color.setHSVValue(h, s, v);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_s_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float h, s, v;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getHSVValue(h, s, v);
  s = me->slider_s->value.getValue();
  color.setHSVValue(h, s, v);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_v_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  float h, s, v;
  SbColor color = PUBLIC(me)->color.getValue();
  color.getHSVValue(h, s, v);
  v = me->slider_v->value.getValue();
  color.setHSVValue(h, s, v);
  PUBLIC(me)->color.setValue(color);
}

void
ColorEditor::update_wheel_cb(void * closure, SoSensor * sensor)
{
  ColorEditor * me = (ColorEditor *) closure;
  SbVec2f value = me->slider_wheel->value.getValue();
  value = value * 2.0f - SbVec2f(1.0f, 1.0f);
  if ( value.length() > 1.0f ) {
    value.normalize();
    SbVec2f reverse = (value + SbVec2f(1.0f, 1.0f)) / 2.0f;
    me->slider_wheel->value.setValue(reverse);
  }
  float h;
  if ( value[0] == 0.0f ) h = ((value[1] < 0.0f) ? 3.0f : 1.0f) * float(M_PI) / 2.0f;
  else h = (float) atan(value[1] / value[0]);
  if ( value[0] < 0.0f ) h += float(M_PI); 
  if ( h < 0.0f ) h += (2.0f * float(M_PI));
  h /= 2.0f * float(M_PI); 
  float s = SoQtMin(value.length(), 1.0f); // float precision bugfix

  float a = 0.0f, b = 0.0f, v = 1.0f;
  SbColor existing = PUBLIC(me)->color.getValue();
  existing.getHSVValue(a, b, v);

  SbColor color;
  color.setHSVValue(h, s, v);
  PUBLIC(me)->color.setValue(color);
}

// *************************************************************************

float
ColorEditor::calculateHue(float x, float y)
{
  float hue;
  if ( x == 0.0f ) hue = ((y < 0.0f) ? 3.0f : 1.0f) * float(M_PI) / 2.0f;
  else hue = (float) atan(y / x);
  if ( x < 0.0f ) hue += float(M_PI); 
  if ( hue < 0.0f ) hue += (2.0f * float(M_PI));
  return hue / (2.0f * float(M_PI)); 
}

SbVec2f
ColorEditor::calculateFromHue(float h, float s, float v)
{
  float hue = h * 2.0f * float(M_PI);
  return SbVec2f(((float) cos(hue) * s + 1.0f) / 2.0f, ((float) sin(hue) * s + 1.0f) / 2.0f);
}

// *************************************************************************

void
ColorEditor::generateSliderTextureR(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_r != NULL);
  float red, green, blue;
  current.getValue(red, green, blue);
  if ( ! wysiwyg ) {
    green = blue = 0.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_r, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    red = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (red * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (green * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (blue * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureG(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_g != NULL);
  float red, green, blue;
  current.getValue(red, green, blue);
  if ( ! wysiwyg ) {
    red = blue = 0.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_g, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    green = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (red * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (green * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (blue * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureB(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_b != NULL);
  float red, green, blue;
  current.getValue(red, green, blue);
  if ( ! wysiwyg ) {
    red = green = 0.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_b, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    blue = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (red * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (green * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (blue * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureH(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_h != NULL);
  float hue, saturation, value;
  current.getHSVValue(hue, saturation, value);
  if ( ! wysiwyg ) {
    saturation = 1.0f;
    value = 1.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_h, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    const float hue = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      float r = 0.0f, g = 0.0f, b = 0.0f;
      SbColor color(r, g, b); 
      color.setHSVValue(hue, saturation, value);
      color.getValue(r, g, b);
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (r * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (g * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (b * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureS(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_s != NULL);
  float hue, saturation, value;
  current.getHSVValue(hue, saturation, value);
  if ( ! wysiwyg ) {
    hue = 0.0f;
    value = 1.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_s, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    const float saturation = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      float r = 0.0f, g = 0.0f, b = 0.0f;
      SbColor color(r, g, b); 
      color.setHSVValue(hue, saturation, value);
      color.getValue(r, g, b);
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (r * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (g * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (b * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureV(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_v != NULL);
  float hue, saturation, value;
  current.getHSVValue(hue, saturation, value);
  if ( ! wysiwyg ) {
    hue = 0.0f;
    saturation = 1.0f;
  }
  SoTexture2 * texture = SO_GET_PART(this->slider_v, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 1);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  for ( x = 0; x < size[0]; x++ ) { 
    const float value = (float) x / (float) (size[0] - 1);
    for ( y = 0; y < size[1]; y++ ) { 
      float r = 0.0f, g = 0.0f, b = 0.0f;
      SbColor color(r, g, b); 
      color.setHSVValue(hue, saturation, value);
      color.getValue(r, g, b);
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (r * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (g * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (b * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

void
ColorEditor::generateSliderTextureHSV(const SbColor & current, SbBool wysiwyg)
{
  assert(this->slider_wheel != NULL);
  float hue, saturation, value;
  current.getHSVValue(hue, saturation, value);
  if ( ! wysiwyg ) {
    value = 1.0f;
  }

  SoTexture2 * texture = SO_GET_PART(this->slider_wheel, "surfaceTexture", SoTexture2);
  assert(texture);
  texture->wrapS.setValue(SoTexture2::CLAMP);
  texture->wrapT.setValue(SoTexture2::CLAMP);
  SbVec2s size(256, 256);
  int nc = 3;
  texture->image.setValue(size, nc, NULL);
  texture->model.setValue(SoTexture2::DECAL);
  unsigned char * bytes = texture->image.startEditing(size, nc);
  int x, y;
  const float halfx = (float) size[0] / 2.0f;
  const float halfy = (float) size[1] / 2.0f;
  for ( y = 0; y < size[1]; y++ ) { 
    const float ypos = ((float) y - halfy) / halfy;
    for ( x = 0; x < size[0]; x++ ) { 
      const float xpos = ((float) x - halfx) / halfx; 
      const float saturation = (float) sqrt(xpos * xpos + ypos * ypos);
      float hue = ColorEditor::calculateHue(xpos, ypos);
      float red = 0.0f, green = 0.0f, blue = 0.0f;
      SbColor color(red, green, blue); 
      if ( saturation <= 1.0f ) color.setHSVValue(hue, saturation, value);
      color.getValue(red, green, blue);
      bytes[(size[0]*y+x)*nc+0] = (unsigned char) (red * 255.0f);
      bytes[(size[0]*y+x)*nc+1] = (unsigned char) (green * 255.0f);
      bytes[(size[0]*y+x)*nc+2] = (unsigned char) (blue * 255.0f);
      // if ( nc > 3 ) bytes[(size[0]*y+x)*nc+4] = 255;
    }
  }
  texture->image.finishEditing();
}

// *************************************************************************

#undef PUBLIC
#undef PRIVATE

