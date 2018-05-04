#
# Example of plotting by using :
#   inlib::histo, HEPVis/[SoPage, SoPlotterRegion, SoPlotter],
#   CoinPython and CoinXt.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import plotter
#

#//////////////////////////////////////
#/// create and fill an histogram : ///
#//////////////////////////////////////
import inlib

h = inlib.histo_h1d('Rand gauss',100,-5,5)

import random
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)

#print h.entries(),h.mean(),h.rms()

#/////////////////////////////////////////
#/// create the Inventor scene graph : ///
#/////////////////////////////////////////
import CoinPython as Inventor
Inventor.SoDB.init()
Inventor.SoNodeKit.init()
Inventor.SoInteraction.init()

import HEPVis
HEPVis.initClasses()

sg = Inventor.SoSeparator()

soPage = HEPVis.SoPage()
sg.addChild(soPage)

soPage.createRegions('SoPlotterRegion',1,1,0)
soPage.titleVisible.setValue(Inventor.FALSE)

soRegion = soPage.currentRegion()
soPlotterRegion = soRegion.cast_SoPlotterRegion()
soPlotterRegion.setStyleROOT_Default()
soPlotter = soPlotterRegion.getPlotter()

po = HEPVis.h1d2plot('h',h)
soPlotter.addPlottableObject(po) #WARNING : soPlotter takes C++ po ownership.
po.thisown = 0

#/////////////////////////////////////////
#/// create the GUI : ////////////////////
#/////////////////////////////////////////
from CoinXt import SoXt as SoGUI
#from CoinXt import SoXtExaminerViewer as ExaminerViewer
from CoinXt import SoXtPlaneViewer as PlaneViewer

top = SoGUI.init('')

#viewer = ExaminerViewer(top)
viewer = PlaneViewer(top)
viewer.setSceneGraph(sg)
    
viewer.setBackgroundColor(Inventor.SbColor(0.4,0.4,0.4))
viewer.viewAll()
#viewer.setSize(Inventor.SbVec2s(700,500))
#viewer.setTitle('CoinXt')
#viewer.setViewing(Inventor.FALSE)
viewer.setDecoration(Inventor.FALSE)
#viewer.setAutoClipping(Inventor.FALSE)
viewer.show()

SoGUI.show(top)

SoGUI.mainLoop()

#/////////////////////////////////////////
#/// end /////////////////////////////////
#/////////////////////////////////////////
soPlotter.clear()

del po
del r
del h
del viewer
del top
