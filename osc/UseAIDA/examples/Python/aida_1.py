#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_1
#

import AIDA

def Mandel(aX,aY):
  # From pawex10.kumac mandel.f.
  NMAX = 30
  X = aX
  Y = aY
  XX = 0.
  YY = 0.
  for N in range(1,NMAX+1):
    TT = XX*XX-YY*YY+X
    YY = 2*XX*YY+Y
    XX = TT
    if (4<(XX*XX+YY*YY)): break
  return N/NMAX

aida = AIDA.createAnalysisFactory()

tf = aida.createTreeFactory()
tree = tf.create()
del tf

histogramFactory = aida.createHistogramFactory(tree)
mandel = histogramFactory.createHistogram2D('mandel','Mandel',100,-2.4,0.8,100,-1.2,1.2)
del histogramFactory

for i in range(0,100):
  x = mandel.xAxis().binLowerEdge(i)+mandel.xAxis().binWidth(i)/2
  for j in range(0,100):
    y = mandel.yAxis().binLowerEdge(j)+mandel.yAxis().binWidth(j)/2
    mandel.fill(x,y,Mandel(x,y))

# Get current plotter :
plotterFactory = aida.createPlotterFactory()
plotter = plotterFactory.create('')
del plotterFactory
plotter.createRegions(1,1,0)
plotter.setTitle('Mandel')
region = plotter.currentRegion()

#FIXME : do the below with styles.
region.setParameter('plotter.binsStyle[0].modeling','wire_boxes') #FIXME
region.plot(mandel)
plotter.show()
plotter.interact()
del region
del plotter

del mandel
del tree
del aida
