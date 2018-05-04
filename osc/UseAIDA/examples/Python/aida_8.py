#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_8
#

#
# ASCII file.
#

import AIDA

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()

import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
fileName = osc_home_dir+'/Resources/AIDA/examples/data/data.ascii'
asciiTree = treeFactory.create(fileName,'ASCII',1,0)
del treeFactory

tupleFactory = aida.createTupleFactory(asciiTree)
tuple = asciiTree.find('data')
if tuple == None: print 'data tuple not found.'
tuple = tuple.cast_ITuple()
if tuple == None: print 'data object not an ITuple.'

#coln = tuple.columns()
#print coln,tuple.rows()
#for coli in range(0,coln):
#  print tuple.columnName(coli)
#  print tuple.columnType(coli)

evaluator = tupleFactory.createEvaluator('v1')
filter = tupleFactory.createFilter('')
del tupleFactory

histogramFactory = aida.createHistogramFactory(memoryTree)
h = histogramFactory.createHistogram1D('h','tuple/v1',10,1,11)
del histogramFactory

tuple.project(h,evaluator,filter)

plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('Ascii tuple example')
plotter.currentRegion().plot(h)

plotter.show()
plotter.interact()

del plotter

del h
del evaluator
del filter
del tuple
del asciiTree

del memoryTree
del aida
