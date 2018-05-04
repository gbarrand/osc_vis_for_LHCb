import hep.aida.*;
import java.util.Random;

public class OnXLab_exa_Java_SLAC_TupleCreate
{
   public static void main(String[] argv)
   {
      IAnalysisFactory af = IAnalysisFactory.create();
      ITree tree = af.createTreeFactory().create();
      ITupleFactory tf = af.createTupleFactory(tree);
      
      String[] columnNames  =
      { "iFlat = 0", " fGauss = 3.", " fFlat =-2." };
      Class[] columnClasses =
      { Integer.TYPE, Float.TYPE, Float.TYPE };
      ITuple tuple1 = tf.create( "tuple1", "tupleLabel1", columnNames, columnClasses, "");
      
      //FIXME String columnString = "int iFlat=0, float fGauss = 3.; fFlat=-2.";
      String columnString = "int iFlat=0; float fGauss = 3.; fFlat=-2."; //FIXME
      ITuple tuple2 = tf.create( "tuple2", "tupleLabel1", columnString, "");
   }
}
