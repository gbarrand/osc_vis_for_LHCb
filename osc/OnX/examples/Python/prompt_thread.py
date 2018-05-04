#
#  Example to show how to steer the GUI 
# by using a thread.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import prompt_thread
#

import Slash
session = Slash.create_session()
ui = session.createUI('prompt.onx')

def ui_steer(aUI):
  # never exit the below loop.
  import time
  while 1:
    aUI.synchronize()
    time.sleep( 0.01 )

import thread
thread.start_new_thread(ui_steer,(ui,)) # non blocking.

