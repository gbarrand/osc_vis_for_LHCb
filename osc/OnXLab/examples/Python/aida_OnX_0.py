#////////////////////////////////////
#// OnX part ////////////////////////
#////////////////////////////////////

import Slash

session = Slash.create_session()
session.loadInterpreter('Python');
ui = session.createUI('aida_OnX_0.onx');

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.deleteRegions()

titleVisible = soPage.titleVisible.getValue()
titleHeight = soPage.titleHeight.getValue()

# Detector :
h = 1.
if titleVisible == 1: h = 1 - titleHeight
soRegion = soPage.createRegion(Inventor.SbString("SoDisplayRegion"),0,0,1,h)
soRegion.color.setValue(Inventor.SbColor(0.8,0.8,0.8))

soSep = soRegion.cast_SoDisplayRegion().getStaticScene()

soScale = Inventor.SoScale()
soScale.scaleFactor.setValue(0.1,0.1,0.1)
soSep.addChild(soScale)

soDet = HEPVis.SoDetectorExample()
soSep.addChild(soDet)
del soDet
del soSep

# Histo region :
soRegion = soPage.createRegion(Inventor.SbString("SoPlotterRegion"),0.01,0.01,0.3,0.3)
soPage.highlightRegion(soRegion)
soViewportRegion = soRegion.getViewportRegion()
soViewportRegion.horizontalBorder.setValue(2)
soViewportRegion.verticalBorder.setValue(2)
soViewportRegion.borderColor.setValue(Inventor.SbColor(0,0,0))

ui.executeScript('DLD','OnXLab OnXLab_initialize_AIDA')

#////////////////////////////////////
#// AIDA part ///////////////////////
#////////////////////////////////////

import AIDA

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
histo = histogramFactory.createHistogram1D('gauss','Gauss',100,-5,5)
del histogramFactory
import random
r = random.Random()
for I in range(1,10000):
  histo.fill(r.gauss(0,1),1)
del r

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('Viewer')
del plotterFactory
#plotter.createRegions(1,2,0)
#plotter.setTitle('Cloud 3D')
region = plotter.currentRegion()
region.plot(histo)

soPage.titleVisible.setValue(Inventor.FALSE)
soPage.setCurrentRegion(0)

plotter.show()
plotter.interact()

del region
del histo
del plotter
del tree
del aida
del soRegion
del soPage

del session
