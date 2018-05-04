/*G.Barrand*/
#if defined(WIN32)
#include "config_win32.h"
#elif defined(__APPLE__)
#include "config_macosx.h"
#elif defined(__alpha)
#include "config_alpha.h"
#else
#include "config_linux.h"
#endif
