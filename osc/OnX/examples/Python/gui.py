#
# Construct a (simple) GUI without a .onx file.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import gui
#

import Slash
session = Slash.create_session()
ui = session.createUI() # create an empty GUI.

ui.createComponent('window','Window','')
ui.setCallback('window','destroy','DLD','OnX ui_exit')

ui.createComponent('menu_bar','MenuBar','window')

ui.createComponent('casc','CascadeButton','menu_bar')
ui.setParameter('casc.label','File')

ui.createComponent('menu','Menu','casc')
ui.createComponent('exit','MenuItem','menu')
ui.setParameter('exit.label','Exit')
ui.setCallback('exit','activate','DLD','OnX ui_exit')

ui.createComponent('viewer','SoExaminerViewer','window')

soViewer = ui.findSoViewer('viewer')

soViewer.readScene('bird.iv','guessed','')

ui.show()
ui.steer()

del session

