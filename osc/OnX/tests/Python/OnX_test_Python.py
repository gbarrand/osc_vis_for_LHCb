import OnX
import sys

for i in range(0,5):
  print 'debug : create OnXMain : +++++++++++++++++++++++'
  #print sys.modules.has_key('OnX_PythonManager')

  onx_main = OnX.Main((),0,0)
  onx_main.loadInterpreter('Python')  #will create the OnX_PythonManager module

  import OnX_PythonManager
  ssession = OnX_PythonManager.sessionPointer()
  print ssession
  sess = OnX.OnX_cast_ISession(ssession)
  print sess
  del onx_main
  print sys.modules.has_key('OnX_PythonManager')
  print 'debug : OnXMain deleted'

print sys.modules.has_key('OnX_PythonManager')
