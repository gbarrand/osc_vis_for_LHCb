import AIDA

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()
del treeFactory
plotterFactory = aida.createPlotterFactory()
histogramFactory = aida.createHistogramFactory(memoryTree)

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)
plotter.setTitle('Histo & data_collect')

h_OnX_Shape_size = histogramFactory.createHistogram1D('h_OnX_Shape_size','OnX_Shape/size',10,0,10)

# aida is a "SWIG reduced OnXLab::Main". See OnXLab/source/OnXLab_SWIG.i.
session = aida.cast_Slash_Core_ISession()

ui = session.ui()
da = session.accessorManager() # data accessor.

ui.executeScript("DLD","OnXExas OnXExas_shape_initialize")
da.collect('OnX_Shape','type=="Cube"')
da.number()

# OnXLab specific :
def da_fill1D(aDA,aHistogram,aExpressionX):
  ss = []
  ss.append("tree=MemoryTree")
  ss.append("path="+aHistogram)
  ss.append("x="+aExpressionX)
  ss.append("create=no")
  aDA.execute("histogram",ss)

# Fill histos with collected things :
da_fill1D(da,'h_OnX_Shape_size','size')

plotter.currentRegion().plot(h_OnX_Shape_size)

plotter.show()
plotter.interact()

del plotter

del h_OnX_Shape_size
del plotterFactory
del histogramFactory
del aida

