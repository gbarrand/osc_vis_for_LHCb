#ifndef Rio_SIType_h
#define Rio_SIType_h

namespace Rio {
namespace Streamer_Info {

  enum Type {              // sizeof :
    BASE = 0,              //  x
    ARRAY = 20,            //  ?
    POINTER = 40,          //  4
    POINTER_INT = 43,      //  4
    POINTER_FLOAT = 45,    //  4
    POINTER_DOUBLE = 48,   //  4
    COUNTER =  6,          //  4
    CHAR =  1,             //  1
    SHORT =  2,            //  2
    INT =  3,              //  4
    FLOAT =  5,            //  4
    DOUBLE =  8,           //  8
    UNSIGNED_CHAR =  11,   //  1
    UNSIGNED_SHORT =  12,  //  2
    UNSIGNED_INT = 13,     //  4
    BOOL = 18,             //  1 ?
    OBJECT = 61,           //  ?
    OBJECT_ANY = 62,       //  ?
    OBJECT_ARROW = 63,     //  ?
    OBJECT_POINTER = 64,   //  ?
    TSTRING = 65,          //  8  
    TOBJECT = 66,          // 12
    TNAMED = 67            // 28
  };

}}

#endif
