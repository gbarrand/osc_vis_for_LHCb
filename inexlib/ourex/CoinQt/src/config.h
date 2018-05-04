
#ifdef WIN32
#define HAVE_WINDOWS_H 1
#else

// SoQtLinuxJoystick does not compile,
// (cpp macro JSIOCGNAME not found in /usr/include/linux/joystick.h).
#undef HAVE_JOYSTICK_LINUX

#ifdef __APPLE__
#else
#define HAVE_GLX 1 /*G.Barrand*/
#endif

#endif

#ifdef __APPLE__
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

#if defined(__GNUC__) && (__GNUC__ >= 3) && !defined(__APPLE__)
#define ENABLE_NLS 1
#define HAVE_GETTEXT 1
#endif

#define PACKAGE "SoQt"
#define PACKAGE_LOCALE_DIR "/usr/local/share/locale"

/* same as SoQtBasic.h */
#if ! defined(SOQT_MAJOR_VERSION)
#define SOQT_MAJOR_VERSION 1
#endif
#if ! defined(SOQT_MINOR_VERSION)
#define SOQT_MINOR_VERSION 3
#endif
#if ! defined(SOQT_MICRO_VERSION)
#define SOQT_MICRO_VERSION 0
#endif
#if ! defined(SOQT_BETA_VERSION)
#define SOQT_BETA_VERSION 0
#endif
#if ! defined(SOQT_VERSION)
#define SOQT_VERSION "1.3.0"
#endif

#define VERSION "1.3.0"

#define WITH_STATIC_DEFAULTS 1
