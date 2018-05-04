#ifndef Lib_ColumnsTokens_h
#define Lib_ColumnsTokens_h

#define STRG 257
#define T_NAME 258
#define NUMBER_INTEGER 259
#define NUMBER_REAL 260
#define T_TRUE 261
#define T_FALSE 262
#define T_AND 263
#define T_OR 264
#define T_SHORT 265
#define T_INT 266
#define T_INT64 267
#define T_FLOAT 268
#define T_DOUBLE 269
#define T_BOOLEAN 270
#define T_BYTE 271
#define T_CHAR 272
#define T_STRING 273
#define T_JAVA_STRING 274
#define T_TUPLE 275
#define UMINUS 276
typedef union {
  int fIndex;
  unsigned long fInteger;
  double fNumber;
  Slash::Core::IValue::Type fType;
  Lib::Value* fVariable;
} YYSTYPE;
extern YYSTYPE Columns_lval;

#endif
