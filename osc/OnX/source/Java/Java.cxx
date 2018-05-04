#include <Lib/Interfaces/ISession.h>
#include <Lib/Interfaces/IOut.h>
#include <Lib/Interfaces/IManager.h>

#include <OnX/Interfaces/IScriptManager.h>

extern "C" {
void OnXJavaInitialize(ISession*);
void OnXJavaFinalize(ISession*);
ISession* OnXJavaGetSession();
}

#include <stdlib.h>

#include <jni.h>

class OnX_Java {
public:
  OnX_Java(ISession& aSession)
  :fSession(aSession)
  ,fJavaVM(0)    
  ,fJNIEnv(0)
  ,fOnXScript(0)
  ,fExecMethod(0)
  {
    if(!getenv("CLASSPATH")) {
      fSession.out().println("env variable CLASSPATH not setted.");
      return;    
    }
    //fSession.out().println("debug : CLASSPATH :%s",getenv("CLASSPATH"));
    std::string opt;
    opt += "-Djava.class.path=";
    opt += getenv("CLASSPATH");
    JavaVMOption options[2];
    options[0].optionString = (char*)opt.c_str();
    options[1].optionString = (char*)"-verbose";
    JavaVMInitArgs args;
    args.version = JNI_VERSION_1_2;
    args.options = options;
    args.nOptions = 1;
    args.ignoreUnrecognized = 1;
    int rc = JNI_CreateJavaVM(&fJavaVM,(void **)&fJNIEnv,&args);
    if(rc < 0) {
      fSession.out().println("Failed to create Java VM.");
      fJavaVM = 0;
      fJNIEnv = 0;
      return;
    }
    // Build an OnX/Script object :
    jclass cls = fJNIEnv->FindClass("OnX/Script");
    if(!cls) {
      fSession.out().println("Could not find OnX/Script class");
      return;
    }
    //fSession.out().println("class OnX/Script found");
    jmethodID constructor = fJNIEnv->GetMethodID(cls,"<init>","()V");
    if(!constructor) {
      fSession.out().println("Could not find OnX/Script constructor.");
      return;
    }
    fExecMethod = 
      fJNIEnv->GetMethodID(cls,"execute","(Ljava/lang/String;J)V");
    if(!fExecMethod) {
      fSession.out().println("Could not find OnX/Script/execute method.");
      return;
    }
    fOnXScript = fJNIEnv->NewObject(cls,constructor);
    if(!fOnXScript) {
      fSession.out().println("Could not create an OnX/Script object");
      return;
    }
  }
  ~OnX_Java(){
    if(fJNIEnv && fOnXScript) fJNIEnv->DeleteLocalRef(fOnXScript);
    if(fJavaVM) fJavaVM->DestroyJavaVM();
  }
  void execute(const std::string& aString) {
    if(!fJNIEnv) return;
    if(!fOnXScript) return;
    jstring jstr = fJNIEnv->NewStringUTF(aString.c_str());
    jlong sess = (jlong)&fSession;
    fJNIEnv->CallVoidMethod(fOnXScript,fExecMethod,jstr,sess);
  }
private:
  ISession& fSession;
  JavaVM* fJavaVM;       
  JNIEnv* fJNIEnv;
  jobject fOnXScript;
  jmethodID fExecMethod;
};

static void java_Function(const std::string&,void*);

static ISession* sSession = 0; // Beurk. 
//////////////////////////////////////////////////////////////////////////////
ISession* OnXJavaGetSession(
) 
//////////////////////////////////////////////////////////////////////////////
// So that we can hook the session from a Java script
// but also work in a SWIG context.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return sSession;
}
//////////////////////////////////////////////////////////////////////////////
void OnXJavaInitialize(
 ISession* aSession
) 
//////////////////////////////////////////////////////////////////////////////
// Executed, by Interpreters, at first java> java code.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  sSession = aSession; // Rebeurk.

  IManager* manager = aSession->findManager("ScriptManager");
  if(manager) {
    IScriptManager* scriptManager = dynamic_cast<IScriptManager*>(manager);
    if(scriptManager) {
      scriptManager->setInterpreterFunction("Java",
					    java_Function,
					    new OnX_Java(*aSession));
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void OnXJavaFinalize(
 ISession* aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IManager* manager = aSession->findManager("ScriptManager");
  if(manager) {
    IScriptManager* scriptManager = dynamic_cast<IScriptManager*>(manager);
    if(scriptManager) {
      IInterpreterFunction f;
      void* tag;
      scriptManager->findInterpreterFunction("Java",f,tag);
      if(tag) delete ((OnX_Java*)tag);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void java_Function(
 const std::string& aString
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnX_Java* onxJava = (OnX_Java*)aTag;
  if(!onxJava) return;
  onxJava->execute(aString);
}




