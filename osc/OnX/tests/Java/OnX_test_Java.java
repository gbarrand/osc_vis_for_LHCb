
import OnX.Test;
import OnX.Printer;

import java.io.*;

class OnX_test_Java {
  public OnX_test_Java() {
    System.loadLibrary("OnX_SWIG_Java");
    Test test = new Test();
    System.out.println("Should print : 11 12 13");
    long l1 = 11;
    long l2 = 12;
    long l3 = 13;
    test.nativeTest(11,12,13);
    System.out.println("Should print : 12345 Hello2");
    test.nativeTestStatic(12345,"Hello2");
    System.out.println("Should print : 12345 Hello3");
    test.ISession_findManager(12345,"Hello3");
    System.out.println("Should print : Hello");
    test.nativeTestString("Hello");
    test.exec();
    // Test Printer :

    String logFileName = "java.log";
    try { 
      File log = new File(logFileName);
      FileOutputStream os = new FileOutputStream(log);
      //prevOut = System.out;
      //prevErr = System.err;
      // Create new streams
      Printer newOut = new Printer(0);
      Printer newErr = new Printer(0);
      System.setOut(newOut);
      System.setErr(newErr);
    } catch(FileNotFoundException e) { 
      System.out.println("Could not openfile " + logFileName);
    }

    //printer.println(2);
    //printer.println("Test OnX.Printer");

    System.out.println("This is written to standard out.");
    System.err.println("This is written to standard err.");

  }
  public static void main(String[] args) {
    new OnX_test_Java();
  }        
}

