#
# Histogram normalization.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_4
#

import AIDA
import random

aida = AIDA.createAnalysisFactory()
tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
h = histogramFactory.createHistogram1D('h','Rand gauss',100,-5,5)
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

# H1D_normalize defined in the OnXLab script.
h_normalized = histogramFactory.createCopy('h_normalized',h)
del histogramFactory

h_normalized.setTitle('Rand gauss normalized')
hsw = h.sumBinHeights()
h_normalized.scale(2/hsw)

print h_normalized.sumBinHeights()

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(2,1,0)
plotter.setTitle('Histo normalize')
plotter.currentRegion().plot(h)
plotter.next().plot(h_normalized)
plotter.show()
plotter.interact()
del plotter

del h_normalized
del h

del tree
del aida
