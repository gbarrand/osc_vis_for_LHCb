import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_Fit
{
   public static void main(String[] args)
   {
      // Create factories
      IAnalysisFactory analysisFactory = IAnalysisFactory.create();
      ITreeFactory treeFactory = analysisFactory.createTreeFactory();
      ITree tree = treeFactory.create();
      IPlotter plotter = analysisFactory.createPlotterFactory().create("Plot");
      IHistogramFactory histogramFactory = analysisFactory.createHistogramFactory(tree);
      IFunctionFactory functionFactory = analysisFactory.createFunctionFactory(tree);
      IFitFactory fitFactory = analysisFactory.createFitFactory();
      
      // Create 1D histogram
      IHistogram1D h1d = histogramFactory.createHistogram1D("Histogram 1D",50,-3,3);
      
      // Fill 1D histogram with Gaussian
      Random r = new Random();
      for (int i=0; i<5000; i++)
      {
         h1d.fill(r.nextGaussian());
      }
      
      // Create Gaussian fitting function
      IFunction f = functionFactory.createFunctionByName("Gaussian", "G");
      
      // Do Fit
      IFitter fitter = fitFactory.createFitter("chi2");
      IFitResult result = fitter.fit(h1d,f);
      
      // Show results
      plotter.createRegions(1,1,0);
      plotter.region(0).plot(h1d);
      plotter.region(0).plot(f);
      plotter.show();
      plotter.interact(); //FIXME
   }
}
