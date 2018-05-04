#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_14
#

import AIDA
import random

aida = AIDA.createAnalysisFactory()

plotterFactory = aida.createPlotterFactory()
treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()
del treeFactory
histogramFactory = aida.createHistogramFactory(memoryTree)

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)
plotter.setTitle('OnXLab')

edges = [-4,-3,-2.5,-2,-1.75,-1.5,-1.25,-1,-0.875,-0.75,-0.625,-0.5,-0.375,-0.25,-0.125,0,0.125,0.25,0.375,0.5,0.625,0.75,0.875,1,1.25,1.5,1.75,2,2.5,3,4]

h = histogramFactory.createHistogram1D('h','Rand gauss',edges,'')
r = random.Random()
for I in range(0,50000):
  h.fill(r.gauss(0,2),1)

region = plotter.currentRegion()
region.plot(h)
region.setParameter('plotter.binsStyle[0].color','0 0 0')
region.setParameter('plotter.binsStyle[0].modeling','wire_boxes')
region.setParameter('plotter.wallStyle.visible','FALSE')  

plotter.show()
plotter.interact()

del edges
del plotter
del h
del r
del plotterFactory
del histogramFactory
del memoryTree
del aida
