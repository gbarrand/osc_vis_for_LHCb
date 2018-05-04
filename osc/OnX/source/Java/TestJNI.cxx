#include <Lib/Printer.h>
#include <Lib/Out.h>

#include <jni.h>

#ifdef WIN32
#undef JNIEXPORT
#undef JNICALL
#define JNIEXPORT
#define JNICALL
#endif

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_OnX_Test_test(JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_OnX_Test_nativeTest(JNIEnv*,jobject,jlong,jlong,jlong);
JNIEXPORT void JNICALL Java_OnX_Test_nativeTestString(JNIEnv*,jobject,jstring);

JNIEXPORT jlong JNICALL Java_OnX_Test_nativeTestStatic(JNIEnv*,jclass,jlong,jstring);
JNIEXPORT jlong JNICALL Java_OnX_Test_Slash::Core::ISession_1findManager(JNIEnv*,jclass,jlong,jstring);
#ifdef __cplusplus
}
#endif

JNIEXPORT void JNICALL Java_OnX_Test_test(JNIEnv*,jclass){
  Lib::Printer printer;
  Lib::Out out(printer);
  out << "debug : Test::test." << Lib::endl;
}
JNIEXPORT void JNICALL Java_OnX_Test_nativeTest(
 JNIEnv* //aEnv
,jobject //aObject
,jlong a1
,jlong a2
,jlong a3
) {
  int i1 = (int)a1;
  int i2 = (int)a2;
  int i3 = (int)a3;
  Lib::Printer printer;
  Lib::Out out(printer);
  out << i1 << " " << i2 << " " << i3 << Lib::endl;
}
JNIEXPORT void JNICALL Java_OnX_Test_nativeTestString(
 JNIEnv* aEnv
,jobject //aObject
,jstring a1
) {
  std::string s = (a1) ? (char*)aEnv->GetStringUTFChars(a1, 0) : "";
  Lib::Printer printer;
  Lib::Out out(printer);
  out << s << " " << Lib::endl;
}
JNIEXPORT jlong JNICALL Java_OnX_Test_nativeTestStatic(
 JNIEnv* aEnv
,jclass //jcls
,jlong jarg1
,jstring jarg2
) {
  printf("debug : Java_OnX_Test_nativeTestStatic : %d %ld %lld\n",
    jarg1,jarg1,jarg1);

  char *arg2 = (jarg2) ? (char *)aEnv->GetStringUTFChars(jarg2, 0) : NULL;
  printf("debug : Java_OnX_Test_nativeTestStatic : \"%s\"\n",arg2);

  jlong jresult = 0 ;
  return jresult;
}
JNIEXPORT jlong JNICALL Java_OnX_Test_Slash::Core::ISession_1findManager(
 JNIEnv* aEnv
,jclass //jcls
,jlong jarg1
,jstring jarg2
) {
  printf("debug : Java_OnX_Test_Slash::Core::ISession_1findManager : %d %ld %lld\n",
    jarg1,jarg1,jarg1);

  char *arg2 = (jarg2) ? (char *)aEnv->GetStringUTFChars(jarg2, 0) : NULL;
  printf("debug : Java_OnX_Test_Slash::Core::ISession_1findManager : \"%s\"\n",arg2);

  jlong jresult = 0 ;
  return jresult;
}
