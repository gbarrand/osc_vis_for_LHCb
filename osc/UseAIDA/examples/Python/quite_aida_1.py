import AIDA
import random

aida = AIDA.createAnalysisFactory()

# Factories :
plotterFactory = aida.createPlotterFactory()
treeFactory = aida.createTreeFactory()

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)

plotter.setTitle('Store an histo')

#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////
print 'Create a Rio tree associated to a store.root file'

# 0 = read only false
# 1 = create new
rioTree = treeFactory.create('store.root','ROOT',0,1)
rioTree.mkdir('histograms')
rioTree.cd('histograms')

# Create an histogram in the Rio tree :
histogramFactory = aida.createHistogramFactory(rioTree)
h = histogramFactory.createHistogram1D('h','Rand gauss',100,-5,5)
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

rioTree.commit()
rioTree.ls()

# Destroy the tree :
del rioTree
del h

#/////////////////////////////////////////////////////////////////////////////
# Reopen the file read mode, get the histo and plot :
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
print 'Reopen the store.root file...'

# 1 = read only
# 0 = create new false
rioTree = treeFactory.create('store.root','ROOT',1,0)
rioTree.cd('histograms')
rioTree.ls()

# cast_xxx is a OnXLab SWIG extension.
h = rioTree.find('h').cast_IHistogram1D()
assert h
plotter.currentRegion().plot(h)

plotter.show()
plotter.interact()

del h

del rioTree

del plotter

del plotterFactory
del treeFactory
del histogramFactory

del aida
