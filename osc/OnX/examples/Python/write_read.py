#
# The scene is read from the osc.iv Inventor file
# by using the Inventor SoInput and SoDB classes.
#
# The exemple writes also the same scene in an out.iv
# file by using an SoOutput and an SoWriteAction node.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import write_read
#

import Slash
session = Slash.create_session()
ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')
soPage.titleVisible.setValue(Inventor.FALSE)
soPage.createRegions(Inventor.SbString('SoDisplayRegion'),1,1,0)

soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.8,0.8,0.8))

soInput = Inventor.SoInput()
soInput.openFile('osc.iv')
soSep = Inventor.SoDB.readAll(soInput)
#sSep ref count is 0.

parent = soRegion.cast_SoDisplayRegion().getStaticScene()
parent.addChild(soSep)

soOutput = Inventor.SoOutput()
soOutput.openFile('out.iv')
#soOutput.setBinary(1)

soWriteAction = Inventor.SoWriteAction(soOutput)
soWriteAction.apply(parent)
soOutput.closeFile()

del soRegion
del soPage

ui.steer()

del session
