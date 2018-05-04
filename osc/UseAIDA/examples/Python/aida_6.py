#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_6
#

#
# Plotters.
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

hbw = histogramFactory.createHistogram1D('h','Rand Breit Wigner',100,-5,5)
del histogramFactory
for I in range(0,10000):
  hbw.fill(rbw(0,1),1)

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(2,1,0)
plotter.setTitle('Two plotters')
plotter.currentRegion().plot(h)
plotter.next().plot(h)
# paper output :
plotter.show()

print 'After exiting the GUI, ps and gif files are going to be produced.'
plotter.interact()

plotter.writeToFile('aida_6.ps')
plotter.writeToFile('aida_6.gif')

del plotter

del hbw
del h
del r
del tree
del aida
