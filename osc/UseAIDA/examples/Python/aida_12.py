#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_12
#

#
# Histogram operations.
#

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import random
import math

M_PI_2 = math.pi / 2.0
r = random.Random()

def rbw(aMean,aGamma):
  rval = 2.0 * r.random() - 1.0
  displ = 0.5 * aGamma * math.tan(rval * M_PI_2)
  return aMean + displ

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////

import AIDA

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(2,2,0)
plotter.setTitle('Histo arithmetic')

BW = histogramFactory.createHistogram1D('BW','Rand BW',100,-4,4)

for I in range(0,10000):
  BW.fill(rbw(0,1),1)

h_add = histogramFactory.add('h_add',BW,BW)
h_add.setTitle('BW+BW')
region = plotter.currentRegion()
#FIXME : do the below with styles :
region.setParameter('color','0.8 0.8 0.8') #FIXME
region.setParameter('viewportRegion.borderColor','0 1 0') #FIXME
region.setParameter('viewportRegion.verticalBorder','2') #FIXME
region.setParameter('viewportRegion.horizontalBorder','2') #FIXME
region.plot(h_add)

h_div = histogramFactory.divide('h_div',BW,BW)
h_div.setTitle('BW/BW')
region = plotter.next()
region.plot(h_div)

h_mul = histogramFactory.multiply('h_mul',BW,BW)
h_mul.setTitle('BW*BW')
region = plotter.next()
region.plot(h_mul)

region = plotter.next()
region.plot(BW)
region.plot(h_add)

region.setParameter('plotter.superposeBins','TRUE') #FIXME
region.setParameter('plotter.binsStyle[0].color','1 0 0') #FIXME
region.setParameter('plotter.binsStyle[0].modeling','boxes') #FIXME
region.setParameter('plotter.binsStyle[1].color','0 0 1') #FIXME
region.setParameter('plotter.binsStyle[1].modeling','boxes') #FIXME

plotter.show()
plotter.interact()

del BW
del h_add
del h_div
del h_mul

del plotter
del region
del histogramFactory

del tree
del aida
