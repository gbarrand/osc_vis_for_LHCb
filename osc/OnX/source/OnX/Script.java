package OnX;

import java.io.*;
import java.lang.reflect.*;

import OnX.Out;

public class Script {
  private Class fCompilerClass;
  private Object fCompilerObject;
  private int fCounter = 0;
  ////////////////////////////////////////////////////////////////////////////
  public Script() 
  ////////////////////////////////////////////////////////////////////////////
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
  {
    fCompilerClass = null;
    fCompilerObject = null;
    try {
      fCompilerClass = Class.forName("sun.tools.javac.Main");
      Class[] compilerCtorArgs = { OutputStream.class , String.class };
      Constructor compilerCtor = 
          fCompilerClass.getConstructor(compilerCtorArgs);
      Object[] compilerNewArgs  = {new Out(),"javac"};
      fCompilerObject = compilerCtor.newInstance(compilerNewArgs);
      //
    } catch (Exception aException) {
      System.out.println("OnX::Script::Script : Could not find class sun.tools.javac.Main");
      fCompilerClass = null;
      fCompilerObject = null;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
  public void finalize() 
  ////////////////////////////////////////////////////////////////////////////
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
  {
  }
  ////////////////////////////////////////////////////////////////////////////
  public void execute(final String aScript,long aNativeSession) 
  ////////////////////////////////////////////////////////////////////////////
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
  {
    if(fCompilerClass==null) return;
    if(fCompilerObject==null) return;
    // Generate a name (to be sure to load a new class) :
    String name = "OnX_JavaCallback_" + fCounter;
    fCounter++;
    File file = new File(name + ".java");
    boolean deleteFile = true;
    FileOutputStream fileStream = null;
    try {
      fileStream = new FileOutputStream(file);
    } catch (IOException aIOExcepriont) {
    }
    DataOutputStream stream = new DataOutputStream(fileStream);
    try {
      stream.writeBytes("import OnX.*;\n");
      stream.writeBytes("public class " + name + " {\n");
      stream.writeBytes("  private long fSession = "+aNativeSession+";\n");
      //System.out.println("debug : Script : "+aNativeSession);
      stream.writeBytes("  public " + name + "(Long aSession) {\n");
      stream.writeBytes("    fSession = aSession.longValue();\n");
      //stream.writeBytes("    System.out.println(\"debug : OnX_JavaCallback\");\n");
      stream.writeBytes("    System.loadLibrary(\"OnXJavaSWIG\");\n");
      stream.writeBytes("  }\n");
      stream.writeBytes("  public long getSession() { return fSession;}\n");
      stream.writeBytes("  public void execute() {\n");
      //stream.writeBytes("    System.out.println(\"debug : OnX_JavaCallback::exec\");\n");
      stream.writeBytes(aScript);
      stream.writeBytes("  }\n");
      stream.writeBytes("}\n");
      //
      // Appending ":." to classPath does not work !
      StringBuffer classPath = 
        new StringBuffer(System.getProperty("java.class.path"));
      //System.out.println("debug : " + classPath);
      String[] args = new String[3];
      args[0] = "-classpath";
      args[1] = classPath.toString();
      args[2] = file.getCanonicalPath();
      Class[] compilerGetMethodArgs = { args.getClass() };
      Method compilerMethod = 
          fCompilerClass.getMethod("compile",compilerGetMethodArgs);
      Object[] compilerInvokeArgs = { args };
      Boolean rc = 
          (Boolean)compilerMethod.invoke(fCompilerObject,compilerInvokeArgs);
      if(rc.booleanValue()) {
          /*debug
         System.out.println("Compilation of " + 
                 file.getCanonicalPath() +
                 " successful");
          */
          deleteFile = executeClass(name,aNativeSession);
      } else {
        System.out.println("OnX::Script::execute : Compilation of " + 
                           file.getCanonicalPath() +
                           " failed");
        deleteFile = false;
      }
    } catch (Exception aException) {
    }
    //deleteFile = false; //debug
    if(deleteFile) file.delete();
    file = new File(name + ".class");
    if(deleteFile) file.delete();
  }
  ////////////////////////////////////////////////////////////////////////////
  public boolean executeClass(final String aName,long aNativeSession)
  ////////////////////////////////////////////////////////////////////////////
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
  {
    if(fCompilerClass==null) return false;
    ClassLoader classLoader = fCompilerClass.getClassLoader();
    try {
      Class cbkClass = null;
      if(classLoader==null) cbkClass = Class.forName(aName);
      else cbkClass = classLoader.loadClass(aName);
      try {
          Long l = new Long(aNativeSession);
          Class[] cbkCtorArgs = {l.getClass()};
          Constructor cbkCtor = cbkClass.getConstructor(cbkCtorArgs);
          Object[] cbkNewArgs  = {l};
          Object cbkObject = cbkCtor.newInstance(cbkNewArgs);
          try {
              Class[] cbkGetMethodArgs = {};
              Method exec = cbkClass.getMethod("execute",cbkGetMethodArgs);
              Object[] cbkInvokeArgs = {};
              exec.invoke(cbkObject,cbkInvokeArgs);
          } catch (Exception aException) {
              System.out.println("OnX::Script::executeClass : Could not execute method execute of class " + aName);
              //System.out.println(aException.getMessage());
              //System.out.println(aException.toString());
              return false;
          }
      } catch (Exception aException) {
          System.out.println("OnX::Script::executeClass : Could not create an object of class " + aName);
          //System.out.println(aException.getMessage());
          return false;
      }
      //Object cbkObject = cbkClass.newInstance();
    } catch (Exception aException) {
      System.out.println("OnX::Script::executeClass : Could not load class " + aName);
      //System.out.println(aException.getMessage());
      return false;
    }
    return true;
  }
}
