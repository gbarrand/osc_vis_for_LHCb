import AIDA
import random

aida = AIDA.createAnalysisFactory()

# Factories :
plotterFactory = aida.createPlotterFactory()
treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()

# Get a plotter (pick up one in the GUI) :
plotter = plotterFactory.create('')

# 2x2 regions, set current region to the first one (index 0) :
plotter.createRegions(1,2,0)

# Set page title :
plotter.setTitle('ROOT TH*')

# Cleanup memoryTree :
memoryTree.rmdir('/')

# Get a ROOT file with TH* in it :
import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
fileName = osc_home_dir+'/Resources/AIDA/examples/data/RootW.root'

# If already loaded close (not delete) the file :
#FIXME : session.destroyManager(fileName)

# 1 = read only
# 0 = create new false
rioTree = treeFactory.create(fileName,'ROOT',1,0)
rioTree.ls()

#/////////////////////////////////////////////////////////////////////////////
region = plotter.currentRegion()

# Get a TH1F :
mainHistogram = rioTree.find('mainHistogram').cast_IHistogram1D()
if mainHistogram == None:
 ui.echo("OnXLab_ROOT : mainHistogram not found or is not an TH1F.")    
else :
 region.plot(mainHistogram)
del mainHistogram

#/////////////////////////////////////////////////////////////////////////////
region = plotter.next()

# Get a TH2F :
# cast_xxx is a OnXLab SWIG extension.
hpxpy = rioTree.find('hpxpy').cast_IHistogram2D()
if hpxpy == None:
 print("OnXLab_ROOT : hpxpy not found or is not an TH2F.")
else :
 region.plot(hpxpy)

plotter.show()
plotter.interact()

del hpxpy

del rioTree
del plotter
del region
del plotterFactory
del treeFactory
