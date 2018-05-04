#
# Do something on highlighted objects.
#
# This script is used by the shapes.py example.
# (See shapes.onx).
#
# This script have to be executed on a OnX Python callback :
# for example on a MenuItem with :
#   <activate exec="Python">import OnX;OnX.x('picked_cbk')</activate>
#

import OnX

session = OnX.session()

da = session.accessorManager() # data accessor.

da.collect('SceneGraph(Viewer)','highlight==true')

da.filter('name')

hit = da.handlersIterator()

print 'Picked objects :'

while 1:
  h = hit.handler()
  if h == None:break
  accessor = h.type()
  obj = h.object(); # void*
  print 'Accessor %s object 0x%lx' % (accessor.name(),obj)
  #print obj
  #s = OnX.smanip_p2s(obj)
  #print s

  hit.next()

hit.thisown = 1
del hit
