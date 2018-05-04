
class OnX_Java_test_swig {
  public OnX_Java_test_swig() {
    //System.out.println("debug : OnX_Java_test_swig::OnX_Java_test_swig");

    System.loadLibrary("OnX_SWIG_Java");

    String s = OnX.smanip.torgbs("red");
    System.out.println(s);
  }
  public static void main(String[] args) {
    new OnX_Java_test_swig();
  }        
}

