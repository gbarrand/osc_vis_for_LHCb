import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_SimpleFit
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
    
    IHistogram1D gaussHist  = histFactory.createHistogram1D("gaussHist","Gaussian Histogram",100,-5,5);
    ICloud1D     gaussCloud = histFactory.createCloud1D("gaussCloud","Gaussian Cloud");
    ITuple       tuple      = tuplFactory.create("tuple","Tuple Example","double gaussDistr");

    Random r = new Random();
    for (int i=0; i<10000; i++) {
      double x = r.nextGaussian();
      gaussHist.fill(x);
      gaussCloud.fill(x);
      tuple.fill(0,x);
      tuple.addRow();
    }

    // Chi2 fit with Minuit
    IFitResult minuitChi2Fit = fitter.fit(gaussHist,"g");
    if(minuitChi2Fit==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }
    
    // Least Squares fit with Minuit
    fitter.setFitMethod("LS");
    IFitResult minuitLeastSquaresFit = fitter.fit(gaussHist,"g");      
    if(minuitLeastSquaresFit==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }

    // Binned Maximum Likelihood fit with Minuit
    fitter.setFitMethod("binnedMaximumLikelihood");
    IFitResult minuitBinnedMaxLikelihoodFit = fitter.fit(gaussHist,"g"); 
    if(minuitBinnedMaxLikelihoodFit==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }

    // Unbinned Maximum Likelihood fit with Uncmin
    fitter.setEngine("uncmin");
    fitter.setFitMethod("uml");
    IFitResult uncminUMLFitToCloud = fitter.fit(gaussCloud,"g");
    if(uncminUMLFitToCloud==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }

    String[] gaussColumn = {"gaussDistr"};
    IFitData fitData = fitFactory.createFitData();
    fitData.createConnection(tuple,gaussColumn);

    IFitResult uncminUMLFitToTuple = fitter.fit(fitData,"g");
    if(uncminUMLFitToTuple==null) { //FIXME
      System.out.println("fit fails.");
      return;
    }

    IHistogram1D gaussProj  = histFactory.createHistogram1D("gaussProj","Gaussian Histogram Projected from ITuple",100,-5,5);
    tuple.project( gaussProj, tuplFactory.createEvaluator("gaussDistr") );
    IPlotter plotter = anFactory.createPlotterFactory().create("Plot");
    plotter.createRegion(0,0,.66,1).plot(gaussHist);
    plotter.destroyRegions();
    plotter.createRegion(0,0,.66,1).plot(gaussHist);
    plotter.region(0).plot( minuitChi2Fit.fittedFunction() );
    plotter.region(0).plot( minuitLeastSquaresFit.fittedFunction() );
    plotter.region(0).plot( minuitBinnedMaxLikelihoodFit.fittedFunction() );
    
    gaussCloud.convert(100,gaussCloud.lowerEdge(),gaussCloud.upperEdge());
    plotter.createRegion(.66,0,.33,.5).plot(gaussCloud.histogram());
    
    IModelFunction cloudFunc = (IModelFunction)uncminUMLFitToCloud.fittedFunction();
    cloudFunc.normalize(false);
    double gaussCloudNorm = gaussCloud.entries()*( gaussCloud.upperEdge()-gaussCloud.lowerEdge() )/gaussCloud.histogram().axis().bins();
    double cloudFuncNorm = gaussCloudNorm/(Math.sqrt(2*Math.PI)*cloudFunc.parameter("sigma"));
    cloudFunc.setParameter("amplitude",cloudFuncNorm);
    plotter.region(1).plot( cloudFunc );

    plotter.createRegion(.66,.5,.33,.5).plot( gaussProj );


    IModelFunction tupleFunc = (IModelFunction)uncminUMLFitToTuple.fittedFunction();
    tupleFunc.normalize(false);
    
    double gaussProjNorm = gaussProj.entries()*( gaussProj.axis().upperEdge()-gaussProj.axis().lowerEdge() )/gaussProj.axis().bins();
    double tupleFuncNorm = gaussProjNorm/(Math.sqrt(2*Math.PI)*tupleFunc.parameter("sigma"));

    tupleFunc.setParameter("amplitude",tupleFuncNorm);
    plotter.region(2).plot( tupleFunc );
    plotter.show();
    plotter.interact(); //FIXME
  }
}
