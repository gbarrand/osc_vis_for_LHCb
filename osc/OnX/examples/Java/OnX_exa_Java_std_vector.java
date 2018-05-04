
public class OnX_exa_Java_std_vector {
  public static void main(String[] aArgs) {

    // Array of strings :
    String[] sitems = {"item 1","item 2","item 3"};
    String[] rsitems = OnX.OnX.OnX_test_std_vector_std_string(sitems);
    System.out.println("returned string items :");
    for(int index=0;index<rsitems.length;index++) {
      System.out.println(" "+rsitems[index]);
    }

    // Array of doubles :
    double[] ditems = {1,2,3};
    double[] rditems = OnX.OnX.OnX_test_std_vector_double(ditems);
    System.out.println("returned double items :");
    for(int index=0;index<rditems.length;index++) {
      System.out.println(" "+rditems[index]);
    }


  }        
}
