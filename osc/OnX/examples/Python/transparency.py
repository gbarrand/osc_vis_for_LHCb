#
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import transparency
#

import Slash

import sys
args = sys.argv
#args.append('-verbose')  
session = Slash.create_session(args)

ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.setTitleVisible(0)
soPage.title.setValue('transparency')

soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0,0,0))
soSep = soRegion.cast_SoDisplayRegion().getStaticScene()

soSep_1 = Inventor.SoSeparator()
soSep.addChild(soSep_1)
soMaterial = Inventor.SoMaterial()
soMaterial.diffuseColor.setValue(1,1,0)
soSep_1.addChild(soMaterial)

soCube = Inventor.SoCube()
soCube.width.setValue(4)
soCube.height.setValue(4)
soSep_1.addChild(soCube)

soSep_2 = Inventor.SoSeparator()
soSep.addChild(soSep_2)

soTranslation = Inventor.SoTranslation()
soTranslation.translation.setValue(Inventor.SbVec3f(0,0,10))
soSep_2.addChild(soTranslation)

soMaterial = Inventor.SoMaterial()
soMaterial.diffuseColor.setValue(1,0,0)
soMaterial.transparency.setValue(0.5)
soSep_2.addChild(soMaterial)

soCube = Inventor.SoCube()
#soCube.depth.setValue(4)
soSep_2.addChild(soCube)

w = ui.findWidget('Viewer')
soViewer = w.cast_ISoViewer()
#soViewer.setDecoration(0)
#soViewer.setViewing(1)

# From coin3d/SoGLRenderAction.h
#    SCREEN_DOOR,
#    ADD, DELAYED_ADD, SORTED_OBJECT_ADD,
#    BLEND, DELAYED_BLEND, SORTED_OBJECT_BLEND,
#    // The remaining are Coin extensions to the common Inventor API
#    SORTED_OBJECT_SORTED_TRIANGLE_ADD,
#    SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
#    NONE, SORTED_LAYERS_BLEND
soViewer.setTransparencyType(Inventor.SoGLRenderAction.BLEND)

#action = soViewer.getGLRenderAction()
#action.setTransparencyType(Inventor.SoGLRenderAction.BLEND)

del soCube
del soMaterial
del soTranslation
del soSep_1
del soSep_2
del soSep

del soViewer
del soRegion
del soPage

ui.steer()

del session

