# 
# Example to loop on a tuple.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import aida_19
#

#////////////////////////////////////////
#////////////////////////////////////////
#////////////////////////////////////////
import AIDA

aida = AIDA.createAnalysisFactory()

import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
data_dir = osc_home_dir+'/Resources/AIDA/examples/data/'

file = data_dir+'SPLCPDisco-allsigvar-mateff-bin100-h0-o0.root'
tree = aida.create_tree(file,'root',True,False)
assert tree

tuple = tree.find('SplGlb')
if tuple == None: print('SplGlb not found.')
tuple = tuple.cast_ITuple()
if tuple == None: print('SplGlb not an ITuple.')

# dump columns name and type :
coln = tuple.columns()
rows = tuple.rows()
print(coln,rows)
for coli in range(0,coln):
  print(tuple.columnName(coli))
  print(tuple.columnType(coli))


# loop on entries :
ichi2 = tuple.findColumn('chi2')
tuple.start()
while tuple.next() == 1:
  chi2 = tuple.getDouble(ichi2)
  print(chi2)

del tuple
del tree
del aida
