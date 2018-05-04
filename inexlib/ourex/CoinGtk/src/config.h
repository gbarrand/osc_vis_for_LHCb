#ifdef WIN32
#define HAVE_WINDOWS_H 1

#else

/*A mess with NULL defined in glib.h to be ((void*)0) */
#ifdef __KCC
#ifndef NULL
#define NULL 0
#endif
#endif

#endif

#ifdef APPLE_GL
#define HAVE_OPENGL_GL_H 1
#define HAVE_OPENGL_GLU_H 1
#else
#define HAVE_GL_GL_H 1
#define HAVE_GL_GLU_H 1
#endif

#ifdef __COIN__
#define HAVE_SOCAMERA_SETSTEREOMODE 1
#define HAVE_SOMOUSEBUTTONEVENT_BUTTON5 1
#define HAVE_SOPOLYGONOFFSET 1
#endif

#define HAVE_SOKEYBOARDEVENT_DELETE 1

#define PACKAGE "SoGtk"
#define PACKAGE_LOCALE_DIR "/usr/local/share/locale"

#ifndef SOGTK_VERSION
#define SOGTK_MAJOR_VERSION 0
#define SOGTK_MICRO_VERSION 99
#define SOGTK_MINOR_VERSION 9
#define SOGTK_VERSION "0.9.99"
#endif

#define VERSION "0.9.99"

#define WITH_STATIC_DEFAULTS 1
