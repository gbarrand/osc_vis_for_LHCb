#ifndef HEPVis_SbBasic_h
#define HEPVis_SbBasic_h

#if !defined(HEPVisXt) && !defined(HEPVisWin) && !defined(HEPVisQt) && !defined(HEPVisGtk)
#ifdef WIN32
#define HEPVisWin
#else
#define HEPVisXt
#endif
#endif

/* For SbFile : */
#if !defined(HEPVis_NT) && !defined(HEPVis_WINNT)
#ifdef WIN32
#define HEPVis_WINNT
#else
#define HEPVis_UNIX
#endif
#endif

#endif
