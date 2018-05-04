#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_16
#


#
# Visualize the compiled function mandel_fun.c
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
codelet = 'compiled:cmandel;'+osc_home_dir+'/Resources/AIDA/examples/data/mandel_func.c'
func = functionFactory.createFunctionByName('f',codelet)
func.annotation().addItem("plotter.color","blue");
#func.annotation().addItem("plotter.modeling","filled_curve");

if func != None:
  plotterFactory = aida.createPlotterFactory()
  if plotterFactory != None:
    plotter = plotterFactory.create('')
    plotter.createRegions(1,1,0)
    plotter.setTitle('Compiled function')
    region = plotter.currentRegion()
    region.setParameter("plotter.wallStyle.visible","FALSE");
    #region.setParameter("plotter.shapeAutomated","FALSE");
    #region.setParameter("plotter.shape","XY");
    region.plot(func)
    plotter.show()
    plotter.interact()
    del plotter
    del plotterFactory
  del func

del functionFactory
del tree
del aida
