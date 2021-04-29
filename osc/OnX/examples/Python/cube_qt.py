#
# Visualize a cube.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import cube
#

import Slash

import sys
args = sys.argv
#args.append('-verbose')  
session = Slash.create_session(args)

ui = session.createUI('page_viewer.onx',True,'Qt')
if ui == None :
  print('cue_qt.py : UI creation for Qt failed. OnX Qt driver built ?')
else :  
  #uimgr = session.uiMan()
  #ui = uimgr.create('','page_viewer.onx',[],1)

  import CoinPython as Inventor
  import HEPVis

  soPage = ui.findSoPage('Viewer')
  
  soPage.title.setValue('SoCube')
  
  soRegion = soPage.currentRegion()
  soRegion.color.setValue(Inventor.SbColor(0,0,0))
  soSep = soRegion.cast_SoDisplayRegion().getStaticScene()
  
  soCube = Inventor.SoCube()
  soCube.width.setValue(4)   # Then no more a cube !

  soSep.addChild(soCube)
  
  del soCube
  del soSep
  del soRegion
  del soPage

  ui.steer()

  del session

