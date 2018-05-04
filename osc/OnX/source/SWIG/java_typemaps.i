//////////////////////////////////////////////////////////////////////////////
/// std::vector<std::string> /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_CONST_STD_VECTOR_STD_STRING "jobjectArray"
%typemap(jtype) ARG_CONST_STD_VECTOR_STD_STRING "String[]"
%typemap(jstype) ARG_CONST_STD_VECTOR_STD_STRING "String[]"
%typemap(in) ARG_CONST_STD_VECTOR_STD_STRING {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  $1 = new std::vector<std::string>();
  for (i = 0; i<size; i++) {
    jstring j_string = (jstring)JCALL2(GetObjectArrayElement, jenv, $input, i);
    const char* c_string = JCALL2(GetStringUTFChars, jenv, j_string, 0);
    $1->push_back(c_string);
    JCALL2(ReleaseStringUTFChars, jenv, j_string, c_string);
    JCALL1(DeleteLocalRef, jenv, j_string);
  }
}

%typemap(freearg) ARG_CONST_STD_VECTOR_STD_STRING {
  delete $1;
}

%typemap(javain) ARG_CONST_STD_VECTOR_STD_STRING "$javainput"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_STD_VECTOR_STD_STRING "jobjectArray"
%typemap(jtype) ARG_STD_VECTOR_STD_STRING "String[]"
%typemap(jstype) ARG_STD_VECTOR_STD_STRING "String[]"
%typemap(in) ARG_STD_VECTOR_STD_STRING {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  for (i = 0; i<size; i++) {
    jstring j_string = (jstring)JCALL2(GetObjectArrayElement, jenv, $input, i);
    const char* c_string = JCALL2(GetStringUTFChars, jenv, j_string, 0);
    $1.push_back(c_string);
    JCALL2(ReleaseStringUTFChars, jenv, j_string, c_string);
    JCALL1(DeleteLocalRef, jenv, j_string);
  }
}

%typemap(freearg) ARG_STD_VECTOR_STD_STRING {
}

%typemap(javain) ARG_STD_VECTOR_STD_STRING "$javainput"


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_STD_VECTOR_STD_STRING {
  const jclass jsclass = JCALL1(FindClass, jenv, "java/lang/String");
  int len = result.size();
  jresult = JCALL3(NewObjectArray, jenv, len, jsclass, NULL);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    jstring j_string = JCALL1(NewStringUTF, jenv, result[i].c_str());
    JCALL3(SetObjectArrayElement, jenv, jresult, i, j_string);
    JCALL1(DeleteLocalRef, jenv, j_string);
  }
}

%typemap(javaout) RET_STD_VECTOR_STD_STRING {
  return $jnicall;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_CONST_STD_VECTOR_STD_STRING {
  const jclass jsclass = JCALL1(FindClass, jenv, "java/lang/String");
  int len = result->size();
  jresult = JCALL3(NewObjectArray, jenv, len, jsclass, NULL);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    jstring j_string = JCALL1(NewStringUTF, jenv, (*result)[i].c_str());
    JCALL3(SetObjectArrayElement, jenv, jresult, i, j_string);
    JCALL1(DeleteLocalRef, jenv, j_string);
  }
}

%typemap(javaout) RET_CONST_STD_VECTOR_STD_STRING {
  return $jnicall;
}

//////////////////////////////////////////////////////////////////////////////
/// std::vector<double> //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_CONST_STD_VECTOR_DOUBLE "jdoubleArray"
%typemap(jtype) ARG_CONST_STD_VECTOR_DOUBLE "double[]"
%typemap(jstype) ARG_CONST_STD_VECTOR_DOUBLE "double[]"
%typemap(in) ARG_CONST_STD_VECTOR_DOUBLE {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  $1 = new std::vector<double>();
  jdouble* j_double = JCALL2(GetDoubleArrayElements,jenv,$input,0);
  for (i = 0; i<size; i++) {
    $1->push_back(j_double[i]);
  }
  JCALL3(ReleaseDoubleArrayElements,jenv,$input,j_double,0);
}

%typemap(freearg) ARG_CONST_STD_VECTOR_DOUBLE {
  delete $1;
}

%typemap(javain) ARG_CONST_STD_VECTOR_DOUBLE "$javainput"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_STD_VECTOR_DOUBLE "jdoubleArray"
%typemap(jtype) ARG_STD_VECTOR_DOUBLE "double[]"
%typemap(jstype) ARG_STD_VECTOR_DOUBLE "double[]"
%typemap(in) ARG_STD_VECTOR_DOUBLE {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  jdouble* j_double = JCALL2(GetDoubleArrayElements,jenv,$input,0);
  for (i = 0; i<size; i++) {
    $1.push_back(j_double[i]);
  }
  JCALL3(ReleaseDoubleArrayElements,jenv,$input,j_double,0);
}

%typemap(freearg) ARG_STD_VECTOR_DOUBLE {
}

%typemap(javain) ARG_STD_VECTOR_DOUBLE "$javainput"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_STD_VECTOR_DOUBLE {
  int len = result.size();
  jresult = JCALL1(NewDoubleArray, jenv, len);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    JCALL4(SetDoubleArrayRegion, jenv, jresult, i, 1, &(result[i]));
  }
}

%typemap(javaout) RET_STD_VECTOR_DOUBLE  {
  return $jnicall;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_CONST_STD_VECTOR_DOUBLE {
  int len = result->size();
  jresult = JCALL1(NewDoubleArray, jenv, len);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    JCALL4(SetDoubleArrayRegion, jenv, jresult, i, 1, &((*result)[i]));
  }
}

%typemap(javaout) RET_CONST_STD_VECTOR_DOUBLE  {
  return $jnicall;
}

//////////////////////////////////////////////////////////////////////////////
/// std::vector<int> //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_CONST_STD_VECTOR_INT "jintArray"
%typemap(jtype) ARG_CONST_STD_VECTOR_INT "int[]"
%typemap(jstype) ARG_CONST_STD_VECTOR_INT "int[]"
%typemap(in) ARG_CONST_STD_VECTOR_INT {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  $1 = new std::vector<int>();
  jint* j_int = JCALL2(GetIntArrayElements,jenv,$input,0);
  for (i = 0; i<size; i++) {
    $1->push_back(j_int[i]);
  }
  JCALL3(ReleaseIntArrayElements,jenv,$input,j_int,0);
}

%typemap(freearg) ARG_CONST_STD_VECTOR_INT {
  delete $1;
}

%typemap(javain) ARG_CONST_STD_VECTOR_INT "$javainput"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(jni) ARG_STD_VECTOR_INT "jintArray"
%typemap(jtype) ARG_STD_VECTOR_INT "int[]"
%typemap(jstype) ARG_STD_VECTOR_INT "int[]"
%typemap(in) ARG_STD_VECTOR_INT {
  int i = 0;
  jint size = JCALL1(GetArrayLength, jenv, $input);
  jint* j_int = JCALL2(GetIntArrayElements,jenv,$input,0);
  for (i = 0; i<size; i++) {
    $1.push_back(j_int[i]);
  }
  JCALL3(ReleaseIntArrayElements,jenv,$input,j_int,0);
}

%typemap(freearg) ARG_STD_VECTOR_INT {
}

%typemap(javain) ARG_STD_VECTOR_INT "$javainput"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_STD_VECTOR_INT {
  int len = result.size();
  jresult = JCALL1(NewIntArray, jenv, len);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    jint j_int = result[i]; //jint is a long.
    JCALL4(SetIntArrayRegion, jenv, jresult, i, 1, &j_int);
  }
}

%typemap(javaout) RET_STD_VECTOR_INT  {
  return $jnicall;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%typemap(out) RET_CONST_STD_VECTOR_INT {
  int len = result->size();
  jresult = JCALL1(NewIntArray, jenv, len);
  // exception checking omitted
  for (int i=0; i<len; i++) {
    jint j_int = (*result)[i]; //jint is a long.
    JCALL4(SetIntArrayRegion, jenv, jresult, i, 1, &j_int);
  }
}

%typemap(javaout) RET_CONST_STD_VECTOR_INT  {
  return $jnicall;
}
