#ifndef OnXLab_SWIG_h
#define OnXLab_SWIG_h

#include <AIDA/AIDA.h>
#include <AIDA/ITupleEntry.h>

#include <amanip/find.h>
#include <amanip/create.h>
#include <amanip/copy.h>

#include <inlib/cast>

#include <Slash/Tools/AIDA.h>

#include <Slash/Store/ITree.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/IHierarchy.h>
#include <Slash/UI/IPlotter.h>

#include <OnXLab/Core/Main.h>

// for %extend nakedHisto :
#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <BatchLab/Core/Histogram.h>

#include <complex>

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
#define int64 AIDA::int64
#endif

#endif


