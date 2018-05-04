#
# Technical note :
# --------------
#  With SWIG, we have not createed a "%module osc"
# but a "%module OnX". Then the DLL is OnX_SWIG_Python
# and the generated .py is OnX.py and not
# this osc.py. In order to "namespace" to osc we
# do the below "form OnX import *".
#  The SWIG files are in OnX/source/SWIG and the
# main one (%module) is OnX_SWIG.i.
#

from OnX import *

