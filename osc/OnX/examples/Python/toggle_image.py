#
# Example to play with pixmap on a toggle and a push button.
# Clicking on each button changes their picture (it toggles
# between logo_LAL.xpm and logo_LHCb.xpm).
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import toggle_image
#

def toggle_callback():
  import Slash
  ui = Slash.session().ui()
  value = ui.parameterValue('toggle.set')
  #print(value)
  if value == 'true':
    ui.setParameter('toggle.pixmap','logo_LHCb.xpm')
  else:
    ui.setParameter('toggle.pixmap','logo_LAL.xpm')

def button_callback():
  import Slash
  session = Slash.session()
  ui = session.ui()
  value = session.parameterValue('button.related_state')
  #print(value)
  if value == 'off':
    ui.setParameter('button.pixmap','logo_LHCb.xpm')
    session.setParameter('button.related_state','on')
  else:
    ui.setParameter('button.pixmap','logo_LAL.xpm')
    session.setParameter('button.related_state','off')

import Slash
session = Slash.create_session()
ui = session.createUI('toggle_image.onx')

ui.steer()

del session

