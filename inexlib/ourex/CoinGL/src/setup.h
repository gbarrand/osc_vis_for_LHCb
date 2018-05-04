/*G.Barrand*/
#ifdef WIN32
#include "setup_win32.h"
#else
#ifdef __APPLE__
#include "setup_macosx.h"
#else
#include "setup_linux.h"
#endif
#endif
