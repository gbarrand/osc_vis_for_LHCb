import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_Cloud
{   
   public static void main(String[] argv)
   {
      
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create();
      IHistogramFactory hf = af.createHistogramFactory(tree);
      
      ICloud1D cl1D = hf.createCloud1D( "cl1D", "1-Dimensional Cloud", 1500, "" );
      ICloud2D cl2D = hf.createCloud2D( "cl2D", "2-Dimensional Cloud", 1500, "" );
      
      int entries = 20000;
      Random r = new Random();
      
      for ( int i = 0; i < entries; i++ )
      {
         double xval = r.nextGaussian();
         double yval = r.nextGaussian();
         double w    = r.nextDouble();
         
         cl1D.fill( xval, w );
         cl2D.fill( xval, yval, w );
      }
      
      IHistogram1D cl1DHist = cl1D.histogram();
      IHistogram2D cl2DHist = cl2D.histogram();
      
      IPlotter plotter = af.createPlotterFactory().create("Plot");
      plotter.createRegions(1,2,0);
      plotter.region(0).plot(cl1DHist);
      plotter.region(1).plot(cl2D);
      plotter.show();     
      plotter.interact(); //FIXME
   }
}
