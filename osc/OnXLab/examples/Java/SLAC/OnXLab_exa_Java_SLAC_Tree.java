import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_Tree 
{
   public static void main(String[] argv)
   {
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create();
      IHistogramFactory hf = af.createHistogramFactory(tree);
      
      hf.createHistogram1D("test 1d",50,-3,3);
      hf.createHistogram2D("test 2d",50,-3,3,50,-3,3);
      
      tree.mkdir("/folder1");
      tree.cd("/folder1");
      hf.createHistogram1D("test 1d",50,-3,3);
      hf.createHistogram2D("test 2d",50,-3,3,50,-3,3);   
      
      tree.mkdir("/folder2");
      tree.cd("/folder2");
      hf.createHistogram1D("test 1d",50,-3,3);
      hf.createHistogram2D("test 2d",50,-3,3,50,-3,3);         
   }
}