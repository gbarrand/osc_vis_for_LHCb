#
# Manipulate data (here some "OnX_Shape") by using its
# data accessor "type".
#
# All the code is in the file :
#   OnX/examples/Callbacks/Shape.cxx
# and put in the OnXExas DLL.
#
# The "user data class", OnX::Shape, is manipulated
# by the "AccessorManager" system through its
# OnX::ShapeType accessor class.
#
# We load a shapes.onx having a page viewer
# but also a menu bar with a "File/Dump picked"
# to do some operation on the picked (highlighted)
# objects.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import shapes
#

def callback():
  import Slash
  session = Slash.session()
  da = session.accessorManager() # data accessor.  
  da.collect('SceneGraph(Viewer)','highlight==true')  
  da.filter('name')  
  hit = da.handlersIterator()  
  print('Picked objects :')
  while 1:
    h = hit.handler()
    if h == None:break
    accessor = h.type()
    obj = h.object(); # void*
    print('Accessor %s object 0x%lx' % (accessor.name(),obj))
    #print(obj)
    #s = OnX.smanip_p2s(obj)
    #print(s)
    hit.next()
  hit.thisown = 1
  del hit

import Slash
session = Slash.create_session()
ui = session.createUI('shapes.onx')

import CoinPython as Inventor
import HEPVis

ui.executeScript('DLD','OnXExas shape_initialize')

da = session.accessorManager() # data accessor.

soPage = ui.findSoPage('Viewer')
soPage.title.setValue('OnX/Shapes')
soPage.titleVisible.setValue(0)
soRegion = soPage.currentRegion()
soRegion.color.setValue(Inventor.SbColor(0.8,0.8,0.8))
del soRegion
del soPage

# same as the OnXExas_shape_visualize C++ callback.
# Then see comments about way to get style here.

where = 'Viewer'

da.collect('OnX_Shape','type=="Cone"')
session.setColor('cyan')
session.setParameter('modeling.modeling','solid')
session.setParameter('modeling.transparency','0')
da.visualize(where)

da.collect('OnX_Shape','type=="Cube"')
# should be in solid red.
da.visualize(where)

da.collect('OnX_Shape','(type=="Hole")&&(size<=1)')
# should be in wire frame blue.
da.visualize(where)

da.collect('OnX_Shape','(type=="Hole")&&(size>1)')
style = 'big_hole'
styleManager = session.styleManager();
#if(styleManager && !styleManager->isStyle(style)) {
styleManager.addStyle(style,'color yellow\nmodeling solid');
# then should be in yellow.
da.visualize(where,style);

da.collect('OnX_Shape','type=="Track*"')
# various line styles.
da.visualize(where);

da.collect('OnX_Shape','(type=="Sphere")&&(size<=2)')
# blue text.
da.visualize(where);

da.collect('OnX_Shape','(type=="Sphere")&&(size>2)')
# local style.
style = Slash.Style()
style.set('color','green');
style.set("modeling","solid");
# then big spheres should be in solid red.
da.visualize(style,where)

widget = ui.findWidget('Viewer')
if widget != None:
  soViewer = widget.cast_ISoViewer()
  if soViewer != None:
    soViewer.setFeedbackVisibility(1)
  
# so that the "Dump picked" works :
ui.executeScript('DLD','OnX OnX_declare_accessors')

ui.steer()
del session
