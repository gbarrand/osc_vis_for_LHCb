
#
# Mag plotting.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_3
#

import AIDA

aida = AIDA.createAnalysisFactory()
tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
h = histogramFactory.createHistogram1D('h','Rand gauss',100,0,100000)
del histogramFactory

for I in range(0,100000):
  h.fill(0,1)

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('Mags')
region = plotter.currentRegion()
region.style().xAxisStyle().setLabel("x")
region.style().yAxisStyle().setLabel("val")
region.plot(h)
plotter.show()
plotter.interact()
del region
del plotter

del h
del tree
del aida
