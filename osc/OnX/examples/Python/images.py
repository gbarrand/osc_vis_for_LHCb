#
# Add image "regions" (done with the HEPVis SoImageRegion nodekit).
# One with the LAL logo, the other one with the LHCb logo.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import images
#

import Slash
session = Slash.create_session()
ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.titleVisible.setValue(Inventor.FALSE)
soPage.currentRegion().color.setValue(Inventor.SbColor(0.4,0.4,0.4))

# LAL logo :
soRegion = soPage.createRegion(Inventor.SbString('SoImageRegion'),0.8,0.01,0.1,0.1)
soViewportRegion = soRegion.getViewportRegion()
soViewportRegion.horizontalBorder.setValue(2)
soViewportRegion.verticalBorder.setValue(2)
soViewportRegion.borderColor.setValue(Inventor.SbColor(0,1,0))

soImageRegion = soRegion.cast_SoImageRegion()
soImageRegion.getImage().fileName.setValue('logo_LAL.gif')

# LHCb logo :
soRegion = soPage.createRegion(Inventor.SbString('SoImageRegion'),0.01,0.75,0.1,0.1)
soViewportRegion = soRegion.getViewportRegion()
soViewportRegion.horizontalBorder.setValue(2)
soViewportRegion.verticalBorder.setValue(2)
soViewportRegion.borderColor.setValue(Inventor.SbColor(0,1,0))
soImageRegion = soRegion.cast_SoImageRegion()
soImageRegion.getImage().fileName.setValue('logo_LHCb.gif')

soPage.highlightRegion(soPage.getRootRegion(0))

del soImageRegion
del soViewportRegion
del soRegion
del soPage

ui.steer()

del session

