#
# A simple Python AIDA example to create, fill and plot an histogram.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_0
#

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

# create a plotter :
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
plotter.show()
plotter.interact()
del region
del plotter

del h
del tree
del aida
