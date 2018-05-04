import AIDA

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()
del treeFactory
plotterFactory = aida.createPlotterFactory()
functionFactory = aida.createFunctionFactory(memoryTree)

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)
plotter.setTitle('Function 2D')

import math

function = functionFactory.createFunctionFromScript('function2D',2,"(sin(x)/x)*(sin(y)/y)","","","");
#function.setTitle(,'sin(x)/x) * (sin(y)/y)')

# Since the function is not defined at 0, to avoid problem
# at plotting, we tilt a bit the visualization grid domain. 
function.annotation().addItem('domain.xAxisMinimum','-10');
function.annotation().addItem('domain.xAxisMaximum','9.999');
function.annotation().addItem('domain.yAxisMinimum','-10');
function.annotation().addItem('domain.yAxisMaximum','9.999');

region = plotter.currentRegion()
region.plot(function)

region.setParameter('plotter.xAxisAutomated','FALSE')
region.setParameter('plotter.xAxisMinimum','-10')
region.setParameter('plotter.xAxisMaximum','10')

region.setParameter('plotter.yAxisAutomated','FALSE')
region.setParameter('plotter.yAxisMinimum','-10')
region.setParameter('plotter.yAxisMaximum','10')

region.setParameter('plotter.functionStyle[0].color','0 1 0')
region.setParameter('plotter.functionStyle[0].painting','BY_VALUE')
region.setParameter('plotter.functionStyle[0].multiNodeLimit','1000000')
region.setParameter('plotter.functionStyle[0].colorMapping','black -0.2 cyan 0. green 0.1 orange 0.4 red')

plotter.show()
plotter.interact()

del region
del plotter
del function
del functionFactory
del plotterFactory
del aida
