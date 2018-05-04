import hep.aida.*;
import java.util.Random;
import java.io.IOException;

public class OnXLab_exa_Java_SLAC_Restore 
{
   public static void main(String[] argv) throws IOException
   {
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create("myFile.aida","xml");
      if(tree==null) { //FIXME
        System.out.println("can't create AIDA::ITree.");
        return;
      }
      
      //FIXME : SLAC : IHistogram1D h1d = (IHistogram1D) tree.find("test 1d");
      //FIXME : SLAC : IHistogram2D h2d = (IHistogram2D) tree.find("test 2d");
      IHistogram1D h1d = tree.find("test 1d").cast_IHistogram1D(); //FIXME
      IHistogram2D h2d = tree.find("test 2d").cast_IHistogram2D(); //FIXME
      
      IPlotter plotter = af.createPlotterFactory().create("Test");
      plotter.createRegions(2,1);
      plotter.region(0).plot(h1d);
      plotter.region(1).plot(h2d);
      plotter.show();
      plotter.interact(); //FIXME
   }
}
