
import csv
import os
import sys
import time     
#from types import *
#AIDA
import AIDA
import gzip

####################################################################
def process(aida, memTree, fileName):
#######
   histogramFactory = aida.createHistogramFactory(memTree)
   numOfSec = 24 * 3600
   nBins = numOfSec/60
   h = aida.create_histogram1D(memTree,'h','Signal Strength',
                               int(nBins),0.,float(numOfSec))
   del histogramFactory


   f_in = gzip.open(fileName, 'rt')
   f_out = open('tmp.csv', 'wt')
   f_out.writelines(f_in.read())
   f_out.close()
   f_in.close()

   cr = csv.reader(open('tmp.csv',"rt"))

   count = 0
   timeStart = 0
   for row in cr:
       count += 1
       #if count > 20:
       #  break
       #print("Row[",count,"] :",len(row))
       if len(row) == 1:
           line = row[0].strip("#").replace(' ','')
           header = line.split("=")
           #print('header ', header," first [",header[0],"]")
           if header[0] == "UTC_StartTime": # there is a blanc at the end
               timeStart = time.mktime(
                   time.strptime(header[1],"%Y-%m-%d%H:%M:%S"))
       elif len(row) == 2:
               strength = float(row[1].strip())
               timeStamp = time.mktime(time.strptime(row[0],"%Y-%m-%d %H:%M:%S")) 
               timeStamp -= timeStart
               h.fill(timeStamp,strength)
               #print("Time [",timeStamp,"], strenght [",strength,"]")
       else:
           print("osc_jec_csv.py::process : error")
           break

   os.remove('tmp.csv')
   return h

####################################################################
def main(*args):
   # AIDA init :
   aida = AIDA.createAnalysisFactory()
   memTree = aida.find_memoryTree()
   # Cleanup memTree :
   memTree.rmdir('/')

   #Data location
   #wd = "E:\Presentations\BAO\SolarFlare\supersid_v1_1_1\Data"
   savewd = os.getcwd()

   #print("osc_jec_csv.py : run on ",os.getcwd())

   # Process
   osc_home_dir = os.getenv("OSC_HOME_DIR")
   data_dir = osc_home_dir+'/Resources/AIDA/examples/data/'
   h0 = process(aida,memTree,data_dir+"jec_11.csv.gz")
   h1 = process(aida,memTree,data_dir+"jec_12.csv.gz")

   # Get current plotter :
   plotterFactory = aida.createPlotterFactory()

   opts = ''

   # Get a plotter (pick up one in the GUI) :
   plotter = plotterFactory.create()
   del plotterFactory
   plotter.createRegions(1,1,0)

   # Set page title :
   plotter.setTitle('Solar Flare example')
   region = plotter.currentRegion()

   region.plot(h0,opts)
   region.plot(h1,opts)

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

   soPlotter = soPlotterRegion.getPlotter()

   soPlotter.gridEnforced.setValue(Inventor.TRUE)

   soPlotter.getWallStyle().visible.setValue(Inventor.FALSE)
   soPlotter.getGridStyle().visible.setValue(Inventor.TRUE)
   soPlotter.getGridStyle().color.setValue(SbColor_black)
   soPlotter.getGridStyle().linePattern.setValue(HEPVis.SbLinePattern_dotted)

   soPlotter.getTitleStyle().visible.setValue(Inventor.FALSE)

   soPlotter.getXAxis().title.setValue('Time (s)') # PAW encoding
   soPlotter.getXAxis().timeLabels.setValue(Inventor.TRUE)
   soPlotter.getXAxis().timeFormat.setValue('%H:%M')
   soPlotter.getXAxis().timeOffset.setValue(0)  # from C lib EPOCH.
   soPlotter.getXAxis().timeOffsetIsGMT.setValue(Inventor.TRUE)
   soPlotter.getXAxis().divisions.setValue(608)

   soPlotter.getYAxis().title.setValue('A.U') # PAW encoding
   soPlotter.getYAxis().getMagStyle().translation.setValue(0.01,0,0)
   
   # legend region :
   soPlotterRegion.legendRegionVisible.setValue(Inventor.TRUE)
   soPlotterRegion.legendRegionAttachedToInfosRegion.setValue(Inventor.FALSE)
   soPlotterRegion.legendRegionOriginUnit.setValue(HEPVis.SoPlotterRegion.AXIS)
   soPlotterRegion.legendAutomated.setValue(Inventor.FALSE)

   soPlotterRegion.legendRegionOrigin.setValue(Inventor.SbVec2f(1.5*10**4,14*10**6))
   soPlotterRegion.legendRegionSize.setValue(Inventor.SbVec2f(0.6,0.16))

   soLegendRegion = soPlotterRegion.getLegendRegion()
   soLegendRegion.text.set1Value(0,Inventor.SbString('Solar flare'))
   soLegendRegion.text.set1Value(1,Inventor.SbString('Reference'))

   soLegendRegion.textInBlack.setValue(Inventor.TRUE)
   soLegendRegion.getStyle(0).color.setValue(SbColor_red)
   soLegendRegion.getStyle(0).fontName.setValue(SbFont_ROOT_62)
   soLegendRegion.getStyle(1).color.setValue(SbColor_blue)
   soLegendRegion.getStyle(1).fontName.setValue(SbFont_ROOT_62)
   soLegendRegion.getStyle(1).encoding.setValue('PAW')

   SbModeling_lines = 'lines'
   soLegendRegion.getStyle(0).modeling.setValue(SbModeling_lines)
   soLegendRegion.getStyle(0).lineWidth.setValue(2)
   soLegendRegion.getStyle(1).modeling.setValue(SbModeling_lines)
   soLegendRegion.getStyle(1).lineWidth.setValue(2)


   # histo0 contour blue :
   #soPlotter.getBinsStyle(0).modeling.setValue('curve')
   soPlotter.getBinsStyle(0).modeling.setValue('lines')
   soPlotter.getBinsStyle(0).color.setValue(SbColor_blue)
   soPlotter.levels.set1Value(0,9)

   # histo1 contour red :
   #soPlotter.getBinsStyle(1).modeling.setValue('curve')
   soPlotter.getBinsStyle(1).modeling.setValue('lines')
   soPlotter.getBinsStyle(1).color.setValue(SbColor_red)
   soPlotter.levels.set1Value(0,9)


   os.chdir(savewd)
   os.getcwd()


   # Show :
   plotter.show()

   #plotter.writeToFile('solarflare_bis.gif')
   plotter.writeToFile('solarflare_bis.jpg','jpeg')

   plotter.interact()


   # clean :
   del plotter
   del aida



if __name__ == '__main__':
 sys.exit(main(*sys.argv))
