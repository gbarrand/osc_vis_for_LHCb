
public class OnX_exa_Java_typemaps {
  public static void main(String[] aArgs) {
    OnX.OnX_SWIG_Test test = new OnX.OnX_SWIG_Test();

    // String [] :
    String[] avs = {"Item 1","Item 2","Item 3"};

    test.arg_const_std_vec_string(avs);
    test.arg_std_vec_string(avs);

    String[] vs = test.ret_std_vec_string();

    for(int index=0;index<vs.length;index++) {
      System.out.println(" "+vs[index]);
    }

    String[] cvs = test.ret_const_std_vec_string();

    for(int index=0;index<cvs.length;index++) {
      System.out.println(" "+cvs[index]);
    }

    // double [] :
    double [] avd = {1.1,2.2,3.3};

    test.arg_const_std_vec_double(avd);
    test.arg_std_vec_double(avd);

    double [] vd = test.ret_std_vec_double();

    for(int index=0;index<vd.length;index++) {
      System.out.println(" "+vd[index]);
    }

    double[] cvd = test.ret_const_std_vec_double();

    for(int index=0;index<cvd.length;index++) {
      System.out.println(" "+cvd[index]);
    }

    // int [] :
    int [] avi = {1,2,3};

    test.arg_const_std_vec_int(avi);
    test.arg_std_vec_int(avi);

    int [] vi = test.ret_std_vec_int();

    for(int index=0;index<vi.length;index++) {
      System.out.println(" "+vi[index]);
    }

    int[] cvi = test.ret_const_std_vec_int();

    for(int index=0;index<cvi.length;index++) {
      System.out.println(" "+cvi[index]);
    }

  }        
}
