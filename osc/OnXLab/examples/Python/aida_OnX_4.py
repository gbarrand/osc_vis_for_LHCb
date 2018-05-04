#
# This script demonstrates a periodic filling and plotting of an histogram.
#


import AIDA

aida = AIDA.createAnalysisFactory()

treeFactory = aida.createTreeFactory()
memoryTree = treeFactory.create()
del treeFactory

# Create some histogram.
# It will be regulary filled by the aida_OnX_4_cyclic script.

plotterFactory = aida.createPlotterFactory()
histogramFactory = aida.createHistogramFactory(memoryTree)

# Get current plotter :
plotter = plotterFactory.create('')

plotter.createRegions(1,1,0)
plotter.setTitle('Fill and update')

h_update = histogramFactory.createHistogram1D('gauss','Gauss',100,-5,5)
plotter.currentRegion().plot(h_update)

# Create a "Cyclic". It will execute all the 1000 msec
# the OnXLab_update_region script.

# WARNING : ICyclic is not AIDA

# The below cast will not work in the cyclic since
# aida is no more a OnXLab::Main but an OnXLab::AnalysisFactory.
#session = aida.cast_Slash_Core_ISession()

import Slash
session = Slash.session() # then get session with OnX.
ui = session.ui()

# Fill the histo :
def fill():
  import random
  r = random.Random()
  for I in range(1,1000):
    h_update.fill(r.gauss(0,1),1)
  del r

cyclic = ui.createCyclic('cyclic',1000,'Python','import aida_OnX_4;aida_OnX_4.fill()')

# A cyclic could be changed as in the below :
#   cyclic.setDelay(1000)
#   cyclic.setScript(1000,'Python','import aida_OnX_4;aida_OnX_4.fill()')

# Start the cyclic.
cyclic.start()

plotter.show()
plotter.interact()

del cyclic
del plotter
del plotterFactory
del histogramFactory
del aida
