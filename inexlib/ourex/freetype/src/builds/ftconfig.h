/* G.Barrand */

/* 
   On Tiger and SLC3 the ftconfig.h produced 
   by configure had been the same. The ftconfig_unix.h 
   is a copy of the Tiger version.
*/

#ifdef WIN32
/* #include "ftconfig_win32.h" */
#else
#include "ftconfig_unix.h"
#endif
