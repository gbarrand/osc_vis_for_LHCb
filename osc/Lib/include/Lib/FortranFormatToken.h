#ifndef Lib_FortranFormatTokens_h
#define Lib_FortranFormatTokens_h

#define NUMBER_INTEGER 257
#define NUMBER_EXPONENT 258
#define T_X 259
#define T_I 260
#define T_F 261
#define T_E 262
#define T_G 263
#define T_A 264
#define T_L 265
#define T_SLASH 266
#define UMINUS 267
typedef union {
  unsigned long fInteger;
  Lib::DataDescriptor* fDesc;
  unsigned int fType;
} YYSTYPE;
extern YYSTYPE FortranFormat_lval;

#endif
