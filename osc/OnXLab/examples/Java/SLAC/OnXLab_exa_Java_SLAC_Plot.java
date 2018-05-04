import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_Plot
{
   public static void main(String[] argv)
   {
      IAnalysisFactory af = IAnalysisFactory.create();
      IHistogramFactory hf = af.createHistogramFactory(af.createTreeFactory().create());
      
      IHistogram2D h2d = hf.createHistogram2D("test 2d",50,-30,30,50,-3,3);
      
      Random r = new Random();
      for (int i=0; i<10000; i++) 
      {
         h2d.fill(10*r.nextGaussian(),r.nextGaussian());
      }
      
      IPlotter plotter = af.createPlotterFactory().create("Plot");
      plotter.destroyRegions();
      plotter.createRegion(0,0,.66,1).plot(h2d);
      plotter.createRegion(.66,0,.33,.5).plot(hf.projectionX("X Projection",h2d));
      plotter.createRegion(.66,.5,.33,.5).plot(hf.projectionY("Y Projection",h2d));
      plotter.show();
      plotter.interact(); //FIXME
   }
}
