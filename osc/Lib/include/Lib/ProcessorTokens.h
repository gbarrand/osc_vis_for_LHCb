#ifndef Lib_ProcessorTokens_h
#define Lib_ProcessorTokens_h

#define STRG 257
#define T_NAME 258
#define NUMBER_INTEGER 259
#define NUMBER_REAL 260
#define T_PI 261
#define T_TRUE 262
#define T_FALSE 263
#define T_AND 264
#define T_OR 265
#define UMINUS 266
typedef union {
  unsigned int fIndex;
  unsigned int fInteger;
  double fNumber;
  Lib::Expression* fExpression;
} YYSTYPE;
extern YYSTYPE Processor_lval;

#endif
