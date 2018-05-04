#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_7
#

#
# Log axes.
#

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

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('Histo & log')
region = plotter.currentRegion()
#FIXME : do the below with styles :
region.setParameter('plotter.xAxisLogScale','TRUE') #FIXME
region.setParameter('plotter.yAxisLogScale','TRUE') #FIXME
region.plot(h)
plotter.show()
plotter.interact()
del plotter

del h
del r

del tree
del aida
