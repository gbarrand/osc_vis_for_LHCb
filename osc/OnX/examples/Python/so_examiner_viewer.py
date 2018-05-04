#
# The viewer is an Inventor SoExaminerViewer.
#
# There is no HEPVis related code in this example.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import so_examiner_viewer
#

import Slash
session = Slash.create_session()
ui = session.createUI('so_examiner_viewer.onx')

import CoinPython as Inventor

soViewer = ui.findSoViewer('Viewer')

soNode = soViewer.sceneGraph()
soSep = Slash.cast_SoSeparator(soNode) #FIXME
#FIXME : soSep = Inventor.cast(soNode,'SoSeparator')

soMaterial = Inventor.SoMaterial()
soMaterial.diffuseColor.setValue(Inventor.SbVec3f(1,1,0))
soSep.addChild(soMaterial)

soCylinder = Inventor.SoCylinder()
soCylinder.radius.setValue(5)
soSep.addChild(soCylinder)

del soMaterial
del soCylinder
del soSep

#soViewer.setDecoration(0)

soViewer.viewAll()

del soViewer

ui.steer()

del session
