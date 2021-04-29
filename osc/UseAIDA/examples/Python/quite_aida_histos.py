
#
# We show how to get the "internal" inlib::histo which
# is behind a AIDA::I[Histogra,Profile][1,2]D and
# then access some data not accessible from the AIDA
# interfaces.

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import quite_aida_histos
#

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import AIDA

import inlib # for inlib::random
rg = inlib.random_gauss()
rbw = inlib.random_bw()

aida = AIDA.createAnalysisFactory()

#  TFile* file = new TFile("histos.root","recreate");
tree = aida.find_memoryTree()

h1D = aida.create_histogram1D(tree,'h1D','Histogram 1D',100,-5,5)
for I in range(0,10000):
  h1D.fill(rg.shoot(),1)

h2D = aida.create_histogram2D(tree,'h2D','Histogram 2D',100,-5,5,100,-4,4)
for I in range(0,10000):
  h2D.fill(rg.shoot(),rbw.shoot(),1)

p1D = aida.create_profile1D(tree,'p1D','Profile 1D',100,-5,5)
for I in range(0,10000):
  p1D.fill(rg.shoot(),rbw.shoot(),1)

p2D = aida.create_profile2D(tree,'p2D','Profile 2D',100,-5,5,100,-3,3)
for I in range(0,10000):
  p2D.fill(rg.shoot(),rbw.shoot(),rbw.shoot(),1)


#////////////////////////////////////////
#/// not AIDA ///////////////////////////
#////////////////////////////////////////

# for an histogram 1D when doing a :
#   fill(x,w)
# in inlib::histo::h1d we do a :
#   find the bin i from x
#   bin_entries[i] ++
#   bin_sum_w[i] += w            # "S" is for Sum.
#   bin_sum_w2[i] += w*w
#   bin_sum_xw[i][0] += x*w      # 0 being for axis x.
#   bin_sum_x2w[i][0] += x*x*w

inlib_h1D = h1D.inlib_object()
Sw = inlib_h1D.bins_sum_w() # std::vector transformed in python tuple.
print(Sw)
Sxw = inlib_h1D.bins_sum_xw() # std::vector transformed in python tuple.
print(Sxw)
  
# for a profile 1D when doing a :
#   fill(x,v,w)
# in inlib::histo::p1d we do a :
#   find the bin i from x
#   bin_entries[i] ++
#   bin_sum_w[i] += w
#   bin_sum_w2[i] += w*w
#   bin_sum_xw[i][0] += x*w
#   bin_sum_x2w[i][0] += x*x*w
#   bin_sum_vw[i] += v*w
#   bin_sum_v2w[i] += v*v*w

inlib_p1D = p1D.inlib_object()
Sw = inlib_p1D.bins_sum_w()   # std::vector transformed in python tuple.
#print(Sw)
Svw = inlib_p1D.bins_sum_vw() # std::vector transformed in python tuple.
#print(Svw)
for i in range(0,len(Sw)):
  print(Sw[i],Svw[i])
  
del rg,rbw
del aida
