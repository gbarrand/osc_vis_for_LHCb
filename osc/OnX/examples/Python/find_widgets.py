#
# Dump the name of the PageViewer widgets in the GUI.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import find_widgets
#

import Slash
session = Slash.create_session()
ui = session.createUI('find_widgets.onx')

widgets = ui.findChildren('session','nameFromClass','PageViewer')
for w in widgets : print(w)

ui.steer()

del session

