
import OnX.Native;

//import OnX.Script;
//import OnX.Initialize;

class OnX_Java_test_3 {
  public OnX_Java_test_3() {
    System.loadLibrary("OnXJavaSWIG");

    long onx = Native.new_OnX_OnX(0);
    long ui = 
      Native.OnX_OnX_createUI(onx,"$ONXROOT/scripts/OnX/OnX.onx","",true,false);

    //Native.IUI_executeScript(ui,"java","x Detector");

/*
    long session = Native.OnX_OnX_session(onx);

    Script script = new Script();
    script.executeClass("OnX.Initialize",session);
    Long sess = new Long(session);
    Initialize init = new Initialize(sess);
    init.execute();
*/
    Native.IUI_steer(ui);
  }
  public static void main(String[] args) {
    new OnX_Java_test_3();
  }        
}

