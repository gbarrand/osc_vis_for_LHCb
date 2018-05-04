#ifndef amanip_tuple_h
#define amanip_tuple_h

#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>
#include <AIDA/IConstants.h>

#include <ostream>

#include "typedefs.h"
#include "inlib.h"

namespace amanip {

//inline std::string name(const AIDA::ITuple& aTuple){
//  const AIDA::IManagedObject* mobject = 
//    Lib_ConstCast(aTuple,AIDA::IManagedObject);
//  if(!mobject) return "";
//  return mobject->name();
//}

inline bool min_max(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,AIDA::IEvaluator& aEvaluator
,AIDA::IFilter& aFilter
,double& aMin
,double& aMax
,std::vector<double>* aVector = 0
){
  aMin = 0;
  aMax = 0;
  if(aVector) aVector->clear();

  if(aTuple.rows()<=0) {
    a_out << "amanip::tuple_evaluateMin :"
          << "Tuple " << ain_lib::sout(aTuple.title()) << "."
          << std::endl;
    return false;
  }

  if(!aEvaluator.initialize(aTuple)) {
    a_out << "tuple_evaluateMin :"
          << " Evaluator initialization failed for tuple " 
          << ain_lib::sout(aTuple.title()) << "."
          << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "tuple_evaluateMin :"
          << " Filter initialization failed for tuple " 
          << ain_lib::sout(aTuple.title()) << "."
          << std::endl;
    return false;
  }

  bool first = true;
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    double value = aEvaluator.evaluateDouble();
    if(first) {
      aMin = value;
      aMax = value;
      first = false;
    } else {
      aMin = (value<aMin?value:aMin);
      aMax = (value>aMax?value:aMax);
    }
    if(aVector) aVector->push_back(value);
  }

  return true;
}

inline bool compatible(std::ostream& a_out,AIDA::ITuple& a_1,AIDA::ITuple& a_2){
  if(a_2.columns()!=a_1.columns()) {
    a_out << "amanip::comaptible :"
          << " booking mismatch :"
          << " columns " << a_2.columns() 
          << " (" << a_1.columns() << " expected)." 
          << std::endl;
    return false;
  } else {
    int coln = a_1.columns();
    for(int coli=0;coli<coln;coli++) {
      if(a_2.columnName(coli)!=a_1.columnName(coli)) {
        a_out << "amanip::comaptible :"
              << " booking mismatch :"
              << " column name " << a_2.columnName(coli) 
              << " (" << a_1.columnName(coli) << " expected)." 
              << std::endl;
        return false;
      } else if(a_2.columnType(coli)!=a_1.columnType(coli)) {
        a_out << "amanip::comaptible :"
              << " booking mismatch :"
              << " column type " << a_2.columnType(coli) 
              << " (" << a_1.columnType(coli) << " expected)." 
              << std::endl;
        return false;
      }
    }
  }
  return true;
}

inline bool copy(std::ostream& a_out,AIDA::ITuple& a_from,AIDA::ITuple& a_to) {
  if(!compatible(a_out,a_from,a_to)) return false;  
  int coln = a_from.columns();
  if(!coln) return true;
  a_from.start();
  while(a_from.next()) {
    for(int index=0;index<coln;index++) {
      std::string type = a_from.columnType(index);
      if(type=="float") {
        if(!a_to.fill(index,a_from.getFloat(index))) return false;
      } else if(type=="double") {
        if(!a_to.fill(index,a_from.getDouble(index))) return false;
      } else if(type=="char") {
        if(!a_to.fill(index,a_from.getChar(index))) return false;
      } else if(type=="short") {
        if(!a_to.fill(index,a_from.getShort(index))) return false;
      } else if(type=="int") {
        if(!a_to.fill(index,a_from.getInt(index))) return false;
      } else if(type=="long") {
        if(!a_to.fill(index,a_from.getLong(index))) return false;
      } else if(type=="boolean") {
        if(!a_to.fill(index,a_from.getBoolean(index))) return false;
      } else if(type=="string") {
        if(!a_to.fill(index,a_from.getString(index))) return false;
      } else if(type=="ITuple") {
        if(!a_to.fill(index,*a_from.getObject(index))) return false;
      } else {
        a_out << "amanip::copy :" 
              << " column type " << type << " not handled."
              << std::endl;
        return false;
      }
    }
    a_to.addRow();
  }
  return true;
}

inline bool copy(std::ostream& a_out,AIDA::ITuple& a_from,AIDA::ITuple& a_to,AIDA::IFilter& a_filter) {
  if(!compatible(a_out,a_from,a_to)) return false;  
  if(!a_filter.initialize(a_from)) {
    a_out << "amanip::copy(filter) :"
          << " Filter initialization failed for tuple " 
          << ain_lib::sout(a_from.title())
          << std::endl;
    return false;
  }
  int coln = a_from.columns();
  if(!coln) return true;
  a_from.start();
  while(a_from.next()) {
    if(!a_filter.accept()) continue;
    for(int index=0;index<coln;index++) {
      std::string type = a_from.columnType(index);
      if(type=="float") {
        if(!a_to.fill(index,a_from.getFloat(index))) return false;
      } else if(type=="double") {
        if(!a_to.fill(index,a_from.getDouble(index))) return false;
      } else if(type=="char") {
        if(!a_to.fill(index,a_from.getChar(index))) return false;
      } else if(type=="short") {
        if(!a_to.fill(index,a_from.getShort(index))) return false;
      } else if(type=="int") {
        if(!a_to.fill(index,a_from.getInt(index))) return false;
      } else if(type=="long") {
        if(!a_to.fill(index,a_from.getLong(index))) return false;
      } else if(type=="boolean") {
        if(!a_to.fill(index,a_from.getBoolean(index))) return false;
      } else if(type=="string") {
        if(!a_to.fill(index,a_from.getString(index))) return false;
      } else if(type=="ITuple") {
        if(!a_to.fill(index,*a_from.getObject(index))) return false;
      } else {
        a_out << "amanip::copy(filter) :" 
              << " column type " << type << " not handled."
              << std::endl;
        return false;
      }
    }
    a_to.addRow();
  }
  return true;
}

// used in BatchLab/AsciiTree.
inline bool get_string(std::ostream& a_out,const AIDA::ITuple& a_o,int a_index,std::string& a_value) {
  std::string type = a_o.columnType(a_index);
  if(type=="float") {
    a_value = ain_lib::d2s(a_o.getFloat(a_index));
  } else if(type=="double") {
    a_value = ain_lib::d2s(a_o.getDouble(a_index));
  } else if(type=="char") {
    // char written as its ascii number.
    a_value = ain_lib::to<unsigned int>(a_o.getChar(a_index));
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(type=="byte") {
    // unsigned char written as its ascii number.
    a_value = ain_lib::to<unsigned int>(a_o.getByte(a_index));
#endif
  } else if(type=="short") {
    a_value = ain_lib::to<short>(a_o.getShort(a_index));
  } else if(type=="int") {
    a_value = ain_lib::to<int>(a_o.getInt(a_index));
  } else if(type=="long") {
    a_value = ain_lib::to<amanip::int64>((amanip::int64)a_o.getLong(a_index));
  } else if(type=="boolean") {
    a_value = ain_lib::to(a_o.getBoolean(a_index));
  } else if(type=="string") {
    a_value = a_o.getString(a_index);
  } else {
    a_value = "";
    a_out << "amanip::get_string : "
          << " make no sense on column type " << type << "."
          << std::endl;
    return false;
  } 
  return true;
}

inline bool write(AIDA::ITuple& a_o,std::ostream& a_io,bool a_header,bool a_tab,std::ostream& a_out){
  if(a_header) {
    // Write title :
    a_io << a_o.title() << std::endl;
  }

  // Check column types :
  int coln = a_o.columns();
  if(coln<=0) return true;
  for(int index=0;index<coln;index++) {
    std::string type = a_o.columnType(index);
    if( (type=="ITuple") ||
        (type=="string") ||
        (type=="char") ||
        (type=="byte") ||
        (type=="boolean")
      ) {
      a_out << "amanip::write(ITuple) :" 
            << " column type " << ain_lib::sout(type) << " not supported."
            << std::endl;
      return false;
    }
  }
  
  if(a_header) {
    // Write column names :
    for(int index=0;index<coln;index++) {
      if(index) a_io << "\t";
      a_io << a_o.columnName(index);
    }
    a_io << std::endl;
  }

  // rows :
  a_o.start();
  while(a_o.next()) {
    for(int index=0;index<coln;index++) {
      std::string value;
      if(!amanip::get_string(a_out,a_o,index,value)) {
        return false;
      }
      if(index) {
        if(a_tab) a_io << "\t";
        else a_io << " ";
      }
      a_io << value;
    }
    a_io << std::endl;
  }

  return true;
}

inline bool fill(std::ostream& a_out,AIDA::ITuple& a_o,int a_index,const std::string& a_value) {
  std::string type = a_o.columnType(a_index);
  if(type=="float") {
    float val;
    if(!ain_lib::to<float>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a float."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,val);
  } else if(type=="double") {
    double val;
    if(!ain_lib::to<double>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a double."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,val);
  } else if(type=="short") {
    short val;
    if(!ain_lib::to<short>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a short."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,val);
  } else if(type=="int") {
    int val;
    if(!ain_lib::to<int>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a int."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,val);
  } else if(type=="boolean") {
    bool val;
    if(!ain_lib::to(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a bool."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,val);
  } else if(type=="string") {
    return a_o.fill(a_index,a_value);

  // messy cases :

  } else if(type=="char") {
    int val;
    if(ain_lib::to<int>(a_value,val)) { //ascii value.
      if((val<-127)||(val>127)) {
        a_out << "amanip::fill :"
              << " can't convert " << ain_lib::sout(a_value) 
              << " to a char."
              << std::endl;
        return false;
      }
      return a_o.fill(a_index,(char)val);
    } else {
      if(a_value.size()!=1) {
        a_out << "amanip::fill :"
              << " bad char value " << ain_lib::sout(a_value)
              << std::endl;
        return false;
      }
      return a_o.fill(a_index,(char)a_value[0]);
    }
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(type=="byte") {
    unsigned int val;
    if(!ain_lib::to<unsigned int>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to an unsigned char."
            << std::endl;
      return false;
    }
    if(val>255) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to an unsigned char."
            << std::endl;
      return false;
    }
    return a_o.fill(a_index,(unsigned char)val);
#endif
  } else if(type=="long") {
    amanip::int64 val;
    if(!ain_lib::to<amanip::int64>(a_value,val)) {
      a_out << "amanip::fill :"
            << " can't convert " << ain_lib::sout(a_value) << " to a int64."
            << std::endl;
      return false;
    }
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
    return a_o.fill(a_index,(long)val);
#else
    return a_o.fill(a_index,(AIDA::int64)val);
#endif
  } else {
    a_out << "amanip::fill :"
          << " make no sense on column type " << type << "."
          << std::endl;
    return false;
  } 
}

}

#endif
