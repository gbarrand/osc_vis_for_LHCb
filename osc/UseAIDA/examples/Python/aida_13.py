#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_13
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

print 'mean %g, sigma %g' % (h.mean(),h.rms())

del h
del tree
del aida
