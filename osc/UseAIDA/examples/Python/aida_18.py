#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_18
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

aida = AIDA.createAnalysisFactory()

tree = aida.find_memoryTree()

hgs = aida.create_histogram1D(tree,'hg2','Rand gauss',100,-5,5)
hbw = aida.create_histogram1D(tree,'hbw','Rand Breit Wigner',100,-5,5)

import random
r = random.Random()
for I in range(0,10000):
  hgs.fill(r.gauss(0,1),1)
  hbw.fill(rbw(0,1),1)
del r

plotter = aida.create_plotter()
plotter.createRegions(1,1,0)
plotter.setTitle('')
region = plotter.currentRegion()
region.setTitle('The famous normal distribution')
region.style().xAxisStyle().setLabel("Random gauss")
region.style().yAxisStyle().setLabel("Entries")
region.plot(hgs)
plotter.show()

plotter = aida.create_plotter()
plotter.createRegions(1,1,0)
plotter.setTitle('')
region = plotter.currentRegion()
region.style().xAxisStyle().setLabel("Random Breit Wigner")
region.style().yAxisStyle().setLabel("Entries")
region.setParameter('plotter.binsStyle[0].color','1 0 0')
region.setParameter('plotter.binsStyle[1].color','0 1 0')
region.setParameter('plotter.binsStyle[1].transparency','0.5')
region.plot(hbw)
region.plot(hgs)
plotter.show()

plotter.interact()

del region
del plotter

del hgs
del hbw
del tree
del aida
