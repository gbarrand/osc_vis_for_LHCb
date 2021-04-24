import AIDA
import random

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()

plotterFactory = aida.createPlotterFactory()
histogramFactory = aida.createHistogramFactory(memoryTree)
functionFactory = aida.createFunctionFactory(memoryTree)

# Get a plotter (pick up one in the GUI) :
plotter = plotterFactory.create('')

# 2x2 regions, set current region to the first one (index 0) :
plotter.createRegions(2,2,0)

# Set page title :
plotter.setTitle('Analysis Rio example')

# Cleanup memoryTree :
memoryTree.rmdir('/')

# Get an AIDA tree in a ROOT file :
import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
fileName = osc_home_dir+'/Resources/AIDA/examples/data/analysis.root'
print(fileName)

# If already loaded close (not delete) the file :
#FIXME : session.destroyManager(fileName)

# 1 = read only
# 0 = create new false
rioTree = treeFactory.create(fileName,'ROOT',1,0)
rioTree.ls()

#/////////////////////////////////////////////////////////////////////////////
# First region :
#  A XV plot with two histograms.  
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
region = plotter.currentRegion()
rioTree.cd('histograms')
rioTree.ls()

#  Get some histograms.
# From an AIDA tree we retreive IManagedObjects, we have to cast.
# cast_xxx is a OnXLab SWIG extension.
BW = rioTree.find('BW').cast_IHistogram1D()
if BW == None:
 print("BW not found or is not an histogram.")
else :
 region.plot(BW)

gauss = rioTree.find('gauss').cast_IHistogram1D()
if gauss == None:
 print("gauss not found or is not an histogram.")
else :
 # Plot histos in the same region :
 region.plot(gauss)


# Set plotting parameters for this region :
region.style().yAxisStyle().tickLabelStyle().setColor("red")
region.style().xAxisStyle().tickLabelStyle().setColor("red")
region.style().xAxisStyle().tickLabelStyle().setFont("TTF/couri")

#FIXME : Have to do the below by using the AIDA styles :
region.setParameter('plotter.binsStyle[0].color','1 0 0')
region.setParameter('plotter.binsStyle[0].modeling','wire_boxes')
region.setParameter('plotter.binsStyle[1].color','0 1 0')
region.setParameter('plotter.binsStyle[1].modeling','boxes')

region.setParameter('infosRegionVisible','TRUE')

del BW
del gauss
#/////////////////////////////////////////////////////////////////////////////
# Second region :
#  A XY plot.  
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Set current region to the adjacent one :
region = plotter.next()
gauss_BW = rioTree.find('gauss_BW').cast_IHistogram2D()
if gauss_BW == None:
 print("gauss_BW not found or is not an histogram.")
else :
 region.plot(gauss_BW)

region.setParameter('infosRegionVisible','FALSE')
region.setParameter('plotter.binsStyle[0].modeling','solid')
region.setParameter('plotter.binsStyle[0].painting','BY_VALUE')
region.setParameter('plotter.binsStyle[0].colorMapping','black 1 cyan 2 green 4 orange')

del gauss_BW
#/////////////////////////////////////////////////////////////////////////////
# Third region :
#  Get a tuple, create an histo of one of its column, plot it. 
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
region = plotter.next()
rioTree.cd('..')
rioTree.cd('tuples')
rioTree.ls()

rioTupleFactory = aida.createTupleFactory(rioTree)
# Get a tuple in the rioTree :
tuple1 = rioTupleFactory.create('tuple1','Rio tuple','')

# Create an histo in the default memory tree :
t1_v2 = histogramFactory.createHistogram1D('t1_v2','tuple1/v2',10,0,10)

# Create an Evaluator. It permits to define
# an operation over row entries. Here we simply get the 'v2'
# entry of the row :
evaluator = rioTupleFactory.createEvaluator('v2')

# Create a Filter. A filter permits to define
# a cut to accept or reject the row. Here nothing
# is done; all rows are accepted.
filter = rioTupleFactory.createFilter('')

# Project the tuple on the histogram according evaluator and filter.
# For each row, the tuple uses the Filter to reject or accept
# the row. If accpeted, the Evaluator is used on the row 
# to compute the value that is used to fill the histogram for this row.
tuple1.project(t1_v2,evaluator,filter)

# Plot : 
region.setParameter('infosRegionVisible','FALSE')
region.plot(t1_v2)

del evaluator
del filter
del tuple1
del t1_v2

# Scripted evaluators and filters can accept script like C statements :
#   evaluator : A + 2 * cos(B). 
#   filter : (fabs(A)<=2)&&(B>=1)
# String that are not common C function (cos, sin, etc...) are assumed
# to be tuple column labels.

#/////////////////////////////////////////////////////////////////////////////
# Fourth region :
#  Get a tuple, create an histo of its column, plot it. 
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
region = plotter.next()
rioTree.cd('..') 
rioTree.cd('tuples') 

tuple2 = rioTupleFactory.create('tuple2','','')

t2_gauss = histogramFactory.createHistogram1D('t2_gauss','tuple2/rand_gauss',100,-5,5)

evaluator = rioTupleFactory.createEvaluator('rand_gauss')
filter = rioTupleFactory.createFilter('(fabs(rand_gauss)<=2)&&(step>=1)')
#filter = riotTupleFactory.createFilter('')
tuple2.project(t2_gauss,evaluator,filter)

region.plot(t2_gauss)

del evaluator
del filter
del tuple2

#/////////////////////////////////////////////////////////////////////////////
# Fitting :
#/////////////////////////////////////////////////////////////////////////////
fitFactory = aida.createFitFactory()
fitter = fitFactory.createFitter('','','')
fitResult = fitter.fit(t2_gauss,'G')
fitGauss = fitResult.fittedFunction()
region.plot(fitGauss)
del fitGauss
del t2_gauss
del fitResult
del fitter
del fitFactory
#/////////////////////////////////////////////////////////////////////////////

plotter.show()
plotter.interact()

del rioTupleFactory

del rioTree
del plotter
del region
del plotterFactory
del treeFactory
del histogramFactory
del functionFactory
del aida
