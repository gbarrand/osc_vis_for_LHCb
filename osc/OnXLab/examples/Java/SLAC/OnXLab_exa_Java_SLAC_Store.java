import hep.aida.*;
import java.util.Random;
import java.io.IOException;

public class OnXLab_exa_Java_SLAC_Store 
{
   public static void main(String[] argv) throws IOException
   {
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create("myFile.aida","xml",false,true);
      IHistogramFactory hf = af.createHistogramFactory(tree);
      
      IHistogram1D h1d = hf.createHistogram1D("test 1d",50,-3,3);
      IHistogram2D h2d = hf.createHistogram2D("test 2d",50,-3,3,50,-3,3);
      
      Random r = new Random();
      for (int i=0; i<10000; i++) 
      {
         h1d.fill(r.nextGaussian());
         h2d.fill(r.nextGaussian(),r.nextGaussian());
      }
      
      tree.commit();
   }
}
