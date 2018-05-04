#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_17
#

#
# Visualize the compiled function rmason_fun.cpp
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
codelet = 'compiled:'+osc_home_dir+'/Resources/AIDA/examples/data/rmason_func.cpp'
func = functionFactory.createFunctionByName('f',codelet)
params = [1,16000,1000,1,20000,2*0.2,0,0.00001]
func.setParameters(params);
func.annotation().addItem("domain.xAxisMinimum","0");
func.annotation().addItem("domain.xAxisMaximum","30000");
func.annotation().addItem("domain.xAxisNumberOfSteps","100000");

#////////////////////////////////////////////////////////
#////////////////////////////////////////////////////////

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
