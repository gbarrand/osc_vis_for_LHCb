#
# Used by the detector.py example.
#

import OnX

ui = OnX.session().ui()

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.title.setValue('Python / Detector')

soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.8,0.8,0.8))
soSep = soRegion.cast_SoDisplayRegion().getStaticScene()

soScale = Inventor.SoScale()
soScale.scaleFactor.setValue(Inventor.SbVec3f(0.1,0.1,0.1))
soSep.addChild(soScale)

soDet = HEPVis.SoDetectorExample()
soSep.addChild(soDet)

del soDet
del soScale
del soSep
del soRegion
del soPage

