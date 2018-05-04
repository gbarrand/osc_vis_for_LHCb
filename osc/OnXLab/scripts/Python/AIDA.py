#
# Technical note :
# --------------
#  With SWIG, we have not createed a "%module AIDA"
# but a "%module OnXLab". Then the DLL is OnXLab_SWIG_Python
# and the generated .py is OnXLab.py and not
# this AIDA.py. In order to "namespace" to AIDA we
# do the below "form OnXLab import *".
#  The SWIG files are in OnXLab/source/SWIG and the
# main one (%module) is OnXLab_SWIG.i.
#

from OnXLab import *

