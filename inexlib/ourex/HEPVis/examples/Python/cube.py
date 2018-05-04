#
# Example of visualizing a cube by using :
#   CoinPython and CoinXt.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import cube
#

#/////////////////////////////////////////
#/// create the Inventor scene graph : ///
#/////////////////////////////////////////
import CoinPython as Inventor
Inventor.SoDB.init()
Inventor.SoNodeKit.init()
Inventor.SoInteraction.init()

import HEPVis
HEPVis.initClasses()

sg = Inventor.SoSeparator()

soMat = Inventor.SoMaterial()
soMat.diffuseColor.setValue(Inventor.SbColor(1,1,0))
sg.addChild(soMat)

soCube = Inventor.SoCube()
soCube.width.setValue(4)   # Then no more a cube !
sg.addChild(soCube)

#/////////////////////////////////////////
#/// create the GUI : ////////////////////
#/////////////////////////////////////////
from CoinXt import SoXt as SoGUI
from CoinXt import SoXtExaminerViewer as ExaminerViewer

top = SoGUI.init('')

viewer = ExaminerViewer(top)
viewer.setSceneGraph(sg)
    
viewer.show()

SoGUI.show(top)

SoGUI.mainLoop()

#/////////////////////////////////////////
#/// end /////////////////////////////////
#/////////////////////////////////////////

del viewer
del top
