#
# convertion of discoCP.C of JEC.
#  May 2010
#
# This example shows how to strongly customize
# a plot by using directly the SoPlotter
# and SoPlotterRegion nodes.
#

#
# Usage :
#   OS> <source osc_vis setup>
#   OS> <setup python>
#   OS> <source osc_vis python setup>
#   OS> <start python>
#   >>> import osc_discoCP
#


nLogSin22q13 = 21
logSin2Min = -3.3
logSin2Max = -1
logSin2BinWidth = (logSin2Max - logSin2Min)/(nLogSin22q13-1.)
logSin2EdgeMax = logSin2Max + logSin2BinWidth/2.
logSin2EdgeMin = logSin2Min - logSin2BinWidth/2.

nDeltaCP  = 21
deltaMin = 0. # rad/pi
deltaMax =  1.
deltaBinWidth = (deltaMax - deltaMin)/(nDeltaCP-1.)
deltaEdgeMax = deltaMax + deltaBinWidth/2.
deltaEdgeMin = deltaMin - deltaBinWidth/2.

title='3[s] CP discovery ([D] [c]^2!=9,1dof)'  # PAW encoding
xmin = 10.**logSin2Min
xmax = 10**logSin2Max
ymin = deltaMin
ymax = deltaMax

import AIDA

aida = AIDA.createAnalysisFactory()

tree = aida.find_memoryTree()

import os
osc_home_dir = os.getenv("OSC_HOME_DIR")
data_dir = osc_home_dir+'/Resources/AIDA/examples/data/'

f35 = aida.create_tree(data_dir+'SPLCPDisco-allsigvar-mateff-bin100-h0-o0.root','root',True,False)
assert f35
tree35 = f35.find('SplGlb')
if tree35 == None: print('SplGlb not found.')
tree35 = tree35.cast_ITuple()
if tree35 == None: print('SplGlb not an ITuple.')

#coln = tree35.columns()
#print(coln)
#for coli in range(0,coln):
#  print(tree35.columnName(coli))
#  print(tree35.columnType(coli))

histo35 = aida.create_histogram2D(tree,
                                  'histo35',' Theta vs Delta',
                                  nLogSin22q13,logSin2EdgeMin,logSin2EdgeMax,
                                  nDeltaCP,deltaEdgeMin,deltaEdgeMax)
x_evaluator = aida.create_evaluator(f35,'log10(pow(sin(2*theta13),2))')
y_evaluator = aida.create_evaluator(f35,'deltaCP/pi')
w_evaluator = aida.create_evaluator(f35,'chi2')
tree35.project(histo35,x_evaluator,y_evaluator,w_evaluator)
del x_evaluator,y_evaluator,w_evaluator

#print(nLogSin22q13,logSin2EdgeMin,logSin2EdgeMax)

#print(histo35.allEntries(),histo35.sumBinHeights())

f36 = aida.create_tree(data_dir+'SPLCPDisco-matbb-h0-o0.root','root',True,False)
tree36 = f36.find('SplGlb')
if tree36 == None: print('SplGlb not found.')
tree36 = tree36.cast_ITuple()
if tree36 == None: print('SplGlb not an ITuple.')

histo36 = aida.create_histogram2D(tree,
                                  'histo36',' Theta vs Delta',
                                  nLogSin22q13,logSin2EdgeMin,logSin2EdgeMax,
                                  nDeltaCP,deltaEdgeMin,deltaEdgeMax)

x_evaluator = aida.create_evaluator(f36,'log10(pow(sin(2*theta13),2))')
y_evaluator = aida.create_evaluator(f36,'deltaCP/pi')
w_evaluator = aida.create_evaluator(f36,'chi2')
tree36.project(histo36,x_evaluator,y_evaluator,w_evaluator)
del x_evaluator,y_evaluator,w_evaluator

#print(histo36.allEntries(),histo36.sumBinHeights())

p = aida.create_plotter()
p.createRegions(1,1,0)
p.setTitle('')
p.show()

region = p.currentRegion()
region.plot(histo35,'overlay=0')
region.plot(histo36,'overlay=0')

#///////////////////////////////////////////
#/// Inventor / HEPVis : ///////////////////
#///////////////////////////////////////////

import CoinPython as Inventor
import HEPVis

SbColor_black = Inventor.SbColor(0,0,0)
SbColor_red = Inventor.SbColor(1,0,0)
SbColor_blue = Inventor.SbColor(0,0,1)
SbFont_ROOT_62 = 'TTF/arialbd'

ui = aida.ui()
soPage = ui.findSoPage('plotter_0')
soRegion = soPage.currentRegion()
soPlotterRegion = soRegion.cast_SoPlotterRegion()

# global style :
soPlotterRegion.setStyleROOT_Default()
soPlotterRegion.getTitleRegion().getStyle(0).encoding.setValue('PAW')
soPlotterRegion.infosRegionVisible.setValue(Inventor.FALSE)

# main hdum :
soPlotter = soPlotterRegion.getPlotter()

soPlotter.titleAutomated.setValue(Inventor.FALSE)  
soPlotter.title.setValue(title)  

soPlotter.xAxisEnforced.setValue(Inventor.TRUE)
soPlotter.xAxisAutomated.setValue(Inventor.FALSE)
soPlotter.xAxisMinimum.setValue(xmin)
soPlotter.xAxisMaximum.setValue(xmax)

soPlotter.yAxisEnforced.setValue(Inventor.TRUE)
soPlotter.yAxisAutomated.setValue(Inventor.FALSE)
soPlotter.yAxisMinimum.setValue(ymin)
soPlotter.yAxisMaximum.setValue(ymax)

soPlotter.gridEnforced.setValue(Inventor.TRUE)

soPlotter.getWallStyle().visible.setValue(Inventor.FALSE)
soPlotter.getGridStyle().visible.setValue(Inventor.TRUE)
soPlotter.getGridStyle().color.setValue(SbColor_black)
soPlotter.getGridStyle().linePattern.setValue(HEPVis.SbLinePattern_dotted)
soPlotter.getTitleStyle().visible.setValue(Inventor.FALSE)
soPlotter.setAxesModeling(Inventor.SbString('hplot'))
soPlotter.getXAxis().divisions.setValue(510)
soPlotter.xAxisLogScale.setValue(Inventor.TRUE)

soPlotter.getXAxis().title.setValue('true sin^2!(2[q]?13!)') # PAW encoding
soPlotter.getYAxis().title.setValue('true [d]?CP!/[p]') # PAW encoding

# overlay histo35 :
soPlotter = soPlotterRegion.getOverlay(0)
soPlotter.getWallStyle().visible.setValue(Inventor.FALSE)
soPlotter.getGridStyle().visible.setValue(Inventor.FALSE)
soPlotter.getXAxis().setVisible(Inventor.FALSE)
soPlotter.getYAxis().setVisible(Inventor.FALSE)
soPlotter.getTitleStyle().visible.setValue(Inventor.FALSE)

# histo35 contour blue :
soPlotter.getBinsStyle(0).modeling.setValue('curve')
soPlotter.getBinsStyle(0).color.setValue(SbColor_blue)
soPlotter.levels.set1Value(0,9)

# histo36 contour red :
soPlotter.getBinsStyle(1).modeling.setValue('curve')
soPlotter.getBinsStyle(1).color.setValue(SbColor_red)
soPlotter.levels.set1Value(0,9)

# legend region :
soPlotterRegion.legendRegionVisible.setValue(Inventor.TRUE)
soPlotterRegion.legendRegionAttachedToInfosRegion.setValue(Inventor.FALSE)
soPlotterRegion.legendRegionOriginUnit.setValue(HEPVis.SoPlotterRegion.AXIS)
soPlotterRegion.legendAutomated.setValue(Inventor.FALSE)
soPlotterRegion.legendRegionOrigin.setValue(Inventor.SbVec2f(10**(-3),0.40))
soPlotterRegion.legendRegionSize.setValue(Inventor.SbVec2f(0.6,0.16))

soLegendRegion = soPlotterRegion.getLegendRegion()
soLegendRegion.text.set1Value(0,Inventor.SbString('matrices de migrations'))
soLegendRegion.text.set1Value(1,Inventor.SbString('matrices de migrations, [b] Beam'))

soLegendRegion.textInBlack.setValue(Inventor.TRUE)
soLegendRegion.getStyle(0).color.setValue(SbColor_red)
soLegendRegion.getStyle(0).fontName.setValue(SbFont_ROOT_62)
soLegendRegion.getStyle(1).color.setValue(SbColor_blue)
soLegendRegion.getStyle(1).fontName.setValue(SbFont_ROOT_62)
soLegendRegion.getStyle(1).encoding.setValue('PAW')

SbMarkerAsterisk = 2
SbModeling_lines = 'lines'
soLegendRegion.getStyle(0).modeling.setValue(SbModeling_lines)
soLegendRegion.getStyle(0).lineWidth.setValue(2)
soLegendRegion.getStyle(1).modeling.setValue(SbModeling_lines)
soLegendRegion.getStyle(1).lineWidth.setValue(2)

#///////////////////////////////////////////
#///////////////////////////////////////////
#///////////////////////////////////////////

p.interact()

del p

del aida
