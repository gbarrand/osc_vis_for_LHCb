#
# This example loads the menu.onx GUI having
# a menu bar. The "File/Create menu" menu item 
# executes the callback() that creates a 
# pulldown in the menu bar.
#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import menu
#

def callback():
  import Slash
  ui = Slash.session().ui()
  # Create a new menu in the default menu bar :
  ui.createComponent('onx_exa_casc','CascadeButton','menu_bar')
  ui.setParameter('onx_exa_casc.label','MyMenu')
  ui.createComponent('onx_exa_menu','Menu','onx_exa_casc')  
  # Create a first item in the menu :
  ui.createComponent('onx_exa_item_1','MenuItem','onx_exa_menu')
  ui.setParameter('onx_exa_item_1.label','Item 1')
  ui.setCallback('onx_exa_item_1','activate','Python','print(\'Item 1\')')  
  # Create a second item in the menu :
  ui.createComponent('onx_exa_item_2','MenuItem','onx_exa_menu')
  ui.setParameter('onx_exa_item_2.label','Item 2')
  ui.setCallback('onx_exa_item_2','activate','Python','print(\'Item 2\')')

import Slash
session = Slash.create_session()
ui = session.createUI('menu.onx')

ui.steer()

del session
