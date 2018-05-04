#
# The viewer is an Inventor SoExaminerViewer.
#
# There is no HEPVis related code in this example.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import so_examiner_viewer
#

def printInfoSelectionCB(sg,selectionPath):    
    #print void,selectionPath,selectionPath.getTail().getTypeId().getName()
    for i in xrange(selectionPath.getLength()-1,0,-1):
        #print selectionPath.getNode(i).getTypeId().getName()
        node = selectionPath.getNode(i)
        node.touch() # to redraw        
        if node.isOfType(Inventor.SoSeparator.getClassTypeId()):
            #print 'found SoSeparator '
            for j in xrange(node.getNumChildren()):
                #print node.getChild(j).getTypeId().getName()
                if node.getChild(j).isOfType(Inventor.SoInfo.getClassTypeId()):
                    print node.getChild(j).string.getValue()

import Slash
session = Slash.create_session()
ui = session.createUI('so_examiner_viewer.onx')

import CoinPython as Inventor

soViewer = ui.findSoViewer('Viewer')
#ra = Inventor.SoLineHighlightRenderAction()
ra = Inventor.SoBoxHighlightRenderAction()
soViewer.setGLRenderAction(ra)

soNode = soViewer.sceneGraph()
soSep = Slash.cast_SoSeparator(soNode) #FIXME
#FIXME :soSep = Inventor.cast(soNode,'SoSeparator')

topSelection = Inventor.SoSelection()
topSelection.policy = Inventor.SoSelection.SINGLE
topSelection.addSelectionCallback(printInfoSelectionCB,soNode)
soSep.addChild(topSelection)

cylSep = Inventor.SoSeparator()
topSelection.addChild(cylSep)

cylMaterial = Inventor.SoMaterial()
cylMaterial.diffuseColor.setValue(Inventor.SbVec3f(1,1,0))
cylSep.addChild(cylMaterial)

cylInfo = Inventor.SoInfo()
cylInfo.string = "This is a cylinder"
cylSep.addChild(cylInfo)

soCylinder = Inventor.SoCylinder()
soCylinder.radius.setValue(5)
cylSep.addChild(soCylinder)

sphSep =  Inventor.SoSeparator()
topSelection.addChild(sphSep)

sphTrans = Inventor.SoTranslation()
sphTrans.translation = Inventor.SbVec3f(10,0,0)
sphSep.addChild(sphTrans)

sphMaterial = Inventor.SoMaterial()
sphMaterial.diffuseColor.setValue(Inventor.SbVec3f(1,0,0))
sphSep.addChild(sphMaterial)

sphInfo = Inventor.SoInfo()
sphInfo.string = "This is a sphere"
sphSep.addChild(sphInfo)

soSphere = Inventor.SoSphere()
sphSep.addChild(soSphere)

del cylMaterial
del cylInfo
del soCylinder
del cylSep
del sphMaterial
del sphTrans
del sphInfo
del soSphere
del topSelection
del soSep

#soViewer.setDecoration(0)

soViewer.viewAll()

del soViewer

ui.steer()

del session
del ra
