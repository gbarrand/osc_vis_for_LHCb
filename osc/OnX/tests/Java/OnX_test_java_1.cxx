#include <jni.h>

#include <stdlib.h>
#include <stdio.h>

#include <string>



//////////////////////////////////////////////////////////////////////////////
int main (
 int
,char**
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  // Init the JVM :
  if(!getenv("CLASSPATH")) {
    printf("env variable CLASSPATH not setted.\n");
    return 0;    
  }
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
  JavaVM* fJavaVM;       
  JNIEnv* fJNIEnv;
  int rc = JNI_CreateJavaVM(&fJavaVM,(void**)&fJNIEnv,&args);
  if(rc < 0) {
    printf("Failed to create Java VM.\n");
    return 0;
  }

  // Build an OnX/Test object :
  jclass cls = fJNIEnv->FindClass("OnX/Test");
  if(!cls) {
    printf("Could not find OnX/Test class\n");
    return 0;
  }
  //printf("class OnX/Script found");
  jmethodID constructor = fJNIEnv->GetMethodID(cls,"<init>","()V");
  if(!constructor) {
      printf("Could not find OnX/Script constructor.");
      return 0;
  }
  jmethodID fExecMethod = fJNIEnv->GetMethodID(cls,"exec","()V");
  if(!fExecMethod) {
      printf("Could not find OnX/Script/execute method.\n");
      return 0;
  }
  jobject fOnXScript = fJNIEnv->NewObject(cls,constructor);
  if(!fOnXScript) {
      printf("Could not create an OnX/Script object\n");
      return 0;
  }
  printf("Test::exec : begin\n");
  fJNIEnv->CallVoidMethod(fOnXScript,fExecMethod);
  printf("Test::exec : end\n");

  fJNIEnv->DeleteLocalRef(fOnXScript);
  fJavaVM->DestroyJavaVM();

  return 0;
}
