#
# Histogram filled with a gaussian distribution plus fit.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_2
#

import AIDA
import random

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
h = histogramFactory.createHistogram1D('h','The famous normal distribution.',100,-5,5)
del histogramFactory

r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

fitFactory = aida.createFitFactory()
fitter = fitFactory.createFitter('','','')
del fitFactory
fitResult = fitter.fit(h,'G')
del fitter
fitGauss = fitResult.fittedFunction()
del fitResult

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('fit')
region = plotter.currentRegion()
region.plot(h)
region.plot(fitGauss)
plotter.show()
plotter.interact()
del plotter
del region

del fitGauss
del h
del tree
del aida
