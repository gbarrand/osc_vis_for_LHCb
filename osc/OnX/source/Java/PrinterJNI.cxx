#include <Slash/Core/ISession.h>
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
JNIEXPORT void JNICALL Java_OnX_Printer_nativePrint(JNIEnv*,jobject,jlong,jstring);
#ifdef __cplusplus
}
#endif

JNIEXPORT void JNICALL Java_OnX_Printer_nativePrint(
 JNIEnv* aEnv
,jobject //aObject
,jlong aSession
,jstring aString
) {
  char* arg = aString ? (char*)aEnv->GetStringUTFChars(aString,0) : NULL;
  Slash::Core::ISession* session = (Slash::Core::ISession*)aSession;
  if(!session) {
    Lib::Printer printer;
    Lib::Out out(printer);
    //out << "Java_OnX_Printer_nativePrint : " << std::string(arg);
    if(arg) out << std::string(arg);
  } else {
    Slash::Core::IWriter& printer = session->printer();
    if(arg) printer.write(arg);
  }
  if(arg) aEnv->ReleaseStringUTFChars(aString,arg);
}
