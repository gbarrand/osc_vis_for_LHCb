import hep.aida.*;

public class OnXLab_exa_Java_SLAC_CloudCreate
{
   public static void main(String[] argv)
   {    
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create();
      IHistogramFactory hf = af.createHistogramFactory(tree);
      
      ICloud1D cl1D = hf.createCloud1D( "cl1D" );
      ICloud2D cl2D = hf.createCloud2D( "cl2D", "2-Dimensional Cloud" );
      ICloud3D cl3D = hf.createCloud3D( "cl3D", "3-Dimensional Cloud", 1500, "autoConvert = false" );     
   }
}
