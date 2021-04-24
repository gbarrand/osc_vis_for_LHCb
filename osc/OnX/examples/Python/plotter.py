#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import plotter
#

import Slash
session = Slash.create_session()
ui = session.createUI('plotter.onx')

import CoinPython as Inventor
import HEPVis

# create and fill an histogram :
import inlib

h = inlib.histo_h1d('Rand gauss',100,-5,5)

import random
r = random.Random()
for I in range(0,10000):
  h.fill(r.gauss(0,1),1)

#print(h.entries(),h.mean(),h.rms())

soPage = ui.findSoPage('plotter')
soRegion = soPage.currentRegion()
soPlotterRegion = soRegion.cast_SoPlotterRegion()
soPlotterRegion.setStyleROOT_Default()

soPlotter = soPlotterRegion.getPlotter()

po = HEPVis.h1d2plot('h',h)
soPlotter.addPlottableObject(po)

ui.steer()

del r
del h
del session
