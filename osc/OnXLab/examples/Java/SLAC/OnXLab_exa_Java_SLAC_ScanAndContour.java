import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_ScanAndContour
{       
  public static void main(String[] argv) throws java.io.IOException
  {        
    IAnalysisFactory  anFactory   = IAnalysisFactory.create();
    //FIXME : where is storeForFittingExample data file ?
    //ITree             tree        = anFactory.createTreeFactory().create("storeForFittingExample");
    ITree             tree        = anFactory.createTreeFactory().create(); //FIXME
    IHistogramFactory histFactory = anFactory.createHistogramFactory( tree );
    ITupleFactory     tuplFactory = anFactory.createTupleFactory( tree );
    IFunctionFactory  funcFactory = anFactory.createFunctionFactory( tree );
    IFitFactory       fitFactory  = anFactory.createFitFactory();
    IFitter           fitter      = fitFactory.createFitter("Chi2","minuit");

    IHistogram1D hist  = histFactory.createHistogram1D("hist","Test Histogram",100,-5,5);

    Random r = new Random();

    for (int i=0; i<10000; i++) {
      double x = r.nextGaussian();
      hist.fill(x);
    }

   IFitData fitData = fitFactory.createFitData();
   fitData.create1DConnection(hist);

   IFitResult fitResult = fitter.fit(fitData,"g");
    if(fitResult==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }

   int meanIndex  = fitResult.fittedFunction().indexOfParameter("mean");
   double meanVal = fitResult.fittedParameters()[meanIndex];
   double meanErr = fitResult.errors()[meanIndex];
   IDataPointSet meanScan = fitter.createScan1D(fitData, fitResult.fittedFunction(),"mean",20, meanVal-3*meanErr, meanVal+3*meanErr);

   int sigmaIndex  = fitResult.fittedFunction().indexOfParameter("sigma");
   double sigmaVal = fitResult.fittedParameters()[sigmaIndex];
   double sigmaErr = fitResult.errors()[sigmaIndex];
   IDataPointSet oneSigmaContour = fitter.createContour(fitData, fitResult, "mean", "sigma", 10, 1);
   IDataPointSet twoSigmaContour = fitter.createContour(fitData, fitResult, "mean", "sigma", 10, 2);

   IPlotter plotter = anFactory.createPlotterFactory().create("Plot");
   plotter.destroyRegions();
   plotter.createRegion(0,0,.66,1).plot(hist);
   plotter.region(0).plot(fitResult.fittedFunction());
   plotter.createRegion(.66,0,.33,.5).plot( meanScan );
   plotter.createRegion(.66,.5,.33,.5).plot( twoSigmaContour );
   plotter.region(2).plot( oneSigmaContour );
   plotter.show();
   plotter.interact(); //FIXME

  }
}
