# hapy version of aida_0

import AIDA

aida = AIDA.createAnalysisFactory()

h = AIDA.H1D(aida,'h','Rand gauss',100,-5,5)

import random
r = random.Random()
for i in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

p = AIDA.Plotter(aida)
p.set_title('The famous normal distribution')
p.set_x_label("Random gauss")
p.set_y_label("Entries")

p.plot(h)

print 'debug : xxx 1'
#print sys.modules[__name__].__getattributes__
#print dir(__name__)

def q():
 global h,p,aida
 del h
 del p
 del aida

#import atexit
#atexit.register(q)
