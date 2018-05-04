
stop = 0

def next_event(a_ui,i):
  #print 'next_event : %d' % i
  a_ui.executeScript("DLD","OnX ui_executeCallbacks * update")

def start(a_ui):
  global stop
  stop = 0
  i = 0
  while stop == 0:
    next_event(a_ui,i)
    a_ui.synchronize()
    i = i + 1

def start_timer(a_ui):
  def timer_proc(a_ui):
    print 'timer_proc : begin'
    global stop
    stop = 0
    i = 0
    # never exit the below loop.
    import time
    while stop == 0:
      next_event(a_ui,i)
      a_ui.synchronize()
      time.sleep( 0.01 ) # 1 sec
      i = i + 1

  print 'start_timer : begin'
  import thread
  thread.start_new_thread(timer_proc,(a_ui,)) # non blocking.

def callback():
  global stop
  stop = 1

import Slash
session = Slash.create_session()
ui = session.createUI('loop.onx')

ui.executeScript('DLD','OnXExas shape_initialize')

start_timer(ui)

#start(ui)
#del session



