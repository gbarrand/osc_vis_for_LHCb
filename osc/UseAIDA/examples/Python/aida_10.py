#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_10
#

# same as aida_0 but with ROOT plotting style.

import AIDA
import random

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
h = histogramFactory.createHistogram1D('h','Rand gauss',100,-5,5)
del histogramFactory
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('')
region = plotter.currentRegion()
region.setTitle('The famous normal distribution')
region.style().xAxisStyle().setLabel("Random gauss")
region.style().yAxisStyle().setLabel("Entries")
region.plot(h,"overall_style=ROOT_Default")

plotter.show()
plotter.interact()
del region
del plotter

del h
del tree
del aida
