#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_15
#

#
# Visualize the compiled function cut_fun.c
#

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import AIDA

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

functionFactory = aida.createFunctionFactory(tree)

import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
codelet = 'compiled:'+osc_home_dir+'/Resources/AIDA/examples/data/cut_func.c'
func = functionFactory.createFunctionByName('f',codelet)

if func != None:
  plotterFactory = aida.createPlotterFactory()
  if plotterFactory != None:
    plotter = plotterFactory.create('')
    plotter.createRegions(1,1,0)
    plotter.setTitle('Compiled function')
    region = plotter.currentRegion()
    region.plot(func)
    plotter.show()
    plotter.interact()
    del plotter
    del plotterFactory
  del func

del functionFactory
del tree
del aida
