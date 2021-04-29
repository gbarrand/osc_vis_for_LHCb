#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_11
#

#
# Rio tuple.
#

import AIDA

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()

# 0 = read only false
# 1 = create new
rioTree = treeFactory.create('tmp.root','ROOT',False,True)
rioTupleFactory = aida.createTupleFactory(rioTree)
tuple = rioTupleFactory.create('tuple','tuple','float rand_gauss,float rand_flat')
evaluator = rioTupleFactory.createEvaluator('rand_gauss')
filter = rioTupleFactory.createFilter('')
del rioTupleFactory

import random
r = random.Random()
for I in range(0,10000):
  tuple.fill(0,r.gauss(0,1))
  tuple.fill(1,r.random())
  tuple.addRow()

histogramFactory = aida.createHistogramFactory(memoryTree)
h = histogramFactory.createHistogram1D('h','tuple/rand_gauss',100,-4,4)
del histogramFactory

tuple.project(h,evaluator,filter)
del evaluator
del filter

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('Rio tuple example')
plotter.currentRegion().plot(h)
plotter.show()
plotter.interact()
del plotter

del r
del h
del tuple
del rioTree
del memoryTree

del treeFactory

del aida
