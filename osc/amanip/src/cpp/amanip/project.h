#ifndef amanip_project_h
#define amanip_project_h

#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>
#include <AIDA/IFunction.h>
#include <AIDA/IConstants.h>

#include <ostream>

namespace amanip {

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,int aIndex
,AIDA::IFilter& aFilter
){
  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  std::string type = aTuple.columnType(aIndex);
  if(type=="float") {
    float v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getFloat(aIndex);
      aFillable.fill((double)v,1);
    }
  } else if(type=="double") {
    double v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getDouble(aIndex);
      aFillable.fill(v,1);
    }
  } else if(type=="char") {
    char v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getChar(aIndex);
      aFillable.fill((double)v,1);
    }
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(type=="byte") {
    unsigned char v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getByte(aIndex);
      aFillable.fill((double)v,1);
    }
#endif
  } else if(type=="short") {
    short v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getShort(aIndex);
      aFillable.fill((double)v,1);
    }
  } else if(type=="int") {
    int v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getInt(aIndex);
      aFillable.fill((double)v,1);
    }
  } else if(type=="long") {
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      aFillable.fill((double)aTuple.getLong(aIndex),1);
    }
  } else if(type=="boolean") {
    bool v;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      v = aTuple.getBoolean(aIndex);
      aFillable.fill((double)v,1);
    }
  } else {
    a_out << "amanip::project : "
        << " make no sense on column type " << type << "."
        << std::endl;
    return false;
  } 
  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IFilter& aFilter
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),1);
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IFilter& aFilter
,AIDA::IEvaluator& aW
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aW.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),aW.evaluateDouble());
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,int aIndexX
,int aIndexY
,AIDA::IFilter& aFilter
){
  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  std::string typex = aTuple.columnType(aIndexX);
  std::string typey = aTuple.columnType(aIndexY);
  if(typex!=typey) {
    a_out << "amanip::project : "
        << " column type mismatch " << typex << " " << typey << "."
        << std::endl;
    return false;
  }
  if(typex=="float") {
    float vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getFloat(aIndexX);
      vy = aTuple.getFloat(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
  } else if(typex=="double") {
    double vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getDouble(aIndexX);
      vy = aTuple.getDouble(aIndexY);
      aFillable.fill(vx,vy,1);
    }
  } else if(typex=="char") {
    char vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getChar(aIndexX);
      vy = aTuple.getChar(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(typex=="byte") {
    unsigned char vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getByte(aIndexX);
      vy = aTuple.getByte(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
#endif
  } else if(typex=="short") {
    short vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getShort(aIndexX);
      vy = aTuple.getShort(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
  } else if(typex=="int") {
    int vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getInt(aIndexX);
      vy = aTuple.getInt(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
  } else if(typex=="long") {
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      aFillable.fill((double)aTuple.getLong(aIndexX),
                     (double)aTuple.getLong(aIndexY),1);
    }
  } else if(typex=="boolean") {
    bool vx,vy;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getBoolean(aIndexX);
      vy = aTuple.getBoolean(aIndexY);
      aFillable.fill((double)vx,(double)vy,1);
    }
  } else {
    a_out << "amanip::project : "
        << " make no sense on column type " << typex << "."
        << std::endl;
    return false;
  } 
  return true;
}


template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IEvaluator& aY
,AIDA::IFilter& aFilter
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aY.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),
                   aY.evaluateDouble(),
                   1);
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IEvaluator& aY
,AIDA::IFilter& aFilter
,AIDA::IEvaluator& aW
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aY.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aW.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),
                   aY.evaluateDouble(),
                   aW.evaluateDouble());
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aEvaluatorX
,AIDA::IEvaluator& aEvaluatorY
,AIDA::IFilter& aFilter
,AIDA::IFilter& aOnEvalFilter
,AIDA::ITuple& a_tmp
){
  if(!aEvaluatorX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aEvaluatorY.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  //NOTE : a ref to the memory AIDA::IFunctions (if any) 
  //       contains in aOnEvalFilter script will be done 
  //       by the below on the aOnEvalFilter instance.
  if(!aOnEvalFilter.initialize(a_tmp)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for script "
        << "\"" << aOnEvalFilter.expression() << "\"."
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
  
    double x = aEvaluatorX.evaluateDouble();
    double y = aEvaluatorY.evaluateDouble();

    a_tmp.reset();
    a_tmp.fill(0,x);
    a_tmp.fill(1,y);
    a_tmp.addRow();
    a_tmp.start();
    a_tmp.next();
    if(!aOnEvalFilter.accept()) continue;    

    aFillable.fill(x,y,1);
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,int aIndexX
,int aIndexY
,int aIndexZ
,AIDA::IFilter& aFilter
){
  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  std::string typex = aTuple.columnType(aIndexX);
  std::string typey = aTuple.columnType(aIndexY);
  std::string typez = aTuple.columnType(aIndexZ);
  if((typex!=typey)||(typey!=typez)) {
    a_out << "amanip::project : "
        << " column type mismatch " 
        << typex << " " 
        << typey << " "
        << typez << "."
        << std::endl;
    return false;
  }
  if(typex=="float") {
    float vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getFloat(aIndexX);
      vy = aTuple.getFloat(aIndexY);
      vz = aTuple.getFloat(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
  } else if(typex=="double") {
    double vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getDouble(aIndexX);
      vy = aTuple.getDouble(aIndexY);
      vz = aTuple.getDouble(aIndexZ);
      aFillable.fill(vx,vy,vz,1);
    }
  } else if(typex=="char") {
    char vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getChar(aIndexX);
      vy = aTuple.getChar(aIndexY);
      vz = aTuple.getChar(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(typex=="byte") {
    unsigned char vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getByte(aIndexX);
      vy = aTuple.getByte(aIndexY);
      vz = aTuple.getByte(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
#endif
  } else if(typex=="short") {
    short vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getShort(aIndexX);
      vy = aTuple.getShort(aIndexY);
      vz = aTuple.getShort(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
  } else if(typex=="int") {
    int vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getInt(aIndexX);
      vy = aTuple.getInt(aIndexY);
      vz = aTuple.getInt(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
  } else if(typex=="long") {
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      aFillable.fill((double)aTuple.getLong(aIndexX),
                     (double)aTuple.getLong(aIndexY),
                     (double)aTuple.getLong(aIndexZ),
                     1);
    }
  } else if(typex=="boolean") {
    bool vx,vy,vz;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      vx = aTuple.getBoolean(aIndexX);
      vy = aTuple.getBoolean(aIndexY);
      vz = aTuple.getBoolean(aIndexZ);
      aFillable.fill((double)vx,(double)vy,(double)vz,1);
    }
  } else {
    a_out << "amanip::project : "
        << " make no sense on column type " << typex << "."
        << std::endl;
    return false;
  } 
  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IEvaluator& aY
,AIDA::IEvaluator& aZ
,AIDA::IFilter& aFilter
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aY.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aZ.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),
                   aY.evaluateDouble(),
                   aZ.evaluateDouble(),
                   1);
  }

  return true;
}

template <class T>
inline bool project(
 std::ostream& a_out
,AIDA::ITuple& aTuple
,T& aFillable
,AIDA::IEvaluator& aX
,AIDA::IEvaluator& aY
,AIDA::IEvaluator& aZ
,AIDA::IFilter& aFilter
,AIDA::IEvaluator& aW
){
  if(!aX.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aY.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aZ.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aFilter.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Filter initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  if(!aW.initialize(aTuple)) {
    a_out << "amanip::project : "
        << " Evaluator initialization failed for tuple " 
        << "\"" << aTuple.title() << "\""
        << std::endl;
    return false;
  }

  // Loop on entries :
  aTuple.start();
  while(aTuple.next()) {
    if(!aFilter.accept()) continue;
    aFillable.fill(aX.evaluateDouble(),
                   aY.evaluateDouble(),
                   aZ.evaluateDouble(),
                   aW.evaluateDouble());
  }

  return true;
}

}

#endif
