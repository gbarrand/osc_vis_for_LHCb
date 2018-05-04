#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_8
#

#
# Cloud3D.
#

import AIDA

import random

aida = AIDA.createAnalysisFactory()
tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)

# Data :
cloud = histogramFactory.createCloud3D("cloud3D","cloud3D")
r = random.Random()
for index in range(0,10000):
 cloud.fill(r.gauss(0,1),r.gauss(0,1),r.gauss(0,1),1)

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,2,0)
plotter.setTitle('Cloud 3D')

region = plotter.currentRegion()

#FIXME : do the below with styles :
region.setParameter("plotter.wallStyle.visible","FALSE") #FIXME

region.setParameter('plotter.xAxisAutomated','FALSE') #FIXME
region.setParameter('plotter.xAxisMinimum','-5') #FIXME
region.setParameter('plotter.xAxisMaximum','5') #FIXME

region.setParameter('plotter.yAxisAutomated','FALSE') #FIXME
region.setParameter('plotter.yAxisMinimum','-5') #FIXME
region.setParameter('plotter.yAxisMaximum','5') #FIXME

region.setParameter('plotter.pointsStyle[0].color','0 1 0') #FIXME

region.setParameter("plotter.shape","XYZ") #FIXME

region.plot(cloud)

plotter.next()

# Convert to an histogram :
cloud.convert(50,-10,10,50,-10,10,50,-10,10)
histogram = cloud.histogram()
# Get a 2D slice :
sliceXY = histogramFactory.projectionXY("slice_XY",histogram)
del histogramFactory

plotter.currentRegion().plot(sliceXY)

plotter.show()
plotter.interact()

del r
del sliceXY
del histogram
del cloud
del plotter

del tree
del aida
