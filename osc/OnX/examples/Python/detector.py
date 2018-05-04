#
# Visualize a detector by using its representation
# done with the HEPVis SoDetectorExample nodekit.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import detector
#

def callback():
  import Slash
  ui = Slash.session().ui()

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
  
  w = ui.findWidget('Viewer')
  soViewer = w.cast_ISoViewer()
  soViewer.setDecoration(0)
  soViewer.setViewing(1)

  del soDet
  del soScale
  del soSep
  del soRegion
  del soPage

import Slash
session = Slash.create_session()
ui = session.createUI('detector.onx')
callback()
ui.steer()
del session
