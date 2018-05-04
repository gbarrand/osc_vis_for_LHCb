
import OnX.Native;

class OnX_Java_test_2 {
  public OnX_Java_test_2() {
    System.loadLibrary("OnXJavaSWIG");
    long ss = OnX.Native.new_std_string("");
  }
  public static void main(String[] args) {
    new OnX_Java_test_2();
  }        
}

