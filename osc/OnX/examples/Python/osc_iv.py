#
# Visualize "OSC" as 3D text.
# The scene is read from a .iv Inventor file.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import osc_iv
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

soRegion.cast_SoDisplayRegion().readScene(Inventor.SbString('osc.iv'),Inventor.SbString(''))

del soRegion
del soPage

ui.steer()

del session
