%module CoinXt

// From Pivy-0.3.0/coin.i :
%define RefCount(...)
  %typemap(newfree) __VA_ARGS__ * { $1->ref(); }
  %extend __VA_ARGS__ { ~__VA_ARGS__() { self->unref(); } }
  %ignore __VA_ARGS__::~__VA_ARGS__();
%enddef

// Problem with g++-3.4.6 :
%ignore SbString::vsprintf(const char*,va_list);

%include pivy_common_typemaps.i

// WARNING : %import and not %include to not rewrap Inventor !
%import iv_all.i

// From : Pivy-0.3.0/Inventor/fields/SoMFShort.i :
%{
static void
convert_SoMFShort_array(PyObject *input, int len, short *temp)
{
  int i;

  for (i=0; i<len; i++) {
    PyObject *oi = PySequence_GetItem(input,i);
    if (PyNumber_Check(oi)) {
      temp[i] = (short) PyInt_AsLong(oi);
    } else {
      PyErr_SetString(PyExc_ValueError,"Sequence elements must be numbers");
      free(temp);       
      return;
    }
  }
  return;
}
%}

// From : Pivy-0.3.0/Inventor/fields/SoMFFloat.i :
%{
static void
convert_SoMFFloat_array(PyObject *input, int len, float *temp)
{
  int i;

  for (i=0; i<len; i++) {
    PyObject *oi = PySequence_GetItem(input,i);
    if (PyNumber_Check(oi)) {
      temp[i] = (float) PyFloat_AsDouble(oi);
    } else {
      PyErr_SetString(PyExc_ValueError,"Sequence elements must be floats");
      free(temp);       
      return;
    }
  }
  return;
}
%}

%inline {
// to avoid message :
//  swig/python detected a memory leak of type 'Widget *', no destructor found.
typedef struct _WidgetRec *Widget;
}

%include <Inventor/Xt/SoXt.h>
%include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
%include <Inventor/Xt/viewers/SoXtPlaneViewer.h>

// From Pivy-0.3.0/coin.i :
// removes all the properties for fields in classes derived from
// SoFieldContainer. this makes way for the dynamic access to fields
// as attributes.
//
//  Note: this has to be the last code in the pivy file, therefore it
// is after all other SWIG declarations!

%pythoncode %{        
import CoinPython as Inventor
for x in locals().values():
  if isinstance(x, type) and issubclass(x, Inventor.SoFieldContainer):
    for name, thing in x.__dict__.items():
      if isinstance(thing, property):
        delattr(x, name)
%}
