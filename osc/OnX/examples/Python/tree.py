#
# Put something in a UI tree widget.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import tree
#

import Slash
session = Slash.create_session()
ui = session.createUI('tree.onx')

#ui.setParameter('tree.items','<tree><treeItem><label>Item 1</label></treeItem><treeItem><label>Item 2</label></treeItem></tree>')

ui.setTreeItems('tree','tree.xml')

ui.steer()

del session
