# hapy version of aida_0

import hapi

h = hapi.H1D('h','Rand gauss',100,-5,5)

import random
r = random.Random()
for i in range(0,10000):
  h.fill(r.gauss(0,1),1)
del r

p = hapi.Plotter()
p.title('The famous normal distribution')
p.x_label("Random gauss")
p.y_label("Entries")

p.plot(h)
p.interact()
