import OnX
import sys

for i in range(0,5):
  print('debug : create OnXMain : +++++++++++++++++++++++')
  #print 'OnXPython' in sys.modules

  onx_main = OnX.Main((),0,0)
  onx_main.loadInterpreter('Python')  #will create the OnXPython module

  import OnXPython
  ssession = OnXPython.sessionPointer()
  print ssession
  sess = OnX.OnX_cast_ISession(ssession)
  print sess
  del onx_main
  print 'OnXPython' in sys.modules
  print('debug : OnXMain deleted')

print 'OnXPython' in sys.modules
