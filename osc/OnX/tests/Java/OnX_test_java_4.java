
import java.io.*;
import java.lang.reflect.*;

import OnX.*;

class OnX_Java_test_4 {
  public OnX_Java_test_4() {
    System.loadLibrary("OnXJavaSWIG");

    System.out.println(System.getProperty("java.class.path"));
    System.out.println(System.getProperty("java.library.path"));

    Class compilerClass = null;
    try {
      compilerClass = Class.forName("sun.tools.javac.Main");
    } catch (Exception aException) {
      System.out.println
        ("OnX_Java_test_4 : could not find class sun.tools.javac.Main");
      compilerClass = null;
    }

    ClassLoader classLoader = compilerClass.getClassLoader();
    System.out.println("OnX_Java_test_4 : classLoader "+classLoader);
    try {
/*
*/
      Class cbkClass = null;
      //String className = "OnX.Initialize";
      String className = "OnX.Test";
      if(classLoader==null) cbkClass = Class.forName(className);
      else cbkClass = classLoader.loadClass(className);

      System.out.println("OnX_Java_test_4 : cbkClass "+cbkClass);

      Object cbkObject = cbkClass.newInstance();
      System.out.println("OnX_Java_test_4 : object "+cbkObject);

      try {
       {Class[] cbkGetMethodArgs = {};
        Method exec = cbkClass.getMethod("exec",cbkGetMethodArgs);
        System.out.println("OnX_Java_test_4 : exec "+exec);
        Object[] cbkInvokeArgs = {};
        exec.invoke(cbkObject,cbkInvokeArgs);}

       {Class[] cbkGetMethodArgs = {Long.TYPE,Long.TYPE,Long.TYPE};
        Method exec = cbkClass.getMethod("nativeTest",cbkGetMethodArgs);
        System.out.println("OnX_Java_test_4 : exec "+exec);
        Long l1 = new Long(11);
        Long l2 = new Long(12);
        Long l3 = new Long(1345678);
        Object[] cbkInvokeArgs = {l1,l2,l3};
        exec.invoke(cbkObject,cbkInvokeArgs);}
      } catch (Exception aException) {
        System.out.println("OnX_Java_test_4 : could not exec method.");
      }

/*
      Class cbkClass = null;
      String className = "detector";
      if(classLoader==null) cbkClass = Class.forName(className);
      else cbkClass = classLoader.loadClass(className);

      System.out.println("OnX_Java_test_4 : cbkClass "+cbkClass);

      Long l = new Long(12345678);
      Class[] cbkCtorArgs = {l.getClass()};
      Constructor cbkCtor = cbkClass.getConstructor(cbkCtorArgs);
      Object[] cbkNewArgs  = {l};
      Object cbkObject = cbkCtor.newInstance(cbkNewArgs);
      System.out.println("OnX_Java_test_4 : object "+cbkObject);

      try {
       {Class[] cbkGetMethodArgs = {};
        Method exec = cbkClass.getMethod("execute",cbkGetMethodArgs);
        System.out.println("OnX_Java_test_4 : exec "+exec);
        Object[] cbkInvokeArgs = {};
        exec.invoke(cbkObject,cbkInvokeArgs);}
      } catch (Exception aException) {
        System.out.println("OnX_Java_test_4 : could not exec method.");
      }
*/
    } catch (Exception aException) {
      System.out.println("OnX_Java_test_4 : could not load class.");
    }

  }
  public static void main(String[] args) {
    new OnX_Java_test_4();
  }        
}
