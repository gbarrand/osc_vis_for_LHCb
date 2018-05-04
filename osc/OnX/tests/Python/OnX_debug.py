print 'debug : Panoramix : 013'

import sys

# Have ROOT.py in batch mode (no ROOT GUI)
if sys.__dict__.has_key('argv'):
  sys.argv.append('Panoramix')
else:
  sys.argv = ['Panoramix']

print sys.platform

if sys.platform == 'win32':
  print 'debug : win32'
else:
  print 'debug : not win32'

