import hep.aida.*;

public class OnXLab_exa_Java_SLAC_DataPointSetCreateFromData
{
   public static void main(String[] argv) throws java.io.IOException
   {    
      IAnalysisFactory af = IAnalysisFactory.create();

      // Create a tree loading the AIDA objects stored in an AIDA file.
      ITree tree = af.createTreeFactory().create("aidaStore.aida");
      if(tree==null) { //FIXME
        System.out.println("can't create AIDA::ITree.");
        return;
      }

      IDataPointSetFactory dpsf = af.createDataPointSetFactory(tree);

      //FIXME IHistogram1D h1 = (IHistogram1D) tree.find("h1");
      //FIXME IProfile2D   p2 = (IProfile2D)   tree.find("p2");
      //FIXME ICloud3D     c3 = (ICloud3D)     tree.find("c3");
        
      IHistogram1D h1 = tree.find("h1").cast_IHistogram1D(); //FIXME
      IProfile2D   p2 = tree.find("p2").cast_IProfile2D(); //FIXME
      ICloud3D     c3 = tree.find("c3").cast_ICloud3D(); //FIXME
        
      // Create IDataPointSets from the the above AIDA objects.
      IDataPointSet dps1DFromHist   = dpsf.create("dps1DFromHist",h1);
      IDataPointSet dps2DFromProf   = dpsf.create("dps2DFromProf",p2);
      IDataPointSet dps3DFromCloud  = dpsf.create("dps2DFromCloud",c3);
   }
}
