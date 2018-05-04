#
# Visualize a cyinder. It is done by using a SoCylinder
# node created from the generic SoType.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import cylinder
#

import Slash

session = Slash.create_session()
ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.title.setValue('SoCylinder')

soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.4,0.4,0.4))
soSep = soRegion.cast_SoDisplayRegion().getStaticScene()

soMaterial = Inventor.SoMaterial()
soMaterial.diffuseColor.setValue(Inventor.SbVec3f(1,1,0))
soSep.addChild(soMaterial)

soType = Inventor.SoCylinder.getClassTypeId()
soNode = soType.createInstance()
#soCylinder = soNode.cast_SoCylinder()
soSep.addChild(soNode)

del soType
del soMaterial
del soNode
del soSep
del soRegion
del soPage

ui.steer()

del session
