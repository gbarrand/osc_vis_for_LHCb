#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import osc_aida_0
#

#
#
# It shows how to get the SoPlotter node
# of a region from an AIDA::IPlotter.
#
#

# NOTE : the cast_Slash_UI_IPlotter() method, used in this
#        example, uses the dynamic_cast (because there is no
#        "safe cast" method on the AIDA::IPlotter of
#        AIDA/v3r2p1). But it appears that on Linux
#        the default Python options to dynamic load
#        a module involves a loss of the RTTI and then
#        the dynamic_cast does not work in these
#        conditions. Then for Linux we enforce the
#        below RTLD_NOW|RTLD_GLOBAL options.
#         In the below code, in case the cast_Slash_UI_IPlotter()
#        returns None, we use another way to get the SoRegion
#        node. (A way that uses the safe cast).

import sys
if sys.platform == 'linux2':
  import dl
  sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)

import AIDA

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
h = histogramFactory.createHistogram1D('h','Rand gauss',100,-5,5)
del histogramFactory

import random
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create()
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('aida_0')
region = plotter.currentRegion()
region.setTitle('The famous normal distribution')
region.style().xAxisStyle().setLabel("Random gauss")
region.style().yAxisStyle().setLabel("Entries")
region.plot(h)

#///////////////////////////////////////////
#/// Inventor / HEPVis : ///////////////////
#///////////////////////////////////////////
import CoinPython as Inventor
import HEPVis

ip = plotter.cast_Slash_UI_IPlotter()
if ip == None :
  print 'NOTE : dynamic_cast of AIDA::IPlotter to Slash::UI::IPlotter failed.'
  # get the SoRegion from another way.
  ui = aida.ui()
  soPage = ui.findSoPage('plotter_0')
  soRegion = soPage.currentRegion()
else:
  soNode = ip.nodeOfInterest('region')
  soRegion = HEPVis.cast_SoRegion(soNode)

soPlotterRegion = soRegion.cast_SoPlotterRegion()

soPlotter = soPlotterRegion.getPlotter()

soPlotter.getWallStyle().visible.setValue(Inventor.FALSE)
soPlotter.getGridStyle().visible.setValue(Inventor.FALSE)

#///////////////////////////////////////////
#///////////////////////////////////////////
#///////////////////////////////////////////

plotter.show()
plotter.interact()
del region
del plotter

del h
del tree
del aida
