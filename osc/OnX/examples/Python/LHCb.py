#
# A viewer with a display region, a text region to dump the "Run Event"
# and an image region to have the LHCb logo.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import LHCb
#

import Slash
session = Slash.create_session()
ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.setTitleVisible(Inventor.FALSE)
#soPage.titleVisible.setValue(Inventor.FALSE)
#soPage.updateChildren()
soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.4,0.4,0.4))
soViewportRegion = soRegion.getViewportRegion()
soViewportRegion.setSizePercent(1,0.9)
soRegion.readScene('LHCb_magnet.hiv','','staticScene')

# Text area :
soRegion = soPage.createRegion(Inventor.SbString('SoTextRegion'),0,0.9,1,0.1)
soTextRegion = soRegion.cast_SoTextRegion()
soTextRegion.text.setValue('Run 0 Event 0')

# LHCb logo :
soRegion = soPage.createRegion(Inventor.SbString('SoImageRegion'),0,0,0.1,0.1)
soImageRegion = soRegion.cast_SoImageRegion()
soImageRegion.getImage().fileName.setValue('logo_LHCb.gif')

soPage.setCurrentRegion(0)

soViewer = ui.findSoViewer('Viewer')
soViewer.viewAll()

del soTextRegion
del soImageRegion
del soRegion
del soPage

ui.steer()

del session

