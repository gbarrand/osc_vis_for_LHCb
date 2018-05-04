#
#  Example to show how to steer the GUI 
# but get the hand on the Python prompt.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import prompt
#

import Slash
session = Slash.create_session()
ui = session.createUI('prompt.onx')

Slash.set_PyOS_InputHook(ui)

#ui.steer() # this is no more needed.

# Since the upper permit to return to the python
# shell then we must not delete session (else the
# GUI will be deleted !).
