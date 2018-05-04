#ifdef WIN32
#include "gdkgl_win32.c"
#else
/*
#ifdef APPLE_GL
#include "gdkgl_AGL.c"
#else
*/
#include "gdkgl_X11.c"
/*#endif*/
#endif
