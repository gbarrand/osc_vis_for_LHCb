
#
# To compare the plotting with examples/ROOT/histos.C
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_histos
#

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import AIDA

import inlib # for inlib::random
rg = inlib.gauss()
rbw = inlib.bw()

aida = AIDA.createAnalysisFactory()

#  TFile* file = new TFile("histos.root","recreate");
tree = aida.find_memoryTree()

h1D = aida.create_histogram1D(tree,'h1D','Histogram 1D',100,-5,5)
for I in range(0,10000):
  h1D.fill(rg.shoot(),1)

h2D = aida.create_histogram2D(tree,'h2D','Histogram 2D',100,-5,5,100,-4,4)
for I in range(0,10000):
  h2D.fill(rg.shoot(),rbw.shoot(),1)

p1D = aida.create_profile1D(tree,'p1D','Profile 1D',100,-5,5)
for I in range(0,10000):
  p1D.fill(rg.shoot(),rbw.shoot(),1)

p2D = aida.create_profile2D(tree,'p2D','Profile 2D',100,-5,5,100,-3,3)
for I in range(0,10000):
  p2D.fill(rg.shoot(),rbw.shoot(),rbw.shoot(),1)

opts = 'overall_style=ROOT_Default'

p = aida.create_plotter()
p.createRegions(2,2,0)
region = p.region(0)
region.plot(h1D,opts)
region.setParameter('infosRegionVisible','TRUE')

region = p.region(1)
region.plot(h2D,opts)
#region.setParameter('plotter.binsStyle[0].modeling','points')
region.setParameter('plotter.binsStyle[0].modeling','boxes')
region.setParameter('infosRegionVisible','TRUE')

region = p.region(2)
region.plot(p1D,opts)
region.setParameter('plotter.binsStyle[0].modeling','points')
#region.setParameter('plotter.binsStyle[0].color','1 0 0')
#region.setParameter('plotter.binsStyle[0].markerStyle','CIRCLE_FILLED')
#region.setParameter('plotter.binsStyle[0].markerSize','2')
region.setParameter('plotter.errorsStyle[0].visible','TRUE')
#region.setParameter('plotter.errorsStyle[0].color','1 0 0')
region.setParameter('infosRegionVisible','TRUE')

region = p.region(3)
region.plot(p2D,opts)
#region.setParameter('plotter.binsStyle[0].modeling','points')
region.setParameter('plotter.binsStyle[0].modeling','boxes')
region.setParameter('infosRegionVisible','TRUE')

p.show()
p.interact()

del rg,rbw

del p
del aida
