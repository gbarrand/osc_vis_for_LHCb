#
# Fitting.
#


#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_5
#


#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import random
import math

M_PI_2 = math.pi / 2.0
r = random.Random()

def rbw(aMean,aGamma):
  rval = 2.0 * r.random() - 1.0
  displ = 0.5 * aGamma * math.tan(rval * M_PI_2)
  return aMean + displ

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import AIDA

import random

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

plotterFactory = aida.createPlotterFactory()
histogramFactory = aida.createHistogramFactory(tree)
fitFactory = aida.createFitFactory()
functionFactory = aida.createFunctionFactory(tree)

fitter = fitFactory.createFitter('','','')

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(2,2,0)

plotter.setTitle('Test fit')

#///////// Gauss ///////////////////////////////////
hgauss = histogramFactory.createHistogram1D('hgauss','Rand gauss',100,-5,5)

r = random.Random()
for I in range(0,10000):
  hgauss.fill(r.gauss(0,1),1)

fitResult = fitter.fit(hgauss,'G')
fit = fitResult.fittedFunction()

region = plotter.currentRegion()
region.setTitle('Gaussian fit.')
region.plot(hgauss)
region.plot(fit)

#///////// Breit Wigner ///////////////////////////////////
region = plotter.next()
hbw = histogramFactory.createHistogram1D('hbw','Rand BW',100,-5,5)

for I in range(0,10000):
  hbw.fill(rbw(0,1),1)

fitResult = fitter.fit(hbw,'BW')
fit = fitResult.fittedFunction()

region.setTitle('Breit-Wigner fit.')
region.plot(hbw)
region.plot(fit)

del hbw

#///////// Exponential ///////////////////////////////////
region = plotter.next()


import math
def func(x):
  return 0.4 * math.exp(x)

hexp = histogramFactory.createHistogram1D('hexp','Exp',100,-5,5)

for I in range(0,100):
  x = I * 0.1 - 5 + 0.05
  hexp.fill(x,func(x))

fitResult = fitter.fit(hexp,'E')
fit = fitResult.fittedFunction()

region.setTitle('Exponential fit.')
region.plot(hexp)
region.plot(fit)

del hexp
#///////// Gauss + pol2 /////////////////////////////////
region = plotter.next()

hgauss_pol2 = histogramFactory.createHistogram1D('hgauss_pol2','Rand gauss',100,-5,5)
for I in range(0,10000):
  hgauss_pol2.fill(r.gauss(0,1),1)

#  Trickier fit. An expression is given using the usual 
# mathematical functions (cos, exp, ...) and using 
# the functions :
#   gauss(x,H,M,W)    :  H * exp(-0.5 * ((x - M)/W)**2 )
#   breit(x,H,M,W)    :  H/(1. + (2 * (x - M)/W)**2 )
#   expo(x,A,B)       : exp(A + B x)
#   pol1(x,A,B)       : A + B x
#   pol2(x,A,B,C)     : A + B x + C x**2 
#   pol3(x,A,B,C,D)   : A + B x + C x**2 + D x**3
#   pol4(x,A,B,C,D,E) : A + B x + C x**2 + D x**3 + E x**4 
#  These functions have parameters that are going to be 
# used as fitting parameters.
#  

func = functionFactory.createFunctionFromScript('scripted',1,'gauss(x,H,M,W)+pol2(x,A,B,C)','','','')
# Starting values :
func.setParameter("H",400)
func.setParameter("M",0)
func.setParameter("W",4)
func.setParameter("A",1)
func.setParameter("B",0)
func.setParameter("C",-1)

region.setTitle('Scripted fit.')
region.plot(hgauss_pol2)

fitResult = fitter.fit(hgauss_pol2,func) #FIXME : it fails !
if not fitResult:
  print('aida_5 : Gauss+pol2 : fit failed.')
else:
  fit = fitResult.fittedFunction()
  region.plot(fit)

del hgauss

plotter.show()
plotter.interact()

#////////////////////////////////////////////////////////
del plotter
del r

del func
del fitResult
del fit
del fitter
del functionFactory
del fitFactory
del plotterFactory
del histogramFactory

del tree
del aida
