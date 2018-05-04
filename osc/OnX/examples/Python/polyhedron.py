#
# Visualize some instances of SoPolyhedron. 
# Some are boolean operations of solids.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import polyhedron
#

import Slash
session = Slash.create_session()
ui = session.createUI('page_viewer.onx')

import CoinPython as Inventor
import HEPVis

soPage = ui.findSoPage('Viewer')

soPage.title.setValue('Python / Polyhedron')

soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.8,0.8,0.8))
soSep = soRegion.cast_SoDisplayRegion().getStaticScene()

soScale = Inventor.SoScale()
soScale.scaleFactor.setValue(Inventor.SbVec3f(0.1,0.1,0.1))
soSep.addChild(soScale)

# SoPolyhedron : a trd :
soSep_trd = Inventor.SoSeparator()
soSep.addChild(soSep_trd)

soMaterial = Inventor.SoMaterial()
soMaterial.diffuseColor.setValue(1,0,0)
soSep_trd.addChild(soMaterial)

soLigthModel = Inventor.SoLightModel()
soLigthModel.model.setValue(Inventor.SoLightModel.BASE_COLOR)
soSep_trd.addChild(soLigthModel)

trd2 = HEPVis.SbPolyhedronTrd2(10,0,10,0,10)
so_trd2 = HEPVis.SoPolyhedron(trd2)
so_trd2.solid.setValue(Inventor.FALSE)
so_trd2.reducedWireFrame.setValue(Inventor.TRUE)
soSep_trd.addChild(so_trd2)

# SoPolyhedron : boolean operation :
soSep_bool_op = Inventor.SoSeparator()
soSep.addChild(soSep_bool_op)

soLigthModel = Inventor.SoLightModel()
soLigthModel.model.setValue(Inventor.SoLightModel.PHONG)
soSep_trd.addChild(soLigthModel)

box_1 = HEPVis.SbPolyhedronBox(20,10,10)
bool_op = HEPVis.SbPolyhedron(box_1);
box_2 = HEPVis.SbPolyhedronBox(30,5,5)
box_2.Transform(HEPVis.SbRotation(Inventor.SbVec3d(0,1,0),1.57),Inventor.SbVec3d(0,0,0))
bool_op = bool_op.subtract(box_2);
bool_op.Transform(HEPVis.SbRotation(),Inventor.SbVec3d(40,0,0))
so_bool_op = HEPVis.SoPolyhedron(bool_op)
#so_bool_op.solid.setValue(Inventor.FALSE)
#so_bool_op.reducedWireFrame.setValue(Inventor.TRUE)
so_bool_op.solid.setValue(Inventor.TRUE)
soSep_bool_op.addChild(so_bool_op)

del soScale
del soSep
del soRegion
del soPage

ui.steer()

del session
