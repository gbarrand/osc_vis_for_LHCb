#include <OnXLab/SWIG/SWIG.h>

#include <jni.h>

#define AWT_LOCK()
#define AWT_UNLOCK()

#ifdef WIN32
//  The default definitions do not permit
// to dynamicly load things from java !
#undef JNIEXPORT
#undef JNICALL
#define JNIEXPORT
#define JNICALL
#endif

#include "OnXLab_SWIG_Java.ic"
